/*!\file NCA9595_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
** \brief NCA9595 Driver procedures
** \details NCA9595: Low-voltage 16-bit I²C and SMBus I/O expander
**/
/****************************************************************/
#ifndef __NCA9595_PROC_H__
	#define __NCA9595_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "NCA9595.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct NCA9595_t
** \brief NCA9595 user interface struct
**/
typedef struct NCA9595_t {
	uNCA9595_REG__IN	NCA9595_in;			//!< Latest read inputs
	uNCA9595_REG__OUT	NCA9595_out;		//!< Latest written outputs
	struct {
	I2C_slave_t *		slave_inst;			//!< Slave structure
	PeripheralGPIO_t	INT_GPIO;			//!< Interrupt GPIO struct
	uNCA9595_REG__CFG	NCA9595_Cfg;		//!< NCA9595 GPIO direction
	} cfg;
} NCA9595_t;

extern NCA9595_t NCA9595[I2C_NCA9595_NB];	//!< NCA9595 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for NCA9595 peripheral
** \weak NCA9595 Init sequence may be user implemented if custom initialization sequence needed
** \note Weak does nothing (keeping all IOs as inputs). Can be implemented with required need for outputs and/or interrupt pin.
** \param[in] pCpnt - Pointer to NCA9595 component
** \return FctERR - error code
**/
FctERR NONNULL__ NCA9595_Init_Sequence(NCA9595_t * const pCpnt);


/*!\brief Handler for NCA9595 peripheral
** \weak NCA9595 handler may be user implemented to suit custom needs
** \note Handler for NCA9595 is made for example (as its implementation depends mostly on I/O used)
** 		 Basically, gets inputs (if any declared) and write outputs to previously stored values (if any declared).
** \note May be called periodically to handle NCA9595 tasks
** \note Alternately may be called when event occurs on NCA9595 pin (or by calling \ref NCA9595_handler_it instead)
** \param[in] pCpnt - Pointer to NCA9595 component
** \return FctERR - error code
**/
FctERR NONNULL__ NCA9595_handler(NCA9595_t * const pCpnt);

/*!\brief Handler for NCA9595 peripheral GPIO interrupt
** \note \ref NCA9595_INT_GPIO_Init has to be called at init before using interrupt handler function
** \weak NCA9595 GPIO interrupt handler may be user implemented to suit custom needs
** \note May be called periodically to handle NCA9595 tasks through interrupts
** \param[in] pCpnt - Pointer to NCA9595 component
** \return FctERR - error code
**/
FctERR NONNULL__ NCA9595_handler_it(NCA9595_t * const pCpnt);


/*!\brief Handler for all NCA9595 peripherals
** \note May be called periodically to handle all NCA9595 tasks
**/
__INLINE void INLINE__ NCA9595_handler_all(void) {
	for (NCA9595_t * pCpnt = NCA9595 ; pCpnt < &NCA9595[I2C_NCA9595_NB] ; pCpnt++) {
		NCA9595_handler(pCpnt); }
}

/*!\brief Handler for all NCA9595 peripherals GPIO interrupt
** \note \ref NCA9595_INT_GPIO_Init has to be called at init before using interrupt handler function
** \note May be called periodically to handle all NCA9595 tasks
**/
__INLINE void INLINE__ NCA9595_handler_it_all(void) {
	for (NCA9595_t * pCpnt = NCA9595 ; pCpnt < &NCA9595[I2C_NCA9595_NB] ; pCpnt++) {
		NCA9595_handler_it(pCpnt); }
}


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* __NCA9595_PROC_H__ */
/****************************************************************/
