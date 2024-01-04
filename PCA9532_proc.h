/*!\file PCA9532_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
** \brief PCA9532 Driver procedures
** \details PCA9532: 16-bit I2C-bus LED dimmer
**/
/****************************************************************/
#ifndef __PCA9532_PROC_H__
	#define __PCA9532_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "PCA9532.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define	PCA9532_NB_CHANNELS		8			//!< PCA9532 Number of channels


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct PCA9532_t
** \brief PCA9532 user interface struct
**/
typedef struct PCA9532_t {
	uPCA9532_REG__LS	LS;				//!< LED selector registers
	struct {
	I2C_slave_t *		slave_inst;		//!< Slave structure
	PeripheralGPIO_t	RST_GPIO;		//!< Reset GPIO struct
	PCA95xx_reg_inc		auto_inc;		//!< Auto increment configuration
	float				Freq0;			//!< Frequency set in PSC0 (in Hz)
	float				Duty0;			//!< Duty Cycle set in PWM0 (in Hz)
	float				Freq1;			//!< Frequency set in PSC1 (in Hz)
	float				Duty1;			//!< Duty Cycle set in PWM1 (in Hz)
	} cfg;
} PCA9532_t;

extern PCA9532_t	PCA9532[I2C_PCA9532_NB];	//!< PCA9532 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for PCA9532 peripheral
** \weak PCA9532 Init sequence may be user implemented if custom initialization sequence needed
** \param[in] pCpnt - Pointer to PCA9532 component
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9532_Init_Sequence(PCA9532_t * const pCpnt);

/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif	/* __PCA9532_PROC_H__ */
/****************************************************************/
