/*!\file MTCH6102_proc.h
** \author SMFSW
** \version v0.2
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief MTCH6102 Driver procedures declarations
** \details MTCH6102: Low-Power Projected Capacitive Touch Controller
**/
/****************************************************************/
#ifndef __MTCH6102_PROC_H__
	#define __MTCH6102_PROC_H__

#include "MTCH6102.h"

#include "sarmfsw.h"
#include CMSIS_INC
#include CMSIS_CFG

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/
// TODO: doxygen


// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern uint8_t MTCH6102_default_core[MTCH__MODE_CON - MTCH__FW_MAJOR + 1];
extern uint8_t MTCH6102_default_cfg[MTCH__I2CADDR - MTCH__NUMBER_OF_X_CHANNELS + 1];

// *****************************************************************************
// Section: Types
// *****************************************************************************
typedef struct MTCH6102_proc {
	struct {
	uint8_t		nb_x;
	uint8_t		nb_y;
	uint8_t		FW_Major;
	uint8_t		FW_Minor;
	uint16_t	APP_ID;
	} cfg;
} MTCH6102_proc;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for MTCH6102 peripheral
** \return FctERR - error code
**/
FctERR MTCH6102_Init_Sequence(void);

FctERR MTCH6102_decode_touch_datas(MTCH6102_gesture * touch, MTCH6102_raw_gest * dat);
FctERR MTCH6102_gesture_to_str(char * str, MTCH6102_GESTURE_STATE state);
FctERR MTCH6102_diag_to_str(char * str, MTCH6102_GESTURE_DIAGNOSTIC diag);


FctERR MTCH6102_handler(void);

/****************************************************************/
#endif
#endif	/* __MTCH6102_PROC_H__ */
/****************************************************************/
