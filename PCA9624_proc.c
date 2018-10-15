/*!\file PCA9624_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief PCA9624 Driver procedures
** \details PCA9624: 8-bit Fm+ I2C-bus 100mA 40V LED driver
**/
/****************************************************************/
#include "PCA9624.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9624)
/****************************************************************/


PCA9624_t PCA9624[I2C_PCA9624_NB];


/****************************************************************/


__WEAK FctERR NONNULL__ PCA9624_Init_Sequence(PCA9624_t * pCpnt)
{
	uint8_t Data[2];

	// MODE1: Auto Increment + Respond to ALLCALL
	// MODE2: Dimming group control, Latch on STOP
	Data[0] = 0x81U;
	Data[1] = 0x20U;
	return PCA9624_Write(pCpnt->cfg.slave_inst, Data, PCA9624__MODE1, sizeof(Data));
}


/****************************************************************/


/****************************************************************/
#endif
#endif
/****************************************************************/
