/*!\file TCS3472_ex.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief TCS3472 Driver extensions code
** \details TCS3472: Color light-to-digital converter with IR filter
**/
/****************************************************************/
#include "TCS3472.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TCS3472)
/****************************************************************/


FctERR TCS3472_Set_PON(bool en)
{
	uTCS3472_REG__ENABLE EN;
	FctERR				err;

	err = TCS3472_Read(&EN.Byte, TCS3472__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.PON = en;
	return TCS3472_Write_En(EN.Byte);
}


FctERR TCS3472_Set_AEN(bool en)
{
	uTCS3472_REG__ENABLE	EN;
	FctERR					err;

	err = TCS3472_Read(&EN.Byte, TCS3472__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.AEN = en;
	return TCS3472_Write_En(EN.Byte);
}


FctERR TCS3472_Set_AIEN(bool en)
{
	uTCS3472_REG__ENABLE	EN;
	FctERR					err;

	err = TCS3472_Read(&EN.Byte, TCS3472__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.AIEN = en;
	return TCS3472_Write_En(EN.Byte);
}


FctERR TCS3472_Set_WEN(bool en)
{
	uTCS3472_REG__ENABLE	EN;
	FctERR					err;

	err = TCS3472_Read(&EN.Byte, TCS3472__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.WEN = en;
	return TCS3472_Write_En(EN.Byte);
}


FctERR TCS3472_Set_Gain(TCS3472_gain gain)
{
	uTCS3472_REG__CONTROL	CTL;
	FctERR					err;

	if (gain > TCS3472__MAXIMUM_GAIN)	{ return ERR_VALUE; }	// Unknown gain

	err = TCS3472_Read(&CTL.Byte, TCS3472__CONTROL, 1);
	if (err)	{ return err; }

	CTL.Byte = 0;
	CTL.Bits.AGAIN = gain;
	err = TCS3472_Write(&CTL.Byte, TCS3472__CONTROL, 1);
	if (err)	{ return err; }

	TCS3472.cfg.Gain = gain;

	return err;
}


FctERR TCS3472_Set_Integration_Time(uint16_t integ)
{
	uint8_t	ATIME;
	FctERR	err;

	if ((integ < 3) || (integ > 615))	{ return ERR_RANGE; }	// Integration time out of range

	// 2.4ms (0xFF) to 700ms (0x00)
	//ATIME = (uint8_t) ((integ - 2.4f) * (0x00 - 0xFF) / (700.0f - 2.4f) + 0xFF);
	ATIME = 256 - (uint8_t) ((float) integ / 2.4f);
	err = TCS3472_Write(&ATIME, TCS3472__ATIME, 1);
	if (err)	{ return err; }

	TCS3472.cfg.Integ = integ;

	return err;
}


FctERR TCS3472_Set_Wait_Time(uint16_t wait)
{
	uTCS3472_REG__CONFIG	CFG;
	uint8_t					WAIT;
	FctERR					err;

	if ((wait < 3) || (wait > 7370))	{ return ERR_RANGE; }	// Wait time out of range

	CFG.Byte = 0;

	if (wait <= 614)
	{
		// 2.4ms (0xFF) to 614ms (0x00)
		//WAIT = (uint8_t) ((wait - 2.4f) * (0x00 - 0xFF) / (614.0f - 2.4f) + 0xFF);
		WAIT = 256 - (uint8_t) ((float) wait / 2.4f);
		CFG.Bits.WLONG = 0;
	}
	else
	{
		// 29ms (0xFF) to 7.4s (0x00)
		//WAIT = (uint8_t) ((wait - 29.0f) * (0x00 - 0xFF) / (7400.0f - 29.0f) + 0xFF);
		WAIT = 256 - (uint8_t) (wait / 28.7f);
		CFG.Bits.WLONG = 1;
	}

	err = TCS3472_Write_Cfg(CFG.Byte);
	if (err)	{ return err; }
	err = TCS3472_Write(&WAIT, TCS3472__WTIME, 1);
	if (err)	{ return err; }

	TCS3472.cfg.Wait = wait;

	return err;
}


FctERR TCS3472_Get_Channels(uint16_t buf[])
{
	uint8_t	TMP[2];
	FctERR	err;

	for (int i = 0 ; i < 4 ; i++)
	{
		err = TCS3472_Read(TMP, TCS3472__CDATAL + (2 * i), 2);
		if (err)	{ return err; }
		buf[i] = (TMP[1] * 0x100) + TMP[0];
	}

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
