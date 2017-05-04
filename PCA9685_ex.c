/*!\file PCA9685_ex.c
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief PCA9685 Driver extensions code (16 channels 16b PWM driver)
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


extern I2C_slave PCA9685;


/*!\brief Algorithm to round float
** \warning Very long for not so much more
** \note Use with caution
**
** \param [in] Val - Value to round
** \return Round value
**/
static float RoundFloat(float Val)
{
	double	pd, Calc;

	Calc = modf(Val, &pd);
	if (Calc > 0.5f)	{ pd += 1.0f; }

	return (float) pd;
}


/*!\brief Sets register value for PCA9685 according to desired output frequency
** \note PCA9685 internal oscillator used (at 25MHz)
** \warning if wrong frequency given, value for 500Hz will be returned
**
** \param [in] fr - Outputs frequency in Hz (from 26 to 1700Hz)
** \return prescaler register value for PCA9685
**/
uint8_t PCA9685_Get_PWM_Prescaler(uint16_t fr)
{
	float p;

	if ((fr > PCA9685_FREQ_HZ_MAX) || (fr < PCA9685_FREQ_HZ_MIN))	{ return 0x0CU; }	// case default: 500Hz

	p = RoundFloat((25000000.0 / 4096.0) / fr);
	p -= 1.0;

	// no need of this test as 1700Hz entry results in 3 for p
	//if ((uint8_t) p < 3)	return 0x03U;	// prescaler value can't be below 3

	return (uint8_t) p;	// return prescaler value
}


/****************************************************************/
/****************************************************************/


/*!\brief Reset of the PCA9685 peripheral
** \param [in] all - Reset all devices
**/
FctERR PCA9685_Reset(bool all)
{
	uint8_t Data = 0x06;
	PCA9685.status = HAL_I2C_Master_Transmit(PCA9685.cfg.inst, all ? PCA96xx_GENERAL_CALL_ADDR : PCA9685.cfg.addr, &Data, 1, PCA9685.cfg.timeout);
	return HALERRtoFCTERR(PCA9685.status);
}



/*!\brief Reads I2C register from PCA9685
**
** \param [in] Reg - Register address to read from
** \param [in,out] Value - Pointer to the data for receive
** \return FctERR - ErrorCode
** \retval ERR_OK - OK
** \retval ERR_RANGE - Wrong channel number
** \retval Some others possible from called func
**/
FctERR PCA9685_ReadRegister(PCA9685_reg Reg, uint8_t * Value)
{
	*Value = 0;

	if ((Reg >= PCA9685__LED15_OFF_H) && (Reg <= PCA9685__ALL_LED_ON_L))	{ return ERR_RANGE; } // Unknown channel

	return PCA9685_Read(Value, Reg, 1);
}


/*!\brief Reads I2C lighting values from a LED (4 bytes) and Computes the corresponding duty cycle value (uint8_t)
**
** \param [in] Channel - Channel number (1 to 16)
** \param [in,out] DutyCycle - Pointer to the data for receive coded on a uint8_t
** \return FctERR - ErrorCode
** \retval ERR_OK - OK
** \retval ERR_RANGE - Wrong channel number
** \retval Some others possible from called func
**/
FctERR PCA9685_ReadVal256(PCA96xx_chan Channel, uint8_t * DutyCycle)
{
	FctERR		err = ERR_OK;
	uint16_t	ONCount = 0, OFFCount = 0;
	uint8_t		Data[4];

	*DutyCycle = 0;

	if (!((Channel >= PCA96xx__PWM1) && (Channel <= PCA96xx__PWM16)))	{ return ERR_RANGE; } // Unknown channel

	err = PCA9685_Read(Data, LED_OFFSET_L(Channel), 4);

	if (Data[1] & DefBitFullOnOff)
	{
		*DutyCycle = 255;
	}
	else if (!(Data[3] & DefBitFullOnOff))
	{
		ONCount = MAKEWORD(Data[0], Data[1]);
		OFFCount = MAKEWORD(Data[2], Data[3]);
		*DutyCycle = (uint8_t) ((OFFCount - ONCount + 1) / 16);
	}

	return err;
}


/*!\brief Reads I2C lighting values from a LED (4 bytes) and Computes the corresponding duty cycle value (uint16_t)
**
** \param [in] Channel - Channel number (1 to 16)
** \param [in,out] DutyCycle - Pointer to the data for receive coded on a uint16_t
** \return FctERR - ErrorCode
** \retval ERR_OK - OK
** \retval ERR_RANGE - Wrong channel number
** \retval Some others possible from called func
**/
FctERR PCA9685_ReadVal1024(PCA96xx_chan Channel, uint16_t * DutyCycle)
{
	FctERR		err;
	uint16_t	ONCount = 0, OFFCount = 0;
	uint8_t		Data[4];

	*DutyCycle = 0;

	if (!((Channel >= PCA96xx__PWM1) && (Channel <= PCA96xx__PWM16)))	{ return ERR_RANGE; } // Unknown channel

	err = PCA9685_Read(Data, LED_OFFSET_L(Channel), 4);

	if (Data[1] & DefBitFullOnOff)
	{
		*DutyCycle = 1023;
	}
	else if (!(Data[3] & DefBitFullOnOff))
	{
		ONCount = MAKEWORD(Data[0], Data[1]);
		OFFCount = MAKEWORD(Data[2], Data[3]);
		*DutyCycle = (uint16_t) ((OFFCount - ONCount + 1) / 4);
	}

	return err;
}


/*!\brief Computes and send I2C lighting values to apply to a particular or all channels of the PCA9685
**
** \param [in] Channel - Channel number (1 to 16 / PCA9685_ALLCALL can be used to address all channels at the same time)
** \param [in] DutyCycle - Duty cycle coded on a uint8_t
** \return FctERR - ErrorCode
** \retval ERR_OK - OK
** \retval ERR_RANGE - Wrong channel number
**/
FctERR PCA9685_PutVal256(PCA96xx_chan Channel, uint8_t DutyCycle)
{
	uint16_t	RegAddr, OFFCount = 0;
	uint8_t		Data[4];

	if ((Channel >= PCA96xx__PWM1) && (Channel <= PCA96xx__PWM16))	{ RegAddr = LED_OFFSET_H(Channel); }	// Regular channel
	else if (Channel == PCA96xx__ALL)								{ RegAddr = PCA9685__ALL_LED_ON_L; }	// All channels at once
	else															{ return ERR_RANGE; }					// Unknown channel

	if (!DutyCycle)
	{// Full OFF
		Data[1] = 0;				// xxx_LED_ON_H (LED not fully ON)
		OFFCount = 4096;
	}
	else if (DutyCycle == 255)
	{// Full ON
		Data[1] = DefBitFullOnOff;	// xxx_LED_ON_H (LED fully ON)
	}
	else
	{// DIM
		Data[1] = 0;				// xxx_LED_ON_H (LED not fully ON)
		OFFCount = (uint16_t) ((DefValDelayON - 1) + ((uint16_t) DutyCycle * 16));	// 4096 = (DelayTime - 1) + ((Duty(255)) * 16)
	}

	Data[0] = 0;					// xxx_LED_ON_H
	Data[1] = LOBYTE(OFFCount);		// xxx_LED_OFF_L
	Data[2] = HIBYTE(OFFCount);		// xxx_LED_OFF_H

	return PCA9685_Write(Data, RegAddr, 3);
}


/*!\brief Computes and send I2C lighting values to apply to a particular or all channels of the PCA9685
**
** \param [in] Channel - Channel number (1 to 16 / PCA9685_ALLCALL can be used to address all channels at the same time)
** \param [in] DutyCycle - Duty cycle coded on a uint16_t
** \return FctERR - ErrorCode
** \retval ERR_OK - OK
** \retval ERR_RANGE - Wrong channel number
** \retval ERR_VALUE - DutyCycle should be lower or equal to 1023
**/
FctERR PCA9685_PutVal1024(PCA96xx_chan Channel, uint16_t DutyCycle)
{
	uint16_t	RegAddr, OFFCount = 0;
	uint8_t		Data[4];

	if (DutyCycle > 1023)	{ return ERR_VALUE; }

	if ((Channel >= PCA96xx__PWM1) && (Channel <= PCA96xx__PWM16))	{ RegAddr = LED_OFFSET_H(Channel); }	// Regular channel
	else if (Channel == PCA96xx__ALL)								{ RegAddr = PCA9685__ALL_LED_ON_L; }	// All channels at once
	else															{ return ERR_RANGE; }					// Unknown channel

	if (!DutyCycle)
	{// Full OFF
		Data[1] = 0;				// xxx_LED_ON_H (LED not fully ON)
		OFFCount = 4096;
	}
	else if (DutyCycle == 1023)
	{// Full ON
		Data[1] = DefBitFullOnOff;	// xxx_LED_ON_H (LED fully ON)
	}
	else
	{// DIM
		Data[1] = 0;				// xxx_LED_ON_H (LED not fully ON)
		OFFCount = (uint16_t) ((DefValDelayON - 1) + ((uint16_t) DutyCycle * 4));	// 4096 = (DelayTime - 1) + ((Duty(1023)) * 4)
	}

	Data[0] = 0;					// xxx_LED_ON_H
	Data[1] = LOBYTE(OFFCount);		// xxx_LED_OFF_L
	Data[2] = HIBYTE(OFFCount);		// xxx_LED_OFF_H

	return PCA9685_Write(Data, RegAddr, 3);
}

/*!\brief Sends I2C lighting ON values to apply to a particular or all channels of the PCA9685
**
** \param [in] Channel - Channel number (1 to 16 / PCA9685_ALLCALL can be used to address all channels at the same time)
** \return FctERR - ErrorCode
** \retval ERR_OK - OK
** \retval ERR_RANGE - Wrong channel number
**/
FctERR PCA9685_SetVal(PCA96xx_chan Channel)
{
	uint16_t	RegAddr;
	uint8_t		Data[4];

	if ((Channel >= PCA96xx__PWM1) && (Channel <= PCA96xx__PWM16))	{ RegAddr = LED_OFFSET_H(Channel); }	// Regular channel
	else if (Channel == PCA96xx__ALL)								{ RegAddr = PCA9685__ALL_LED_ON_L; }	// All channels at once
	else															{ return ERR_RANGE; }					// Unknown channel

	Data[0] = DefBitFullOnOff;	// xxx_LED_ON_H (b4 pour LED full OFF)
	Data[1] = 0x00U;			// xxx_LED_OFF_L
	Data[2] = 0x00U;			// xxx_LED_OFF_H

	return PCA9685_Write(Data, RegAddr, 3);
}


/*!\brief Sends I2C PWm OFF values to apply to a particular or all channels of the PCA9685
**
** \param [in] Channel - Channel number (1 to 16 / PCA9685_ALLCALL can be used to address all channels at the same time)
** \return FctERR - ErrorCode
** \retval ERR_OK - OK
** \retval ERR_RANGE - Wrong channel number
**/
FctERR PCA9685_ClrVal(PCA96xx_chan Channel)
{
	uint16_t	RegAddr;
	uint8_t		Data[4];

	if ((Channel >= PCA96xx__PWM1) && (Channel <= PCA96xx__PWM16))	{ RegAddr = LED_OFFSET_H(Channel); }	// Regular channel
	else if (Channel == PCA96xx__ALL)								{ RegAddr = PCA9685__ALL_LED_ON_L; }	// All channels at once
	else															{ return ERR_RANGE; }					// Unknown channel

	Data[0] = 0x00U;			// xxx_LED_ON_H
	Data[1] = 0x00U;			// xxx_LED_OFF_L
	Data[2] = DefBitFullOnOff;	// xxx_LED_OFF_H (b4 pour LED full OFF)

	return PCA9685_Write(Data, RegAddr, 3);
}


/****************************************************************/
#endif
#endif
/****************************************************************/
