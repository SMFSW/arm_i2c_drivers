/*!\file TSL2591_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief TSL2591 Driver extensions
** \details TSL2591: Very-high sensitivity light-to-digital converter
**/
/****************************************************************/
#include "TSL2591.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TSL2591)
/****************************************************************/


FctERR NONNULL__ TSL2591_Set_PON(TSL2591_t * const pCpnt, const bool en)
{
	uTSL2591_REG__ENABLE	EN;
	FctERR					err;

	err = TSL2591_Read(pCpnt->cfg.slave_inst, &EN.Byte, TSL2591__ENABLE, 1U);
	if (err != ERROR_OK)	{ return err; }

	EN.Bits.PON = en;
	return TSL2591_Write_En(pCpnt, EN.Byte);
}


FctERR NONNULL__ TSL2591_Set_AEN(TSL2591_t * const pCpnt, const bool en)
{
	uTSL2591_REG__ENABLE	EN;
	FctERR					err;

	err = TSL2591_Read(pCpnt->cfg.slave_inst, &EN.Byte, TSL2591__ENABLE, 1U);
	if (err != ERROR_OK)	{ return err; }

	EN.Bits.AEN = en;
	return TSL2591_Write_En(pCpnt, EN.Byte);
}


FctERR NONNULL__ TSL2591_Set_AIEN(TSL2591_t * const pCpnt, const bool en)
{
	uTSL2591_REG__ENABLE	EN;
	FctERR					err;

	err = TSL2591_Read(pCpnt->cfg.slave_inst, &EN.Byte, TSL2591__ENABLE, 1U);
	if (err != ERROR_OK)	{ return err; }

	EN.Bits.AIEN = en;
	err = TSL2591_Write_En(pCpnt, EN.Byte);
	if (err != ERROR_OK)	{ return err; }

	pCpnt->cfg.AIEN = en;
	return err;
}


FctERR NONNULL__ TSL2591_Set_Gain(TSL2591_t * const pCpnt, const TSL2591_gain gain)
{
	uTSL2591_REG__CONFIG	CFG;
	FctERR					err;

	if (gain > TSL2591__MAXIMUM_GAIN)	{ return ERROR_VALUE; }	// Unknown gain

	err = TSL2591_Read(pCpnt->cfg.slave_inst, &CFG.Byte, TSL2591__CONFIG, 1U);
	if (err != ERROR_OK)	{ return err; }

	CFG.Bits.AGAIN = gain;
	err = TSL2591_Write(pCpnt->cfg.slave_inst, &CFG.Byte, TSL2591__CONFIG, 1U);
	if (err != ERROR_OK)	{ return err; }

	pCpnt->cfg.Gain = gain;
	TSL2591_Set_CPL(pCpnt);

	return err;
}


FctERR NONNULL__ TSL2591_Set_Integration_Time(TSL2591_t * const pCpnt, const TSL2591_integ integ)
{
	uTSL2591_REG__CONFIG	CFG;
	FctERR					err;

	if (integ > TSL2591__INTEG_600MS)	{ return ERROR_VALUE; }	// Unknown integration time

	err = TSL2591_Read(pCpnt->cfg.slave_inst, &CFG.Byte, TSL2591__CONFIG, 1U);
	if (err != ERROR_OK)	{ return err; }

	CFG.Bits.ATIME = integ;
	err = TSL2591_Write(pCpnt->cfg.slave_inst, &CFG.Byte, TSL2591__CONFIG, 1U);
	if (err != ERROR_OK)	{ return err; }

	pCpnt->cfg.Integ = integ;
	TSL2591_Set_CPL(pCpnt);

	return err;
}


/****************************************************************/


__WEAK void NONNULL__ TSL2591_INT_GPIO_Init(TSL2591_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&pCpnt->cfg.INT_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK bool NONNULL__ TSL2591_INT_GPIO_Get(TSL2591_t * const pCpnt) {
	return I2C_peripheral_GPIO_get(&pCpnt->cfg.INT_GPIO); }


/****************************************************************/
#endif
#endif
/****************************************************************/
