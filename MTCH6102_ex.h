/*!\file MTCH6102_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief MTCH6102 Driver extensions
** \details MTCH6102: Low-Power Projected Capacitive Touch Controller
**/
/****************************************************************/
#ifndef __MTCH6102_EX_H__
	#define __MTCH6102_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "MTCH6102.h"

#if defined(HAL_I2C_MODULE_ENABLED)
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
typedef enum PACK__ MTCH6102_COMMAND {
	MTCH_StoreToNV = 0x00,	//!< Write to non volatile storage
	MTCH_RestoreDefaults,	//!< Restore controller to default configuration values
	MTCH_Configure,			//!< Configure controller (after parameters have been changed)
	MTCH_ManufacturingTest,	//!< Execute manufacturing test
	MTCH_ForceBaseline,		//!< Force baseline (re-calibration) to occur
} MTCH6102_COMMAND;


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
/*!\brief Set MTCH6102 gesture decoding
** \param[in] mode - Gesture decoding mode
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ MTCH6102_Set_Mode(const MTCH6102_MODE mode) {
	if (mode > RawADC)	{ return ERROR_VALUE; }
	return MTCH6102_Write(&mode, MTCH__MODE, 1); }

/*!\brief Get MTCH6102 gesture decoding
** \param[in,out] mode - Gesture decoding mode
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ MTCH6102_Get_Mode(MTCH6102_MODE * mode) {
	return MTCH6102_Read(mode, MTCH__MODE, 1); }

/*!\brief Execute MTCH6102 command
** \param[in] cmd - Command to execute
** \return FctERR - error code
**/
FctERR MTCH6102_Command(const MTCH6102_COMMAND cmd);

/*!\brief Restore MTCH6102 controller to default configuration values
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ MTCH6102_Restore_Defaults(void) {
	return MTCH6102_Command(MTCH_RestoreDefaults); }

/*!\brief Write MTCH6102 parameters to non volatile storage
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ MTCH6102_Store_To_Non_Volatile(void) {
	return MTCH6102_Command(MTCH_StoreToNV); }

/*!\brief Configure MTCH6102controller (after parameters have been changed)
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ MTCH6102_Configuration_Request(void) {
	return MTCH6102_Command(MTCH_Configure); }

/*!\brief Execute MTCH6102 manufacturing test
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ MTCH6102_Manufacturing_Test(void) {
	return MTCH6102_Command(MTCH_ManufacturingTest); }

/*!\brief Force MTCH6102 baseline (re-calibration) to occur
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ MTCH6102_Force_Baseline(void) {
	return MTCH6102_Command(MTCH_ForceBaseline); }


/*** Touch Frame Control ***/
__INLINE FctERR INLINE__ MTCH6102_Set_Active_Period(const uint16_t period) {
	uint16_t per = perVal2perReg(period);
	uint8_t DAT[2] = { LOBYTE(per), HIBYTE(per) };
	return MTCH6102_Write(DAT, MTCH__ACTIVE_PERIOD_L, sizeof(DAT)); }

__INLINE FctERR INLINE__ MTCH6102_Set_Idle_Period(const uint16_t period) {
	uint16_t per = perVal2perReg(period);
	uint8_t DAT[2] = { LOBYTE(per), HIBYTE(per) };
	return MTCH6102_Write(DAT, MTCH__IDLE_PERIOD_L, sizeof(DAT)); }

__INLINE FctERR INLINE__ MTCH6102_Set_Idle_Timeout(const uint8_t time) {
	return MTCH6102_Write(&time, MTCH__IDLE_TIMEOUT, 1); }

__INLINE FctERR INLINE__ MTCH6102_Set_Debounce(const uint8_t touch, const uint8_t release) {
	uint8_t DAT[2] = { release, touch };
	return MTCH6102_Write(DAT, MTCH__DEBOUNCE_UP, sizeof(DAT)); }

FctERR MTCH6102_Get_Active_Period(uint16_t * period);

FctERR MTCH6102_Get_Idle_Period(uint16_t * period);


/*** Acquisition and Touch Parameters ***/
/*!\brief Set MTCH6102 raw acquisition filter
** \param[in] type - Filter type
** \param[in] strength - Filter strength
** \param[in] base_filter - 0: Acquisition filter, 1: Baseline filter
** \return FctERR - error code
**/
FctERR MTCH6102_Set_Filter(const MTCH6102_FILTER_TYPE type, const uint8_t strength, const bool baseline_filter);

__INLINE FctERR INLINE__ MTCH6102_Set_Scan_Count(const uint8_t count) {
	return MTCH6102_Write(&count, MTCH__SCAN_COUNT, 1); }

__INLINE FctERR INLINE__ MTCH6102_Set_Touch_Threshold(const uint8_t xthr, const uint8_t ythr) {
	uint8_t DAT[2] = { xthr, ythr };
	return MTCH6102_Write(DAT, MTCH__TOUCH_THRESH_X, sizeof(DAT)); }

__INLINE FctERR INLINE__ MTCH6102_Set_Hysteresis(const uint8_t hyst) {
	return MTCH6102_Write(&hyst, MTCH__HYSTERESIS, 1); }

__INLINE FctERR INLINE__ MTCH6102_Set_Large_Activation_Threshold(const uint16_t thr) {
	uint8_t DAT[2] = { LOBYTE(thr), HIBYTE(thr) };
	return MTCH6102_Write(DAT, MTCH__LARGE_ACTIVATION_THRESH_L, sizeof(DAT)); }


/*** Baseline ***/
__INLINE FctERR INLINE__ MTCH6102_Set_Base_Interval(const uint16_t interval) {
	uint8_t DAT[2] = { LOBYTE(interval), HIBYTE(interval) };
	return MTCH6102_Write(DAT, MTCH__BASE_INTERVAL_L, sizeof(DAT)); }

__INLINE FctERR INLINE__ MTCH6102_Set_Base_Filter(const uint8_t pos, const uint8_t neg) {
	uint8_t DAT[2] = { pos, neg };
	return MTCH6102_Write(DAT, MTCH__BASE_POS_FILTER, sizeof(DAT)); }


/*** Acquisition ***/
__INLINE FctERR INLINE__ MTCH6102_Get_Raw(MTCH6102_raw_sense * raw) {
	return MTCH6102_Read((uint8_t *) raw, MTCH__SENSOR_VALUE_RX0, sizeof(MTCH6102_raw_sense)); }

__INLINE FctERR INLINE__ MTCH6102_Get_Gest(MTCH6102_raw_gest * gest) {
	return MTCH6102_Read((uint8_t *) gest, MTCH__TOUCH_STATE, sizeof(MTCH6102_raw_gest)); }


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif	/* __MTCH6102_EX_H__ */
/****************************************************************/
