/*!\file PCA9952_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief PCA9952 Driver extensions
** \details PCA9952: 16-channel Fm+ I2C-bus 57 mA constant current LED driver
**/
/****************************************************************/
#include "PCA9952.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9952)
/****************************************************************/
// std libs
#include <stdlib.h>
#include <math.h>
/****************************************************************/


FctERR NONNULL__ PCA9952_Set_Auto_Increment(PCA9952_t * const pCpnt, const PCA962x_reg_inc inc)
{
	FctERR err;

	if (RSHIFT(inc, 5U) > RSHIFT(PCA9xxx__AUTO_INC_BRIGHT_GLOBAL, 5U))	{ err = ERROR_VALUE; }
	else
	{
		uPCA9952_REG__MODE1 MODE1;

		pCpnt->cfg.auto_inc = inc & PCA9xxx__AUTO_INC_BRIGHT_GLOBAL;		// Mask inc just in case

		err = PCA9952_Read(pCpnt->cfg.slave_inst, &MODE1.Byte, PCA9952__MODE1, sizeof(MODE1));
		if (err != ERROR_OK)	{ goto ret; }

		MODE1.Byte = (MODE1.Byte & ~0xE0) | pCpnt->cfg.auto_inc;
		err = PCA9952_Write(pCpnt->cfg.slave_inst, &MODE1.Byte, PCA9952__MODE1, sizeof(MODE1));
	}

	ret:
	return err;
}


FctERR NONNULL__ PCA9952_Set_Latch(PCA9952_t * const pCpnt, const PCA96xx_latch latch)
{
	FctERR err;

	if (latch > PCA9xxx__LATCH_ON_ACK)	{ err = ERROR_VALUE; }	// Unknown latch mode
	else
	{
		uPCA9952_REG__MODE2 MODE2;

		err = PCA9952_Read(pCpnt->cfg.slave_inst, &MODE2.Byte, PCA9952__MODE2, sizeof(MODE2));
		if (err != ERROR_OK)	{ goto ret; }

		MODE2.Bits.OCH = latch;
		err = PCA9952_Write(pCpnt->cfg.slave_inst, &MODE2.Byte, PCA9952__MODE2, sizeof(MODE2));
	}

	ret:
	return err;
}


FctERR NONNULL__ PCA9952_Set_Mode_LED(PCA9952_t * const pCpnt, const PCA9xxx_chan chan, const PCA995x_ledout mode)
{
	FctERR err = ERROR_OK;

	if (chan > PCA9xxx__PWM16)			{ err = ERROR_RANGE; }	// Unknown channel
	if (mode > PCA9xxx__GROUP_BRIGHT)	{ err = ERROR_VALUE; }	// Unknown control mode
	if (err != ERROR_OK)				{ goto ret; }

	const uintCPU_t offset = chan / 4U;
	const uintCPU_t shift = chan * 2U;

	const uint16_t mask = LSHIFT(3U, shift);
	const uint16_t val = LSHIFT(mode, shift);

	SET_BITS_VAL(pCpnt->LDR.DWord, mask, val);

	const uint8_t LDR = RSHIFT(pCpnt->LDR.DWord, offset * 8U);
	err = PCA9952_Write(pCpnt->cfg.slave_inst, &LDR, PCA9952__LEDOUT0 + offset, sizeof(LDR));

	ret:
	return err;
}


FctERR NONNULL__ PCA9952_Set_Mode_LEDs(PCA9952_t * const pCpnt, const uint16_t chans, const PCA995x_ledout mode)
{
	FctERR err = ERROR_OK;

	if (mode > PCA9xxx__GROUP_BRIGHT)		{ err = ERROR_VALUE; }	// Unknown control mode
	if ((chans == 0) || (err != ERROR_OK))	{ goto ret; }

	uint32_t	mask = 0, val = 0;
	uint8_t		LDR[4];

	for (PCA9xxx_chan chan = PCA9xxx__PWM1 ; chan <= PCA9xxx__PWM16 ; chan++)
	{
		if ((LSHIFT(1U, chan) & chans) != 0)
		{
			const uintCPU_t shift = chan * 2U;
			mask |= LSHIFT(0x3, shift);
			val |= LSHIFT(mode, shift);
		}
	}

	SET_BITS_VAL(pCpnt->LDR.DWord, mask, val);

	for (uintCPU_t i = 0 ; i < sizeof(LDR) ; i++)	{ LDR[i] = RSHIFT(pCpnt->LDR.DWord, i * 8U); }

	err = PCA9952_Write(pCpnt->cfg.slave_inst, LDR, PCA9952__LEDOUT0, sizeof(LDR));

	ret:
	return err;
}


FctERR NONNULL__ PCA9952_Set_IREF(PCA9952_t * const pCpnt, const PCA9xxx_chan chan, const uint8_t iref)
{
	FctERR err;

	if ((chan > PCA9xxx__PWM16) && (chan != PCA9xxx__ALL))	{ err = ERROR_RANGE; }	// Unknown channel
	else
	{
		const uint8_t reg = (chan == PCA9xxx__ALL) ? PCA9952__IREFALL : PCA9952__IREF0 + chan;
		err = PCA9952_Write(pCpnt->cfg.slave_inst, &iref, reg, sizeof(iref));
	}

	return err;
}


FctERR NONNULL__ PCA9952_Set_IREFs(PCA9952_t * const pCpnt, const uint16_t chans, const uint8_t iref)
{
	FctERR err = ERROR_OK;

	for (PCA9xxx_chan chan = PCA9xxx__PWM1 ; chan <= PCA9xxx__PWM16 ; chan++)
	{
		if ((LSHIFT(1U, chan) & chans) != 0)
		{
			err |= PCA9952_Write(pCpnt->cfg.slave_inst, &iref, PCA9952__IREF0 + chan, sizeof(iref));
		}
	}

	return err;
}


FctERR NONNULL__ PCA9952_Set_Offset(PCA9952_t * const pCpnt, const PCA9952_offset offset)
{
	FctERR err;

	if (offset > PCA9952__OFFSET_11_CLOCK_CYCLE)	{ err = ERROR_VALUE; }	// Unknown delay value
	else											{ err = PCA9952_Write(pCpnt->cfg.slave_inst, &offset, PCA9952__OFFSET, sizeof(offset)); }

	return err;
}


FctERR NONNULL__ PCA9952_ReadVals(PCA9952_t * const pCpnt, uint8_t pDuty[], const bool indexed, const PCA9xxx_chan start, const PCA9xxx_chan end)
{
	FctERR err;

	if ((start > PCA9xxx__PWM16) || (end > PCA9xxx__PWM16))	{ err = ERROR_RANGE; }	// Unknown channel
	else
	{
		uint8_t * const pArray = pDuty + (indexed ? start : 0);
		err = PCA9952_Read(pCpnt->cfg.slave_inst, pArray, PCA9952__PWM0 + start, end - start + 1U);
	}

	return err;
}


FctERR NONNULL__ PCA9952_ReadVal(PCA9952_t * const pCpnt, const PCA9xxx_chan chan, uint8_t * const pDuty)
{
	FctERR err;

	if ((chan > PCA9xxx__PWM16) && (chan != PCA9xxx__ALL))	{ err = ERROR_RANGE; }	// Unknown channel
	else
	{
		const uint8_t reg = (chan == PCA9xxx__ALL) ? PCA9952__PWMALL : PCA9952__PWM0 + chan;
		err = PCA9952_Read(pCpnt->cfg.slave_inst, pDuty, reg, sizeof(uint8_t));
	}

	return err;
}


FctERR NONNULL__ PCA9952_PutVals(PCA9952_t * const pCpnt, const uint8_t pDuty[], const bool indexed, const PCA9xxx_chan start, const PCA9xxx_chan end)
{
	FctERR err;

	if ((start > PCA9xxx__PWM16) || (end > PCA9xxx__PWM16))	{ err = ERROR_RANGE; }	// Unknown channel
	else
	{
		const uint8_t * const pArray = pDuty + (indexed ? start : 0);
		err = PCA9952_Write(pCpnt->cfg.slave_inst, pArray, PCA9952__PWM0 + start, end - start + 1U);
	}

	return err;
}


FctERR NONNULL__ PCA9952_PutVal(PCA9952_t * const pCpnt, const PCA9xxx_chan chan, const uint8_t duty)
{
	FctERR err;

	if ((chan > PCA9xxx__PWM16) && (chan != PCA9xxx__ALL))	{ err = ERROR_RANGE; }	// Unknown channel
	else
	{
		const uint8_t reg = (chan == PCA9xxx__ALL) ? PCA9952__PWMALL : PCA9952__PWM0 + chan;
		err = PCA9952_Write(pCpnt->cfg.slave_inst, &duty, reg, sizeof(duty));
	}

	return err;
}


FctERR NONNULL__ PCA9952_SetVal(PCA9952_t * const pCpnt, const PCA9xxx_chan chan)
{
	FctERR err;

	if (chan > PCA9xxx__PWM16)	{ err = ERROR_RANGE; }	// Unknown channel
	else
	{
		const uint8_t VAL = (uint8_t) -1;
		err = PCA9952_Write(pCpnt->cfg.slave_inst, &VAL, PCA9952__PWM0 + chan, sizeof(VAL));
	}

	return err;
}


FctERR NONNULL__ PCA9952_ClrVal(PCA9952_t * const pCpnt, const PCA9xxx_chan chan)
{
	FctERR err;

	if (chan > PCA9xxx__PWM16)	{ err = ERROR_RANGE; }	// Unknown channel
	else
	{
		const uint8_t VAL = 0;
		err = PCA9952_Write(pCpnt->cfg.slave_inst, &VAL, PCA9952__PWM0 + chan, sizeof(VAL));
	}

	return err;
}


FctERR NONNULL__ PCA9952_ReadEFLAGs(PCA9952_t * const pCpnt, uPCA9952_REG__EFLAG * const eflags)
{
	uint8_t EFLAG[2];

	FctERR err = PCA9952_Read(pCpnt->cfg.slave_inst, EFLAG, PCA9952__EFLAG0, sizeof(EFLAG));
	if (err != ERROR_OK)	{ goto ret; }

	eflags->Word = 0;
	for (uintCPU_t i = 0 ; i < sizeof(EFLAG) ; i++)	{ eflags->Word |= LSHIFT(EFLAG[i], i * 8U); }

	ret:
	return err;
}


FctERR NONNULL__ PCA9952_ReadRegister(PCA9952_t * const pCpnt, const PCA9952_reg reg, uint8_t * val)
{
	FctERR err;

	if (reg > PCA9952__EFLAG1)	{ err = ERROR_RANGE; } // Unknown register
	else						{ err = PCA9952_Read(pCpnt->cfg.slave_inst, val, reg, sizeof(uint8_t)); }

	return err;
}


/****************************************************************/


__WEAK void NONNULL__ PCA9952_RST_GPIO_Init(PCA9952_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&pCpnt->cfg.RST_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK void NONNULL__ PCA9952_RST_GPIO_Set(PCA9952_t * const pCpnt, const bool state) {
	I2C_peripheral_GPIO_set(&pCpnt->cfg.RST_GPIO, state); }


__WEAK void NONNULL__ PCA9952_OE_GPIO_Init(PCA9952_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&pCpnt->cfg.OE_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK void NONNULL__ PCA9952_OE_GPIO_Set(PCA9952_t * const pCpnt, const bool state) {
	I2C_peripheral_GPIO_set(&pCpnt->cfg.OE_GPIO, state); }


/****************************************************************/
#endif
#endif
/****************************************************************/
