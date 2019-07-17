/*!\file PCA9685_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
** \brief PCA9685 Driver procedures
** \details PCA9685: 16-channel, 12-bit PWM Fm+ I2C-bus LED controller
**/
/****************************************************************/
#include "PCA9685.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9685)
/****************************************************************/
// std libs
#include <math.h>
/****************************************************************/


PCA9685_t PCA9685[I2C_PCA9685_NB];


/****************************************************************/


__WEAK FctERR NONNULL__ PCA9685_Init_Sequence(PCA9685_t * pCpnt)
{
	FctERR				err;
	uint8_t				DATA[4];
	uPCA9685_REG__MODE1	MODE1 = { 0 };

	// Set Delay time & all led OFF
	DATA[0] = DefValDelayON;	// ALL_LED_ON_L (1% delay to write once)
	DATA[1] = 0x00U;			// ALL_LED_ON_H
	DATA[2] = 0x00U;			// ALL_LED_OFF_L
	DATA[3] = DefBitFullOnOff;	// ALL_LED_OFF_H (b4 pour LED full OFF)
	err = PCA9685_Write(pCpnt->cfg.slave_inst, DATA, PCA9685__ALL_LED_ON_L, sizeof(DATA));
	if (err)	{ return err; }

	// MODE1: SLEEP + Respond to ALLCALL
	MODE1.Bits.ALLCALL = true;
	MODE1.Bits.SLEEP = true;
	err = PCA9685_Write(pCpnt->cfg.slave_inst, &MODE1.Byte, PCA9685__MODE1, sizeof(MODE1));
	if (err)	{ return err; }

	// Send prescaler to obtain desired frequency (only in SLEEP)
	DATA[0] = PCA9685_Get_PWM_Prescaler(PCA9685_FREQ);
	err = PCA9685_Write(pCpnt->cfg.slave_inst, DATA, PCA9685__PRE_SCALE, 1);
	if (err)	{ return err; }
	pCpnt->cfg.Frequency = PCA9685_FREQ;

	// MODE1: Restart Enabled + Auto Increment
	MODE1.Bits.RESTART = true;
	MODE1.Bits.AI = true;
	MODE1.Bits.SLEEP = false;
	return PCA9685_Write(pCpnt->cfg.slave_inst, &MODE1.Byte, PCA9685__MODE1, sizeof(MODE1));
}


/****************************************************************/


/*!\brief Algorithm to round float
** \warning Very long for not so much more
** \note Use with caution
**
** \param [in] val - Value to round
** \return Round value
**/
static float RoundFloat(const float val)
{
	double pd, calc;

	calc = modf(val, &pd);
	if (calc > 0.5f)	{ pd += 1.0f; }

	return (float) pd;
}


uint8_t PCA9685_Get_PWM_Prescaler(const uint16_t freq)
{
	if ((freq > PCA9685_FREQ_HZ_MAX) || (freq < PCA9685_FREQ_HZ_MIN))	{ return 0x0CU; }	// case default: 500Hz

	return (uint8_t) (RoundFloat((25000000.0f / 4096.0f) / freq) - 1.0f);
}


/****************************************************************/
#endif
#endif
/****************************************************************/
