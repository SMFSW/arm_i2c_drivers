/*!\file AT42QT1244_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
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
	FctERR err = ERROR_OK;

	if ((cmd > AT42QT__RESET_DEVICE) && (cmd < AT42QT__LOW_LEVEL_CALIBRATION) && (cmd != AT42QT__WRITE_SETUPS))	{ err = ERROR_VALUE; }
	if (err != ERROR_OK)																						{ goto ret; }

	err = AT42QT1244_Write(pCpnt->cfg.slave_inst, (const uint8_t *) &cmd, AT42QT__CONTROL_COMMAND, 1U);

	ret:
	return err;
}


FctERR NONNULL__ AT42QT1244_Send_Setup(AT42QT1244_t * const pCpnt, uint16_t * const hcrc, const uint8_t * setup, const uint8_t addr, const uint8_t nb)
{
	uint8_t	SETUP[111];
	FctERR	err = ERROR_OK;

	*hcrc = 0;

	if ((addr < AT42QT__SETUP_KEYS_THRESHOLD_0) || (addr > AT42QT__SETUP_NOISE))	{ err = ERROR_VALUE; }
	if (nb > (AT42QT__SETUP_NOISE - AT42QT__SETUP_KEYS_THRESHOLD_0) + 1U)			{ err = ERROR_RANGE; }
	if (err != ERROR_OK)															{ goto ret; }

	I2C_Watchdog_Refresh();		// Refresh watchdog (as the whole procedure may take up some time)

	err = AT42QT1244_Read(pCpnt->cfg.slave_inst, &SETUP[1], AT42QT__SETUP_KEYS_THRESHOLD_0, sizeof(SETUP) - 3U);	// No need to read COMMAND & CRC registers
	if (err != ERROR_OK)	{ goto ret; }

	SETUP[0] = AT42QT__WRITE_SETUPS;
	UNUSED_RET memcpy(&SETUP[addr - AT42QT__SETUP_KEYS_THRESHOLD_0] + 1U, setup, nb);

	// Compute CRC excluding COMMAND & CRC registers
	for (uintCPU_t i = 1U ; i < sizeof(SETUP) - 2U ; i++)	{ *hcrc = AT42QT1244_crc16(*hcrc, SETUP[i]); }
	SETUP[sizeof(SETUP) - 2U] = LOBYTE(*hcrc);
	SETUP[sizeof(SETUP) - 1U] = HIBYTE(*hcrc);

	err |= AT42QT1244_Write(pCpnt->cfg.slave_inst, SETUP, AT42QT__CONTROL_COMMAND, sizeof(SETUP));

	ret:
	return err;
}


FctERR NONNULL__ AT42QT1244_Setup_Keys(AT42QT1244_t * const pCpnt, uint16_t * const hcrc, const uint32_t mask_keys, const bool use)
{
	const uint8_t				NDIL_Val = 4U;				// 4 is the default NDIL value
	uAT42QT_REG__SETUP_165_188	TMP[AT42QT1244_MAX_KEYS];
	FctERR						err;

	err = AT42QT1244_Read(pCpnt->cfg.slave_inst, &TMP[0].Byte, AT42QT__SETUP_KEYS_MODE_0, sizeof(TMP));	// 165 is the NDIL register of the 1st key
	if (err != ERROR_OK)	{ goto ret; }

	for (uintCPU_t i = 0, j = 1U ; i < AT42QT1244_MAX_KEYS ; i++, j <<= 1U)
	{
		if (!(mask_keys & j))	{ continue; }	// Skip key if not in the mask

		TMP[i].Bits.NDIL = use ? NDIL_Val : 0;
	}

	err = AT42QT1244_Send_Setup(pCpnt, hcrc, &TMP[0].Byte, AT42QT__SETUP_KEYS_MODE_0, sizeof(TMP));

	ret:
	return err;
}


FctERR NONNULL__ AT42QT1244_Setup_FHM(AT42QT1244_t * const pCpnt, uint16_t * const hcrc, const AT42QT_FHM FHM)
{
	uAT42QT_REG__SETUP_244	TMP;
	FctERR					err = ERROR_OK;

	if (FHM > AT42QT__FHM_FREQUENCY_SWEEP)	{ err = ERROR_VALUE; }
	if (err != ERROR_OK)					{ goto ret; }

	err = AT42QT1244_Read(pCpnt->cfg.slave_inst, &TMP.Byte, AT42QT__SETUP_FREQ_HOPING_DWELL, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	TMP.Bits.FHM = FHM;
	err = AT42QT1244_Send_Setup(pCpnt, hcrc, &TMP.Byte, AT42QT__SETUP_FREQ_HOPING_DWELL, 1U);

	ret:
	return err;
}


FctERR NONNULL__ AT42QT1244_Reset(AT42QT1244_t * const pCpnt)
{
	AT42QT1244_Set_Reset_Time(pCpnt);
	FctERR err = AT42QT1244_Send_Command(pCpnt, AT42QT__RESET_DEVICE);

	if (err == ERROR_OK)	{ AT42QT1244_Delay_PowerOn(pCpnt); }

	return err;
}


FctERR NONNULL__ AT42QT1244_Get_Keys(AT42QT1244_t * const pCpnt, uint32_t * Keys)
{
	uint8_t TMP[3];

	FctERR	err = AT42QT1244_Read(pCpnt->cfg.slave_inst, TMP, AT42QT__DETECT_STATUS_1, sizeof(TMP));
	if (err != ERROR_OK)	{ goto ret; }

	*Keys = (LSHIFT(TMP[2], 16U) + LSHIFT(TMP[1], 8U) + TMP[0]) & 0x00FFFFFFUL;

	ret:
	return err;
}


bool NONNULL__ AT42QT1244_is_Calib_Pending(AT42QT1244_t * const pCpnt)
{
	uAT42QT_REG__DEVICE_STATUS ST;
	bool calib = false;

	FctERR err = AT42QT1244_Get_Status(pCpnt, &ST);
	if (err == ERROR_OK)	{ calib = binEval(ST.Bits.Key_In_Calib); }

	return calib;
}


/****************************************************************/


__WEAK void NONNULL__ AT42QT1244_CHANGE_GPIO_Init(AT42QT1244_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&pCpnt->cfg.CHANGE_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK bool NONNULL__ AT42QT1244_CHANGE_GPIO_Get(const AT42QT1244_t * const pCpnt) {
	return I2C_peripheral_GPIO_get(&pCpnt->cfg.CHANGE_GPIO); }


__WEAK void NONNULL__ AT42QT1244_RST_GPIO_Init(AT42QT1244_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&pCpnt->cfg.RST_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK void NONNULL__ AT42QT1244_RST_GPIO_Set(AT42QT1244_t * const pCpnt, const bool state) {
	I2C_peripheral_GPIO_set(&pCpnt->cfg.RST_GPIO, state); }


/****************************************************************/
#endif
#endif
/****************************************************************/
