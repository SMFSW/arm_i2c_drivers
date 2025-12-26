/*!\file PCA9952_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
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
	uPCA9952_REG__MODE1	MODE1;
	FctERR				err;

	if (RSHIFT(inc, 5) > RSHIFT(PCA9xxx__AUTO_INC_BRIGHT_GLOBAL, 5))	{ return ERROR_VALUE; }

	pCpnt->cfg.auto_inc = inc & PCA9xxx__AUTO_INC_BRIGHT_GLOBAL;		// Mask inc just in case

	err = PCA9952_Read(pCpnt->cfg.slave_inst, (uint8_t *) &MODE1, PCA9952__MODE1, sizeof(MODE1));
	if (err != ERROR_OK)	{ return err; }

	MODE1.Byte = (MODE1.Byte & ~0xE0) | pCpnt->cfg.auto_inc;
	return PCA9952_Write(pCpnt->cfg.slave_inst, (uint8_t *) &MODE1, PCA9952__MODE1, sizeof(MODE1));
}


FctERR NONNULL__ PCA9952_Set_Latch(PCA9952_t * const pCpnt, const PCA96xx_latch latch)
{
	uPCA9952_REG__MODE2	MODE2;
	FctERR				err;

	if (latch > PCA9xxx__LATCH_ON_ACK)	{ return ERROR_VALUE; }	// Unknown latch mode

	err = PCA9952_Read(pCpnt->cfg.slave_inst, (uint8_t *) &MODE2, PCA9952__MODE2, sizeof(MODE2));
	if (err != ERROR_OK)	{ return err; }

	MODE2.Bits.OCH = latch;
	return PCA9952_Write(pCpnt->cfg.slave_inst, (uint8_t *) &MODE2, PCA9952__MODE2, sizeof(MODE2));
}


FctERR NONNULL__ PCA9952_Set_Mode_LED(PCA9952_t * const pCpnt, const PCA9xxx_chan chan, const PCA995x_ledout mode)
{
	if (chan > PCA9xxx__PWM16)			{ return ERROR_RANGE; }	// Unknown channel
	if (mode > PCA9xxx__GROUP_BRIGHT)	{ return ERROR_VALUE; }	// Unknown control mode

	const uintCPU_t offset = chan / 4;
	const uintCPU_t shift = chan * 2;

	const uint16_t mask = LSHIFT(0x3, shift);
	const uint16_t val = LSHIFT(mode, shift);

	SET_BITS_VAL(pCpnt->LDR.DWord, mask, val);

	const uint8_t LDR = RSHIFT(pCpnt->LDR.DWord, offset * 8);
	return PCA9952_Write(pCpnt->cfg.slave_inst, &LDR, PCA9952__LEDOUT0 + offset, sizeof(LDR));
}


FctERR NONNULL__ PCA9952_Set_Mode_LEDs(PCA9952_t * const pCpnt, const uint16_t chans, const PCA995x_ledout mode)
{
	if (!chans)							{ return ERROR_OK; }	// Nothing to do
	if (mode > PCA9xxx__GROUP_BRIGHT)	{ return ERROR_VALUE; }	// Unknown control mode

	uint32_t	mask = 0, val = 0;
	uint8_t		LDR[4];

	for (PCA9xxx_chan chan = PCA9xxx__PWM1 ; chan <= PCA9xxx__PWM16 ; chan++)
	{
		if (LSHIFT(1, chan) & chans)
		{
			const uintCPU_t shift = chan * 2;
			mask |= LSHIFT(0x3, shift);
			val |= LSHIFT(mode, shift);
		}
	}

	SET_BITS_VAL(pCpnt->LDR.DWord, mask, val);

	for (uintCPU_t i = 0 ; i < sizeof(LDR) ; i++)	{ LDR[i] = RSHIFT(pCpnt->LDR.DWord, i * 8); }

	return PCA9952_Write(pCpnt->cfg.slave_inst, LDR, PCA9952__LEDOUT0, sizeof(LDR));
}


FctERR NONNULL__ PCA9952_Set_IREF(PCA9952_t * const pCpnt, const PCA9xxx_chan chan, const uint8_t iref)
{
	if ((chan > PCA9xxx__PWM16) && (chan != PCA9xxx__ALL))	{ return ERROR_RANGE; }	// Unknown channel

	const uint8_t reg = (chan == PCA9xxx__ALL) ? PCA9952__IREFALL : PCA9952__IREF0 + chan;
	return PCA9952_Write(pCpnt->cfg.slave_inst, &iref, reg, sizeof(iref));
}


FctERR NONNULL__ PCA9952_Set_IREFs(PCA9952_t * const pCpnt, const uint16_t chans, const uint8_t iref)
{
	FctERR err = ERROR_OK;

	if (!chans)		{ return ERROR_OK; }	// Nothing to do

	for (PCA9xxx_chan chan = PCA9xxx__PWM1 ; chan <= PCA9xxx__PWM16 ; chan++)
	{
		if (LSHIFT(1, chan) & chans)
		{
			err |= PCA9952_Write(pCpnt->cfg.slave_inst, &iref, PCA9952__IREF0 + chan, sizeof(iref));
		}
	}

	return err;
}


FctERR NONNULL__ PCA9952_Set_Offset(PCA9952_t * const pCpnt, const PCA9952_offset offset)
{
	if (offset > PCA9952__OFFSET_11_CLOCK_CYCLE)	{ return ERROR_VALUE; }	// Unknown delay value

	return PCA9952_Write(pCpnt->cfg.slave_inst, &offset, PCA9952__OFFSET, sizeof(offset));
}


FctERR NONNULL__ PCA9952_ReadVals(PCA9952_t * const pCpnt, uint8_t pDuty[], const bool indexed, const PCA9xxx_chan start, const PCA9xxx_chan end)
{
	if (start > PCA9xxx__PWM16)	{ return ERROR_RANGE; }	// Unknown channel
	if (end > PCA9xxx__PWM16)	{ return ERROR_RANGE; }	// Unknown channel

	uint8_t * const pArray = pDuty + (indexed ? start : 0);
	return PCA9952_Read(pCpnt->cfg.slave_inst, pArray, PCA9952__PWM0 + start, end - start + 1);
}


FctERR NONNULL__ PCA9952_ReadVal(PCA9952_t * const pCpnt, const PCA9xxx_chan chan, uint8_t * const pDuty)
{
	if ((chan > PCA9xxx__PWM16) && (chan != PCA9xxx__ALL))	{ return ERROR_RANGE; }	// Unknown channel

	const uint8_t reg = (chan == PCA9xxx__ALL) ? PCA9952__PWMALL : PCA9952__PWM0 + chan;
	return PCA9952_Read(pCpnt->cfg.slave_inst, pDuty, reg, sizeof(uint8_t));
}


FctERR NONNULL__ PCA9952_PutVals(PCA9952_t * const pCpnt, const uint8_t pDuty[], const bool indexed, const PCA9xxx_chan start, const PCA9xxx_chan end)
{
	if (start > PCA9xxx__PWM16)	{ return ERROR_RANGE; }	// Unknown channel
	if (end > PCA9xxx__PWM16)	{ return ERROR_RANGE; }	// Unknown channel

	const uint8_t * const pArray = pDuty + (indexed ? start : 0);
	return PCA9952_Write(pCpnt->cfg.slave_inst, pArray, PCA9952__PWM0 + start, end - start + 1);
}


FctERR NONNULL__ PCA9952_PutVal(PCA9952_t * const pCpnt, const PCA9xxx_chan chan, const uint8_t duty)
{
	if ((chan > PCA9xxx__PWM16) && (chan != PCA9xxx__ALL))	{ return ERROR_RANGE; }	// Unknown channel

	const uint8_t reg = (chan == PCA9xxx__ALL) ? PCA9952__PWMALL : PCA9952__PWM0 + chan;
	return PCA9952_Write(pCpnt->cfg.slave_inst, &duty, reg, sizeof(duty));
}


FctERR NONNULL__ PCA9952_SetVal(PCA9952_t * const pCpnt, const PCA9xxx_chan chan)
{
	const uint8_t VAL = (uint8_t) -1;

	if (chan > PCA9xxx__PWM16)	{ return ERROR_RANGE; }	// Unknown channel

	return PCA9952_Write(pCpnt->cfg.slave_inst, &VAL, PCA9952__PWM0 + chan, sizeof(VAL));
}


FctERR NONNULL__ PCA9952_ClrVal(PCA9952_t * const pCpnt, const PCA9xxx_chan chan)
{
	const uint8_t VAL = 0;

	if (chan > PCA9xxx__PWM16)	{ return ERROR_RANGE; }	// Unknown channel

	return PCA9952_Write(pCpnt->cfg.slave_inst, &VAL, PCA9952__PWM0 + chan, sizeof(VAL));
}


FctERR NONNULL__ PCA9952_ReadEFLAGs(PCA9952_t * const pCpnt, uPCA9952_REG__EFLAG * const eflags)
{
	uint8_t EFLAG[2];
	FctERR err = PCA9952_Read(pCpnt->cfg.slave_inst, EFLAG, PCA9952__EFLAG0, sizeof(EFLAG));
	if (err != ERROR_OK)	{ return err; }

	eflags->Word = 0;
	for (uintCPU_t i = 0 ; i < sizeof(EFLAG) ; i++)	{ eflags->Word |= LSHIFT(EFLAG[i], i * 8); }

	return ERROR_OK;
}


FctERR NONNULL__ PCA9952_ReadRegister(PCA9952_t * const pCpnt, const PCA9952_reg reg, uint8_t * val)
{
	if (reg > PCA9952__EFLAG1)	{ return ERROR_RANGE; } // Unknown register

	return PCA9952_Read(pCpnt->cfg.slave_inst, val, reg, sizeof(uint8_t));
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
