/*!\file MTCH6102_ex.h
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Microchip Capacitive Driver extensions declarations
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
#define		perVal2perReg(ms)	(uint16_t) ((((float) ms * 1000.0f) / 31.0f) + 1.0f)	//!< Compute reg value for period \b ms
#define		perReg2perVal(rv)	(uint16_t) (((rv - 1.0f) * 31.0f) / 1000.0f)			//!< Get period in ms from register \b rv

#define		per10ms		0x0142		//!< example period register value for 10ms
#define		per20ms		0x0284		//!< example period register value for 20ms
#define		per50ms		0x064C		//!< example period register value for 50ms
#define		per100ms	0x0C99		//!< example period register value for 100ms


extern uint8_t MTCH6102_default_core[MTCH__MODE_CON - MTCH__FW_MAJOR + 1];
extern uint8_t MTCH6102_default_cfg[MTCH__I2CADDR - MTCH__NUMBER_OF_X_CHANNELS + 1];


typedef struct StructDecodedTouch {
	uint16_t						X_pos;
	uint16_t						Y_pos;
	MTCH6102_GESTURE_STATE			State;
	MTCH6102_GESTURE_DIAGNOSTIC		Diag;
	uint8_t							Frame;
	bool							Touch;
	bool							Gesture;
	bool							Large;
} DecodedTouch;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// High level functions
FctERR MTCH6102_init(uint8_t i2caddr);
FctERR MTCH6102_read_regs(uint8_t addr, uint8_t * dat, uint8_t sz);
FctERR MTCH6102_write_regs(uint8_t addr, uint8_t * dat, uint8_t sz);
FctERR MTCH6102_decode_touch_datas(uint8_t * dat, DecodedTouch * touch);
FctERR MTCH6102_gesture_to_str(char * str, MTCH6102_GESTURE_STATE state);
FctERR MTCH6102_diag_to_str(char * str, MTCH6102_GESTURE_DIAGNOSTIC diag);

#define MTCH6102_raz()	{	uMTCH_REG__CMD cmd;		\
							cmd.Bits.DEF = true;	\
							MTCH6102_Write(&cmd, MTCH__CMD, 1); }


/****************************************************************/
#endif
#endif	/* __MTCH6102_EX_H__ */
/****************************************************************/
