/*!\file PCA9956_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
** \brief PCA9956 Driver extensions
** \details PCA9956: 24-channel Fm+ I2C-bus 57 mA/20 V constant current LED driver
**/
/****************************************************************/
#include "PCA9956.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9956)
/****************************************************************/
// std libs
#include <stdlib.h>
#include <math.h>
/****************************************************************/


FctERR NONNULL__ PCA9956_Set_Auto_Increment(PCA9956_t * const pCpnt, const PCA962x_reg_inc inc)
{
	uPCA9956_REG__MODE1	MODE1;
	FctERR				err;

	if (RSHIFT(inc, 5) > RSHIFT(PCA9xxx__AUTO_INC_BRIGHT_GLOBAL, 5))	{ return ERROR_VALUE; }

	pCpnt->cfg.auto_inc = inc & PCA9xxx__AUTO_INC_BRIGHT_GLOBAL;		// Mask inc just in case

	err = PCA9956_Read(pCpnt->cfg.slave_inst, (uint8_t *) &MODE1, PCA9956__MODE1, sizeof(MODE1));
	if (err)	{ return err; }

	MODE1.Byte = (MODE1.Byte & ~0xE0) | pCpnt->cfg.auto_inc;
	return PCA9956_Write(pCpnt->cfg.slave_inst, (uint8_t *) &MODE1, PCA9956__MODE1, sizeof(MODE1));
}


FctERR NONNULL__ PCA9956_Set_Latch(PCA9956_t * const pCpnt, const PCA96xx_latch latch)
{
	uPCA9956_REG__MODE2	MODE2;
	FctERR				err;

	if (latch > PCA9xxx__LATCH_ON_ACK)	{ return ERROR_VALUE; }	// Unknown latch mode

	err = PCA9956_Read(pCpnt->cfg.slave_inst, (uint8_t *) &MODE2, PCA9956__MODE2, sizeof(MODE2));
	if (err)	{ return err; }

	MODE2.Bits.OCH = latch;
	return PCA9956_Write(pCpnt->cfg.slave_inst, (uint8_t *) &MODE2, PCA9956__MODE2, sizeof(MODE2));
}


FctERR NONNULL__ PCA9956_Set_Mode_LED(PCA9956_t * const pCpnt, const PCA9xxx_chan chan, const PCA995x_ledout mode)
{
	if (chan > PCA9xxx__PWM24)			{ return ERROR_RANGE; }	// Unknown channel
	if (mode > PCA9xxx__GROUP_BRIGHT)	{ return ERROR_VALUE; }	// Unknown control mode

	const uintCPU_t offset = chan / 4;
	const uintCPU_t shift = chan * 2;

	const uint16_t mask = LSHIFT(0x3, shift);
	const uint16_t val = LSHIFT(mode, shift);

	SET_BITS_VAL(pCpnt->LDR.LWord, mask, val);

	const uint8_t LDR = RSHIFT64(pCpnt->LDR.LWord, offset * 8);
	return PCA9956_Write(pCpnt->cfg.slave_inst, &LDR, PCA9956__LEDOUT0 + offset, sizeof(LDR));
}


FctERR NONNULL__ PCA9956_Set_Mode_LEDs(PCA9956_t * const pCpnt, const uint32_t chans, const PCA995x_ledout mode)
{
	if (!chans)							{ return ERROR_OK; }	// Nothing to do
	if (mode > PCA9xxx__GROUP_BRIGHT)	{ return ERROR_VALUE; }	// Unknown control mode

	uint64_t	mask = 0, val = 0;
	uint8_t		LDR[6];

	for (PCA9xxx_chan chan = PCA9xxx__PWM1 ; chan <= PCA9xxx__PWM24 ; chan++)
	{
		if (LSHIFT(1, chan) & chans)
		{
			const uintCPU_t shift = chan * 2;
			mask |= LSHIFT64(0x3, shift);
			val |= LSHIFT64(mode, shift);
		}
	}

	SET_BITS_VAL(pCpnt->LDR.LWord, mask, val);

	for (uintCPU_t i = 0 ; i < sizeof(LDR) ; i++)	{ LDR[i] = RSHIFT64(pCpnt->LDR.LWord, i * 8); }

	return PCA9956_Write(pCpnt->cfg.slave_inst, LDR, PCA9956__LEDOUT0, sizeof(LDR));
}


FctERR NONNULL__ PCA9956_Set_IREF(PCA9956_t * const pCpnt, const PCA9xxx_chan chan, const uint8_t iref)
{
	if ((chan > PCA9xxx__PWM24) && (chan != PCA9xxx__ALL))	{ return ERROR_RANGE; }	// Unknown channel

	const uint8_t reg = (chan == PCA9xxx__ALL) ? PCA9956__IREFALL : PCA9956__IREF0 + chan;
	return PCA9956_Write(pCpnt->cfg.slave_inst, &iref, reg, sizeof(iref));
}


FctERR NONNULL__ PCA9956_Set_IREFs(PCA9956_t * const pCpnt, const uint32_t chans, const uint8_t iref)
{
	FctERR err = ERROR_OK;

	if (!chans)		{ return ERROR_OK; }	// Nothing to do

	for (PCA9xxx_chan chan = PCA9xxx__PWM1 ; chan <= PCA9xxx__PWM24 ; chan++)
	{
		if (LSHIFT(1, chan) & chans)
		{
			err |= PCA9956_Write(pCpnt->cfg.slave_inst, &iref, PCA9956__IREF0 + chan, sizeof(iref));
		}
	}

	return err;
}


FctERR NONNULL__ PCA9956_Set_Offset(PCA9956_t * const pCpnt, const PCA9956_offset offset)
{
	if (offset > PCA9956__OFFSET_11_CLOCK_CYCLE)	{ return ERROR_VALUE; }	// Unknown delay value

	return PCA9956_Write(pCpnt->cfg.slave_inst, &offset, PCA9956__OFFSET, sizeof(offset));
}


FctERR NONNULL__ PCA9956_ReadVals(PCA9956_t * const pCpnt, uint8_t pDuty[], const bool indexed, const PCA9xxx_chan start, const PCA9xxx_chan end)
{
	if (start > PCA9xxx__PWM24)	{ return ERROR_RANGE; }	// Unknown channel
	if (end > PCA9xxx__PWM24)	{ return ERROR_RANGE; }	// Unknown channel

	uint8_t * const pArray = pDuty + (indexed ? start : 0);
	return PCA9956_Read(pCpnt->cfg.slave_inst, pArray, PCA9956__PWM0 + start, end - start + 1);
}


FctERR NONNULL__ PCA9956_ReadVal(PCA9956_t * const pCpnt, const PCA9xxx_chan chan, uint8_t * const pDuty)
{
	if ((chan > PCA9xxx__PWM24) && (chan != PCA9xxx__ALL))	{ return ERROR_RANGE; }	// Unknown channel

	const uint8_t reg = (chan == PCA9xxx__ALL) ? PCA9956__PWMALL : PCA9956__PWM0 + chan;
	return PCA9956_Read(pCpnt->cfg.slave_inst, pDuty, reg, sizeof(uint8_t));
}


FctERR NONNULL__ PCA9956_PutVals(PCA9956_t * const pCpnt, const uint8_t pDuty[], const bool indexed, const PCA9xxx_chan start, const PCA9xxx_chan end)
{
	if (start > PCA9xxx__PWM24)	{ return ERROR_RANGE; }	// Unknown channel
	if (end > PCA9xxx__PWM24)	{ return ERROR_RANGE; }	// Unknown channel

	const uint8_t * const pArray = pDuty + (indexed ? start : 0);
	return PCA9956_Write(pCpnt->cfg.slave_inst, pArray, PCA9956__PWM0 + start, end - start + 1);
}


FctERR NONNULL__ PCA9956_PutVal(PCA9956_t * const pCpnt, const PCA9xxx_chan chan, const uint8_t duty)
{
	if ((chan > PCA9xxx__PWM24) && (chan != PCA9xxx__ALL))	{ return ERROR_RANGE; }	// Unknown channel

	const uint8_t reg = (chan == PCA9xxx__ALL) ? PCA9956__PWMALL : PCA9956__PWM0 + chan;
	return PCA9956_Write(pCpnt->cfg.slave_inst, &duty, reg, sizeof(duty));
}


FctERR NONNULL__ PCA9956_SetVal(PCA9956_t * const pCpnt, const PCA9xxx_chan chan)
{
	const uint8_t VAL = (uint8_t) -1;

	if (chan > PCA9xxx__PWM24)	{ return ERROR_RANGE; }	// Unknown channel

	return PCA9956_Write(pCpnt->cfg.slave_inst, &VAL, PCA9956__PWM0 + chan, sizeof(VAL));
}


FctERR NONNULL__ PCA9956_ClrVal(PCA9956_t * const pCpnt, const PCA9xxx_chan chan)
{
	const uint8_t VAL = 0;

	if (chan > PCA9xxx__PWM24)	{ return ERROR_RANGE; }	// Unknown channel

	return PCA9956_Write(pCpnt->cfg.slave_inst, &VAL, PCA9956__PWM0 + chan, sizeof(VAL));
}


FctERR NONNULL__ PCA9956_ReadEFLAGs(PCA9956_t * const pCpnt, uPCA9956_REG__EFLAG * const eflags)
{
	uint8_t EFLAG[6];
	FctERR err = PCA9956_Read(pCpnt->cfg.slave_inst, EFLAG, PCA9956__EFLAG0, sizeof(EFLAG));
	if (err)	{ return err; }

	eflags->LWord = 0;
	for (uintCPU_t i = 0 ; i < sizeof(EFLAG) ; i++)	{ eflags->LWord |= LSHIFT64(EFLAG[i], i * 8); }

	return ERROR_OK;
}


FctERR NONNULL__ PCA9956_ReadRegister(PCA9956_t * const pCpnt, const PCA9956_reg reg, uint8_t * val)
{
	if (reg > PCA9956__EFLAG5)	{ return ERROR_RANGE; } // Unknown register

	return PCA9956_Read(pCpnt->cfg.slave_inst, val, reg, sizeof(uint8_t));
}


/****************************************************************/


__WEAK FctERR NONNULL__ PCA9956_RST_GPIO_Init(PCA9956_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	return I2C_peripheral_GPIO_init(&pCpnt->cfg.RST_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK FctERR NONNULL__ PCA9956_RST_GPIO_Set(PCA9956_t * const pCpnt, const bool state) {
	return I2C_peripheral_GPIO_set(&pCpnt->cfg.RST_GPIO, state); }


__WEAK FctERR NONNULL__ PCA9956_OE_GPIO_Init(PCA9956_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	return I2C_peripheral_GPIO_init(&pCpnt->cfg.OE_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK FctERR NONNULL__ PCA9956_OE_GPIO_Set(PCA9956_t * const pCpnt, const bool state) {
	return I2C_peripheral_GPIO_set(&pCpnt->cfg.OE_GPIO, state); }


/****************************************************************/
#endif
#endif
/****************************************************************/
