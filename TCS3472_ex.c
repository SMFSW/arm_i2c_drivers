/*!\file TCS3472_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
** \brief TCS3472 Driver extensions
** \details TCS3472: Color light-to-digital converter with IR filter
**/
/****************************************************************/
#include "TCS3472.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TCS3472)
/****************************************************************/


FctERR NONNULL__ TCS3472_Set_PON(TCS3472_t * pCpnt, const bool en)
{
	uTCS3472_REG__ENABLE EN;
	FctERR				err;

	err = TCS3472_Read(pCpnt->cfg.slave_inst, &EN.Byte, TCS3472__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.PON = en;
	return TCS3472_Write_En(pCpnt, EN.Byte);
}


FctERR NONNULL__ TCS3472_Set_AEN(TCS3472_t * pCpnt, const bool en)
{
	uTCS3472_REG__ENABLE	EN;
	FctERR					err;

	err = TCS3472_Read(pCpnt->cfg.slave_inst, &EN.Byte, TCS3472__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.AEN = en;
	return TCS3472_Write_En(pCpnt, EN.Byte);
}


FctERR NONNULL__ TCS3472_Set_AIEN(TCS3472_t * pCpnt, const bool en)
{
	uTCS3472_REG__ENABLE	EN;
	FctERR					err;

	err = TCS3472_Read(pCpnt->cfg.slave_inst, &EN.Byte, TCS3472__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.AIEN = en;
	err = TCS3472_Write_En(pCpnt, EN.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.AIEN = en;
	return err;
}


FctERR NONNULL__ TCS3472_Set_WEN(TCS3472_t * pCpnt, const bool en)
{
	uTCS3472_REG__ENABLE	EN;
	FctERR					err;

	err = TCS3472_Read(pCpnt->cfg.slave_inst, &EN.Byte, TCS3472__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.WEN = en;
	err = TCS3472_Write_En(pCpnt, EN.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.WEN = en;
	return err;
}


FctERR NONNULL__ TCS3472_Set_Gain(TCS3472_t * pCpnt, const TCS3472_gain gain)
{
	uTCS3472_REG__CONTROL	CTL;
	FctERR					err;

	if (gain > TCS3472__MAXIMUM_GAIN)	{ return ERROR_VALUE; }	// Unknown gain

	err = TCS3472_Read(pCpnt->cfg.slave_inst, &CTL.Byte, TCS3472__CONTROL, 1);
	if (err)	{ return err; }

	CTL.Bits.AGAIN = gain;
	err = TCS3472_Write(pCpnt->cfg.slave_inst, &CTL.Byte, TCS3472__CONTROL, 1);
	if (err)	{ return err; }

	pCpnt->cfg.Gain = gain;

	return err;
}


FctERR NONNULL__ TCS3472_Set_Integration_Time(TCS3472_t * pCpnt, const uint16_t integ)
{
	uint8_t	ATIME;
	FctERR	err;

	if ((integ < 3) || (integ > 614))	{ return ERROR_RANGE; }	// Integration time out of range

	// 2.4ms (0xFF) to 700ms (0x00)
	//ATIME = (uint8_t) ((integ - 2.4f) * (0x00 - 0xFF) / (614.0f - 2.4f) + 0xFF);
	ATIME = 256 - (uint8_t) ((float) integ / 2.4f);
	err = TCS3472_Write(pCpnt->cfg.slave_inst, &ATIME, TCS3472__ATIME, 1);
	if (err)	{ return err; }

	pCpnt->cfg.Integ = integ;
	pCpnt->cfg.Integ_reg = ATIME;

	return err;
}


FctERR NONNULL__ TCS3472_Set_Wait_Time(TCS3472_t * pCpnt, const uint16_t wait)
{
	uTCS3472_REG__CONFIG	CFG = { 0 };
	uint8_t					WAIT;
	FctERR					err;

	if ((wait < 3) || (wait > 7400))	{ return ERROR_RANGE; }	// Wait time out of range

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
		WAIT = 256 - (uint8_t) (wait / 29.0f);
		CFG.Bits.WLONG = 1;
	}

	err = TCS3472_Write_Cfg(pCpnt, CFG.Byte);
	if (err)	{ return err; }
	err = TCS3472_Write(pCpnt->cfg.slave_inst, &WAIT, TCS3472__WTIME, 1);
	if (err)	{ return err; }

	pCpnt->cfg.Wait = wait;

	return err;
}


FctERR NONNULL__ TCS3472_Get_Channels(TCS3472_t * pCpnt, uint16_t buf[])
{
	uint8_t	TMP[2];
	FctERR	err;

	for (int i = 0 ; i < 4 ; i++)
	{
		err = TCS3472_Read(pCpnt->cfg.slave_inst, TMP, TCS3472__CDATAL + (2 * i), 2);
		if (err)	{ return err; }
		buf[i] = MAKEWORD(TMP[0], TMP[1]);
	}

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
