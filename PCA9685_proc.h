/*!\file PCA9685_proc.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief PCA9685 Driver procedures declarations
** \details PCA9685: 16-channel, 12-bit PWM Fm+ I2C-bus LED controller
**/
/****************************************************************/
#ifndef __PCA9685_PROC_H__
	#define __PCA9685_PROC_H__

#include "sarmfsw.h"
#include "PCA9685.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define PCA9685_FREQ			500			//!< Frequency to affect to PCA9685 register (in Hz, range from 26 to 1700Hz)
//\warning Outputs frequency in Hz may range from 26 to 1700Hz

#define	PCA9685_FREQ_HZ_MIN		0x1AU		//!< Lower bound of interval for method PCA9685_Get_PWM_Prescaler
#define	PCA9685_FREQ_HZ_MAX		0x06A4U		//!< Upper bound of interval for method PCA9685_Get_PWM_Prescaler


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for PCA9685 peripheral
** \weak PCA9685 Init sequence may be user implemented if custom initialization sequence needed
** \return FctERR - ErrorCode
**/
__WEAK FctERR PCA9685_Init_Sequence(void);


/*!\brief Sets register value for PCA9685 according to desired output frequency
** \note PCA9685 internal oscillator used (at 25MHz)
** \warning if wrong frequency given, value for 500Hz will be returned
**
** \param [in] freq - Outputs frequency in Hz (from 26 to 1700Hz)
** \return prescaler register value for PCA9685
**/
uint8_t PCA9685_Get_PWM_Prescaler(uint16_t freq);


/****************************************************************/
#endif
#endif	/* __PCA9685_PROC_H__ */
/****************************************************************/
