/*!\file PCA9685_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief PCA9685 Driver extensions
** \details PCA9685: 16-channel, 12-bit PWM Fm+ I2C-bus LED controller
**/
/****************************************************************/
#include "PCA9685.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9685)
/****************************************************************/
// std libs
#include <stdlib.h>
#include <math.h>
/****************************************************************/


FctERR NONNULL__ PCA9685_CalcVal(uint8_t val[4], const uint16_t duty, const uint16_t delay)
{
	FctERR err = ERROR_OK;

	if ((duty > 4095U) || (delay >= 4095U))	{ err = ERROR_RANGE; }
	else
	{
		uint16_t ONCount = delay;
		uint16_t OFFCount = (delay + duty) % 4095U;

		if (duty == 0)			{ OFFCount |= 0x1000U; }	// Off count set to 4096 to fill bit FullOnOff
		else if (duty == 4095U)	{ ONCount |= 0x1000U; }		// On count set to 4096 to fill bit FullOnOff

		val[0] = LOBYTE(ONCount);
		val[1] = HIBYTE(ONCount);
		val[2] = LOBYTE(OFFCount);
		val[3] = HIBYTE(OFFCount);
	}

	return err;
}


FctERR NONNULL__ PCA9685_CalcVal_NoDelay(uint8_t val[4], const uint16_t duty)
{
	FctERR err = ERROR_OK;

	if (duty > 4095U)	{ err = ERROR_RANGE; }
	else
	{
		uint16_t ONCount = 0;
		uint16_t OFFCount = duty;

		if (duty == 0)			{ OFFCount |= 0x1000U; }	// Off count set to 4096 to fill bit FullOnOff
		else if (duty == 4095U)	{ ONCount |= 0x1000U; }		// On count set to 4096 to fill bit FullOnOff

		val[0] = LOBYTE(ONCount);
		val[1] = HIBYTE(ONCount);
		val[2] = LOBYTE(OFFCount);
		val[3] = HIBYTE(OFFCount);
	}

	return err;
}

FctERR NONNULL__ PCA9685_Freq_To_Byte(const PCA9685_t * const pCpnt, uint8_t * const byte, const uint16_t freq)
{
	FctERR err = ERROR_OK;

	if (	(freq > (uint16_t) (((float) pCpnt->cfg.Clock / PCA9685_CLOCK_FREQ) * PCA9685_FREQ_HZ_MAX))
		||	(freq < (uint16_t) (((float) pCpnt->cfg.Clock / PCA9685_CLOCK_FREQ) * PCA9685_FREQ_HZ_MIN)))	{ err = ERROR_RANGE; }
	else	{ *byte = (uint8_t) (round((float) pCpnt->cfg.Clock / (4096.0f * freq)) - 1.0f); }

	return err;
}

FctERR NONNULL__ PCA9685_Set_Latch(PCA9685_t * const pCpnt, const PCA96xx_latch latch)
{
	uPCA9685_REG__MODE2	MODE2;
	FctERR				err;

	if (latch > PCA9xxx__LATCH_ON_ACK)	{ err = ERROR_VALUE; }	// Unknown latch mode
	else
	{
		err = PCA9685_Read(pCpnt->cfg.slave_inst, &MODE2.Byte, PCA9685__MODE2, sizeof(MODE2));
		if (err != ERROR_OK)	{ goto ret; }

		MODE2.Bits.OCH = latch;
		err = PCA9685_Write(pCpnt->cfg.slave_inst, &MODE2.Byte, PCA9685__MODE2, sizeof(MODE2));
	}

	ret:
	return err;
}


FctERR NONNULL__ PCA9685_Set_Frequency(PCA9685_t * const pCpnt, const uint16_t freq)
{
	uPCA9685_REG__MODE1	MODE1;
	uint8_t				DATA;
	FctERR				err = PCA9685_Freq_To_Byte(pCpnt, &DATA, freq);

	if (err == ERROR_OK)
	{
		err = PCA9685_Read(pCpnt->cfg.slave_inst, &MODE1.Byte, PCA9685__MODE1, sizeof(MODE1));
		if (err != ERROR_OK)	{ goto ret; }

		MODE1.Bits.SLEEP = true;
		err = PCA9685_Write(pCpnt->cfg.slave_inst, &MODE1.Byte, PCA9685__MODE1, sizeof(MODE1));
		if (err != ERROR_OK)	{ goto ret; }

		// Send prescaler to obtain desired frequency (only in SLEEP)
		err = PCA9685_Write(pCpnt->cfg.slave_inst, &DATA, PCA9685__PRE_SCALE, sizeof(DATA));
		if (err != ERROR_OK)	{ goto ret; }

		pCpnt->cfg.Frequency = PCA9685_Byte_To_Freq(pCpnt, DATA);

		MODE1.Bits.SLEEP = false;
		err = PCA9685_Write(pCpnt->cfg.slave_inst, &MODE1.Byte, PCA9685__MODE1, sizeof(MODE1));
	}

	ret:
	return err;
}


FctERR NONNULL__ PCA9685_ReadVal(PCA9685_t * const pCpnt, const PCA9xxx_chan chan, uint16_t * duty)
{
	FctERR		err;
	uint8_t		DATA[4] = { 0 };

	if (chan > PCA9xxx__PWM16)	{ err = ERROR_RANGE; } // Unknown channel
	else
	{
		err = PCA9685_Read(pCpnt->cfg.slave_inst, DATA, LED_OFFSET_L(chan), sizeof(DATA));
		if (err != ERROR_OK)	{ goto ret; }

		if (DATA[1] & DefBitFullOnOff)
		{
			*duty = 4095U;
		}
		else if (!(DATA[3] & DefBitFullOnOff))
		{
			const int16_t ONCount = MAKEWORD(DATA[0], DATA[1]);
			const int16_t OFFCount = MAKEWORD(DATA[2], DATA[3]);
			*duty = (uint16_t) abs(OFFCount - ONCount + 1);
		}
	}

	ret:
	return err;
}


FctERR NONNULL__ PCA9685_ReadValByte(PCA9685_t * const pCpnt, const PCA9xxx_chan chan, uint8_t * duty)
{
	FctERR		err;
	uint8_t		DATA[4] = { 0 };

	if (chan > PCA9xxx__PWM16)		{ err = ERROR_RANGE; } // Unknown channel
	else
	{
		err = PCA9685_Read(pCpnt->cfg.slave_inst, DATA, LED_OFFSET_L(chan), sizeof(DATA));
		if (err != ERROR_OK)	{ goto ret; }

		if (DATA[1] & DefBitFullOnOff)
		{
			*duty = 255U;
		}
		else if (!(DATA[3] & DefBitFullOnOff))
		{
			const int16_t ONCount = MAKEWORD(DATA[0], DATA[1]);
			const int16_t OFFCount = MAKEWORD(DATA[2], DATA[3]);
			*duty = (uint8_t) (abs(OFFCount - ONCount + 1) / 16);
		}
	}

	ret:
	return err;
}


FctERR NONNULL__ PCA9685_PutVal(PCA9685_t * const pCpnt, const PCA9xxx_chan chan, const uint16_t duty, const uint16_t delay)
{
	PCA9685_reg RegAddr;
	uint8_t		DATA[4];
	FctERR		err = ERROR_OK;

	if (chan == PCA9xxx__ALL)			{ RegAddr = PCA9685__ALL_LED_ON_L; }	// All channels at once
	else if (chan <= PCA9xxx__PWM16)	{ RegAddr = LED_OFFSET_L(chan); }		// Regular channel
	else								{ err = ERROR_RANGE; }					// Unknown channel
	if (err != ERROR_OK)				{ goto ret; }

	err = PCA9685_CalcVal(DATA, duty, delay);
	if (err != ERROR_OK)	{ goto ret; }

	err = PCA9685_Write(pCpnt->cfg.slave_inst, DATA, RegAddr, sizeof(DATA));

	ret:
	return err;
}


FctERR NONNULL__ PCA9685_PutValPerc(PCA9685_t * const pCpnt, const PCA9xxx_chan chan, const float duty, const float delay)
{
	FctERR err;

	if (	((duty < 0.0f) || (duty > 100.0f))
		||	((delay < 0.0f) || (delay >= 100.0f)))	{ err = ERROR_RANGE; }
	else											{ err = PCA9685_PutVal(pCpnt, chan, (uint16_t) ((duty / 100.0f) * 4095U), (uint16_t) ((delay / 100.0f) * 4095U)); }

	return err;
}


FctERR NONNULL__ PCA9685_SetVal(PCA9685_t * const pCpnt, const PCA9xxx_chan chan)
{
	PCA9685_reg	RegAddr;
	FctERR		err = ERROR_OK;

	if (chan == PCA9xxx__ALL)			{ RegAddr = PCA9685__ALL_LED_ON_L; }	// All channels at once
	else if (chan <= PCA9xxx__PWM16)	{ RegAddr = LED_OFFSET_L(chan); }		// Regular channel
	else								{ err = ERROR_RANGE; }					// Unknown channel
	if (err != ERROR_OK)				{ goto ret; }

	const uint8_t DATA[4] = { 0, DefBitFullOnOff, 0, 0 };
	err = PCA9685_Write(pCpnt->cfg.slave_inst, DATA, RegAddr, sizeof(DATA));

	ret:
	return err;
}


FctERR NONNULL__ PCA9685_ClrVal(PCA9685_t * const pCpnt, const PCA9xxx_chan chan)
{
	PCA9685_reg RegAddr;
	FctERR		err = ERROR_OK;

	if (chan == PCA9xxx__ALL)			{ RegAddr = PCA9685__ALL_LED_ON_L; }	// All channels at once
	else if (chan <= PCA9xxx__PWM16)	{ RegAddr = LED_OFFSET_L(chan); }		// Regular channel
	else								{ err = ERROR_RANGE; }					// Unknown channel
	if (err != ERROR_OK)				{ goto ret; }

	const uint8_t DATA[4] = { 0, 0, 0, DefBitFullOnOff };
	err = PCA9685_Write(pCpnt->cfg.slave_inst, DATA, RegAddr, sizeof(DATA));

	ret:
	return err;
}


FctERR NONNULL__ PCA9685_Reset(PCA9685_t * const pCpnt)
{
	uint8_t			DATA = 0x06U;
	I2C_slave_t *	pSlave = pCpnt->cfg.slave_inst;

	pSlave->status = HAL_I2C_Master_Transmit(pSlave->cfg.bus_inst, pSlave->cfg.addr, &DATA, 1, pSlave->cfg.timeout);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ PCA9685_Reset_All(I2C_HandleTypeDef * const hi2c)
{
	uint8_t DATA = 0x06U;

	return HALERRtoFCTERR(HAL_I2C_Master_Transmit(hi2c, I2C_ADDR_General_Call, &DATA, 1, I2C_slave_timeout));
}


FctERR NONNULL__ PCA9685_ReadRegister(PCA9685_t * const pCpnt, const PCA9685_reg reg, uint8_t * const val)
{
	FctERR err = ERROR_OK;

	if ((reg > PCA9685__LED15_OFF_H) && (reg < PCA9685__ALL_LED_ON_L))	{ err = ERROR_RANGE; } // Unknown register
	else																{ err = PCA9685_Read(pCpnt->cfg.slave_inst, val, reg, sizeof(uint8_t)); }

	return err;
}


/****************************************************************/


__WEAK void NONNULL__ PCA9685_OE_GPIO_Init(PCA9685_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&pCpnt->cfg.OE_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK void NONNULL__ PCA9685_OE_GPIO_Set(PCA9685_t * const pCpnt, const bool state) {
	I2C_peripheral_GPIO_set(&pCpnt->cfg.OE_GPIO, state); }


/****************************************************************/
#endif
#endif
/****************************************************************/
