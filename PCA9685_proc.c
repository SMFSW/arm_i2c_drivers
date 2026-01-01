/*!\file PCA9685_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
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


PCA9685_t PCA9685[I2C_PCA9685_NB] = { 0 };


/****************************************************************/


__WEAK FctERR NONNULL__ PCA9685_Init_Sequence(PCA9685_t * const pCpnt)
{
	FctERR				err;
	uPCA9685_REG__MODE1	MODE1 = { 0 };

	UNUSED_RET PCA9685_Set_Clock_Freq(pCpnt, PCA9685_CLOCK_FREQ);	// Assuming internal clock used

	// Set Delay time & all led OFF
	uint8_t DATA[4] = { 0, 0, 0, DefBitFullOnOff };
	err = PCA9685_Write(pCpnt->cfg.slave_inst, DATA, PCA9685__ALL_LED_ON_L, sizeof(DATA));
	if (err != ERROR_OK)	{ goto ret; }

	// MODE1: SLEEP + Respond to ALLCALL
	MODE1.Bits.ALLCALL = true;
	MODE1.Bits.SLEEP = true;
	err = PCA9685_Write(pCpnt->cfg.slave_inst, &MODE1.Byte, PCA9685__MODE1, sizeof(MODE1));
	if (err != ERROR_OK)	{ goto ret; }

	// Send prescaler to obtain desired frequency (only in SLEEP)
	UNUSED_RET PCA9685_Freq_To_Byte(pCpnt, &DATA[0], PCA9685_DEF_FREQ);
	err = PCA9685_Write(pCpnt->cfg.slave_inst, DATA, PCA9685__PRE_SCALE, 1U);
	if (err != ERROR_OK)	{ goto ret; }
	pCpnt->cfg.Frequency = PCA9685_Byte_To_Freq(pCpnt, DATA[0]);

	// MODE1: Restart Enabled + Auto Increment
	MODE1.Bits.RESTART = true;
	MODE1.Bits.AI = true;
	MODE1.Bits.SLEEP = false;
	err = PCA9685_Write(pCpnt->cfg.slave_inst, &MODE1.Byte, PCA9685__MODE1, sizeof(MODE1));

	ret:
	return err;
}


/****************************************************************/


FctERR NONNULL__ PCA9685_Set_Clock_Freq(PCA9685_t * const pCpnt, const uint32_t freq)
{
	FctERR err = ERROR_OK;

	if (freq > PCA9685_MAX_CLOCK_FREQ)	{ err = ERROR_VALUE; }
	else								{ pCpnt->cfg.Clock = freq; }

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
