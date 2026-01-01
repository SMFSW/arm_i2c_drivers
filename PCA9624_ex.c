/*!\file PCA9624_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief PCA9624 Driver extensions
** \details PCA9624: 8-bit Fm+ I2C-bus 100mA 40V LED driver
**/
/****************************************************************/
#include "PCA9624.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9624)
/****************************************************************/


FctERR NONNULL__ PCA9624_Set_Auto_Increment(PCA9624_t * const pCpnt, const PCA962x_reg_inc inc)
{
	FctERR err = ERROR_OK;

	if (RSHIFT(inc, 5U) > RSHIFT(PCA9xxx__AUTO_INC_BRIGHT_GLOBAL, 5U))	{ err = ERROR_VALUE; }
	else																{ pCpnt->cfg.auto_inc = inc & PCA9xxx__AUTO_INC_BRIGHT_GLOBAL; }	// Mask inc just in case

	return err;
}


FctERR NONNULL__ PCA9624_Set_Latch(PCA9624_t * const pCpnt, const PCA96xx_latch latch)
{
	uPCA9624_REG__MODE2	MODE2;
	FctERR				err = ERROR_OK;

	if (latch > PCA9xxx__LATCH_ON_ACK)	{ err = ERROR_VALUE; }	// Unknown latch mode
	if (err != ERROR_OK)				{ goto ret; }

	err = PCA9624_Read(pCpnt->cfg.slave_inst, &MODE2.Byte, PCA9624__MODE2, sizeof(MODE2));
	if (err != ERROR_OK)	{ goto ret; }

	MODE2.Bits.OCH = latch;
	err = PCA9624_Write(pCpnt->cfg.slave_inst, &MODE2.Byte, PCA9624__MODE2, sizeof(MODE2));

	ret:
	return err;
}


FctERR NONNULL__ PCA9624_Set_Mode_LED(PCA9624_t * const pCpnt, const PCA9xxx_chan chan, const PCA962x_ledout mode)
{
	FctERR err = ERROR_OK;

	if (chan > PCA9xxx__PWM8)			{ err = ERROR_RANGE; }	// Unknown channel
	if (mode > PCA9xxx__GROUP_BRIGHT)	{ err = ERROR_VALUE; }	// Unknown control mode
	if (err != ERROR_OK)				{ goto ret; }

	const uintCPU_t offset = chan / 4U;
	const uintCPU_t shift = chan * 2U;

	const uint16_t mask = LSHIFT(3U, shift);
	const uint16_t val = LSHIFT(mode, shift);

	SET_BITS_VAL(pCpnt->LDR.Word, mask, val);

	const uint8_t LDR = RSHIFT(pCpnt->LDR.Word, offset * 8U);
	err = PCA9624_Write(pCpnt->cfg.slave_inst, &LDR, PCA9624__LEDOUT0 + offset, sizeof(LDR));

	ret:
	return err;
}


FctERR NONNULL__ PCA9624_Set_Mode_LEDs(PCA9624_t * const pCpnt, const uint8_t chans, const PCA962x_ledout mode)
{
	FctERR err = ERROR_OK;

	if (mode > PCA9xxx__GROUP_BRIGHT)		{ err = ERROR_VALUE; }	// Unknown control mode
	if ((chans == 0) || (err != ERROR_OK))	{ goto ret; }

	uint16_t	mask = 0, val = 0;
	uint8_t		LDR[2];

	for (PCA9xxx_chan chan = PCA9xxx__PWM1 ; chan <= PCA9xxx__PWM8 ; chan++)
	{
		if ((LSHIFT(1U, chan) & chans) != 0)
		{
			const uintCPU_t shift = chan * 2U;
			mask |= LSHIFT(3U, shift);
			val |= LSHIFT(mode, shift);
		}
	}

	SET_BITS_VAL(pCpnt->LDR.Word, mask, val);

	for (uintCPU_t i = 0 ; i < sizeof(LDR) ; i++)	{ LDR[i] = RSHIFT(pCpnt->LDR.Word, i * 8U); }

	err = PCA9624_Write(pCpnt->cfg.slave_inst, LDR, PCA9624__LEDOUT0, sizeof(LDR));

	ret:
	return err;
}


FctERR NONNULL__ PCA9624_ReadVals(PCA9624_t * const pCpnt, uint8_t pDuty[], const bool indexed, const PCA9xxx_chan start, const PCA9xxx_chan end)
{
	FctERR err = ERROR_OK;

	if ((start > PCA9xxx__PWM8) || (end > PCA9xxx__PWM8))	{ err = ERROR_RANGE; }	// Unknown channel
	else
	{
		uint8_t * const pArray = pDuty + (indexed ? start : 0);
		err = PCA9624_Read(pCpnt->cfg.slave_inst, pArray, PCA9624__PWM0 + start, end - start + 1U);
	}

	return err;
}


FctERR NONNULL__ PCA9624_ReadVal(PCA9624_t * const pCpnt, const PCA9xxx_chan chan, uint8_t * const pDuty)
{
	FctERR err = ERROR_OK;

	if (chan > PCA9xxx__PWM8)	{ err = ERROR_RANGE; }	// Unknown channel
	else						{ err = PCA9624_Read(pCpnt->cfg.slave_inst, pDuty, PCA9624__PWM0 + chan, sizeof(uint8_t)); }

	return err;
}


FctERR NONNULL__ PCA9624_PutVals(PCA9624_t * const pCpnt, const uint8_t pDuty[], const bool indexed, const PCA9xxx_chan start, const PCA9xxx_chan end)
{
	FctERR err = ERROR_OK;

	if ((start > PCA9xxx__PWM8) || (end > PCA9xxx__PWM8))	{ err = ERROR_RANGE; }	// Unknown channel
	else
	{
		const uint8_t * const pArray = pDuty + (indexed ? start : 0);
		err = PCA9624_Write(pCpnt->cfg.slave_inst, pArray, PCA9624__PWM0 + start, end - start + 1U);
	}

	return err;
}


FctERR NONNULL__ PCA9624_PutVal(PCA9624_t * const pCpnt, const PCA9xxx_chan chan, const uint8_t duty)
{
	FctERR err = ERROR_OK;

	if (chan > PCA9xxx__PWM8)	{ err = ERROR_RANGE; }	// Unknown channel
	else						{ err = PCA9624_Write(pCpnt->cfg.slave_inst, &duty, PCA9624__PWM0 + chan, sizeof(duty)); }

	return err;
}


FctERR NONNULL__ PCA9624_SetVal(PCA9624_t * const pCpnt, const PCA9xxx_chan chan)
{
	FctERR err = ERROR_OK;

	if (chan > PCA9xxx__PWM8)	{ err = ERROR_RANGE; }	// Unknown channel
	else
	{
		const uint8_t VAL = (uint8_t) -1;
		err = PCA9624_Write(pCpnt->cfg.slave_inst, &VAL, PCA9624__PWM0 + chan, sizeof(VAL));
	}

	return err;
}


FctERR NONNULL__ PCA9624_ClrVal(PCA9624_t * const pCpnt, const PCA9xxx_chan chan)
{
	FctERR err = ERROR_OK;

	if (chan > PCA9xxx__PWM8)	{ err = ERROR_RANGE; }	// Unknown channel
	else
	{
		const uint8_t VAL = 0;
		err = PCA9624_Write(pCpnt->cfg.slave_inst, &VAL, PCA9624__PWM0 + chan, sizeof(VAL));
	}

	return err;
}


FctERR NONNULL__ PCA9624_Reset(PCA9624_t * const pCpnt)
{
	uint8_t			DATA = 0x06U;
	I2C_slave_t *	pSlave = pCpnt->cfg.slave_inst;

	pSlave->status = HAL_I2C_Master_Transmit(pSlave->cfg.bus_inst, pSlave->cfg.addr, &DATA, sizeof(DATA), pSlave->cfg.timeout);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ PCA9624_Reset_All(I2C_HandleTypeDef * const hi2c)
{
	uint8_t DATA = 0x06U;

	return HALERRtoFCTERR(HAL_I2C_Master_Transmit(hi2c, I2C_ADDR_General_Call, &DATA, sizeof(DATA), I2C_slave_timeout));
}


FctERR NONNULL__ PCA9624_ReadRegister(PCA9624_t * const pCpnt, const PCA9624_reg reg, uint8_t * const val)
{
	FctERR err = ERROR_OK;

	if (reg > PCA9624__ALLCALLADR)	{ err = ERROR_RANGE; }		// Unknown register
	else							{ err = PCA9624_Read(pCpnt->cfg.slave_inst, val, reg, sizeof(uint8_t)); }

	return err;
}


/****************************************************************/


__WEAK void NONNULL__ PCA9624_OE_GPIO_Init(PCA9624_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&pCpnt->cfg.OE_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK void NONNULL__ PCA9624_OE_GPIO_Set(PCA9624_t * const pCpnt, const bool state) {
	I2C_peripheral_GPIO_set(&pCpnt->cfg.OE_GPIO, state); }


/****************************************************************/
#endif
#endif
/****************************************************************/
