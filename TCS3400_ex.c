/*!\file TCS3400_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief TCS3400 Driver extensions
** \details TCS3400: Color Light-to-Digital Converter
**/
/****************************************************************/
#include "TCS3400.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TCS3400)
/****************************************************************/


FctERR NONNULL__ TCS3400_Set_PON(TCS3400_t * const pCpnt, const bool en)
{
	uTCS3400_REG__ENABLE EN;
	FctERR				err;

	err = TCS3400_Read(pCpnt->cfg.slave_inst, &EN.Byte, TCS3400__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.PON = en;

	err = TCS3400_Write_En(pCpnt, EN.Byte);

	ret:
	return err;
}


FctERR NONNULL__ TCS3400_Set_AEN(TCS3400_t * const pCpnt, const bool en)
{
	uTCS3400_REG__ENABLE	EN;
	FctERR					err;

	err = TCS3400_Read(pCpnt->cfg.slave_inst, &EN.Byte, TCS3400__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.AEN = en;

	err = TCS3400_Write_En(pCpnt, EN.Byte);

	ret:
	return err;
}


FctERR NONNULL__ TCS3400_Set_AIEN(TCS3400_t * const pCpnt, const bool en)
{
	uTCS3400_REG__ENABLE	EN;
	FctERR					err;

	err = TCS3400_Read(pCpnt->cfg.slave_inst, &EN.Byte, TCS3400__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.AIEN = en;

	err = TCS3400_Write_En(pCpnt, EN.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.AIEN = en;

	ret:
	return err;
}


FctERR NONNULL__ TCS3400_Set_WEN(TCS3400_t * const pCpnt, const bool en)
{
	uTCS3400_REG__ENABLE	EN;
	FctERR					err;

	err = TCS3400_Read(pCpnt->cfg.slave_inst, &EN.Byte, TCS3400__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.WEN = en;

	err = TCS3400_Write_En(pCpnt, EN.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.WEN = en;

	ret:
	return err;
}


FctERR NONNULL__ TCS3400_Set_SAI(TCS3400_t * const pCpnt, const bool en)
{
	uTCS3400_REG__ENABLE	EN;
	FctERR					err;

	err = TCS3400_Read(pCpnt->cfg.slave_inst, &EN.Byte, TCS3400__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.SAI = en;

	err = TCS3400_Write_En(pCpnt, EN.Byte);

	ret:
	return err;
}


FctERR NONNULL__ TCS3400_Set_Gain(TCS3400_t * const pCpnt, const TCS3400_gain gain)
{
	uTCS3400_REG__CONTROL	CTL;
	FctERR					err = ERROR_OK;

	if (gain > TCS3400__MAXIMUM_GAIN)	{ err = ERROR_VALUE; }	// Unknown gain
	if (err != ERROR_OK)				{ goto ret; }

	err = TCS3400_Read(pCpnt->cfg.slave_inst, &CTL.Byte, TCS3400__CONTROL, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	CTL.Bits.AGAIN = gain;

	err = TCS3400_Write(pCpnt->cfg.slave_inst, &CTL.Byte, TCS3400__CONTROL, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.Gain = gain;

	ret:
	return err;
}


FctERR NONNULL__ TCS3400_Set_Integration_Time(TCS3400_t * const pCpnt, const uint16_t integ)
{
	uint8_t	ATIME;
	FctERR	err = ERROR_OK;

	if ((integ < 3U) || (integ > 712U))	{ err = ERROR_RANGE; }	// Integration time out of range
	if (err != ERROR_OK)				{ goto ret; }

	// 2.78ms (0xFF) to 712ms (0x00)
	//ATIME = (uint8_t) ((integ - 2.78f) * (0x00 - 0xFF) / (712.0f - 2.78f) + 0xFF);
	ATIME = (uint8_t) (256U - (uintCPU_t) ((float) integ / 2.78f));

	err = TCS3400_Write(pCpnt->cfg.slave_inst, &ATIME, TCS3400__ATIME, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.Integ = integ;
	pCpnt->cfg.Integ_reg = ATIME;

	ret:
	return err;
}


FctERR NONNULL__ TCS3400_Set_Wait_Time(TCS3400_t * const pCpnt, const uint16_t wait)
{
	uTCS3400_REG__CONFIG	CFG;
	uint8_t					WAIT;
	FctERR					err = ERROR_OK;

	if ((wait < 3U) || (wait > 8540U))	{ err = ERROR_RANGE; }	// Wait time out of range
	if (err != ERROR_OK)				{ goto ret; }

	CFG.Byte = 0x40U;	// Bit Reserved1 shall be set to 1

	if (wait <= 712U)
	{
		// 2.78ms (0xFF) to 712ms (0x00)
		//WAIT = (uint8_t) ((wait - 2.78f) * (0x00 - 0xFF) / (712.0f - 2.78f) + 0xFF);
		WAIT = (uint8_t) (256U - (uintCPU_t) ((float) wait / 2.78f));
		CFG.Bits.WLONG = 0;
	}
	else
	{
		// 30ms (0xFF) to 8.54s (0x00)
		//WAIT = (uint8_t) ((wait - 30.0f) * (0x00 - 0xFF) / (8540.0f - 30.0f) + 0xFF);
		WAIT = (uint8_t) (256U - (uintCPU_t) (wait / 30.0f));
		CFG.Bits.WLONG = 1U;
	}

	err = TCS3400_Write_Cfg(pCpnt, CFG.Byte);
	if (err != ERROR_OK)	{ goto ret; }
	err = TCS3400_Write(pCpnt->cfg.slave_inst, &WAIT, TCS3400__WTIME, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.Wait = wait;

	ret:
	return err;
}


FctERR NONNULL__ TCS3400_Get_Channels(TCS3400_t * const pCpnt, uint16_t buf[])
{
	uint8_t	TMP[2];
	FctERR	err;

	for (uintCPU_t i = 0 ; i < 4U ; i++)
	{
		err = TCS3400_Read(pCpnt->cfg.slave_inst, TMP, TCS3400__CDATAL + (2U * i), 2U);
		if (err != ERROR_OK)	{ goto ret; }

		buf[i] = LSHIFT(TMP[1], 8U) + TMP[0];
	}

	ret:
	return err;
}


FctERR NONNULL__ TCS3400_Get_Channel(TCS3400_t * const pCpnt, uint16_t * const val, const TCS3400_chan chan)
{
	FctERR err;

	if (chan > TCS3400__CHAN_BLUE)	{ err = ERROR_VALUE; }	// Unknown channel
	else							{ err = TCS3400_Read_Word(pCpnt->cfg.slave_inst, val, TCS3400__CDATAL + (2U * chan)); }

	return err;
}


/****************************************************************/


__WEAK void NONNULL__ TCS3400_INT_GPIO_Init(TCS3400_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&pCpnt->cfg.INT_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK bool NONNULL__ TCS3400_INT_GPIO_Get(const TCS3400_t * const pCpnt) {
	return I2C_peripheral_GPIO_get(&pCpnt->cfg.INT_GPIO); }


/****************************************************************/
#endif
#endif
/****************************************************************/
