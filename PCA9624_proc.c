/*!\file PCA9624_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief PCA9624 Driver procedures
** \details PCA9624: 8-bit Fm+ I2C-bus 100mA 40V LED driver
**/
/****************************************************************/
#include "PCA9624.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9624)
/****************************************************************/


PCA9624_t PCA9624[I2C_PCA9624_NB] = { 0 };


/****************************************************************/


__WEAK FctERR NONNULL__ PCA9624_Init_Sequence(PCA9624_t * const pCpnt)
{
	FctERR			err;
	const uint8_t	DATA = 0x81U;

	// MODE1: Auto Increment + Respond to ALLCALL + Normal mode
	err = PCA9624_Write(pCpnt->cfg.slave_inst, &DATA, PCA9624__MODE1, sizeof(DATA));
	if (err != ERROR_OK)	{ goto ret; }

	// LEDOUTx: allow individual brightness control for all channels
	err = PCA9624_Set_Mode_LEDs(pCpnt, 0xFFU, PCA9xxx__INDIV_BRIGHT);

	ret:
	return err;
}


/****************************************************************/


/****************************************************************/
#endif
#endif
/****************************************************************/
