/*!\file PCA9624_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief PCA9624 Driver extensions
** \details PCA9624: 8-bit Fm+ I2C-bus 100mA 40V LED driver
**/
/****************************************************************/
#include "PCA9624.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9624)
/****************************************************************/
// std libs
#include <stdlib.h>
/****************************************************************/


FctERR PCA9624_Set_Latch(const PCA96xx_latch latch)
{
	uPCA9624_REG__MODE2	MODE2;
	FctERR				err;

	if (latch > PCA96xx__LATCH_ON_ACK)	{ return ERROR_VALUE; }	// Unknown latch mode

	err = PCA9624_Read((uint8_t *) &MODE2, PCA9624__MODE2, 1);
	if (err)	{ return err; }

	MODE2.Bits.OCH = latch;
	return PCA9624_Write((uint8_t *) &MODE2, PCA9624__MODE2, 1);
}


FctERR PCA9624_Set_Mode_LED(const PCA96xx_chan chan, const PCA9624_ledout mode)
{
	uPCA9624_REG__LEDOUT0	LED;
	FctERR					err;
	uint8_t					idx = (chan > PCA96xx__PWM4) ? 1 : 0;

	if ((chan < PCA96xx__PWM1) && (chan > PCA96xx__PWM8))	{ return ERROR_RANGE; }	// Unknown channel
	if (mode > PCA9624__GROUP_BRIGHT)						{ return ERROR_VALUE; }	// Unknown control mode

	err = PCA9624_Read((uint8_t *) &LED, PCA9624__LEDOUT0 + idx, 1);
	if (err)	{ return err; }

	switch (chan)
	{
		case PCA96xx__PWM1:
		case PCA96xx__PWM5:
			LED.Bits.LDR0 = mode;
			break;

		case PCA96xx__PWM2:
		case PCA96xx__PWM6:
			LED.Bits.LDR1 = mode;
			break;

		case PCA96xx__PWM3:
		case PCA96xx__PWM7:
			LED.Bits.LDR2 = mode;
			break;

		case PCA96xx__PWM4:
		case PCA96xx__PWM8:
			LED.Bits.LDR3 = mode;
			break;

		default:
			break;
	}

	return PCA9624_Write((uint8_t *) &LED, PCA9624__LEDOUT0 + idx, 1);
}


FctERR PCA9624_Set_Mode_LEDs(const uint8_t chans, const PCA9624_ledout mode)
{
	uPCA9624_REG__LEDOUT0	LED[2];
	FctERR					err;

	if (!chans)							{ return ERROR_OK; }		// Nothing to do
	if (mode > PCA9624__GROUP_BRIGHT)	{ return ERROR_VALUE; }	// Unknown control mode

	err = PCA9624_Read((uint8_t *) LED, PCA9624__LEDOUT0, 2);
	if (err)	{ return err; }

	for (int i = PCA96xx__PWM1, j = 1 ; i <= PCA96xx__PWM8 ; i++, j <<= 1)
	{
		if (chans & j)
		{
			div_t tmp = div(i, 4);

			if (tmp.rem == 0)		{ LED[tmp.quot].Bits.LDR0 = mode; }
			else if (tmp.rem == 1)	{ LED[tmp.quot].Bits.LDR1 = mode; }
			else if (tmp.rem == 2)	{ LED[tmp.quot].Bits.LDR2 = mode; }
			else if (tmp.rem == 3)	{ LED[tmp.quot].Bits.LDR3 = mode; }
		}
	}

	return PCA9624_Write((uint8_t *) LED, PCA9624__LEDOUT0, 2);
}


FctERR PCA9624_Reset(const bool all)
{
	uint8_t Data = 0x06;
	PCA9624_hal.status = HAL_I2C_Master_Transmit(PCA9624_hal.cfg.bus_inst, all ? PCA96xx_GENERAL_CALL_ADDR : PCA9624_hal.cfg.addr, &Data, 1, PCA9624_hal.cfg.timeout);
	return HALERRtoFCTERR(PCA9624_hal.status);
}



FctERR PCA9624_ReadRegister(const PCA9624_reg reg, uint8_t * val)
{
	*val = 0;

	if (reg > PCA9624__ALLCALLADR)		{ return ERROR_RANGE; }		// Unknown register

	return PCA9624_Read(val, reg, 1);
}


FctERR PCA9624_ReadVal(const PCA96xx_chan chan, uint8_t * duty)
{
	*duty = 0;
	if ((chan < PCA96xx__PWM1) && (chan > PCA96xx__PWM8))	{ return ERROR_RANGE; }	// Unknown channel
	return PCA9624_Read(duty, PCA9624__PWM0 + chan - 1, 1);
}


FctERR PCA9624_PutVal(const PCA96xx_chan chan, uint8_t duty)
{
	if ((chan < PCA96xx__PWM1) && (chan > PCA96xx__PWM8))	{ return ERROR_RANGE; }	// Unknown channel
	return PCA9624_Write(&duty, PCA9624__PWM0 + chan - 1, 1);
}


FctERR PCA9624_SetVal(const PCA96xx_chan chan)
{
	const uint8_t val = 0xFF;
	if ((chan < PCA96xx__PWM1) && (chan > PCA96xx__PWM8))	{ return ERROR_RANGE; }	// Unknown channel
	return PCA9624_Write((uint8_t *) &val, PCA9624__PWM0 + chan - 1, 1);
}


FctERR PCA9624_ClrVal(const PCA96xx_chan chan)
{
	const uint8_t val = 0;
	if ((chan < PCA96xx__PWM1) && (chan > PCA96xx__PWM8))	{ return ERROR_RANGE; }	// Unknown channel
	return PCA9624_Write((uint8_t *) &val, PCA9624__PWM0 + chan - 1, 1);
}


/****************************************************************/
#endif
#endif
/****************************************************************/
