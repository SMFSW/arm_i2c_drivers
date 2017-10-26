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

#include "sarmfsw.h"
#include "PCA9624.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for PCA9624 peripheral
** \weak PCA9624 Init sequence may be user implemented if custom initialization sequence needed
** \return FctERR - error code
**/
__WEAK FctERR PCA9624_Init_Sequence(void);

/****************************************************************/
#endif
#endif	/* __PCA9624_PROC_H__ */
/****************************************************************/
