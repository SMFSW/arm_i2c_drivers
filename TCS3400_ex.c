/*!\file TCS3400_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief TCS3400 Driver extensions
** \details TCS3400: Color Light-to-Digital Converter
**/
/****************************************************************/
#include "TCS3400.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TCS3400)
/****************************************************************/


FctERR NONNULL__ TCS3400_Set_PON(TCS3400_t * pCpnt, const bool en)
{
	uTCS3400_REG__ENABLE EN;
	FctERR				err;

	err = TCS3400_Read(pCpnt->cfg.slave_inst, &EN.Byte, TCS3400__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.PON = en;
	return TCS3400_Write_En(pCpnt, EN.Byte);
}


FctERR NONNULL__ TCS3400_Set_AEN(TCS3400_t * pCpnt, const bool en)
{
	uTCS3400_REG__ENABLE	EN;
	FctERR					err;

	err = TCS3400_Read(pCpnt->cfg.slave_inst, &EN.Byte, TCS3400__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.AEN = en;
	return TCS3400_Write_En(pCpnt, EN.Byte);
}


FctERR NONNULL__ TCS3400_Set_AIEN(TCS3400_t * pCpnt, const bool en)
{
	uTCS3400_REG__ENABLE	EN;
	FctERR					err;

	err = TCS3400_Read(pCpnt->cfg.slave_inst, &EN.Byte, TCS3400__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.AIEN = en;
	err = TCS3400_Write_En(pCpnt, EN.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.AIEN = en;
	return err;
}


FctERR NONNULL__ TCS3400_Set_WEN(TCS3400_t * pCpnt, const bool en)
{
	uTCS3400_REG__ENABLE	EN;
	FctERR					err;

	err = TCS3400_Read(pCpnt->cfg.slave_inst, &EN.Byte, TCS3400__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.WEN = en;
	err = TCS3400_Write_En(pCpnt, EN.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.WEN = en;
	return err;
}


FctERR NONNULL__ TCS3400_Set_SAI(TCS3400_t * pCpnt, const bool en)
{
	uTCS3400_REG__ENABLE	EN;
	FctERR					err;

	err = TCS3400_Read(pCpnt->cfg.slave_inst, &EN.Byte, TCS3400__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.SAI = en;
	return TCS3400_Write_En(pCpnt, EN.Byte);
}


FctERR NONNULL__ TCS3400_Set_Gain(TCS3400_t * pCpnt, const TCS3400_gain gain)
{
	uTCS3400_REG__CONTROL	CTL;
	FctERR					err;

	if (gain > TCS3400__MAXIMUM_GAIN)	{ return ERROR_VALUE; }	// Unknown gain

	err = TCS3400_Read(pCpnt->cfg.slave_inst, &CTL.Byte, TCS3400__CONTROL, 1);
	if (err)	{ return err; }

	CTL.Bits.AGAIN = gain;
	err = TCS3400_Write(pCpnt->cfg.slave_inst, &CTL.Byte, TCS3400__CONTROL, 1);
	if (err)	{ return err; }

	pCpnt->cfg.Gain = gain;

	return err;
}


FctERR NONNULL__ TCS3400_Set_Integration_Time(TCS3400_t * pCpnt, const uint16_t integ)
{
	uint8_t	ATIME;
	FctERR	err;

	if ((integ < 3) || (integ > 712))	{ return ERROR_RANGE; }	// Integration time out of range

	// 2.78ms (0xFF) to 712ms (0x00)
	//ATIME = (uint8_t) ((integ - 2.78f) * (0x00 - 0xFF) / (712.0f - 2.78f) + 0xFF);
	ATIME = 256 - (uint8_t) ((float) integ / 2.78f);
	err = TCS3400_Write(pCpnt->cfg.slave_inst, &ATIME, TCS3400__ATIME, 1);
	if (err)	{ return err; }

	pCpnt->cfg.Integ = integ;

	return err;
}


FctERR NONNULL__ TCS3400_Set_Wait_Time(TCS3400_t * pCpnt, const uint16_t wait)
{
	uTCS3400_REG__CONFIG	CFG;
	uint8_t					WAIT;
	FctERR					err;

	if ((wait < 3) || (wait > 8540))	{ return ERROR_RANGE; }	// Wait time out of range

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
		WAIT = 256 - (uint8_t) (wait / 30.0f);
		CFG.Bits.WLONG = 1;
	}

	err = TCS3400_Write_Cfg(pCpnt, CFG.Byte);
	if (err)	{ return err; }
	err = TCS3400_Write(pCpnt->cfg.slave_inst, &WAIT, TCS3400__WTIME, 1);
	if (err)	{ return err; }

	pCpnt->cfg.Wait = wait;

	return err;
}


FctERR NONNULL__ TCS3400_Get_Channels(TCS3400_t * pCpnt, uint16_t buf[])
{
	uint8_t	TMP[2];
	FctERR	err;

	for (int i = 0 ; i < 4 ; i++)
	{
		err = TCS3400_Read(pCpnt->cfg.slave_inst, TMP, TCS3400__CDATAL + (2 * i), 2);
		if (err)	{ return err; }
		buf[i] = LSHIFT(TMP[1], 8) + TMP[0];
	}

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
