/*!\file AT42QT1244_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief AT42QT1244 Driver procedures
** \details AT42QT1244: 24-key QMatrix FMEA IEC/EN/UL60730 Touch Sensor
**/
/****************************************************************/
#ifndef __AT42QT1244_PROC_H__
	#define __AT42QT1244_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "AT42QT1244.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/
// TODO: doxygen


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct AT42QT1244_t
** \brief AT42QT1244 user interface struct
**/
typedef struct AT42QT1244_t {
	struct {
	I2C_slave_t *	slave_inst;			//!< Slave structure
	} cfg;
} AT42QT1244_t;

extern AT42QT1244_t AT42QT1244[I2C_AT42QT1244_NB];	//!< AT42QT1244 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for AT42QT1244 peripheral
** \weak AT42QT1244 Init sequence may be user implemented if custom initialization sequence needed
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \return FctERR - error code
**/
FctERR NONNULL__ AT42QT1244_Init_Sequence(AT42QT1244_t * pCpnt);

FctERR NONNULL__ AT42QT1244_Calibrate_Freq_Offset(AT42QT1244_t * pCpnt);


FctERR NONNULL__ AT42QT1244_Calibrate_Low_Level(AT42QT1244_t * pCpnt);


FctERR NONNULL__ AT42QT1244_Calibrate_All_Keys(AT42QT1244_t * pCpnt);


FctERR NONNULL__ AT42QT1244_Calibrate_Key(AT42QT1244_t * pCpnt, uint8_t Key);


/*!\brief Handler for AT42QT1244 peripheral
** \weak AT42QT1244 handler may be user implemented to suit custom needs
** \note May be called periodically to handle AT42QT1244 tasks
** \note Alternately may be called when event occurs on AT42QT1244 pin
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \return FctERR - error code
**/
FctERR NONNULL__ AT42QT1244_handler(AT42QT1244_t * pCpnt);


/*!\brief Handler for all AT42QT1244 peripherals
** \note May be called periodically to handle all AT42QT1244 tasks
**/
__INLINE void INLINE__ AT42QT1244_handler_all(void) {
	for (AT42QT1244_t * pCpnt = AT42QT1244 ; pCpnt < &AT42QT1244[I2C_AT42QT1244_NB] ; pCpnt++) {
		AT42QT1244_handler(pCpnt); }
}


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __AT42QT1244_PROC_H__ */
/****************************************************************/
