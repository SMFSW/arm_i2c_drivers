/*!\file TCS3472_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief TCS3472 Driver extensions
** \details TCS3472: Color light-to-digital converter with IR filter
**/
/****************************************************************/
#include "TCS3472.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TCS3472)
/****************************************************************/


FctERR NONNULL__ TCS3472_Set_PON(TCS3472_t * const pCpnt, const bool en)
{
	uTCS3472_REG__ENABLE EN;
	FctERR				err;

	err = TCS3472_Read(pCpnt->cfg.slave_inst, &EN.Byte, TCS3472__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.PON = en;

	err = TCS3472_Write_En(pCpnt, EN.Byte);

	ret:
	return err;
}


FctERR NONNULL__ TCS3472_Set_AEN(TCS3472_t * const pCpnt, const bool en)
{
	uTCS3472_REG__ENABLE	EN;
	FctERR					err;

	err = TCS3472_Read(pCpnt->cfg.slave_inst, &EN.Byte, TCS3472__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.AEN = en;

	err = TCS3472_Write_En(pCpnt, EN.Byte);

	ret:
	return err;
}


FctERR NONNULL__ TCS3472_Set_AIEN(TCS3472_t * const pCpnt, const bool en)
{
	uTCS3472_REG__ENABLE	EN;
	FctERR					err;

	err = TCS3472_Read(pCpnt->cfg.slave_inst, &EN.Byte, TCS3472__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.AIEN = en;

	err = TCS3472_Write_En(pCpnt, EN.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.AIEN = en;

	ret:
	return err;
}


FctERR NONNULL__ TCS3472_Set_WEN(TCS3472_t * const pCpnt, const bool en)
{
	uTCS3472_REG__ENABLE	EN;
	FctERR					err;

	err = TCS3472_Read(pCpnt->cfg.slave_inst, &EN.Byte, TCS3472__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.WEN = en;

	err = TCS3472_Write_En(pCpnt, EN.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.WEN = en;

	ret:
	return err;
}


FctERR NONNULL__ TCS3472_Set_Gain(TCS3472_t * const pCpnt, const TCS3472_gain gain)
{
	uTCS3472_REG__CONTROL	CTL;
	FctERR					err = ERROR_OK;

	if (gain > TCS3472__MAXIMUM_GAIN)	{ err = ERROR_VALUE; }	// Unknown gain
	if (err != ERROR_OK)				{ goto ret; }

	err = TCS3472_Read(pCpnt->cfg.slave_inst, &CTL.Byte, TCS3472__CONTROL, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	CTL.Bits.AGAIN = gain;

	err = TCS3472_Write(pCpnt->cfg.slave_inst, &CTL.Byte, TCS3472__CONTROL, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.Gain = gain;

	ret:
	return err;
}


FctERR NONNULL__ TCS3472_Set_Integration_Time(TCS3472_t * const pCpnt, const uint16_t integ)
{
	uint8_t	ATIME;
	FctERR	err = ERROR_OK;

	if ((integ < 3U) || (integ > 614U))	{ err = ERROR_RANGE; }	// Integration time out of range
	if (err != ERROR_OK)				{ goto ret; }

	// 2.4ms (0xFF) to 700ms (0x00)
	//ATIME = (uint8_t) ((integ - 2.4f) * (0x00 - 0xFF) / (614.0f - 2.4f) + 0xFF);
	ATIME = (uint8_t) (256U - (uintCPU_t) ((float) integ / 2.4f));

	err = TCS3472_Write(pCpnt->cfg.slave_inst, &ATIME, TCS3472__ATIME, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.Integ = integ;
	pCpnt->cfg.Integ_reg = ATIME;

	ret:
	return err;
}


FctERR NONNULL__ TCS3472_Set_Wait_Time(TCS3472_t * const pCpnt, const uint16_t wait)
{
	uTCS3472_REG__CONFIG	CFG = { 0 };
	uint8_t					WAIT;
	FctERR					err = ERROR_OK;

	if ((wait < 3U) || (wait > 7400U))	{ err = ERROR_RANGE; }	// Wait time out of range
	if (err != ERROR_OK)				{ goto ret; }

	if (wait <= 614U)
	{
		// 2.4ms (0xFF) to 614ms (0x00)
		//WAIT = (uint8_t) ((wait - 2.4f) * (0x00 - 0xFF) / (614.0f - 2.4f) + 0xFF);
		WAIT = (uint8_t) (256U - (uintCPU_t) ((float) wait / 2.4f));
		CFG.Bits.WLONG = 0;
	}
	else
	{
		// 29ms (0xFF) to 7.4s (0x00)
		//WAIT = (uint8_t) ((wait - 29.0f) * (0x00 - 0xFF) / (7400.0f - 29.0f) + 0xFF);
		WAIT = (uint8_t) (256U - (uintCPU_t) ((float) wait / 29.0f));
		CFG.Bits.WLONG = 1U;
	}

	err = TCS3472_Write_Cfg(pCpnt, CFG.Byte);
	if (err != ERROR_OK)	{ goto ret; }
	err = TCS3472_Write(pCpnt->cfg.slave_inst, &WAIT, TCS3472__WTIME, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.Wait = wait;

	ret:
	return err;
}


FctERR NONNULL__ TCS3472_Get_Channels(TCS3472_t * const pCpnt, uint16_t buf[])
{
	uint8_t	TMP[2];
	FctERR	err;

	for (uintCPU_t i = 0 ; i < 4U ; i++)
	{
		err = TCS3472_Read(pCpnt->cfg.slave_inst, TMP, TCS3472__CDATAL + (2U * i), 2U);
		if (err != ERROR_OK)	{ goto ret; }

		buf[i] = MAKEWORD(TMP[0], TMP[1]);
	}

	ret:
	return err;
}


FctERR NONNULL__ TCS3472_Get_Channel(TCS3472_t * const pCpnt, uint16_t * const val, const TCS3472_chan chan)
{
	FctERR err;

	if (chan > TCS3472__CHAN_BLUE)	{ err = ERROR_VALUE; }	// Unknown channel
	else							{ err = TCS3472_Read_Word(pCpnt->cfg.slave_inst, val, TCS3472__CDATAL + (2U * chan)); }

	return err;
}


/****************************************************************/


__WEAK void NONNULL__ TCS3472_INT_GPIO_Init(TCS3472_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&pCpnt->cfg.INT_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK bool NONNULL__ TCS3472_INT_GPIO_Get(const TCS3472_t * const pCpnt) {
	return I2C_peripheral_GPIO_get(&pCpnt->cfg.INT_GPIO); }


/****************************************************************/
#endif
#endif
/****************************************************************/
