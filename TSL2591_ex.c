/*!\file TSL2591_ex.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief TSL2591 Driver extensions code
** \details TSL2591: Very-high sensitivity light-to-digital converter
**/
/****************************************************************/
#include "TSL2591.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TSL2591)
/****************************************************************/


/*!\brief TSL2591 oscillator Enable / Disable
**/
FctERR TSL2591_Set_PON(bool en)
{
	uTSL2591_REG__ENABLE	EN;
	FctERR					err;

	err = TSL2591_Read(&EN.Byte, TSL2591__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.PON = en;
	return TSL2591_Write_En(EN.Byte);
}

/*!\brief TSL2591 ALS module Enable / Disable
**/
FctERR TSL2591_Set_AEN(bool en)
{
	uTSL2591_REG__ENABLE	EN;
	FctERR					err;

	err = TSL2591_Read(&EN.Byte, TSL2591__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.AEN = en;
	return TSL2591_Write_En(EN.Byte);
}

/*!\brief TSL2591 ALS interrupt module Enable / Disable
**/
FctERR TSL2591_Set_AIEN(bool en)
{
	uTSL2591_REG__ENABLE	EN;
	FctERR					err;

	err = TSL2591_Read(&EN.Byte, TSL2591__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.AIEN = en;
	return TSL2591_Write_En(EN.Byte);
}


/*!\brief TSL2591 gain configuration
**/
FctERR TSL2591_Set_Gain(TSL2591_gain gain)
{
	uTSL2591_REG__CONFIG	CFG;
	FctERR					err;

	if (gain > TSL2591__MAXIMUM_GAIN)	{ return ERR_VALUE; }	// Unknown gain

	err = TSL2591_Read(&CFG.Byte, TSL2591__CONFIG, 1);
	if (err)	{ return err; }

	CFG.Byte = 0;
	CFG.Bits.AGAIN = gain;
	err = TSL2591_Write(&CFG.Byte, TSL2591__CONFIG, 1);
	if (err)	{ return err; }

	TSL2591.cfg.Gain = gain;
	TSL2591_Set_CPL();

	return err;
}

/*!\brief TSL2591 integration time configuration
**/
FctERR TSL2591_Set_Integration_Time(TSL2591_integ integ)
{
	uTSL2591_REG__CONFIG	CFG;
	FctERR					err;

	if (integ > TSL2591__INTEG_600MS)	{ return ERR_VALUE; }	// Unknown integration time

	err = TSL2591_Read(&CFG.Byte, TSL2591__CONFIG, 1);
	if (err)	{ return err; }

	CFG.Byte = 0;
	CFG.Bits.ATIME = integ;
	err = TSL2591_Write(&CFG.Byte, TSL2591__CONFIG, 1);
	if (err)	{ return err; }

	TSL2591.cfg.Integ = integ;
	TSL2591_Set_CPL();

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
