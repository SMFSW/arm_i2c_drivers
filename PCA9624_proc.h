/*!\file PCA9624_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief PCA9624 Driver procedures
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
// Section: Types
// *****************************************************************************
/*!\struct PCA9624_proc
** \brief PCA9624 user interface struct
**/
typedef struct PCA9624_proc {
	struct {
	I2C_slave *	slave_inst;		//!< Slave structure
	} cfg;
} PCA9624_proc;

extern PCA9624_proc	PCA9624;	//!< PCA9624 User structure


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
