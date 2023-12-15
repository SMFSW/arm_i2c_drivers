/*!\file AT42QT1244_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief AT42QT1244 Driver procedures
** \details AT42QT1244: 24-key QMatrix FMEA IEC/EN/UL60730 Touch Sensor
**/
/****************************************************************/
#include "AT42QT1244.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_AT42QT1244)

// HARMcksL libs
#include "tick_utils.h"
/****************************************************************/


AT42QT1244_t AT42QT1244[I2C_AT42QT1244_NB] = { 0 };


uint16_t AT42QT1244_crc16(uint16_t crc, const uint8_t data)
{
	crc ^= (uint16_t) LSHIFT(data, 8);
	for (uintCPU_t i = 0 ; i < 8 ; i++)
	{
		if (crc & 0x8000)	{ crc = (crc << 1) ^ 0x1021; }
		else				{ crc <<= 1; }
	}

	return crc;
}


/****************************************************************/


__WEAK FctERR NONNULL__ AT42QT1244_Init_Sequence(AT42QT1244_t * const pCpnt)
{
	FctERR	err;
	uint8_t	DATA[2];

	AT42QT1244_Delay_PowerOn(pCpnt);	// Trying to ensure component is ready to communicate (assuming host and component are powered and starting almost simultaneously)

	err = AT42QT1244_Read(pCpnt->cfg.slave_inst, DATA, AT42QT__SETUP_HOST_CRC_LSB, 2);	// Read HCRC from device

/*! \code
	// Example: Setup the component to disable unused keys (last 14 ones)
	if (!err)
	{
		const uint8_t	idx = pCpnt - AT42QT1244;
		const uint16_t	HCRC = NVM_Get_AT42QT1244_HCRC(idx);	// USER IMPLEMENTED: Get previously computed HCRC from host to check against component one

		if (HCRC != (MAKEWORD(DATA[0], DATA[1])))	// HCRC mismatch between host and component, setup needs to be performed
		{
			const uint8_t DATA[14] = { 0 };
			err |= AT42QT1244_Send_Setup(pCpnt, DATA, AT42QT__SETUP_KEYS_MODE_10, sizeof(DATA));

			// Some issues encountered while writing setup block (reason unidentified), resetting the MCU and trying to write again seems to solve issue
			if (err)	{ NVIC_SystemReset(); }
			else
			{
				NVM_Save_AT42QT1244_HCRC(idx, crc);				// USER IMPLEMENTED: Save new HCRC to host non-volatile memory

				AT42QT1244_Reset(pCpnt);
				NVIC_SystemReset();								// Ensure CRC is properly written in NVM (not relying on interrupts) before reset
			}
		}
	}
\endcode */

	return err;
}


/****************************************************************/


void NONNULL__ AT42QT1244_Delay_PowerOn(AT42QT1244_t * const pCpnt)
{
	const uint8_t	loop_delay = 2;		// 2ms delay for each loop (consider watchdog period if used, on ms basis)
	const int32_t	PON_time = 100;		// Power on time with initialization given in datasheet (95ms rounded to 100)
	const int32_t	loops = max(0, (int32_t) (PON_time - (HAL_GetTick() - pCpnt->hPowerOn))) / loop_delay;

	for (uintCPU_t i = 0 ; i < loops ; i++)
	{
		#if defined(HAL_IWDG_MODULE_ENABLED)
			HAL_IWDG_Refresh(&hiwdg);
		#endif
		HAL_Delay(loop_delay);
	}
}


FctERR NONNULL__ AT42QT1244_Calibrate_Freq_Hopping(AT42QT1244_t * const pCpnt, uint16_t * const hcrc)
{
	FctERR		err;
	intCPU_t	calib = 1, i, j;
	uint8_t		Freqs[3], CFO[2][24];
	uint16_t	Refs[3][24], crc;

	// Low Level calibration to set automatically FREQ0 to FREQ2
	err = AT42QT1244_Calibrate_Low_Level(pCpnt);
	if (err)	{ return err; }

	// Wait for end of low level calibration
	while (calib) {
		#if defined(HAL_IWDG_MODULE_ENABLED)
			HAL_IWDG_Refresh(&hiwdg);
		#endif
		calib = AT42QT1244_is_Calib_Pending(pCpnt);
		if (calib == -1)	{ return ERROR_NOTAVAIL; }
	}

	// Get all FREQ
	err = AT42QT1244_Read(pCpnt->cfg.slave_inst, Freqs, AT42QT__SETUP_FREQ0, sizeof(Freqs));	// Get FREQ0, FREQ1, FREQ2
	if (err)	{ return err; }

	// Disable frequency hoping mode
	err = AT42QT1244_Setup_FHM(pCpnt, &crc, AT42QT__FHM_OFF);
	if (err)	{ return err; }

	for (i = 0 ; i < 2 ; i++)
	{
		calib = 1;	// Reset calib for while loop

		// Send FREQ0, FREQ1, then FREQ2 to FREQ0 reg
		err = AT42QT1244_Send_Setup(pCpnt, &crc, &Freqs[i], AT42QT__SETUP_FREQ0, 1);
		if (err)	{ return err; }

		// Launch calibration for all keys
		err = AT42QT1244_Calibrate_All_Keys(pCpnt);
		if (err)	{ return err; }

		// Wait for end of keys calibration
		while (calib) {
			#if defined(HAL_IWDG_MODULE_ENABLED)
				HAL_IWDG_Refresh(&hiwdg);
			#endif
			Delay_us(1000);
			calib = AT42QT1244_is_Calib_Pending(pCpnt);
			if (calib == -1)	{ return ERROR_NOTAVAIL; }
		}

		// Get reference for keys
		for (j = 0 ; j <= AT42QT__CALIBRATE_KEY_23 ; j++) {
			uAT42QT_REG__KEY_DATA dat;
			err = AT42QT1244_Get_Key_Data(pCpnt, &dat, j);
			if (err)	{ return err; }

			Refs[i][j] = dat.Data.Reference;
		}
	}

	// Write FREQ0 back
	err = AT42QT1244_Send_Setup(pCpnt, &crc, &Freqs[0], AT42QT__SETUP_FREQ0, 1);
	if (err)	{ return err; }

	// Compute CFO values
	for (i = 0 ; i < 2 ; i++) {
		for (j = 0 ; j < AT42QT__CALIBRATE_KEY_23 ; j++) {
			CFO[i][j] = (uint8_t) (Refs[i+1][j] - Refs[i][j]);
		}
	}

	// Write CFO values
	err = AT42QT1244_Send_Setup(pCpnt, &crc, &CFO[0][0], AT42QT__SETUP_CFO_1_0, sizeof(CFO));
	if (err)	{ return err; }

	// Enable frequency hopping mode
	err |= AT42QT1244_Setup_FHM(pCpnt, &crc, AT42QT__FHM_ADJ_KEYS_REF_DURING_HOP);

	*hcrc = crc;
	return err;
}


FctERR NONNULL__ AT42QT1244_Calibrate_Low_Level(AT42QT1244_t * const pCpnt)
{
	if (AT42QT1244_is_Calib_Pending(pCpnt))	{ return ERROR_BUSY; }

	return AT42QT1244_Send_Command(pCpnt, AT42QT__LOW_LEVEL_CALIBRATION);
}


FctERR NONNULL__ AT42QT1244_Calibrate_All_Keys(AT42QT1244_t * const pCpnt)
{
	if (AT42QT1244_is_Calib_Pending(pCpnt))	{ return ERROR_BUSY; }

	return AT42QT1244_Send_Command(pCpnt, AT42QT__CALIBRATE_ALL_KEYS);
}


FctERR NONNULL__ AT42QT1244_Calibrate_Key(AT42QT1244_t * const pCpnt, uint8_t Key)
{
	if (Key > AT42QT__CALIBRATE_KEY_23)		{ return ERROR_VALUE; }
	if (AT42QT1244_is_Calib_Pending(pCpnt))	{ return ERROR_BUSY; }

	return AT42QT1244_Send_Command(pCpnt, Key);
}


/****************************************************************/


__WEAK FctERR NONNULL__ AT42QT1244_handler(AT42QT1244_t * const pCpnt)
{
	FctERR err;

#if AT42QT1244_GET_KEYS_ONLY
	err = AT42QT1244_Get_Keys(pCpnt, &pCpnt->keys);
#else
	uint8_t TMP[7];
	err = AT42QT1244_Read(pCpnt->cfg.slave_inst, TMP, AT42QT__CNT_100MS, sizeof(TMP));

	if (!err)
	{
		pCpnt->cnt_100ms = TMP[0];
		pCpnt->cnt_SignalFail = TMP[1];
		pCpnt->cnt_MatrixScan = TMP[2];
		pCpnt->status.Byte = TMP[3];
		pCpnt->keys = (LSHIFT(TMP[6], 16) + LSHIFT(TMP[5], 8) + TMP[4]) & 0x00FFFFFFUL;
	}
#endif

#if defined(VERBOSE)
	const uint8_t idx = pCpnt - AT42QT1244;
	printf("AT42QT1244 id%d: keys state 0x%lX", idx, pCpnt->keys);
#endif

	return err;
}


__WEAK FctERR NONNULL__ AT42QT1244_handler_it(AT42QT1244_t * const pCpnt)
{
	FctERR	err;
	bool	interrupt;

	err = AT42QT1244_CHANGE_GPIO_Get(pCpnt, &interrupt);
	if ((!err) && interrupt)	{ err = AT42QT1244_handler(pCpnt); }

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
