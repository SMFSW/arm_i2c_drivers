/*!\file MTCH6102_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief MTCH6102 Driver extensions
** \details MTCH6102: Low-Power Projected Capacitive Touch Controller
**/
/****************************************************************/
#include "MTCH6102.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MTCH6102)
/****************************************************************/


FctERR NONNULL__ MTCH6102_Set_Mode(const MTCH6102_t * const pCpnt, const MTCH6102_MODE mode)
{
	FctERR err;

	if (mode > RawADC)	{ err = ERROR_VALUE; }
	else				{ err = MTCH6102_Write(pCpnt->cfg.slave_inst, &mode, MTCH__MODE, 1U); }

	return err;
}


FctERR NONNULL__ MTCH6102_Command(const MTCH6102_t * const pCpnt, const MTCH6102_COMMAND cmd)
{
	uMTCH_REG__CMD	MTCH_CMD = { 0 }, MEM_MTCH_CMD;
	FctERR			err;

	switch (cmd)
	{
		case MTCH_StoreToNV:
			MTCH_CMD.Bits.NV = 1U;
			break;

		case MTCH_RestoreDefaults:
			MTCH_CMD.Bits.DEF = 1U;
			break;

		case MTCH_Configure:
			MTCH_CMD.Bits.CFG = 1U;
			break;

		case MTCH_ManufacturingTest:
			MTCH_CMD.Bits.MFG = 1U;
			break;

		case MTCH_ForceBaseline:
			MTCH_CMD.Bits.BS = 1U;
			break;

		default:
			err = ERROR_VALUE;
			goto ret;
	}

	MEM_MTCH_CMD = MTCH_CMD;

	// Send command
	err = MTCH6102_Write(pCpnt->cfg.slave_inst, &MTCH_CMD.Byte, MTCH__CMD, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	// Wait for command to complete
	while ((MTCH_CMD.Byte & MEM_MTCH_CMD.Byte) != 0)
	{
		err = MTCH6102_Read(pCpnt->cfg.slave_inst, &MTCH_CMD.Byte, MTCH__CMD, 1U);
		if (err != ERROR_OK)	{ goto ret; }
		//HAL_Delay(1);
	}

	ret:
	return err;
}


FctERR NONNULL__ MTCH6102_Set_Active_Period(const MTCH6102_t * const pCpnt, const uint16_t period)
{
	const uint16_t	per = perVal2perReg(period);
	const uint8_t	DAT[2] = { LOBYTE(per), HIBYTE(per) };

	return MTCH6102_Write(pCpnt->cfg.slave_inst, DAT, MTCH__ACTIVE_PERIOD_L, sizeof(DAT));
}


FctERR NONNULL__ MTCH6102_Set_Idle_Period(const MTCH6102_t * const pCpnt, const uint16_t period)
{
	const uint16_t	per = perVal2perReg(period);
	const uint8_t	DAT[2] = { LOBYTE(per), HIBYTE(per) };

	return MTCH6102_Write(pCpnt->cfg.slave_inst, DAT, MTCH__IDLE_PERIOD_L, sizeof(DAT));
}


FctERR NONNULL__ MTCH6102_Get_Active_Period(const MTCH6102_t * const pCpnt, uint16_t * const period)
{
	uint8_t DAT[2];

	FctERR err = MTCH6102_Read(pCpnt->cfg.slave_inst, DAT, MTCH__ACTIVE_PERIOD_L, sizeof(DAT));
	if (err != ERROR_OK)	{ goto ret; }

	*period = perReg2perVal(LSHIFT(DAT[1], 8U) + DAT[0]);

	ret:
	return err;
}


FctERR NONNULL__ MTCH6102_Get_Idle_Period(const MTCH6102_t * const pCpnt, uint16_t * const period)
{
	uint8_t DAT[2];

	FctERR err = MTCH6102_Read(pCpnt->cfg.slave_inst, DAT, MTCH__IDLE_PERIOD_L, sizeof(DAT));
	if (err != ERROR_OK)	{ goto ret; }

	*period = perReg2perVal(LSHIFT(DAT[1], 8U) + DAT[0]);

	ret:
	return err;
}


FctERR NONNULL__ MTCH6102_Set_Filter(const MTCH6102_t * const pCpnt, const MTCH6102_FILTER_TYPE type, const uint8_t strength, const bool baseline_filter)
{
	uint8_t		MTCH_FILTER[2];
	FctERR		err;

	switch (type)
	{
		case Filter_None:
			MTCH_FILTER[0] = Filter_None;
			MTCH_FILTER[1] = 0;
			break;

		case Filter_Median:
			MTCH_FILTER[0] = Filter_Median;
			MTCH_FILTER[1] = ((strength >= 3U) && (strength <= 9U) && (strength & 0x01U)) ? strength : 5U;	// Default Median strength (5)
			break;

		case Filter_IIR:
			MTCH_FILTER[0] = Filter_IIR;
			MTCH_FILTER[1] = ((strength > 0) && (strength <= 3U)) ? strength : 1U;	// Default IIR strength (1)
			break;

		case Filter_Average:
			MTCH_FILTER[0] = Filter_Average;
			MTCH_FILTER[1] = ((strength > 0) && (strength <= 3U)) ? strength : 1U;	// Default Average strength (1)
			break;

		default:
			err = ERROR_VALUE;
			goto ret;
	}

	// Send configuration parameters
	err = MTCH6102_Write(pCpnt->cfg.slave_inst, &MTCH_FILTER[0], baseline_filter ? MTCH__BASE_FILTER_TYPE : MTCH__FILTER_TYPE, 2U);
	if (err != ERROR_OK)	{ goto ret; }

	// Send configuration request
	err = MTCH6102_Configuration_Request(pCpnt);

	ret:
	return err;
}


/****************************************************************/


__WEAK void NONNULL__ MTCH6102_INT_GPIO_Init(MTCH6102_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&pCpnt->cfg.INT_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK bool NONNULL__ MTCH6102_INT_GPIO_Get(const MTCH6102_t * const pCpnt) {
	return I2C_peripheral_GPIO_get(&pCpnt->cfg.INT_GPIO); }


__WEAK void NONNULL__ MTCH6102_SYNC_GPIO_Init(MTCH6102_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&pCpnt->cfg.SYNC_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK bool NONNULL__ MTCH6102_SYNC_GPIO_Get(const MTCH6102_t * const pCpnt) {
	return I2C_peripheral_GPIO_get(&pCpnt->cfg.SYNC_GPIO); }


/****************************************************************/
#endif
#endif
/****************************************************************/

