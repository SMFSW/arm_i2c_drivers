/*!\file MTCH6102_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief MTCH6102 Driver procedures
** \details MTCH6102: Low-Power Projected Capacitive Touch Controller
**/
/****************************************************************/
#ifndef __MTCH6102_PROC_H__
	#define __MTCH6102_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "MTCH6102.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/
// TODO: doxygen


// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern uint8_t MTCH6102_default_cfg[MTCH__I2CADDR - MTCH__NUMBER_OF_X_CHANNELS + 1];

// *****************************************************************************
// Section: Types
// *****************************************************************************
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


typedef struct MTCH6102_t {
	int16_t			min_x;
	int16_t			max_x;
	int16_t			min_y;
	int16_t			max_y;
	struct {
	I2C_slave_t *	slave_inst;		//!< Slave structure
	uint8_t			nb_x;
	uint8_t			nb_y;
	uint8_t			FW_Major;
	uint8_t			FW_Minor;
	uint16_t		APP_ID;
	bool			Centered;		//!< Centering 0,0 point on the middle of the pad (allowing it's rotation afterwards using MTCH6102_rotate)
	} cfg;
} MTCH6102_t;

extern MTCH6102_t MTCH6102[I2C_MTCH6102_NB];	//!< MTCH6102 User structure

// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for MTCH6102 peripheral
** \weak MTCH6102 Init sequence may be user implemented if custom initialization sequence needed
** \param[in] pCpnt - Pointer to MTCH6102 component
** \return FctERR - error code
**/
FctERR NONNULL__ MTCH6102_Init_Sequence(MTCH6102_t * pCpnt);

/*!\brief Calculates compensation values regarding read values and writes them to MTCH6102
** \note Beware, averaging is done through all read values, not picking a repeated value
** 			(this may not suit your needs, but it appeared that the average result was close to repeated values on custom capacitive board)
** \param[in] pCpnt - Pointer to MTCH6102 component
** \return FctERR - error code
**/
FctERR NONNULL__ MTCH6102_Set_Compensation(MTCH6102_t * pCpnt);

/*!\brief Get manufacturing test results
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in,out] res - Channels shorted to GND in least significant 16b ; Channels shorted to Vdd in most significant 16b
** \return FctERR - error code
**/
FctERR NONNULL__ MTCH6102_Get_MFG_Results(MTCH6102_t * pCpnt, uint32_t * res);

FctERR NONNULL__ MTCH6102_decode_touch_datas(MTCH6102_t * pCpnt, MTCH6102_gesture * touch, const MTCH6102_raw_gest * dat);

MTCH6102_Coord MTCH6102_rotate(const MTCH6102_Coord c, int16_t deg);

FctERR NONNULL__ MTCH6102_gesture_to_str(char * str, const MTCH6102_GESTURE_STATE state);
FctERR NONNULL__ MTCH6102_diag_to_str(char * str, const MTCH6102_GESTURE_DIAGNOSTIC diag);

__INLINE void NONNULL_INLINE__ MTCH6102_Set_Centered_Coord(MTCH6102_t * pCpnt, const bool centered) {
	pCpnt->cfg.Centered = centered; }

/*!\brief Handler for MTCH6102 peripheral
** \weak MTCH6102 handler may be user implemented to suit custom needs
** \note May be called periodically to handle MTCH6102 tasks
** \note Alternately may be called when event occurs on MTCH6102 pin
** \param[in] pCpnt - Pointer to MTCH6102 component
** \return FctERR - error code
**/
FctERR NONNULL__ MTCH6102_handler(MTCH6102_t * pCpnt);


/*!\brief Handler for all MTCH6102 peripherals
** \note May be called periodically to handle all MTCH6102 tasks
**/
__INLINE void INLINE__ MTCH6102_handler_all(void) {
	for (MTCH6102_t * pCpnt = MTCH6102 ; pCpnt < &MTCH6102[I2C_MTCH6102_NB] ; pCpnt++) {
		MTCH6102_handler(pCpnt); }
}


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif	/* __MTCH6102_PROC_H__ */
/****************************************************************/
