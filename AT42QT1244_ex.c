/*!\file AT42QT1244_ex.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief AT42QT1244 Driver extensions code
** \details AT42QT1244: 24-key QMatrix FMEA IEC/EN/UL60730 Touch Sensor
**/
/****************************************************************/
#include "AT42QT1244.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_AT42QT1244)
/****************************************************************/


FctERR AT42QT1244_Send_Command(AT42QT_cmd cmd)
{
	if ((cmd > AT42QT__RESET_DEVICE) && (cmd < AT42QT__LOW_LEVEL_CALIBRATION))	{ return ERROR_VALUE; }
	return AT42QT1244_Write(&cmd, AT42QT__CONTROL_COMMAND, 1);
}


FctERR AT42QT1244_Send_Setup(uint8_t * setup, uint8_t addr, uint8_t Nb)
{
	FctERR	err = AT42QT1244_Send_Command(AT42QT__WRITE_SETUPS);
	if (err)	{ return err; }

	return AT42QT1244_Write(setup, addr, Nb);
}


FctERR AT42QT1244_Setup_Key(uint8_t Key, bool use)
{
	const uint8_t				NDIL_Val = 4;		// 4 is the default NDIL value
	uAT42QT_REG__SETUP_165_188	TMP;
	FctERR						err;

	if (Key > AT42QT__CALIBRATE_KEY_23)	{ return ERROR_VALUE; }

	err = AT42QT1244_Read((uint8_t *) &TMP, AT42QT__SETUP_KEYS_MODE_0 + Key, sizeof(TMP));	// 165 is the NDIL register of the 1st key
	if (err)	{ return err; }

	TMP.Bits.NDIL = use ? NDIL_Val : 0;
	return AT42QT1244_Send_Setup((uint8_t *) &TMP, AT42QT__SETUP_KEYS_MODE_0 + Key, sizeof(TMP));
}

FctERR AT42QT1244_Setup_FHM(AT42QT_FHM FHM)
{
	uAT42QT_REG__SETUP_244	TMP;
	FctERR					err;

	if (FHM > AT42QT__FHM_FREQUENCY_SWEEP)	{ return ERROR_VALUE; }

	err = AT42QT1244_Read((uint8_t *) &TMP, AT42QT__SETUP_FREQ_HOPING_DWELL, 1);
	if (err)	{ return err; }

	TMP.Bits.FHM = FHM;
	return AT42QT1244_Send_Setup((uint8_t *) &TMP, AT42QT__SETUP_FREQ_HOPING_DWELL, 1);
}


FctERR AT42QT1244_Get_Keys(uint32_t * Keys)
{
	uint8_t TMP[3];
	FctERR	err = AT42QT1244_Read(TMP, AT42QT__DETECT_STATUS_1, sizeof(TMP));
	if (err)	{ return err; }

	*Keys = (LSHIFT(TMP[2], 16) + LSHIFT(TMP[1], 8) + TMP[0]) & 0x00FFFFFFUL;
	return ERROR_OK;
}


int AT42QT1244_is_Calib_Pending(void)
{
	uAT42QT_REG__DEVICE_STATUS ST;
	FctERR err = AT42QT1244_Get_Status(&ST);
	if (err)	{ return -1; }

	return ST.Bits.Key_In_Calib;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
