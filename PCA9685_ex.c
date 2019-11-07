/*!\file PCA9685_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
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


FctERR NONNULL__ PCA9685_Set_Latch(PCA9685_t * pCpnt, const PCA96xx_latch latch)
{
	uPCA9685_REG__MODE2	MODE2;
	FctERR				err;

	if (latch > PCA96xx__LATCH_ON_ACK)	{ return ERROR_VALUE; }	// Unknown latch mode

	err = PCA9685_Read(pCpnt->cfg.slave_inst, (uint8_t *) &MODE2, PCA9685__MODE2, sizeof(MODE2));
	if (err)	{ return err; }

	MODE2.Bits.OCH = latch;
	return PCA9685_Write(pCpnt->cfg.slave_inst, (uint8_t *) &MODE2, PCA9685__MODE2, sizeof(MODE2));
}


FctERR NONNULL__ PCA9685_Set_Frequency(PCA9685_t * pCpnt, const uint16_t freq)
{
	if (	(freq > ((float) pCpnt->cfg.Clock / PCA9685_CLOCK_FREQ) * PCA9685_FREQ_HZ_MAX)
		||	(freq < ((float) pCpnt->cfg.Clock / PCA9685_CLOCK_FREQ) * PCA9685_FREQ_HZ_MIN))	{ return ERROR_RANGE; }

	uPCA9685_REG__MODE1	MODE1;
	uint8_t				DATA;
	FctERR				err = PCA9685_Freq_To_Byte(pCpnt, &DATA, freq);

	if (!err)
	{
		err = PCA9685_Read(pCpnt->cfg.slave_inst, &MODE1.Byte, PCA9685__MODE1, sizeof(MODE1));
		if (err)	{ return err; }

		MODE1.Bits.SLEEP = true;
		err = PCA9685_Write(pCpnt->cfg.slave_inst, &MODE1.Byte, PCA9685__MODE1, sizeof(MODE1));
		if (err)	{ return err; }

		// Send prescaler to obtain desired frequency (only in SLEEP)
		err = PCA9685_Write(pCpnt->cfg.slave_inst, &DATA, PCA9685__PRE_SCALE, sizeof(DATA));
		if (err)	{ return err; }

		pCpnt->cfg.Frequency = PCA9685_Byte_To_Freq(pCpnt, DATA);

		MODE1.Bits.SLEEP = false;
		err = PCA9685_Write(pCpnt->cfg.slave_inst, &MODE1.Byte, PCA9685__MODE1, sizeof(MODE1));
	}

	return err;
}


FctERR NONNULL__ PCA9685_Reset(PCA9685_t * pCpnt)
{
	uint8_t			DATA = 0x06;
	I2C_slave_t *	pSlave = pCpnt->cfg.slave_inst;

	pSlave->status = HAL_I2C_Master_Transmit(pSlave->cfg.bus_inst, pSlave->cfg.addr, &DATA, 1, pSlave->cfg.timeout);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ PCA9685_Reset_All(I2C_HandleTypeDef * hi2c)
{
	uint8_t DATA = 0x06;

	return HALERRtoFCTERR(HAL_I2C_Master_Transmit(hi2c, PCA96xx_GENERAL_CALL_ADDR, &DATA, 1, I2C_slave_timeout));
}


FctERR NONNULL__ PCA9685_ReadRegister(PCA9685_t * pCpnt, const PCA9685_reg reg, uint8_t * val)
{
	*val = 0;

	if ((reg >= PCA9685__LED15_OFF_H) && (reg <= PCA9685__ALL_LED_ON_L))	{ return ERROR_RANGE; } // Unknown channel

	return PCA9685_Read(pCpnt->cfg.slave_inst, val, reg, 1);
}


FctERR NONNULL__ PCA9685_ReadVal(PCA9685_t * pCpnt, const PCA9xxx_chan chan, uint16_t * duty)
{
	FctERR		err;
	uint8_t		DATA[4];

	*duty = 0;

	if (chan > PCA9xxx__PWM16)		{ return ERROR_RANGE; } // Unknown channel

	err = PCA9685_Read(pCpnt->cfg.slave_inst, DATA, LED_OFFSET_L(chan), sizeof(DATA));

	if (DATA[1] & DefBitFullOnOff)
	{
		*duty = 4095;
	}
	else if (!(DATA[3] & DefBitFullOnOff))
	{
		const int16_t ONCount = MAKEWORD(DATA[0], DATA[1]);
		const int16_t OFFCount = MAKEWORD(DATA[2], DATA[3]);
		*duty = (uint16_t) abs(OFFCount - ONCount + 1);
	}

	return err;
}


FctERR NONNULL__ PCA9685_ReadValByte(PCA9685_t * pCpnt, const PCA9xxx_chan chan, uint8_t * duty)
{
	FctERR		err = ERROR_OK;
	uint8_t		DATA[4];

	*duty = 0;

	if (chan > PCA9xxx__PWM16)		{ return ERROR_RANGE; } // Unknown channel

	err = PCA9685_Read(pCpnt->cfg.slave_inst, DATA, LED_OFFSET_L(chan), sizeof(DATA));

	if (DATA[1] & DefBitFullOnOff)
	{
		*duty = 255;
	}
	else if (!(DATA[3] & DefBitFullOnOff))
	{
		const int16_t ONCount = MAKEWORD(DATA[0], DATA[1]);
		const int16_t OFFCount = MAKEWORD(DATA[2], DATA[3]);
		*duty = (uint8_t) (abs(OFFCount - ONCount + 1) / 16);
	}

	return err;
}


FctERR NONNULL__ PCA9685_PutVal(PCA9685_t * pCpnt, const PCA9xxx_chan chan, const uint16_t duty, const uint16_t delay)
{
	PCA9685_reg RegAddr;

	if ((duty > 4095) || (delay >= 4095))		{ return ERROR_RANGE; }

	if (chan == PCA96xx__ALL)					{ RegAddr = PCA9685__ALL_LED_ON_L; }	// All channels at once
	else if (chan <= PCA9xxx__PWM16)			{ RegAddr = LED_OFFSET_L(chan); }		// Regular channel
	else										{ return ERROR_RANGE; }					// Unknown channel

	uint16_t ONCount = delay;
	uint16_t OFFCount = (delay + duty) % 4095;

	if (duty == 0)			{ OFFCount |= 0x1000; }	// Off count set to 4096 to fill bit FullOnOff
	else if (duty == 4095)	{ ONCount |= 0x1000; }	// On count set to 4096 to fill bit FullOnOff

	const uint8_t DATA[4] = { LOBYTE(ONCount), HIBYTE(ONCount), LOBYTE(OFFCount), HIBYTE(OFFCount) };
	return PCA9685_Write(pCpnt->cfg.slave_inst, DATA, RegAddr, sizeof(DATA));
}


FctERR NONNULL__ PCA9685_SetVal(PCA9685_t * pCpnt, const PCA9xxx_chan chan)
{
	PCA9685_reg RegAddr;

	if (chan == PCA96xx__ALL)			{ RegAddr = PCA9685__ALL_LED_ON_L; }	// All channels at once
	else if (chan <= PCA9xxx__PWM16)	{ RegAddr = LED_OFFSET_L(chan); }		// Regular channel
	else								{ return ERROR_RANGE; }					// Unknown channel

	const uint8_t DATA[4] = { 0, DefBitFullOnOff, 0, 0 };
	return PCA9685_Write(pCpnt->cfg.slave_inst, DATA, RegAddr, sizeof(DATA));
}


FctERR NONNULL__ PCA9685_ClrVal(PCA9685_t * pCpnt, const PCA9xxx_chan chan)
{
	PCA9685_reg RegAddr;

	if (chan == PCA96xx__ALL)			{ RegAddr = PCA9685__ALL_LED_ON_L; }	// All channels at once
	else if (chan <= PCA9xxx__PWM16)	{ RegAddr = LED_OFFSET_L(chan); }		// Regular channel
	else								{ return ERROR_RANGE; }					// Unknown channel

	const uint8_t DATA[4] = { 0, 0, 0, DefBitFullOnOff };
	return PCA9685_Write(pCpnt->cfg.slave_inst, DATA, RegAddr, sizeof(DATA));
}


/****************************************************************/
#endif
#endif
/****************************************************************/
