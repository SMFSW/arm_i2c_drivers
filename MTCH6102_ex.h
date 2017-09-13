/*!\file MTCH6102_ex.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief MTCH6102 Driver extensions declarations
** \details MTCH6102: Low-Power Projected Capacitive Touch Controller
**/
/****************************************************************/
#ifndef __MTCH6102_EX_H__
	#define __MTCH6102_EX_H__

#include "MTCH6102.h"

#include "sarmfsw.h"
#include CMSIS_INC
#include CMSIS_CFG

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/
// TODO: doxygen


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define		MTCH_RES_STEP		64		//!< MTCH6102 Resolution of a single RX/TX

#define		perVal2perReg(ms)	(uint16_t) ((((float) ms * 1000.0f) / 31.0f) + 1.0f)	//!< Compute reg value for period \b ms
#define		perReg2perVal(rv)	(uint16_t) (((rv - 1.0f) * 31.0f) / 1000.0f)			//!< Get period in ms from register \b rv

#define		per10ms		0x0142		//!< example period register value for 10ms
#define		per20ms		0x0284		//!< example period register value for 20ms
#define		per50ms		0x064C		//!< example period register value for 50ms
#define		per100ms	0x0C99		//!< example period register value for 100ms


// *****************************************************************************
// Section: Types
// *****************************************************************************
typedef struct MTCH6102_raw {
	uint8_t	sensor[16];
} MTCH6102_raw_sense;


typedef struct MTCH6102_Coordinate {
	int16_t	x;	//!< x coordinate (0 at the center of the capacitive pad)
	int16_t	y;	//!< y coordinate (0 at the center of the capacitive pad)
} MTCH6102_Coord;


typedef struct MTCH6102_gest {
	uMTCH_REG__TOUCHSTATE		Touch_state;
	uMTCH_REG__TOUCHX			Touch_x;
	uMTCH_REG__TOUCHY			Touch_y;
	uMTCH_REG__TOUCHLSB			Touch_lsb;
	MTCH6102_GESTURE_STATE		Gest_state;
	MTCH6102_GESTURE_DIAGNOSTIC	Gest_diag;
} MTCH6102_raw_gest;


typedef struct MTCH6102_gesture {
	MTCH6102_Coord					Coords;		//!< Coordinates
	MTCH6102_GESTURE_STATE			State;		//!< State
	MTCH6102_GESTURE_DIAGNOSTIC		Diag;		//!< Diagnostic
	uint8_t							Frame;		//!< Frame count
	bool							Touch;		//!< Is Touch
	bool							Gesture;	//!< Is Gesture
	bool							Large;		//!< Is Large
} MTCH6102_gesture;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

__INLINE FctERR INLINE__ MTCH6102_Get_Raw(MTCH6102_raw_sense * raw) {
	return MTCH6102_Read((uint8_t *) raw, MTCH__SENSOR_VALUE_RX0, sizeof(MTCH6102_raw_sense)); }

__INLINE FctERR INLINE__ MTCH6102_Get_Gest(MTCH6102_raw_gest * gest) {
	return MTCH6102_Read((uint8_t *) gest, MTCH__TOUCH_STATE, sizeof(MTCH6102_raw_gest)); }


#define MTCH6102_raz()	{	uMTCH_REG__CMD cmd;		\
							cmd.Bits.DEF = true;	\
							MTCH6102_Write(&cmd, MTCH__CMD, 1); }


/****************************************************************/
#endif
#endif	/* __MTCH6102_EX_H__ */
/****************************************************************/
