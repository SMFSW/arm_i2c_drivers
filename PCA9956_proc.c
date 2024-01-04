/*!\file PCA9956_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
** \brief PCA9956 Driver procedures
** \details PCA9956: 24-channel Fm+ I2C-bus 57 mA/20 V constant current LED driver
**/
/****************************************************************/
#include "PCA9956.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9956)
/****************************************************************/
// std libs
#include <math.h>
/****************************************************************/


PCA9956_t PCA9956[I2C_PCA9956_NB] = { 0 };


/****************************************************************/


__WEAK FctERR NONNULL__ PCA9956_Init_Sequence(PCA9956_t * const pCpnt)
{
	FctERR				err;
	uPCA9956_REG__MODE1	MODE1 = { 0 };

	// MODE1: SLEEP + Respond to ALLCALL
	MODE1.Bits.ALLCALL = true;
	MODE1.Bits.SLEEP = true;
	err = PCA9956_Write(pCpnt->cfg.slave_inst, &MODE1.Byte, PCA9956__MODE1, sizeof(MODE1));
	if (err)	{ return err; }

	err |= PCA9956_Set_Auto_Increment(pCpnt, PCA9xxx__AUTO_INC_ALL);

	// LEDOUTx: allow individual brightness control for all channels
	err |= PCA9956_Set_Mode_LEDs(pCpnt, 0xFFFFFF, PCA9xxx__INDIV_BRIGHT);

	// Set IREF to 1/4 of max capability for all channels
	err |= PCA9956_Set_IREF(pCpnt, PCA9xxx__ALL, 64);

	// Set PWM shifting to 6 clock cycles
	err |= PCA9956_Set_Offset(pCpnt, PCA9956__OFFSET_6_CLOCK_CYCLE);

	// MODE1: Restart Enabled + Auto Increment
	MODE1.Byte |= pCpnt->cfg.auto_inc;
	MODE1.Bits.SLEEP = false;
	return PCA9956_Write(pCpnt->cfg.slave_inst, &MODE1.Byte, PCA9956__MODE1, sizeof(MODE1));
}


FctERR PCA9956_calc_IREF(uint8_t * const pIREF, const float Rext, const float current)
{
	// IO(LEDn) (mA) = IREFx * (0.9 / 4) / Rext (KOhm)
	const uint32_t iref = (current * Rext * (4 / 0.9f));

	*pIREF = min(255, iref);

	return (iref > 255) ? ERROR_OVERFLOW : ERROR_OK;
}

/****************************************************************/


/****************************************************************/
#endif
#endif
/****************************************************************/
