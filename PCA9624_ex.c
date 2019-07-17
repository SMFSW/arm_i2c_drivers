/*!\file PCA9624_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
** \brief PCA9624 Driver extensions
** \details PCA9624: 8-bit Fm+ I2C-bus 100mA 40V LED driver
**/
/****************************************************************/
#include "PCA9624.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9624)
/****************************************************************/


FctERR NONNULL__ PCA9624_Set_Latch(PCA9624_t * const pCpnt, const PCA96xx_latch latch)
{
	uPCA9624_REG__MODE2	MODE2;
	FctERR				err;

	if (latch > PCA96xx__LATCH_ON_ACK)	{ return ERROR_VALUE; }	// Unknown latch mode

	err = PCA9624_Read(pCpnt->cfg.slave_inst, (uint8_t *) &MODE2, PCA9624__MODE2, sizeof(MODE2));
	if (err)	{ return err; }

	MODE2.Bits.OCH = latch;
	return PCA9624_Write(pCpnt->cfg.slave_inst, (uint8_t *) &MODE2, PCA9624__MODE2, sizeof(MODE2));
}


FctERR NONNULL__ PCA9624_Set_Mode_LED(PCA9624_t * const pCpnt, const PCA96xx_chan chan, const PCA962x_ledout mode)
{
	uPCA9624_REG__LEDOUT	LED;
	FctERR					err;

	if (chan > PCA96xx__PWM8)			{ return ERROR_RANGE; }	// Unknown channel
	if (mode > PCA962x__GROUP_BRIGHT)	{ return ERROR_VALUE; }	// Unknown control mode

	err = PCA9624_Read(pCpnt->cfg.slave_inst, (uint8_t *) &LED, PCA9624__LEDOUT0, sizeof(LED));
	if (err)	{ return err; }

	if (chan == PCA96xx__PWM1)		{ LED.Bits.LDR0 = mode; }
	else if (chan == PCA96xx__PWM2)	{ LED.Bits.LDR1 = mode; }
	else if (chan == PCA96xx__PWM3)	{ LED.Bits.LDR2 = mode; }
	else if (chan == PCA96xx__PWM4)	{ LED.Bits.LDR3 = mode; }
	else if (chan == PCA96xx__PWM5)	{ LED.Bits.LDR4 = mode; }
	else if (chan == PCA96xx__PWM6)	{ LED.Bits.LDR5 = mode; }
	else if (chan == PCA96xx__PWM7)	{ LED.Bits.LDR6 = mode; }
	else if (chan == PCA96xx__PWM8)	{ LED.Bits.LDR7 = mode; }

	return PCA9624_Write(pCpnt->cfg.slave_inst, (uint8_t *) &LED, PCA9624__LEDOUT0, sizeof(LED));
}


FctERR NONNULL__ PCA9624_Set_Mode_LEDs(PCA9624_t * const pCpnt, const uint8_t chans, const PCA962x_ledout mode)
{
	uPCA9624_REG__LEDOUT	LED;
	FctERR					err;

	if (!chans)							{ return ERROR_OK; }	// Nothing to do
	if (mode > PCA962x__GROUP_BRIGHT)	{ return ERROR_VALUE; }	// Unknown control mode

	err = PCA9624_Read(pCpnt->cfg.slave_inst, (uint8_t *) &LED, PCA9624__LEDOUT0, sizeof(LED));
	if (err)	{ return err; }

	if (chans & LSHIFT(1, PCA96xx__PWM1))	{ LED.Bits.LDR0 = mode; }
	if (chans & LSHIFT(1, PCA96xx__PWM2))	{ LED.Bits.LDR1 = mode; }
	if (chans & LSHIFT(1, PCA96xx__PWM3))	{ LED.Bits.LDR2 = mode; }
	if (chans & LSHIFT(1, PCA96xx__PWM4))	{ LED.Bits.LDR3 = mode; }
	if (chans & LSHIFT(1, PCA96xx__PWM5))	{ LED.Bits.LDR4 = mode; }
	if (chans & LSHIFT(1, PCA96xx__PWM6))	{ LED.Bits.LDR5 = mode; }
	if (chans & LSHIFT(1, PCA96xx__PWM7))	{ LED.Bits.LDR6 = mode; }
	if (chans & LSHIFT(1, PCA96xx__PWM8))	{ LED.Bits.LDR7 = mode; }

	return PCA9624_Write(pCpnt->cfg.slave_inst, (uint8_t *) &LED, PCA9624__LEDOUT0, sizeof(LED));
}


FctERR NONNULL__ PCA9624_Reset(PCA9624_t * const pCpnt)
{
	uint8_t			DATA = 0x06;
	I2C_slave_t *	pSlave = pCpnt->cfg.slave_inst;

	pSlave->status = HAL_I2C_Master_Transmit(pSlave->cfg.bus_inst, pSlave->cfg.addr, &DATA, sizeof(DATA), pSlave->cfg.timeout);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ PCA9624_Reset_All(const I2C_HandleTypeDef * const hi2c)
{
	uint8_t DATA = 0x06;

	return HALERRtoFCTERR(HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c, PCA96xx_GENERAL_CALL_ADDR, &DATA, sizeof(DATA), I2C_slave_timeout));
}


FctERR NONNULL__ PCA9624_ReadRegister(PCA9624_t * const pCpnt, const PCA9624_reg reg, uint8_t * const val)
{
	*val = 0;

	if (reg > PCA9624__ALLCALLADR)	{ return ERROR_RANGE; }		// Unknown register

	return PCA9624_Read(pCpnt->cfg.slave_inst, val, reg, 1);
}


FctERR NONNULL__ PCA9624_ReadVal(PCA9624_t * const pCpnt, const PCA96xx_chan chan, uint8_t * const duty)
{
	*duty = 0;

	if (chan > PCA96xx__PWM8)	{ return ERROR_RANGE; }	// Unknown channel

	return PCA9624_Read(pCpnt->cfg.slave_inst, duty, PCA9624__PWM0 + chan, 1);
}


FctERR NONNULL__ PCA9624_PutVal(PCA9624_t * const pCpnt, const PCA96xx_chan chan, const uint8_t duty)
{
	if (chan > PCA96xx__PWM8)	{ return ERROR_RANGE; }	// Unknown channel

	return PCA9624_Write(pCpnt->cfg.slave_inst, &duty, PCA9624__PWM0 + chan, sizeof(duty));
}


FctERR NONNULL__ PCA9624_SetVal(PCA9624_t * const pCpnt, const PCA96xx_chan chan)
{
	const uint8_t VAL = (uint8_t) -1;

	if (chan > PCA96xx__PWM8)	{ return ERROR_RANGE; }	// Unknown channel

	return PCA9624_Write(pCpnt->cfg.slave_inst, (uint8_t *) &VAL, PCA9624__PWM0 + chan, sizeof(VAL));
}


FctERR NONNULL__ PCA9624_ClrVal(PCA9624_t * const pCpnt, const PCA96xx_chan chan)
{
	const uint8_t VAL = 0;

	if (chan > PCA96xx__PWM8)	{ return ERROR_RANGE; }	// Unknown channel

	return PCA9624_Write(pCpnt->cfg.slave_inst, (uint8_t *) &VAL, PCA9624__PWM0 + chan, sizeof(VAL));
}


/****************************************************************/
#endif
#endif
/****************************************************************/
