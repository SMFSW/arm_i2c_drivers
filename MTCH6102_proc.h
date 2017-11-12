/*!\file MTCH6102_proc.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief MTCH6102 Driver procedures declarations
** \details MTCH6102: Low-Power Projected Capacitive Touch Controller
**/
/****************************************************************/
#ifndef __MTCH6102_PROC_H__
	#define __MTCH6102_PROC_H__

#include "sarmfsw.h"
#include "MTCH6102.h"

#if defined(HAL_I2C_MODULE_ENABLED)
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
	int16_t		min_x;
	int16_t		max_x;
	int16_t		min_y;
	int16_t		max_y;
	struct {
	uint8_t		nb_x;
	uint8_t		nb_y;
	uint8_t		FW_Major;
	uint8_t		FW_Minor;
	uint16_t	APP_ID;
	bool		Centered;	//!< Centering 0,0 point on the middle of the pad (allowing it's rotation afterwards using MTCH6102_rotate)
	} cfg;
} MTCH6102_proc;

extern MTCH6102_proc MTCH6102;

// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for MTCH6102 peripheral
** \weak MTCH6102 Init sequence may be user implemented if custom initialization sequence needed
** \return FctERR - error code
**/
__WEAK FctERR MTCH6102_Init_Sequence(void);

FctERR MTCH6102_decode_touch_datas(MTCH6102_gesture * touch, const MTCH6102_raw_gest * dat);

MTCH6102_Coord MTCH6102_rotate(const MTCH6102_Coord c, int16_t deg);

FctERR MTCH6102_gesture_to_str(char * str, const MTCH6102_GESTURE_STATE state);
FctERR MTCH6102_diag_to_str(char * str, const MTCH6102_GESTURE_DIAGNOSTIC diag);

__INLINE void INLINE__ MTCH6102_Set_Centered_Coord(const bool centered) {
	MTCH6102.cfg.Centered = centered; }

/*!\brief Handler for MTCH6102 peripheral
** \weak MTCH6102 handler may be user implemented to suit custom needs
** \note May be called periodically to handle MTCH6102 tasks
** \note Alternately may be called when event occurs on MTCH6102 pin
** \return FctERR - error code
**/
__WEAK FctERR MTCH6102_handler(void);

/****************************************************************/
#endif
#endif	/* __MTCH6102_PROC_H__ */
/****************************************************************/
