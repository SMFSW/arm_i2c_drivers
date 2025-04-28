/*!\file PCA9952_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief PCA9952 Driver procedures
** \details PCA9952: 16-channel Fm+ I2C-bus 57 mA constant current LED driver
**/
/****************************************************************/
#include "PCA9952.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9952)
/****************************************************************/
// std libs
#include <math.h>
/****************************************************************/


PCA9952_t PCA9952[I2C_PCA9952_NB] = { 0 };


/****************************************************************/


__WEAK FctERR NONNULL__ PCA9952_Init_Sequence(PCA9952_t * const pCpnt)
{
	FctERR				err;
	uPCA9952_REG__MODE1	MODE1 = { 0 };

	// MODE1: SLEEP + Respond to ALLCALL
	MODE1.Bits.ALLCALL = true;
	MODE1.Bits.SLEEP = true;
	err = PCA9952_Write(pCpnt->cfg.slave_inst, &MODE1.Byte, PCA9952__MODE1, sizeof(MODE1));
	if (err)	{ return err; }

	err |= PCA9952_Set_Auto_Increment(pCpnt, PCA9xxx__AUTO_INC_ALL);

	// LEDOUTx: allow individual brightness control for all channels
	err |= PCA9952_Set_Mode_LEDs(pCpnt, 0xFFFF, PCA9xxx__INDIV_BRIGHT);

	// Set IREF to 1/4 of max capability for all channels
	err |= PCA9952_Set_IREF(pCpnt, PCA9xxx__ALL, 64);

	// Set PWM shifting to 6 clock cycles
	err |= PCA9952_Set_Offset(pCpnt, PCA9952__OFFSET_6_CLOCK_CYCLE);

	// MODE1: Restart Enabled + Auto Increment
	MODE1.Byte |= pCpnt->cfg.auto_inc;
	MODE1.Bits.SLEEP = false;
	return PCA9952_Write(pCpnt->cfg.slave_inst, &MODE1.Byte, PCA9952__MODE1, sizeof(MODE1));
}


FctERR PCA9952_calc_IREF(uint8_t * const pIREF, const float Rext, const float current)
{
	// IO(LEDn) (mA) = IREFx * (0.9 / 4) / Rext (KOhm)
	const uint32_t iref = (current * Rext * (4 / 0.9f));

	*pIREF = min(255UL, iref);

	return (iref > 255UL) ? ERROR_OVERFLOW : ERROR_OK;
}

/****************************************************************/


/****************************************************************/
#endif
#endif
/****************************************************************/
