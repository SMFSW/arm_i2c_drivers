/*!\file PCA9685_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief PCA9685 Driver extensions
** \details PCA9685: 16-channel, 12-bit PWM Fm+ I2C-bus LED controller
**/
/****************************************************************/
#include "PCA9685.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9685)
/****************************************************************/
// std libs
#include <math.h>
/****************************************************************/
// TODO: more extensions like PCA9624


FctERR PCA9685_Reset(const bool all)
{
	uint8_t Data = 0x06;
	PCA9685_hal.status = HAL_I2C_Master_Transmit(PCA9685_hal.cfg.bus_inst, all ? PCA96xx_GENERAL_CALL_ADDR : PCA9685_hal.cfg.addr, &Data, 1, PCA9685_hal.cfg.timeout);
	return HALERRtoFCTERR(PCA9685_hal.status);
}


FctERR PCA9685_ReadRegister(const PCA9685_reg reg, uint8_t * val)
{
	*val = 0;

	if ((reg >= PCA9685__LED15_OFF_H) && (reg <= PCA9685__ALL_LED_ON_L))	{ return ERROR_RANGE; } // Unknown channel

	return PCA9685_Read(val, reg, 1);
}


FctERR PCA9685_ReadVal256(const PCA96xx_chan chan, uint8_t * duty)
{
	FctERR		err = ERROR_OK;
	uint16_t	ONCount = 0, OFFCount = 0;
	uint8_t		Data[4];

	*duty = 0;

	if (!((chan >= PCA96xx__PWM1) && (chan <= PCA96xx__PWM16)))	{ return ERROR_RANGE; } // Unknown channel

	err = PCA9685_Read(Data, LED_OFFSET_L(chan), 4);

	if (Data[1] & DefBitFullOnOff)
	{
		*duty = 255;
	}
	else if (!(Data[3] & DefBitFullOnOff))
	{
		ONCount = MAKEWORD(Data[0], Data[1]);
		OFFCount = MAKEWORD(Data[2], Data[3]);
		*duty = (uint8_t) ((OFFCount - ONCount + 1) / 16);
	}

	return err;
}


FctERR PCA9685_ReadVal1024(const PCA96xx_chan chan, uint16_t * duty)
{
	FctERR		err;
	uint16_t	ONCount = 0, OFFCount = 0;
	uint8_t		Data[4];

	*duty = 0;

	if (!((chan >= PCA96xx__PWM1) && (chan <= PCA96xx__PWM16)))	{ return ERROR_RANGE; } // Unknown channel

	err = PCA9685_Read(Data, LED_OFFSET_L(chan), 4);

	if (Data[1] & DefBitFullOnOff)
	{
		*duty = 1023;
	}
	else if (!(Data[3] & DefBitFullOnOff))
	{
		ONCount = MAKEWORD(Data[0], Data[1]);
		OFFCount = MAKEWORD(Data[2], Data[3]);
		*duty = (uint16_t) ((OFFCount - ONCount + 1) / 4);
	}

	return err;
}


FctERR PCA9685_PutVal256(const PCA96xx_chan chan, const uint8_t duty)
{
	uint16_t	RegAddr, OFFCount = 0;
	uint8_t		Data[4];

	if ((chan >= PCA96xx__PWM1) && (chan <= PCA96xx__PWM16))	{ RegAddr = LED_OFFSET_H(chan); }		// Regular channel
	else if (chan == PCA96xx__ALL)								{ RegAddr = PCA9685__ALL_LED_ON_L; }	// All channels at once
	else														{ return ERROR_RANGE; }					// Unknown channel

	if (!duty)
	{// Full OFF
		Data[1] = 0;				// xxx_LED_ON_H (LED not fully ON)
		OFFCount = 4096;
	}
	else if (duty == 255)
	{// Full ON
		Data[1] = DefBitFullOnOff;	// xxx_LED_ON_H (LED fully ON)
	}
	else
	{// DIM
		Data[1] = 0;				// xxx_LED_ON_H (LED not fully ON)
		OFFCount = (uint16_t) ((DefValDelayON - 1) + (duty * 16));		// 4096 = (DelayTime - 1) + ((Duty(255)) * 16)
	}

	Data[0] = 0;					// xxx_LED_ON_H
	Data[1] = LOBYTE(OFFCount);		// xxx_LED_OFF_L
	Data[2] = HIBYTE(OFFCount);		// xxx_LED_OFF_H

	return PCA9685_Write(Data, RegAddr, 3);
}


FctERR PCA9685_PutVal1024(const PCA96xx_chan chan, const uint16_t duty)
{
	uint16_t	RegAddr, OFFCount = 0;
	uint8_t		Data[4];

	if (duty > 1023)	{ return ERROR_VALUE; }

	if ((chan >= PCA96xx__PWM1) && (chan <= PCA96xx__PWM16))	{ RegAddr = LED_OFFSET_H(chan); }		// Regular channel
	else if (chan == PCA96xx__ALL)								{ RegAddr = PCA9685__ALL_LED_ON_L; }	// All channels at once
	else														{ return ERROR_RANGE; }					// Unknown channel

	if (!duty)
	{// Full OFF
		Data[1] = 0;				// xxx_LED_ON_H (LED not fully ON)
		OFFCount = 4096;
	}
	else if (duty == 1023)
	{// Full ON
		Data[1] = DefBitFullOnOff;	// xxx_LED_ON_H (LED fully ON)
	}
	else
	{// DIM
		Data[1] = 0;				// xxx_LED_ON_H (LED not fully ON)
		OFFCount = (uint16_t) ((DefValDelayON - 1) + (duty * 4));		// 4096 = (DelayTime - 1) + ((Duty(1023)) * 4)
	}

	Data[0] = 0;					// xxx_LED_ON_H
	Data[1] = LOBYTE(OFFCount);		// xxx_LED_OFF_L
	Data[2] = HIBYTE(OFFCount);		// xxx_LED_OFF_H

	return PCA9685_Write(Data, RegAddr, 3);
}


FctERR PCA9685_SetVal(const PCA96xx_chan chan)
{
	uint16_t	RegAddr;
	uint8_t		Data[4];

	if ((chan >= PCA96xx__PWM1) && (chan <= PCA96xx__PWM16))	{ RegAddr = LED_OFFSET_H(chan); }		// Regular channel
	else if (chan == PCA96xx__ALL)								{ RegAddr = PCA9685__ALL_LED_ON_L; }	// All channels at once
	else														{ return ERROR_RANGE; }					// Unknown channel

	Data[0] = DefBitFullOnOff;	// xxx_LED_ON_H (b4 pour LED full OFF)
	Data[1] = 0x00U;			// xxx_LED_OFF_L
	Data[2] = 0x00U;			// xxx_LED_OFF_H

	return PCA9685_Write(Data, RegAddr, 3);
}


FctERR PCA9685_ClrVal(const PCA96xx_chan chan)
{
	uint16_t	RegAddr;
	uint8_t		Data[4];

	if ((chan >= PCA96xx__PWM1) && (chan <= PCA96xx__PWM16))	{ RegAddr = LED_OFFSET_H(chan); }		// Regular channel
	else if (chan == PCA96xx__ALL)								{ RegAddr = PCA9685__ALL_LED_ON_L; }	// All channels at once
	else														{ return ERROR_RANGE; }					// Unknown channel

	Data[0] = 0x00U;			// xxx_LED_ON_H
	Data[1] = 0x00U;			// xxx_LED_OFF_L
	Data[2] = DefBitFullOnOff;	// xxx_LED_OFF_H (b4 pour LED full OFF)

	return PCA9685_Write(Data, RegAddr, 3);
}


/****************************************************************/
#endif
#endif
/****************************************************************/
