/*!\file PCA9624_proc.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief PCA9624 Driver procedures declarations
** \details PCA9624: 8-bit Fm+ I2C-bus 100mA 40V LED driver
**/
/****************************************************************/
#ifndef __PCA9624_PROC_H__
	#define __PCA9624_PROC_H__

#include "PCA9624.h"

#include "sarmfsw.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for PCA9624 peripheral
** \return FctERR - error code
**/
FctERR PCA9624_Init_Sequence(void);

/****************************************************************/
#endif
#endif	/* __PCA9624_PROC_H__ */
/****************************************************************/
