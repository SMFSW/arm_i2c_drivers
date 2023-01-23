/*!\file MTCH6102_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
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


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define		MTCH_RES_STEP		64		//!< MTCH6102 Resolution of a single RX/TX

#define		perVal2perReg(ms)	(uint16_t) (((ms * 1000) / 31) + 1)		//!< Compute reg value for period \b ms
#define		perReg2perVal(rv)	(uint16_t) (((rv - 1) * 31) / 1000)		//!< Get period in ms from register \b rv

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


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/
/*!\brief Set MTCH6102 gesture decoding
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in] mode - Gesture decoding mode
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MTCH6102_Set_Mode(const MTCH6102_t * const pCpnt, const MTCH6102_MODE mode) {
	if (mode > RawADC)	{ return ERROR_VALUE; }
	return MTCH6102_Write(pCpnt->cfg.slave_inst, &mode, MTCH__MODE, 1); }

/*!\brief Get MTCH6102 gesture decoding
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in,out] mode - Gesture decoding mode
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MTCH6102_Get_Mode(const MTCH6102_t * const pCpnt, MTCH6102_MODE * const mode) {
	return MTCH6102_Read(pCpnt->cfg.slave_inst, mode, MTCH__MODE, 1); }

/*!\brief Execute MTCH6102 command
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in] cmd - Command to execute
** \return FctERR - error code
**/
FctERR NONNULL__ MTCH6102_Command(const MTCH6102_t * const pCpnt, const MTCH6102_COMMAND cmd);

/*!\brief Restore MTCH6102 controller to default configuration values
** \param[in] pCpnt - Pointer to MTCH6102 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MTCH6102_Restore_Defaults(const MTCH6102_t * const pCpnt) {
	return MTCH6102_Command(pCpnt, MTCH_RestoreDefaults); }

/*!\brief Write MTCH6102 parameters to non volatile storage
** \param[in] pCpnt - Pointer to MTCH6102 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MTCH6102_Store_To_Non_Volatile(const MTCH6102_t * const pCpnt) {
	return MTCH6102_Command(pCpnt, MTCH_StoreToNV); }

/*!\brief Configure MTCH6102controller (after parameters have been changed)
** \param[in] pCpnt - Pointer to MTCH6102 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MTCH6102_Configuration_Request(const MTCH6102_t * const pCpnt) {
	return MTCH6102_Command(pCpnt, MTCH_Configure); }

/*!\brief Execute MTCH6102 manufacturing test
** \param[in] pCpnt - Pointer to MTCH6102 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MTCH6102_Manufacturing_Test(const MTCH6102_t * const pCpnt) {
	return MTCH6102_Command(pCpnt, MTCH_ManufacturingTest); }

/*!\brief Force MTCH6102 baseline (re-calibration) to occur
** \param[in] pCpnt - Pointer to MTCH6102 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MTCH6102_Force_Baseline(const MTCH6102_t * const pCpnt) {
	return MTCH6102_Command(pCpnt, MTCH_ForceBaseline); }


/*** Touch Frame Control ***/
/*!\brief Set MTCH6102 active period
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in] period - Active period
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MTCH6102_Set_Active_Period(const MTCH6102_t * const pCpnt, const uint16_t period) {
	const uint16_t per = perVal2perReg(period);
	const uint8_t DAT[2] = { LOBYTE(per), HIBYTE(per) };
	return MTCH6102_Write(pCpnt->cfg.slave_inst, DAT, MTCH__ACTIVE_PERIOD_L, sizeof(DAT)); }

/*!\brief Set MTCH6102 idle period
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in] period - Idle period
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MTCH6102_Set_Idle_Period(const MTCH6102_t * const pCpnt, const uint16_t period) {
	const uint16_t per = perVal2perReg(period);
	const uint8_t DAT[2] = { LOBYTE(per), HIBYTE(per) };
	return MTCH6102_Write(pCpnt->cfg.slave_inst, DAT, MTCH__IDLE_PERIOD_L, sizeof(DAT)); }

/*!\brief Set MTCH6102 idle timeout
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in] timeout - Idle timeout
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MTCH6102_Set_Idle_Timeout(const MTCH6102_t * const pCpnt, const uint8_t timeout) {
	return MTCH6102_Write(pCpnt->cfg.slave_inst, &timeout, MTCH__IDLE_TIMEOUT, 1); }

/*!\brief Set MTCH6102 debounce period
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in] touch - Debounce for touch
** \param[in] release - Debounce for release
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MTCH6102_Set_Debounce(const MTCH6102_t * const pCpnt, const uint8_t touch, const uint8_t release) {
	const uint8_t DAT[2] = { release, touch };
	return MTCH6102_Write(pCpnt->cfg.slave_inst, DAT, MTCH__DEBOUNCE_UP, sizeof(DAT)); }

/*!\brief Get MTCH6102 Active period
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in] period - Pointer to active period result
** \return FctERR - error code
**/
FctERR NONNULL__ MTCH6102_Get_Active_Period(const MTCH6102_t * const pCpnt, uint16_t * const period);

/*!\brief Get MTCH6102 Idle period
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in] period - Pointer to idle period result
** \return FctERR - error code
**/
FctERR NONNULL__ MTCH6102_Get_Idle_Period(const MTCH6102_t * const pCpnt, uint16_t * const period);


/*** Acquisition and Touch Parameters ***/
/*!\brief Set MTCH6102 raw acquisition filter
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in] type - Filter type
** \param[in] strength - Filter strength
** \param[in] baseline_filter - 0: Acquisition filter, 1: Baseline filter
** \return FctERR - error code
**/
FctERR NONNULL__ MTCH6102_Set_Filter(const MTCH6102_t * const pCpnt, const MTCH6102_FILTER_TYPE type, const uint8_t strength, const bool baseline_filter);

/*!\brief Set MTCH6102 scan count
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in] count - Scan count
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MTCH6102_Set_Scan_Count(const MTCH6102_t * const pCpnt, const uint8_t count) {
	return MTCH6102_Write(pCpnt->cfg.slave_inst, &count, MTCH__SCAN_COUNT, 1); }

/*!\brief Set MTCH6102 touch threshold
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in] xthr - Rx lines touch threshold
** \param[in] ythr - Tx lines touch threshold
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MTCH6102_Set_Touch_Threshold(const MTCH6102_t * const pCpnt, const uint8_t xthr, const uint8_t ythr) {
	const uint8_t DAT[2] = { xthr, ythr };
	return MTCH6102_Write(pCpnt->cfg.slave_inst, DAT, MTCH__TOUCH_THRESH_X, sizeof(DAT)); }

/*!\brief Set MTCH6102 touch to release hysteresis
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in] hyst - Hysteresis
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MTCH6102_Set_Hysteresis(const MTCH6102_t * const pCpnt, const uint8_t hyst) {
	return MTCH6102_Write(pCpnt->cfg.slave_inst, &hyst, MTCH__HYSTERESIS, 1); }

/*!\brief Set MTCH6102 large activation threshold
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in] thr - Large activation threshold
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MTCH6102_Set_Large_Activation_Threshold(const MTCH6102_t * const pCpnt, const uint16_t thr) {
	const uint8_t DAT[2] = { LOBYTE(thr), HIBYTE(thr) };
	return MTCH6102_Write(pCpnt->cfg.slave_inst, DAT, MTCH__LARGE_ACTIVATION_THRESH_L, sizeof(DAT)); }


/*** Baseline ***/
/*!\brief Set MTCH6102 base interval
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in] interval - Time interval between baselines
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MTCH6102_Set_Base_Interval(const MTCH6102_t * const pCpnt, const uint16_t interval) {
	const uint8_t DAT[2] = { LOBYTE(interval), HIBYTE(interval) };
	return MTCH6102_Write(pCpnt->cfg.slave_inst, DAT, MTCH__BASE_INTERVAL_L, sizeof(DAT)); }

/*!\brief Set MTCH6102 baseline filter thresholds
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in] pos - Positive filter value
** \param[in] neg - Negative filter value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MTCH6102_Set_Base_Filter(const MTCH6102_t * const pCpnt, const uint8_t pos, const uint8_t neg) {
	const uint8_t DAT[2] = { pos, neg };
	return MTCH6102_Write(pCpnt->cfg.slave_inst, DAT, MTCH__BASE_POS_FILTER, sizeof(DAT)); }


/*** Acquisition ***/
/*!\brief Get MTCH6102 Raw sensor values data
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in] raw - Pointer to raw data result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MTCH6102_Get_Raw(const MTCH6102_t * const pCpnt, MTCH6102_raw_sense * const raw) {
	return MTCH6102_Read(pCpnt->cfg.slave_inst, (uint8_t *) raw, MTCH__SENSOR_VALUE_RX0, sizeof(MTCH6102_raw_sense)); }

/*!\brief Get MTCH6102 gesture data
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in] gest - Pointer to gesture data result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MTCH6102_Get_Gest(const MTCH6102_t * const pCpnt, MTCH6102_raw_gest * const gest) {
	return MTCH6102_Read(pCpnt->cfg.slave_inst, (uint8_t *) gest, MTCH__TOUCH_STATE, sizeof(MTCH6102_raw_gest)); }


/*******************/
/*** GPIO access ***/
/*******************/

/*!\brief Interrupt GPIO pin init for MTCH6102
** \weak MTCH6102 Interrupt GPIO pin init may be user implemented if needed
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in] GPIOx - INT port
** \param[in] GPIO_Pin - INT pin
** \param[in] GPIO_Active: INT pin active state
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ MTCH6102_INT_GPIO_Init(MTCH6102_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active);

/*!\brief Interrupt GPIO pin getter for MTCH6102
** \weak MTCH6102 Interrupt GPIO pin getter may be user implemented if needed
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in,out] pState - Pointer to INT pin state variable (0: inactive, 1: active)
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ MTCH6102_INT_GPIO_Get(MTCH6102_t * const pCpnt, bool * const pState);


/*!\brief Synchro GPIO pin init for MTCH6102
** \weak MTCH6102 Synchro GPIO pin init may be user implemented if needed
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in] GPIOx - SYNC port
** \param[in] GPIO_Pin - SYNC pin
** \param[in] GPIO_Active: SYNC pin active state
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ MTCH6102_SYNC_GPIO_Init(MTCH6102_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active);

/*!\brief Synchro GPIO pin getter for MTCH6102
** \weak MTCH6102 Synchro GPIO pin getter may be user implemented if needed
** \param[in] pCpnt - Pointer to MTCH6102 component
** \param[in,out] pState - Pointer to SYNC pin state variable (0: inactive, 1: active)
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ MTCH6102_SYNC_GPIO_Get(MTCH6102_t * const pCpnt, bool * const pState);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif	/* __MTCH6102_EX_H__ */
/****************************************************************/
