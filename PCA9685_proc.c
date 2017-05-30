/*!\file PCA9685_proc.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief PCA9685 Driver procedures code
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
#include <string.h>
/****************************************************************/
// TODO: more extensions like PCA9624


/****************************************************************/


FctERR PCA9685_Init_Sequence(void)
{
	FctERR	err;
	uint8_t Data[5];

	// Set Delay time & all led OFF
	Data[0] = DefValDelayON;	// ALL_LED_ON_L (1% delay to write once)
	Data[1] = 0x00U;			// ALL_LED_ON_H
	Data[2] = 0x00U;			// ALL_LED_OFF_L
	Data[3] = DefBitFullOnOff;	// ALL_LED_OFF_H (b4 pour LED full OFF)
	err = PCA9685_Write(Data, PCA9685__ALL_LED_ON_L, 4);
	if (err)	{ return err; }

	// MODE1: SLEEP + Respond to ALLCALL
	Data[0] = 0x11U;
	err = PCA9685_Write(Data, PCA9685__MODE1, 1);
	if (err)	{ return err; }

	// Send prescaler to obtain desired frequency (only in SLEEP)
	Data[0] = PCA9685_Get_PWM_Prescaler(PCA9685_FREQ);
	err = PCA9685_Write(Data, PCA9685__PRE_SCALE, 1);
	if (err)	{ return err; }
	// TODO: write frequency value in PCA9685.cfg.Freq

	// MODE1: Restart Enabled + Auto Increment + Respond to ALLCALL
	Data[0] = 0xA1U;
	err = PCA9685_Write(Data, PCA9685__MODE1, 1);
	return err;
}


/****************************************************************/


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


uint8_t PCA9685_Get_PWM_Prescaler(uint16_t freq)
{
	float pr;

	if ((freq > PCA9685_FREQ_HZ_MAX) || (freq < PCA9685_FREQ_HZ_MIN))	{ return 0x0CU; }	// case default: 500Hz

	pr = RoundFloat((25000000.0 / 4096.0) / freq);
	pr -= 1.0;

	// no need of this test as 1700Hz entry results in 3 for pr
	//if ((uint8_t) pr < 3)	return 0x03U;	// prescaler value can't be below 3

	return (uint8_t) pr;	// return prescaler value
}


/****************************************************************/
#endif
#endif
/****************************************************************/
