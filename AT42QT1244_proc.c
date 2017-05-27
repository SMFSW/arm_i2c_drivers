/*!\file AT42QT1244_proc.c
** \author SMFSW
** \version v0.2
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief AT42QT1244 Driver procedures code
** \details AT42QT1244: 24-key QMatrix FMEA IEC/EN/UL60730 Touch Sensor
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


/****************************************************************/


FctERR AT42QT1244_Init_Sequence(void)
{
	return ERR_OK;
}


/****************************************************************/


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
	}

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
		}

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
		for (j = 0 ; j < AT42QT__CALIBRATE_KEY_23 ; j++) {
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
