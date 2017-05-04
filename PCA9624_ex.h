/*!\file PCA9624_ex.h
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief PCA9624 Driver extensions declarations (8 channels 8b PWM driver)
**/
/****************************************************************/
#ifndef __PCA9624_EX_H__
	#define __PCA9624_EX_H__

#include "PCA9624.h"

#include "sarmfsw.h"
#include CMSIS_INC
#include CMSIS_CFG

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// High level functions
FctERR PCA9624_Set_Latch(PCA96xx_latch latch);
FctERR PCA9624_Set_Mode_LED(PCA96xx_chan Chan, PCA9624_ledout mode);
FctERR PCA9624_Set_Mode_LEDs(uint8_t Chans, PCA9624_ledout mode);

FctERR PCA9624_Reset(bool all);
FctERR PCA9624_ReadRegister(PCA9624_reg Reg, uint8_t * Value);
FctERR PCA9624_ReadVal(PCA96xx_chan Chan, uint8_t * DutyCycle);
FctERR PCA9624_PutVal(PCA96xx_chan Chan, uint8_t DutyCycle);
FctERR PCA9624_SetVal(PCA96xx_chan Chan);
FctERR PCA9624_ClrVal(PCA96xx_chan Chan);


/****************************************************************/
#endif
#endif	/* __PCA9624_EX_H__ */
/****************************************************************/
