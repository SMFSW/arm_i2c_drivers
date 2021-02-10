/*!\file AMG88_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief AMG88 Driver procedures
** \details AMG88: Infrared Array Sensor (Grid-EYE)
**/
/****************************************************************/
#ifndef __AMG88_PROC_H__
	#define __AMG88_PROC_H__

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
/*!\struct AMG88_t
** \brief AMG88 user interface struct
**/
typedef struct AMG88_t {
	uint16_t		Pixels[8][8];		//!< Grid-EYE current raw temperature
	uint16_t		Thermistor;			//!< Thermistor raw temperature
	struct {
	I2C_slave_t *	slave_inst;			//!< Slave structure
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
FctERR NONNULL__ AMG88_Init_Sequence(AMG88_t * pCpnt);


/*!\brief Handler for AMG88 peripheral
** \weak AMG88 handler may be user implemented to suit custom needs
** \note May be called periodically to handle AMG88 tasks
** \note Alternately may be called when event occurs on AMG88 pin
** \param[in] pCpnt - Pointer to AMG88 component
** \return FctERR - error code
**/
FctERR NONNULL__ AMG88_handler(AMG88_t * pCpnt);


/*!\brief Handler for all AMG88 peripherals
** \note May be called periodically to handle all AMG88 tasks
**/
__INLINE void INLINE__ AMG88_handler_all(void) {
	for (AMG88_t * pCpnt = AMG88 ; pCpnt < &AMG88[I2C_AMG88_NB] ; pCpnt++) {
		AMG88_handler(pCpnt); }
}


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* __AMG88_PROC_H__ */
/****************************************************************/
