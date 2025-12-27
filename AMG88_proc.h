/*!\file AMG88_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief AMG88 Driver procedures
** \details AMG88: Infrared Array Sensor (Grid-EYE)
**/
/****************************************************************/
#ifndef AMG88_PROC_H__
	#define AMG88_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "AMG88.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define AMG88_CHIP_ID		0x00		//!< AMG88 Chip ID to check against



// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct _AMG88_t
** \brief AMG88 user interface struct
**/
typedef struct _AMG88_t {
	uint16_t			Pixels[8][8];		//!< Grid-EYE current raw temperature
	uint16_t			Thermistor;			//!< Thermistor raw temperature
	struct {
	I2C_slave_t *		slave_inst;			//!< Slave structure
	PeripheralGPIO_t	INT_GPIO;			//!< Interrupt GPIO struct
	} cfg;
} AMG88_t;

extern AMG88_t AMG88[I2C_AMG88_NB];		//!< AMG88 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for AMG88 peripheral
** \weak AMG88 Init sequence may be user implemented if custom initialization sequence needed
** \param[in] pCpnt - Pointer to AMG88 component
** \return FctERR - error code
**/
FctERR NONNULL__ AMG88_Init_Sequence(AMG88_t * const pCpnt);


/*!\brief Handler for AMG88 peripheral
** \weak AMG88 handler may be user implemented to suit custom needs
** \note May be called periodically to handle AMG88 tasks
** \note Alternately may be called when event occurs on AMG88 pin (or by calling \ref AMG88_handler_it instead)
** \param[in] pCpnt - Pointer to AMG88 component
** \return FctERR - error code
**/
FctERR NONNULL__ AMG88_handler(AMG88_t * const pCpnt);

/*!\brief Handler for AMG88 peripheral GPIO interrupt
** \note \ref AMG88_INT_GPIO_Init has to be called at init before using interrupt handler function
** \weak AMG88 GPIO interrupt handler may be user implemented to suit custom needs
** \note May be called periodically to handle AMG88 tasks through interrupts
** \param[in] pCpnt - Pointer to AMG88 component
** \return FctERR - error code
**/
FctERR NONNULL__ AMG88_handler_it(AMG88_t * const pCpnt);

/*!\brief Handler for all AMG88 peripherals
** \note May be called periodically to handle all AMG88 tasks
** \return FctERR - error code
**/
FctERR AMG88_handler_all(void);

/*!\brief Handler for all AMG88 peripherals GPIO interrupt
** \note \ref AMG88_INT_GPIO_Init has to be called at init before using interrupt handler function
** \note May be called periodically to handle all AMG88 tasks
** \return FctERR - error code
**/
FctERR AMG88_handler_it_all(void);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* AMG88_PROC_H__ */
/****************************************************************/
