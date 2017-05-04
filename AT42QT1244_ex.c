/*!\file AT42QT1244_ex.c
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Atmel Capacitive Driver extensions code
**/
/****************************************************************/
#include "AT42QT1244.h"
#include "globals.h"

#if defined(HAL_IWDG_MODULE_ENABLED)
#include "iwdg.h"
#endif

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_AT42QT1244)
/****************************************************************/
// TODO: doxygen


FctERR AT42QT1244_Send_Command(AT42QT_cmd cmd)
{
	if ((cmd > AT42QT__RESET_DEVICE) && (cmd < AT42QT__LOW_LEVEL_CALIBRATION))	{ return ERR_VALUE; }
	return AT42QT1244_Write(&cmd, AT42QT__CONTROL_COMMAND, 1);
}


FctERR AT42QT1244_Send_Setup(uint8_t * setup, uint8_t Addr, uint8_t Nb)
{
	FctERR	err = AT42QT1244_Send_Command(AT42QT__WRITE_SETUPS);
	if (err)	{ return err; }

	return AT42QT1244_Write(setup, Addr, Nb);
}


FctERR AT42QT1244_Setup_Key(uint8_t Key, bool use)
{
	const uint8_t				NDIL_Val = 4;		// 4 is the default NDIL value
	uAT42QT_REG__SETUP_165_188	tmp;
	FctERR						err;

	if (Key > AT42QT__CALIBRATE_KEY_23)	{ return ERR_VALUE; }

	err = AT42QT1244_Read((uint8_t *) &tmp, AT42QT__SETUP_KEYS_MODE_0 + Key, sizeof(tmp));	// 165 is the NDIL register of the 1st key
	if (err)	{ return err; }

	tmp.Bits.NDIL = use ? NDIL_Val : 0;
	return AT42QT1244_Send_Setup((uint8_t *) &tmp, AT42QT__SETUP_KEYS_MODE_0 + Key, sizeof(tmp));
}

FctERR AT42QT1244_Setup_FHM(AT42QT_FHM FHM)
{
	uAT42QT_REG__SETUP_244	tmp;
	FctERR					err;

	if (FHM > AT42QT__FHM_FREQUENCY_SWEEP)	{ return ERR_VALUE; }

	err = AT42QT1244_Read((uint8_t *) &tmp, AT42QT__SETUP_FREQ_HOPING_DWELL, 1);
	if (err)	{ return err; }

	tmp.Bits.FHM = FHM;
	return AT42QT1244_Send_Setup((uint8_t *) &tmp, AT42QT__SETUP_FREQ_HOPING_DWELL, 1);
}


FctERR AT42QT1244_Get_Keys(uint32_t * Keys)
{
	uint8_t tmp[3];
	FctERR	err = AT42QT1244_Read(tmp, AT42QT__DETECT_STATUS_1, sizeof(tmp));
	if (err)	{ return err; }

	*Keys = ((tmp[2] * 0x10000UL) + (tmp[1] * 0x100UL) + tmp[0]) & 0x00FFFFFFUL;
	return ERR_OK;
}


int AT42QT1244_is_Calib_Pending(void)
{
	uAT42QT_REG__DEVICE_STATUS st;
	FctERR err = AT42QT1244_Get_Status(&st);
	if (err)	{ return -1; }

	return st.Bits.Key_In_Calib;
}


FctERR AT42QT1244_Calibrate_Freq_Offset(void)
{
	FctERR		err;
	int			calib = 1, i, j;
	uint8_t		Freqs[3], CFO[2][24];
	uint16_t	Refs[3][24];

	// Low Level calibration to set automatically FREQ0 to FREQ2
	err = AT42QT1244_Calibrate_Low_Level();
	if (err)	{ return err; }

	// Wait for end of low level calibration
	while (calib) {
		#if defined(HAL_IWDG_MODULE_ENABLED)
			HAL_IWDG_Refresh(&hiwdg);
		#endif
		calib = AT42QT1244_is_Calib_Pending();
		if (calib == -1)	{ return ERR_NOTAVAIL; }
	};

	// Get all FREQ
	err = AT42QT1244_Read(Freqs, AT42QT__SETUP_FREQ0, sizeof(Freqs));	// Get FREQ0, FREQ1, FREQ2
	if (err)	{ return err; }

	// Disable frequency hoping mode
	err = AT42QT1244_Setup_FHM(AT42QT__FHM_OFF);
	if (err)	{ return err; }

	for (i = 0 ; i < 2 ; i++)
	{
		calib = 1;	// Reset calib for while loop

		// Send FREQ0, FREQ1, then FREQ2 to FREQ0 reg
		err = AT42QT1244_Send_Setup(&Freqs[i], AT42QT__SETUP_FREQ0, 1);
		if (err)	{ return err; }

		// Launch calibration for all keys
		err = AT42QT1244_Calibrate_All_Keys();
		if (err)	{ return err; }

		// Wait for end of keys calibration
		while (calib) {
			#if defined(HAL_IWDG_MODULE_ENABLED)
				HAL_IWDG_Refresh(&hiwdg);
			#endif
			calib = AT42QT1244_is_Calib_Pending();
			if (calib == -1)	{ return ERR_NOTAVAIL; }
		};

		// Get reference for keys
		for (j = 0 ; j <= AT42QT__CALIBRATE_KEY_23 ; j++) {
			uAT42QT_REG__KEY_DATA dat;
			err = AT42QT1244_Get_Key_Data(&dat, j);
			if (err)	{ return err; }

			Refs[i][j] = dat.Data.Reference;
		}
	}

	// Write FREQ0 back
	err = AT42QT1244_Send_Setup(&Freqs[0], AT42QT__SETUP_FREQ0, 1);
	if (err)	{ return err; }

	// Compute CFO values
	for (i = 0 ; i < 2 ; i++) {
		for (j = 0; j < AT42QT__CALIBRATE_KEY_23 ; j++) {
			CFO[i][j] = (uint8_t) (Refs[i+1][j] - Refs[i][j]);
		}
	}

	// Write CFO values
	err = AT42QT1244_Send_Setup(&CFO[0][0], AT42QT__SETUP_CFO_1_0, sizeof(CFO));
	if (err)	{ return err; }

	// Enable frequency hoping mode
	return AT42QT1244_Setup_FHM(AT42QT__FHM_ADJ_KEYS_REF_DURING_HOP);
}


/****************************************************************/
#endif
#endif
/****************************************************************/
