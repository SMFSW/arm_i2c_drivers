/*!\file AT42QT1244_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief AT42QT1244 Driver procedures
** \details AT42QT1244: 24-key QMatrix FMEA IEC/EN/UL60730 Touch Sensor
**/
/****************************************************************/
#ifndef __AT42QT1244_PROC_H__
	#define __AT42QT1244_PROC_H__

#include "sarmfsw.h"
#include "AT42QT1244.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/
// TODO: doxygen


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct AT42QT1244_proc
** \brief AT42QT1244 user interface struct
**/
typedef struct AT42QT1244_proc {
	struct {
	I2C_slave *	slave_inst;			//!< Slave structure
	} cfg;
} AT42QT1244_proc;

extern AT42QT1244_proc AT42QT1244;	//!< AT42QT1244 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for AT42QT1244 peripheral
** \weak AT42QT1244 Init sequence may be user implemented if custom initialization sequence needed
** \return FctERR - error code
**/
__WEAK FctERR AT42QT1244_Init_Sequence(void);

FctERR AT42QT1244_Calibrate_Freq_Offset(void);


__INLINE FctERR INLINE__ AT42QT1244_Calibrate_Low_Level(void) {
	if (AT42QT1244_is_Calib_Pending())	{ return ERROR_BUSY; }
	return AT42QT1244_Send_Command(AT42QT__LOW_LEVEL_CALIBRATION); }


__INLINE FctERR INLINE__ AT42QT1244_Calibrate_All_Keys(void) {
	if (AT42QT1244_is_Calib_Pending())	{ return ERROR_BUSY; }
	return AT42QT1244_Send_Command(AT42QT__CALIBRATE_ALL_KEYS); }


__INLINE FctERR INLINE__ AT42QT1244_Calibrate_Key(uint8_t Key) {
	if (Key > AT42QT__CALIBRATE_KEY_23)	{ return ERROR_VALUE; }
	if (AT42QT1244_is_Calib_Pending())	{ return ERROR_BUSY; }
	return AT42QT1244_Send_Command(Key); }


/*!\brief Handler for AT42QT1244 peripheral
** \weak AT42QT1244 handler may be user implemented to suit custom needs
** \note May be called periodically to handle AT42QT1244 tasks
** \note Alternately may be called when event occurs on AT42QT1244 pin
** \return FctERR - error code
**/
__WEAK FctERR AT42QT1244_handler(void);


/****************************************************************/
#endif
#endif /* __AT42QT1244_PROC_H__ */
/****************************************************************/
