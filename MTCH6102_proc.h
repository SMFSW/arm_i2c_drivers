/*!\file MTCH6102_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
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


// *****************************************************************************
// Section: Datas
// *****************************************************************************
#define MTCH6102_CARTESIAN_CENTERED			//!< Define MTCH6102 to use default Cartesian reference centered on pad (using demo board routing)
//#define MTCH6102_DEFAULT_CONFIG_DEF		//!< Define default config constants

#if defined(MTCH6102_DEFAULT_CONFIG_DEF)
extern uint8_t MTCH6102_default_cfg[MTCH__I2CADDR - MTCH__NUMBER_OF_X_CHANNELS + 1];
#endif


// *****************************************************************************
// Section: Types
// *****************************************************************************
typedef struct MTCH6102_raw_gesture {
	uMTCH_REG__TOUCHSTATE		Touch_state;	//!< Raw touch state
	uMTCH_REG__TOUCHX			Touch_x;		//!< Raw x coordinate (MSB)
	uMTCH_REG__TOUCHY			Touch_y;		//!< Raw y coordinate (MSB)
	uMTCH_REG__TOUCHLSB			Touch_lsb;		//!< Raw x,y coordinate (LSB)
	MTCH6102_GESTURE_STATE		Gest_state;		//!< Raw state
	MTCH6102_GESTURE_DIAGNOSTIC	Gest_diag;		//!< Raw diagnostic
} MTCH6102_raw_gest;


typedef struct MTCH6102_gesture {
	sCoord2D						Coords;		//!< Coordinates
	MTCH6102_GESTURE_STATE			State;		//!< State
	MTCH6102_GESTURE_DIAGNOSTIC		Diag;		//!< Diagnostic
	uint8_t							Frame;		//!< Frame count
	bool							Touch;		//!< Is Touch
	bool							Gesture;	//!< Is Gesture
	bool							Large;		//!< Is Large
} MTCH6102_gest;


typedef struct MTCH6102_t {
	MTCH6102_gest		touch;			//!< MTCH6102 decoded coordinates & gesture
	int16_t				min_x;			//!< Minimum possible x value
	int16_t				max_x;			//!< Maximum possible x value
	int16_t				min_y;			//!< Minimum possible y value
	int16_t				max_y;			//!< Maximum possible y value
	struct {
	I2C_slave_t *		slave_inst;		//!< Slave structure
	PeripheralGPIO_t	INT_GPIO;		//!< Interrupt GPIO struct
	PeripheralGPIO_t	SYNC_GPIO;		//!< Synchro GPIO struct
	uint8_t				nb_rx;			//!< Number of MTCH6102 Rx used
	uint8_t				nb_tx;			//!< Number of MTCH6102 Tx used
	uint16_t			max_rx;			//!< Maximum Rx grid value (grid delta)
	uint16_t			max_tx;			//!< Maximum Tx grid value (grid delta)
	uint8_t				FW_Major;		//!< MTCH6102 Major FW version
	uint8_t				FW_Minor;		//!< MTCH6102 Minor FW version
	uint16_t			APP_ID;			//!< MTCH6102 App Identifier
	int16_t				Rotation;		//!< Referential rotation angle (in degrees, counter clockwise): only used when Centered set to true
	bool				Centered	:1;	//!< Centering 0,0 point on the middle of the pad (allowing it's rotation afterwards using MTCH6102_rotate)
	bool				RxDownwards	:1;	//!< Set to true if Rx lines are designed to go downwards (as per datasheet guidelines and demo board routing)
	bool				TxDownwards	:1;	//!< Set to true if Tx lines are designed to go downwards (contrary to datasheet guidelines and demo board routing)
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
** \param[in,out] pCpnt - Pointer to MTCH6102 component
** \return FctERR - error code
**/
FctERR NONNULL__ MTCH6102_Init_Sequence(MTCH6102_t * const pCpnt);

/*!\brief Configure MTCH6102 parameters needing configure step
** \param[in,out] pCpnt - Pointer to MTCH6102 component
** \param[in] store_to_nv - Store to non-volatile memory after configuration
** \param[in] active_per - Active period value
** \param[in] idle_per - Idle period value
** \param[in] filter - Filter type
** \param[in] filter_str - Filter strength
** \param[in] base_filter - Base filter type
** \param[in] base_filter_str - Base filter strength
** \param[in] rx - Number of RX lines
** \param[in] tx - Number of TX lines
** \return FctERR - error code
**/
FctERR NONNULL__ MTCH6102_Configure(MTCH6102_t * const pCpnt, const bool store_to_nv,
									const uint16_t active_per, const uint16_t idle_per,
									const MTCH6102_FILTER_TYPE filter, const uint8_t filter_str,
									const MTCH6102_FILTER_TYPE base_filter, const uint8_t base_filter_str,
									const uint8_t rx, const uint8_t tx);

/*!\brief Set MTCH6102 grid min & max values
** \param[in,out] pCpnt - Pointer to MTCH6102 component
**/
void NONNULL__ MTCH6102_Set_Grid(MTCH6102_t * const pCpnt);

/*!\brief Calculates compensation values regarding read values and writes them to MTCH6102
** \note Beware, averaging is done through all read values, not picking a repeated value
** 			(this may not suit your needs, but it appeared that the average result was close to repeated values on custom capacitive board)
** \param[in] pCpnt - Pointer to MTCH6102 component
** \return FctERR - error code
**/
FctERR NONNULL__ MTCH6102_Set_Compensation(MTCH6102_t * const pCpnt);

/*!\brief Set MTCH6102 Tx Direction
** \param[in,out] pCpnt - Pointer to MTCH6102 component
** \param[in] rx_dw - false: rx0 origin left / true: rx0 origin right
** \param[in] tx_dw - false: tx0 origin up / true: tx0 origin down
**/
__INLINE void NONNULL_INLINE__ MTCH6102_Set_RxTx_Direction(MTCH6102_t * const pCpnt, const bool rx_dw, const bool tx_dw) {
	pCpnt->cfg.RxDownwards = rx_dw;
	pCpnt->cfg.TxDownwards = tx_dw; }

/*!\brief Set MTCH6102 coordinates reference
** \param[in,out] pCpnt - Pointer to MTCH6102 component
** \param[in] centered - false: reference in the corner / true: reference in the middle of the pad
**/
__INLINE void NONNULL_INLINE__ MTCH6102_Set_Centered_Coord(MTCH6102_t * const pCpnt, const bool centered) {
	pCpnt->cfg.Centered = centered; }

/*!\brief Set MTCH6102 coordinates rotation
** \param[in,out] pCpnt - Pointer to MTCH6102 component
** \param[in] angle - Rotation angle (in degrees, counter clockwise)
**/
__INLINE void NONNULL_INLINE__ MTCH6102_Set_Rotation(MTCH6102_t * const pCpnt, const int16_t angle) {
	pCpnt->cfg.Rotation = angle % 360; }


/*!\brief Get manufacturing test results
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in,out] res - Channels shorted to GND in least significant 16b ; Channels shorted to Vdd in most significant 16b
** \return FctERR - error code
**/
FctERR NONNULL__ MTCH6102_Get_MFG_Results(MTCH6102_t * const pCpnt, uint32_t * const res);


/*!\brief Decode raw MTCH6102 datas to touch datas
** \warning You would have to use MTCH6102_Set_RxTx_Direction to change behavior (complementing default demo board design to complement Tx to a Cartesian reference for example)
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in] dat - Raw touch datas
** \return Decoded touch datas
**/
FctERR NONNULL__ MTCH6102_decode_touch_datas(MTCH6102_t * const pCpnt, const MTCH6102_raw_gest * const dat);


/*!\brief Get gesture string from gesture data
** \param[in,out] str - Pointer to output string
** \param[in,out] state - Raw datas
** \return FctERR - error code
**/
FctERR NONNULL__ MTCH6102_gesture_to_str(char * const str, const MTCH6102_GESTURE_STATE state);

/*!\brief Get diagnostic string from diagnostic data
** \param[in,out] str - Pointer to output string
** \param[in,out] diag - Raw datas
** \return FctERR - error code
**/
FctERR NONNULL__ MTCH6102_diag_to_str(char * const str, const MTCH6102_GESTURE_DIAGNOSTIC diag);


/*!\brief Handler for MTCH6102 peripheral
** \weak MTCH6102 handler may be user implemented to suit custom needs
** \note May be called periodically to handle MTCH6102 tasks
** \note Alternately may be called when event occurs on MTCH6102 pin (or by calling \ref MTCH6102_handler_it instead)
** \param[in] pCpnt - Pointer to MTCH6102 component
** \return FctERR - error code
**/
FctERR NONNULL__ MTCH6102_handler(MTCH6102_t * const pCpnt);

/*!\brief Handler for MTCH6102 peripheral GPIO interrupt
** \note \ref MTCH6102_INT_GPIO_Init has to be called at init before using interrupt handler function
** \weak MTCH6102 GPIO interrupt handler may be user implemented to suit custom needs
** \note May be called periodically to handle MTCH6102 tasks through interrupts
** \param[in] pCpnt - Pointer to MTCH6102 component
** \return FctERR - error code
**/
FctERR NONNULL__ MTCH6102_handler_it(MTCH6102_t * const pCpnt);


/*!\brief Handler for all MTCH6102 peripherals
** \note May be called periodically to handle all MTCH6102 tasks
**/
__INLINE void INLINE__ MTCH6102_handler_all(void) {
	for (MTCH6102_t * pCpnt = MTCH6102 ; pCpnt < &MTCH6102[I2C_MTCH6102_NB] ; pCpnt++) {
		MTCH6102_handler(pCpnt); }
}

/*!\brief Handler for all MTCH6102 peripherals GPIO interrupt
** \note \ref MTCH6102_INT_GPIO_Init has to be called at init before using interrupt handler function
** \note May be called periodically to handle all MTCH6102 tasks
**/
__INLINE void INLINE__ MTCH6102_handler_it_all(void) {
	for (MTCH6102_t * pCpnt = MTCH6102 ; pCpnt < &MTCH6102[I2C_MTCH6102_NB] ; pCpnt++) {
		MTCH6102_handler_it(pCpnt); }
}


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif	/* __MTCH6102_PROC_H__ */
/****************************************************************/
