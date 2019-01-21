/*!\file AT42QT1244_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
** \brief AT42QT1244 Driver procedures
** \details AT42QT1244: 24-key QMatrix FMEA IEC/EN/UL60730 Touch Sensor
**/
/****************************************************************/
#include "AT42QT1244.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_AT42QT1244)
/****************************************************************/


AT42QT1244_t AT42QT1244[I2C_AT42QT1244_NB];



/****************************************************************/


__WEAK FctERR NONNULL__ AT42QT1244_Init_Sequence(AT42QT1244_t * pCpnt)
{
	return ERROR_OK;
}


/****************************************************************/


FctERR NONNULL__ AT42QT1244_Calibrate_Freq_Offset(AT42QT1244_t * pCpnt)
{
	FctERR		err;
	int			calib = 1, i, j;
	uint8_t		Freqs[3], CFO[2][24];
	uint16_t	Refs[3][24];

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
	err = AT42QT1244_Setup_FHM(pCpnt, AT42QT__FHM_OFF);
	if (err)	{ return err; }

	for (i = 0 ; i < 2 ; i++)
	{
		calib = 1;	// Reset calib for while loop

		// Send FREQ0, FREQ1, then FREQ2 to FREQ0 reg
		err = AT42QT1244_Send_Setup(pCpnt, &Freqs[i], AT42QT__SETUP_FREQ0, 1);
		if (err)	{ return err; }

		// Launch calibration for all keys
		err = AT42QT1244_Calibrate_All_Keys(pCpnt);
		if (err)	{ return err; }

		// Wait for end of keys calibration
		while (calib) {
			#if defined(HAL_IWDG_MODULE_ENABLED)
				HAL_IWDG_Refresh(&hiwdg);
			#endif
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
	err = AT42QT1244_Send_Setup(pCpnt, &Freqs[0], AT42QT__SETUP_FREQ0, 1);
	if (err)	{ return err; }

	// Compute CFO values
	for (i = 0 ; i < 2 ; i++) {
		for (j = 0 ; j < AT42QT__CALIBRATE_KEY_23 ; j++) {
			CFO[i][j] = (uint8_t) (Refs[i+1][j] - Refs[i][j]);
		}
	}

	// Write CFO values
	err = AT42QT1244_Send_Setup(pCpnt, &CFO[0][0], AT42QT__SETUP_CFO_1_0, sizeof(CFO));
	if (err)	{ return err; }

	// Enable frequency hoping mode
	return AT42QT1244_Setup_FHM(pCpnt, AT42QT__FHM_ADJ_KEYS_REF_DURING_HOP);
}


FctERR NONNULL__ AT42QT1244_Calibrate_Low_Level(AT42QT1244_t * pCpnt)
{
	if (AT42QT1244_is_Calib_Pending(pCpnt))	{ return ERROR_BUSY; }

	return AT42QT1244_Send_Command(pCpnt, AT42QT__LOW_LEVEL_CALIBRATION);
}


FctERR NONNULL__ AT42QT1244_Calibrate_All_Keys(AT42QT1244_t * pCpnt)
{
	if (AT42QT1244_is_Calib_Pending(pCpnt))	{ return ERROR_BUSY; }

	return AT42QT1244_Send_Command(pCpnt, AT42QT__CALIBRATE_ALL_KEYS);
}


FctERR NONNULL__ AT42QT1244_Calibrate_Key(AT42QT1244_t * pCpnt, uint8_t Key)
{
	if (Key > AT42QT__CALIBRATE_KEY_23)		{ return ERROR_VALUE; }
	if (AT42QT1244_is_Calib_Pending(pCpnt))	{ return ERROR_BUSY; }

	return AT42QT1244_Send_Command(pCpnt, Key);
}


/****************************************************************/


__WEAK FctERR NONNULL__ AT42QT1244_handler(AT42QT1244_t * pCpnt)
{
	return ERROR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
