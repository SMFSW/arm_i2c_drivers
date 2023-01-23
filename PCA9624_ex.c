/*!\file PCA9624_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
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

	if (latch > PCA9xxx__LATCH_ON_ACK)	{ return ERROR_VALUE; }	// Unknown latch mode

	err = PCA9624_Read(pCpnt->cfg.slave_inst, (uint8_t *) &MODE2, PCA9624__MODE2, sizeof(MODE2));
	if (err)	{ return err; }

	MODE2.Bits.OCH = latch;
	return PCA9624_Write(pCpnt->cfg.slave_inst, (uint8_t *) &MODE2, PCA9624__MODE2, sizeof(MODE2));
}


FctERR NONNULL__ PCA9624_Set_Mode_LED(PCA9624_t * const pCpnt, const PCA9xxx_chan chan, const PCA962x_ledout mode)
{
	if (chan > PCA9xxx__PWM8)			{ return ERROR_RANGE; }	// Unknown channel
	if (mode > PCA9xxx__GROUP_BRIGHT)	{ return ERROR_VALUE; }	// Unknown control mode

	const unsigned int offset = chan / 4;
	const unsigned int shift = chan * 2;

	const uint16_t mask = LSHIFT(0x3, shift);
	const uint16_t val = LSHIFT(mode, shift);

	SET_BITS_VAL(pCpnt->LDR.Word, mask, val);

	const uint8_t LDR = RSHIFT(pCpnt->LDR.Word, offset * 8);
	return PCA9624_Write(pCpnt->cfg.slave_inst, &LDR, PCA9624__LEDOUT0 + offset, sizeof(LDR));
}


FctERR NONNULL__ PCA9624_Set_Mode_LEDs(PCA9624_t * const pCpnt, const uint8_t chans, const PCA962x_ledout mode)
{
	if (!chans)							{ return ERROR_OK; }	// Nothing to do
	if (mode > PCA9xxx__GROUP_BRIGHT)	{ return ERROR_VALUE; }	// Unknown control mode

	uint16_t	mask = 0, val = 0;
	uint8_t		LDR[2];

	for (PCA9xxx_chan chan = PCA9xxx__PWM1 ; chan <= PCA9xxx__PWM8 ; chan++)
	{
		if (LSHIFT(1, chan) & chans)
		{
			const unsigned int shift = chan * 2;
			mask |= LSHIFT(0x3, shift);
			val |= LSHIFT(mode, shift);
		}
	}

	SET_BITS_VAL(pCpnt->LDR.Word, mask, val);

	for (unsigned int i = 0 ; i < sizeof(LDR) ; i++)	{ LDR[i] = RSHIFT(pCpnt->LDR.Word, i * 8); }

	return PCA9624_Write(pCpnt->cfg.slave_inst, LDR, PCA9624__LEDOUT0, sizeof(LDR));
}


FctERR NONNULL__ PCA9624_ReadVals(PCA9624_t * const pCpnt, uint8_t pDuty[], const bool indexed, const PCA9xxx_chan start, const PCA9xxx_chan end)
{
	if (start > PCA9xxx__PWM8)	{ return ERROR_RANGE; }	// Unknown channel
	if (end > PCA9xxx__PWM8)	{ return ERROR_RANGE; }	// Unknown channel

	uint8_t * const pArray = pDuty + (indexed ? start : 0);
	return PCA9624_Read(pCpnt->cfg.slave_inst, pArray, PCA9624__PWM0 + start, end - start + 1);
}


FctERR NONNULL__ PCA9624_ReadVal(PCA9624_t * const pCpnt, const PCA9xxx_chan chan, uint8_t * const pDuty)
{
	if (chan > PCA9xxx__PWM8)	{ return ERROR_RANGE; }	// Unknown channel

	return PCA9624_Read(pCpnt->cfg.slave_inst, pDuty, PCA9624__PWM0 + chan, sizeof(uint8_t));
}


FctERR NONNULL__ PCA9624_PutVals(PCA9624_t * const pCpnt, const uint8_t pDuty[], const bool indexed, const PCA9xxx_chan start, const PCA9xxx_chan end)
{
	if (start > PCA9xxx__PWM8)	{ return ERROR_RANGE; }	// Unknown channel
	if (end > PCA9xxx__PWM8)	{ return ERROR_RANGE; }	// Unknown channel


	const uint8_t * const pArray = pDuty + (indexed ? start : 0);
	return PCA9624_Write(pCpnt->cfg.slave_inst, pArray, PCA9624__PWM0 + start, end - start + 1);
}


FctERR NONNULL__ PCA9624_PutVal(PCA9624_t * const pCpnt, const PCA9xxx_chan chan, const uint8_t duty)
{
	if (chan > PCA9xxx__PWM8)	{ return ERROR_RANGE; }	// Unknown channel

	return PCA9624_Write(pCpnt->cfg.slave_inst, &duty, PCA9624__PWM0 + chan, sizeof(duty));
}


FctERR NONNULL__ PCA9624_SetVal(PCA9624_t * const pCpnt, const PCA9xxx_chan chan)
{
	const uint8_t VAL = (uint8_t) -1;

	if (chan > PCA9xxx__PWM8)	{ return ERROR_RANGE; }	// Unknown channel

	return PCA9624_Write(pCpnt->cfg.slave_inst, &VAL, PCA9624__PWM0 + chan, sizeof(VAL));
}


FctERR NONNULL__ PCA9624_ClrVal(PCA9624_t * const pCpnt, const PCA9xxx_chan chan)
{
	const uint8_t VAL = 0;

	if (chan > PCA9xxx__PWM8)	{ return ERROR_RANGE; }	// Unknown channel

	return PCA9624_Write(pCpnt->cfg.slave_inst, &VAL, PCA9624__PWM0 + chan, sizeof(VAL));
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
	if (reg > PCA9624__ALLCALLADR)	{ return ERROR_RANGE; }		// Unknown register

	return PCA9624_Read(pCpnt->cfg.slave_inst, val, reg, sizeof(uint8_t));
}


/****************************************************************/


__WEAK FctERR NONNULL__ PCA9624_OE_GPIO_Init(PCA9624_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	return I2C_peripheral_GPIO_init(&pCpnt->cfg.OE_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK FctERR NONNULL__ PCA9624_OE_GPIO_Set(PCA9624_t * const pCpnt, const bool state) {
	return I2C_peripheral_GPIO_set(&pCpnt->cfg.OE_GPIO, state); }


/****************************************************************/
#endif
#endif
/****************************************************************/
