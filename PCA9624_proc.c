/*!\file PCA9624_t.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief PCA9624 Driver procedures
** \details PCA9624: 8-bit Fm+ I2C-bus 100mA 40V LED driver
**/
/****************************************************************/
#include "PCA9624.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9624)
/****************************************************************/


PCA9624_t PCA9624 = { { &PCA9624_hal } };


/****************************************************************/


__WEAK FctERR PCA9624_Init_Sequence(void)
{
	uint8_t Data[2];

	// MODE1: Auto Increment + Respond to ALLCALL
	// MODE2: Dimming group control, Latch on STOP
	Data[0] = 0x81U;
	Data[1] = 0x20U;
	return PCA9624_Write(Data, PCA9624__MODE1, sizeof(Data));
}


/****************************************************************/


/****************************************************************/
#endif
#endif
/****************************************************************/
