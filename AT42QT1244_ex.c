/*!\file AT42QT1244_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
** \brief AT42QT1244 Driver extensions
** \details AT42QT1244: 24-key QMatrix FMEA IEC/EN/UL60730 Touch Sensor
**/
/****************************************************************/
#include "AT42QT1244.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_AT42QT1244)
/****************************************************************/


FctERR NONNULL__ AT42QT1244_Send_Command(AT42QT1244_t * const pCpnt, const AT42QT_cmd cmd)
{
	if ((cmd > AT42QT__RESET_DEVICE) && (cmd < AT42QT__LOW_LEVEL_CALIBRATION) && (cmd != AT42QT__WRITE_SETUPS))	{ return ERROR_VALUE; }
	return AT42QT1244_Write(pCpnt->cfg.slave_inst, (uint8_t *) &cmd, AT42QT__CONTROL_COMMAND, 1);
}


FctERR NONNULL__ AT42QT1244_Send_Setup(AT42QT1244_t * const pCpnt, uint16_t * const hcrc, const uint8_t * setup, const uint8_t addr, const uint8_t Nb)
{
	uint8_t	SETUP[111];
	FctERR	err;

	*hcrc = 0;

	if ((addr < AT42QT__SETUP_KEYS_THRESHOLD_0) || (addr > AT42QT__SETUP_NOISE))	{ return ERROR_VALUE; }
	if (Nb > AT42QT__SETUP_NOISE - AT42QT__SETUP_KEYS_THRESHOLD_0 + 1)				{ return ERROR_RANGE; }

#if defined(HAL_IWDG_MODULE_ENABLED)
	HAL_IWDG_Refresh(&hiwdg);	// Refresh watchdog (as the whole procedure may take up some time)
#endif

	err = AT42QT1244_Read(pCpnt->cfg.slave_inst, &SETUP[1], AT42QT__SETUP_KEYS_THRESHOLD_0, sizeof(SETUP) - 3);	// No need to read COMMAND & CRC registers

	if (!err)
	{
		SETUP[0] = AT42QT__WRITE_SETUPS;
		memcpy(&SETUP[addr - AT42QT__SETUP_KEYS_THRESHOLD_0] + 1, setup, Nb);

		// Compute CRC excluding COMMAND & CRC registers
		for (uintCPU_t i = 1 ; i < sizeof(SETUP) - 2 ; i++)	{ *hcrc = AT42QT1244_crc16(*hcrc, SETUP[i]); }
		SETUP[sizeof(SETUP) - 2] = LOBYTE(*hcrc);
		SETUP[sizeof(SETUP) - 1] = HIBYTE(*hcrc);

		err |= AT42QT1244_Write(pCpnt->cfg.slave_inst, SETUP, AT42QT__CONTROL_COMMAND, sizeof(SETUP));
	}

	return err;
}


FctERR NONNULL__ AT42QT1244_Setup_Keys(AT42QT1244_t * const pCpnt, uint16_t * const hcrc, const uint32_t mask_keys, const bool use)
{
	const uint8_t				NDIL_Val = 4;				// 4 is the default NDIL value
	uAT42QT_REG__SETUP_165_188	TMP[AT42QT1244_MAX_KEYS];
	FctERR						err;

	err = AT42QT1244_Read(pCpnt->cfg.slave_inst, &TMP[0].Byte, AT42QT__SETUP_KEYS_MODE_0, sizeof(TMP));	// 165 is the NDIL register of the 1st key
	if (err)	{ return err; }

	for (uintCPU_t i = 0, j = 1 ; i < AT42QT1244_MAX_KEYS ; i++, j <<= 1)
	{
		if (!(mask_keys & j))	{ continue; }	// Skip key if not in the mask

		TMP[i].Bits.NDIL = use ? NDIL_Val : 0;
	}

	return AT42QT1244_Send_Setup(pCpnt, hcrc, &TMP[0].Byte, AT42QT__SETUP_KEYS_MODE_0, sizeof(TMP));
}


FctERR NONNULL__ AT42QT1244_Setup_FHM(AT42QT1244_t * const pCpnt, uint16_t * const hcrc, const AT42QT_FHM FHM)
{
	uAT42QT_REG__SETUP_244	TMP;
	FctERR					err;

	if (FHM > AT42QT__FHM_FREQUENCY_SWEEP)	{ return ERROR_VALUE; }

	err = AT42QT1244_Read(pCpnt->cfg.slave_inst, &TMP.Byte, AT42QT__SETUP_FREQ_HOPING_DWELL, 1);
	if (err)	{ return err; }

	TMP.Bits.FHM = FHM;
	return AT42QT1244_Send_Setup(pCpnt, hcrc, &TMP.Byte, AT42QT__SETUP_FREQ_HOPING_DWELL, 1);
}


FctERR NONNULL__ AT42QT1244_Reset(AT42QT1244_t * const pCpnt)
{
	FctERR err;

	AT42QT1244_Set_Reset_Time(pCpnt);
	err = AT42QT1244_Send_Command(pCpnt, AT42QT__RESET_DEVICE);

	if (!err)	{ AT42QT1244_Delay_PowerOn(pCpnt); }

	return err;
}


FctERR NONNULL__ AT42QT1244_Get_Keys(AT42QT1244_t * const pCpnt, uint32_t * Keys)
{
	uint8_t TMP[3];
	FctERR	err = AT42QT1244_Read(pCpnt->cfg.slave_inst, TMP, AT42QT__DETECT_STATUS_1, sizeof(TMP));
	if (err)	{ return err; }

	*Keys = (LSHIFT(TMP[2], 16) + LSHIFT(TMP[1], 8) + TMP[0]) & 0x00FFFFFFUL;
	return ERROR_OK;
}


intCPU_t NONNULL__ AT42QT1244_is_Calib_Pending(AT42QT1244_t * const pCpnt)
{
	uAT42QT_REG__DEVICE_STATUS ST;
	FctERR err = AT42QT1244_Get_Status(pCpnt, &ST);
	if (err)	{ return -1; }

	return ST.Bits.Key_In_Calib;
}


/****************************************************************/


__WEAK FctERR NONNULL__ AT42QT1244_CHANGE_GPIO_Init(AT42QT1244_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	return I2C_peripheral_GPIO_init(&pCpnt->cfg.CHANGE_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK FctERR NONNULL__ AT42QT1244_CHANGE_GPIO_Get(AT42QT1244_t * const pCpnt, bool * const pState) {
	return I2C_peripheral_GPIO_get(&pCpnt->cfg.CHANGE_GPIO, pState); }


__WEAK FctERR NONNULL__ AT42QT1244_RST_GPIO_Init(AT42QT1244_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	return I2C_peripheral_GPIO_init(&pCpnt->cfg.RST_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK FctERR NONNULL__ AT42QT1244_RST_GPIO_Set(AT42QT1244_t * const pCpnt, const bool state) {
	return I2C_peripheral_GPIO_set(&pCpnt->cfg.RST_GPIO, state); }


/****************************************************************/
#endif
#endif
/****************************************************************/
