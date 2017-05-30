/*!\file TCS3400_ex.c
** \author SMFSW
** \version v0.3
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief TCS3400 Driver extensions code
** \details TCS3400: Color Light-to-Digital Converter
**/
/****************************************************************/
#include "TCS3400.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TCS3400)
/****************************************************************/


extern TCS3400_proc TCS3400;


FctERR TCS3400_Set_PON(bool en)
{
	uTCS3400_REG__ENABLE EN;
	FctERR				err;

	err = TCS3400_Read(&EN.Byte, TCS3400__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.PON = en;
	return TCS3400_Write_En(EN.Byte);
}


FctERR TCS3400_Set_AEN(bool en)
{
	uTCS3400_REG__ENABLE	EN;
	FctERR					err;

	err = TCS3400_Read(&EN.Byte, TCS3400__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.AEN = en;
	return TCS3400_Write_En(EN.Byte);
}


FctERR TCS3400_Set_AIEN(bool en)
{
	uTCS3400_REG__ENABLE	EN;
	FctERR					err;

	err = TCS3400_Read(&EN.Byte, TCS3400__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.AIEN = en;
	return TCS3400_Write_En(EN.Byte);
}


FctERR TCS3400_Set_WEN(bool en)
{
	uTCS3400_REG__ENABLE	EN;
	FctERR					err;

	err = TCS3400_Read(&EN.Byte, TCS3400__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.WEN = en;
	return TCS3400_Write_En(EN.Byte);
}


FctERR TCS3400_Set_SAI(bool en)
{
	uTCS3400_REG__ENABLE	EN;
	FctERR					err;

	err = TCS3400_Read(&EN.Byte, TCS3400__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.SAI = en;
	return TCS3400_Write_En(EN.Byte);
}


FctERR TCS3400_Set_Gain(TCS3400_gain gain)
{
	uTCS3400_REG__CONTROL	CTL;
	FctERR					err;

	if (gain > TCS3400__MAXIMUM_GAIN)	{ return ERR_VALUE; }	// Unknown gain

	err = TCS3400_Read(&CTL.Byte, TCS3400__CONTROL, 1);
	if (err)	{ return err; }

	CTL.Byte = 0;
	CTL.Bits.AGAIN = gain;
	err = TCS3400_Write(&CTL.Byte, TCS3400__CONTROL, 1);
	if (err)	{ return err; }

	TCS3400.cfg.Gain = gain;

	return err;
}


FctERR TCS3400_Set_Integration_Time(uint16_t integ)
{
	uint8_t	ATIME;
	FctERR	err;

	if ((integ < 3) || (integ > 712))	{ return ERR_RANGE; }	// Integration time out of range

	// 2.78ms (0xFF) to 712ms (0x00)
	//ATIME = (uint8_t) ((integ - 2.78f) * (0x00 - 0xFF) / (712.0f - 2.78f) + 0xFF);
	ATIME = 256 - (uint8_t) ((float) integ / 2.78f);
	err = TCS3400_Write(&ATIME, TCS3400__ATIME, 1);
	if (err)	{ return err; }

	TCS3400.cfg.Integ = integ;

	return err;
}


FctERR TCS3400_Set_Wait_Time(uint16_t wait)
{
	uTCS3400_REG__CONFIG	CFG;
	uint8_t					WAIT;
	FctERR					err;

	if ((wait < 3) || (wait > 8540))	{ return ERR_RANGE; }	// Wait time out of range

	CFG.Byte = 0x40;	// Bit Reserved1 shall be set to 1

	if (wait <= 712)
	{
		// 2.78ms (0xFF) to 712ms (0x00)
		//WAIT = (uint8_t) ((wait - 2.78f) * (0x00 - 0xFF) / (712.0f - 2.78f) + 0xFF);
		WAIT = 256 - (uint8_t) ((float) wait / 2.78f);
		CFG.Bits.WLONG = 0;
	}
	else
	{
		// 30ms (0xFF) to 8.54s (0x00)
		//WAIT = (uint8_t) ((wait - 30.0f) * (0x00 - 0xFF) / (8540.0f - 30.0f) + 0xFF);
		WAIT = 256 - (uint8_t) (wait / 33.3f);
		CFG.Bits.WLONG = 1;
	}

	err = TCS3400_Write_Cfg(CFG.Byte);
	if (err)	{ return err; }
	err = TCS3400_Write(&WAIT, TCS3400__WTIME, 1);
	if (err)	{ return err; }

	TCS3400.cfg.Wait = wait;

	return err;
}


FctERR TCS3400_Get_Channels(uint16_t buf[])
{
	uint8_t	TMP[2];
	FctERR	err;

	for (int i = 0 ; i < 4 ; i++)
	{
		err = TCS3400_Read(TMP, TCS3400__CDATAL + (2 * i), 2);
		if (err)	{ return err; }
		buf[i] = (TMP[1] * 0x100) + TMP[0];
	}

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
