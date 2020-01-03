/*!\file TSL2591_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief TSL2591 Driver extensions
** \details TSL2591: Very-high sensitivity light-to-digital converter
**/
/****************************************************************/
#include "TSL2591.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TSL2591)
/****************************************************************/


FctERR NONNULL__ TSL2591_Set_PON(TSL2591_t * pCpnt, const bool en)
{
	uTSL2591_REG__ENABLE	EN;
	FctERR					err;

	err = TSL2591_Read(pCpnt->cfg.slave_inst, &EN.Byte, TSL2591__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.PON = en;
	return TSL2591_Write_En(pCpnt, EN.Byte);
}


FctERR NONNULL__ TSL2591_Set_AEN(TSL2591_t * pCpnt, const bool en)
{
	uTSL2591_REG__ENABLE	EN;
	FctERR					err;

	err = TSL2591_Read(pCpnt->cfg.slave_inst, &EN.Byte, TSL2591__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.AEN = en;
	return TSL2591_Write_En(pCpnt, EN.Byte);
}


FctERR NONNULL__ TSL2591_Set_AIEN(TSL2591_t * pCpnt, const bool en)
{
	uTSL2591_REG__ENABLE	EN;
	FctERR					err;

	err = TSL2591_Read(pCpnt->cfg.slave_inst, &EN.Byte, TSL2591__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.AIEN = en;
	err = TSL2591_Write_En(pCpnt, EN.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.AIEN = en;
	return err;
}


FctERR NONNULL__ TSL2591_Set_Gain(TSL2591_t * pCpnt, const TSL2591_gain gain)
{
	uTSL2591_REG__CONFIG	CFG;
	FctERR					err;

	if (gain > TSL2591__MAXIMUM_GAIN)	{ return ERROR_VALUE; }	// Unknown gain

	err = TSL2591_Read(pCpnt->cfg.slave_inst, &CFG.Byte, TSL2591__CONFIG, 1);
	if (err)	{ return err; }

	CFG.Bits.AGAIN = gain;
	err = TSL2591_Write(pCpnt->cfg.slave_inst, &CFG.Byte, TSL2591__CONFIG, 1);
	if (err)	{ return err; }

	pCpnt->cfg.Gain = gain;
	TSL2591_Set_CPL(pCpnt);

	return err;
}


FctERR NONNULL__ TSL2591_Set_Integration_Time(TSL2591_t * pCpnt, const TSL2591_integ integ)
{
	uTSL2591_REG__CONFIG	CFG;
	FctERR					err;

	if (integ > TSL2591__INTEG_600MS)	{ return ERROR_VALUE; }	// Unknown integration time

	err = TSL2591_Read(pCpnt->cfg.slave_inst, &CFG.Byte, TSL2591__CONFIG, 1);
	if (err)	{ return err; }

	CFG.Bits.ATIME = integ;
	err = TSL2591_Write(pCpnt->cfg.slave_inst, &CFG.Byte, TSL2591__CONFIG, 1);
	if (err)	{ return err; }

	pCpnt->cfg.Integ = integ;
	TSL2591_Set_CPL(pCpnt);

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
