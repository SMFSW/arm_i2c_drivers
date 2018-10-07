/*!\file MTCH6102.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief MTCH6102 Driver
** \details MTCH6102: Low-Power Projected Capacitive Touch Controller
**/
/****************************************************************/
#ifndef __MTCH6102_H__
	#define __MTCH6102_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "globals.h"

#include "I2C_component.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/
// TODO: doxygen


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define MTCH6102_BASE_ADDR	0x25		//!< MTCH6102 default address value


// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern I2C_slave_t MTCH6102_hal;	//!< MTCH6102 Slave structure


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*** REGISTERS MEMORY MAP ENUMERATIONS ***/

typedef enum PACK__ MTCH6102_touch_addr_map {
	MTCH__TOUCH_STATE = 0x10,	// 0x00
	MTCH__TOUCH_X,				// 0x00
	MTCH__TOUCH_Y,				// 0x00
	MTCH__TOUCH_LSB,			// 0x00
	MTCH__GESTURE_STATE,		// 0x00
	MTCH__GESTURE_DIAG			// 0x00
} MTCH6102_touch;

typedef enum PACK__ MTCH6102_acquisition_addr_map {
	MTCH__SENSOR_VALUE_RX0 = 0x80,
	MTCH__SENSOR_VALUE_RX1,
	MTCH__SENSOR_VALUE_RX2,
	MTCH__SENSOR_VALUE_RX3,
	MTCH__SENSOR_VALUE_RX4,
	MTCH__SENSOR_VALUE_RX5,
	MTCH__SENSOR_VALUE_RX6,
	MTCH__SENSOR_VALUE_RX7,
	MTCH__SENSOR_VALUE_RX8,
	MTCH__SENSOR_VALUE_RX9,
	MTCH__SENSOR_VALUE_RX10,
	MTCH__SENSOR_VALUE_RX11,
	MTCH__SENSOR_VALUE_RX12,
	MTCH__SENSOR_VALUE_RX13,
	MTCH__SENSOR_VALUE_RX14,
	// NOT USED,
	MTCH__RAW_VALUE_RX0_L = 0x90,
	MTCH__RAW_VALUE_RX0_H,
	MTCH__RAW_VALUE_RX1_L,
	MTCH__RAW_VALUE_RX1_H,
	MTCH__RAW_VALUE_RX2_L,
	MTCH__RAW_VALUE_RX2_H,
	MTCH__RAW_VALUE_RX3_L,
	MTCH__RAW_VALUE_RX3_H,
	MTCH__RAW_VALUE_RX4_L,
	MTCH__RAW_VALUE_RX4_H,
	MTCH__RAW_VALUE_RX5_L,
	MTCH__RAW_VALUE_RX5_H,
	MTCH__RAW_VALUE_RX6_L,
	MTCH__RAW_VALUE_RX6_H,
	MTCH__RAW_VALUE_RX7_L,
	MTCH__RAW_VALUE_RX7_H,
	MTCH__RAW_VALUE_RX8_L,
	MTCH__RAW_VALUE_RX8_H,
	MTCH__RAW_VALUE_RX9_L,
	MTCH__RAW_VALUE_RX9_H,
	MTCH__RAW_VALUE_RX10_L,
	MTCH__RAW_VALUE_RX10_H,
	MTCH__RAW_VALUE_RX11_L,
	MTCH__RAW_VALUE_RX11_H,
	MTCH__RAW_VALUE_RX12_L,
	MTCH__RAW_VALUE_RX12_H,
	MTCH__RAW_VALUE_RX13_L,
	MTCH__RAW_VALUE_RX13_H,
	MTCH__RAW_VALUE_RX14_L,
	MTCH__RAW_VALUE_RX14_H,
	// NOT USED x3,
	MTCH__BASE_VALUE_RX0_L = 0xB0,
	MTCH__BASE_VALUE_RX0_H,
	MTCH__BASE_VALUE_RX1_L,
	MTCH__BASE_VALUE_RX1_H,
	MTCH__BASE_VALUE_RX2_L,
	MTCH__BASE_VALUE_RX2_H,
	MTCH__BASE_VALUE_RX3_L,
	MTCH__BASE_VALUE_RX3_H,
	MTCH__BASE_VALUE_RX4_L,
	MTCH__BASE_VALUE_RX4_H,
	MTCH__BASE_VALUE_RX5_L,
	MTCH__BASE_VALUE_RX5_H,
	MTCH__BASE_VALUE_RX6_L,
	MTCH__BASE_VALUE_RX6_H,
	MTCH__BASE_VALUE_RX7_L,
	MTCH__BASE_VALUE_RX7_H,
	MTCH__BASE_VALUE_RX8_L,
	MTCH__BASE_VALUE_RX8_H,
	MTCH__BASE_VALUE_RX9_L,
	MTCH__BASE_VALUE_RX9_H,
	MTCH__BASE_VALUE_RX10_L,
	MTCH__BASE_VALUE_RX10_H,
	MTCH__BASE_VALUE_RX11_L,
	MTCH__BASE_VALUE_RX11_H,
	MTCH__BASE_VALUE_RX12_L,
	MTCH__BASE_VALUE_RX12_H,
	MTCH__BASE_VALUE_RX13_L,
	MTCH__BASE_VALUE_RX13_H,
	MTCH__BASE_VALUE_RX14_L,
	MTCH__BASE_VALUE_RX14_H,
	// NOT USED x3,
	MTCH__RAW_ADC_00 = 0xD0,
	MTCH__RAW_ADC_01,
	MTCH__RAW_ADC_02,
	MTCH__RAW_ADC_03,
	MTCH__RAW_ADC_04,
	MTCH__RAW_ADC_05,
	MTCH__RAW_ADC_06,
	MTCH__RAW_ADC_07,
	MTCH__RAW_ADC_08,
	MTCH__RAW_ADC_09,
	MTCH__RAW_ADC_10,
	MTCH__RAW_ADC_11,
	MTCH__RAW_ADC_12,
	MTCH__RAW_ADC_13,
	MTCH__RAW_ADC_14,
	MTCH__RAW_ADC_15,
	MTCH__RAW_ADC_16,
	MTCH__RAW_ADC_17,
	MTCH__RAW_ADC_18,
	MTCH__RAW_ADC_19,
	MTCH__RAW_ADC_20,
	MTCH__RAW_ADC_21,
	MTCH__RAW_ADC_22,
	MTCH__RAW_ADC_23,
	MTCH__RAW_ADC_24,
	MTCH__RAW_ADC_25,
	MTCH__RAW_ADC_26,
	MTCH__RAW_ADC_27,
	MTCH__RAW_ADC_28,
	MTCH__RAW_ADC_29,
	MTCH__RAW_ADC_30,
	MTCH__RAW_ADC_31
} MTCH6102_acquisition;

// TODO: add reg names
typedef enum PACK__ MTCH6102_compensation_addr_map {
	MTCH__SENSOR_COMP_RX0 = 0x50,
	MTCH__SENSOR_COMP_RX1,
	MTCH__SENSOR_COMP_RX2,
	MTCH__SENSOR_COMP_RX3,
	MTCH__SENSOR_COMP_RX4,
	MTCH__SENSOR_COMP_RX5,
	MTCH__SENSOR_COMP_RX6,
	MTCH__SENSOR_COMP_RX7,
	MTCH__SENSOR_COMP_RX8,
	MTCH__SENSOR_COMP_RX9,
	MTCH__SENSOR_COMP_RX10,
	MTCH__SENSOR_COMP_RX11,
	MTCH__SENSOR_COMP_RX12,
	MTCH__SENSOR_COMP_RX13,
	MTCH__SENSOR_COMP_RX14,
} MTCH6102_compensation;


typedef enum PACK__ MTCH6102_core_addr_map {
	MTCH__FW_MAJOR = 0x00,		// 0x02
	MTCH__FW_MINOR,				// 0x00
	MTCH__APP_ID_H,				// 0x00
	MTCH__APP_ID_L,				// 0x12
	MTCH__CMD,					// 0x00
	MTCH__MODE,					// 0x03
	MTCH__MODE_CON,				// 0x00
} MTCH6102_core;

extern uint8_t MTCH6102_default_core[MTCH__MODE_CON - MTCH__FW_MAJOR + 1];


typedef enum PACK__ MTCH6102_reg_map {
	MTCH__NUMBER_OF_X_CHANNELS = 0x20,	// 0x09
	MTCH__NUMBER_OF_Y_CHANNELS,			// 0x06
	MTCH__SCAN_COUNT,					// 0x06
	MTCH__TOUCH_THRESH_X,				// 0x37
	MTCH__TOUCH_THRESH_Y,				// 0x28
	MTCH__ACTIVE_PERIOD_L,				// 0x85
	MTCH__ACTIVE_PERIOD_H,				// 0x02
	MTCH__IDLE_PERIOD_L,				// 0x4C
	MTCH__IDLE_PERIOD_H,				// 0x06
	MTCH__IDLE_TIMEOUT,					// 0x10
	MTCH__HYSTERESIS,					// 0x04
	MTCH__DEBOUNCE_UP,					// 0x01
	MTCH__DEBOUNCE_DOWN,				// 0x01
	MTCH__BASE_INTERVAL_L,				// 0x0A
	MTCH__BASE_INTERVAL_H,				// 0x00
	MTCH__BASE_POS_FILTER,				// 0x14
	MTCH__BASE_NEG_FILTER,				// 0x14
	MTCH__FILTER_TYPE,					// 0x02
	MTCH__FILTER_STRENGTH,				// 0x01
	MTCH__BASE_FILTER_TYPE,				// 0x01
	MTCH__BASE_FILTER_STRENGTH,			// 0x05
	MTCH__LARGE_ACTIVATION_THRESH_L,	// 0x00
	MTCH__LARGE_ACTIVATION_THRESH_H,	// 0x00
	MTCH__HORIZONTAL_SWIPE_DISTANCE,	// 0x40
	MTCH__VERTICAL_SWIPE_DISTANCE,		// 0x40
	MTCH__SWIPE_HOLD_BOUNDARY,			// 0x19
	MTCH__TAP_DISTANCE,					// 0x19
	MTCH__DISTANCE_BETWEEN_TAPS,		// 0x40
	MTCH__TAP_HOLD_TIME_L,				// 0x32
	MTCH__TAP_HOLD_TIME_H,				// 0x09
	MTCH__GESTURE_CLICK_TIME,			// 0x0C
	MTCH__SWIPE_HOLD_THRESH,			// 0x20
	MTCH__MIN_SWIPE_VELOCITY,			// 0x04
	MTCH__HORIZONTAL_GESTURE_ANGLE,		// 0x2D
	MTCH__VERTICAL_GESTURE_ANGLE,		// 0x2D
	MTCH__I2CADDR						// 0x25
} MTCH6102_reg;

extern uint8_t MTCH6102_default_cfg[MTCH__I2CADDR - MTCH__NUMBER_OF_X_CHANNELS + 1];

// Register fields enumerations
typedef enum PACK__ MTCH6102_MODE {
	Standby = 0x00,		//!< Standby
	Gesture,			//!< Gesture only
	Touch,				//!< Touch only
	Full,				//!< Touch & Gesture
	RawADC				//!< Raw ADC values
} MTCH6102_MODE;

typedef enum PACK__ MTCH6102_TYPE {
	Computed = 0x00,		//!< (1023 - Res1) + Res2
	Result1,				//!< Res1 only
	Result2,				//!< Res2 only
} MTCH6102_TYPE;

typedef enum PACK__ MTCH6102_CH {
	RX0 = 0x00,		//!< RX0
	RX1,			//!< RX1
	RX2,			//!< RX2
	RX3,			//!< RX3
	RX4,			//!< RX4
	RX5,			//!< RX5
	RX6,			//!< RX6
	RX7,			//!< RX7
	RX8,			//!< RX8
	RX9,			//!< RX9
	RX10,			//!< RX10
	RX11,			//!< RX11
	RX12,			//!< RX12
	RX13,			//!< RX13
	RX14,			//!< RX14
	// Reserved,	//!< Reserved, do not use
} MTCH6102_CH;

typedef enum PACK__ MTCH6102_FILTER_TYPE {
	Filter_None = 0x00,	//!< No filtering
	Filter_Median,		//!< Size of median window filtering
	Filter_IIR,			//!< Weighting of previous to current value filtering
	Filter_Average,		//!< Size of average window filtering
} MTCH6102_FILTER_TYPE;

typedef enum PACK__ MTCH6102_GESTURE_STATE {
	NoGesture = 0x00,			//!< No Gesture Present
	SingleClick = 0x10,			//!< Single Click gesture
	ClickNHold = 0x11,			//!< Click and Hold gesture
	DoubleClick = 0x20,			//!< Double click gesture
	DownSwipe = 0x31,			//!< Down Swipe gesture
	DownSwipeNHold = 0x32,		//!< Down Swipe and Hold gesture
	RightSwipe = 0x41,			//!< Right Swipe gesture
	RightSwipeNHold = 0x42,		//!< Right Swipe and Hold gesture
	UpSwipe = 0x51,				//!< Up Swipe gesture
	UpSwipeNHold = 0x52,		//!< Up Swipe and Hold gesture
	LeftSwipe = 0x61,			//!< Left Swipe gesture
	LeftSwipeNHold = 0x62		//!< Left Swipe and Hold gesture
} MTCH6102_GESTURE_STATE;


typedef enum PACK__ MTCH6102_GESTURE_DIAGNOSTIC {
	ClickTimeout = 0x00,			//!< Click Timeout
	SwipeTimeout,					//!< Swipe Timeout
	GeneralTimeout,					//!< General Timeout
	ClickThreshExceed,				//!< Click threshold exceeded
	SwipeThreshExceed,				//!< Swipe threshold exceeded
	SwipeNHoldThreshExceed,			//!< Swipe and Hold threshold exceeded
	SwipeOppositeDirThreshExceed,	//!< Swipe Opposite Direction threshold exceeded
	Reserved,						//!< Reserved
	SwipeNHoldValExceed,			//!< Swipe and Hold value exceeded
	OutsideSwipeAngle				//!< Outside Swipe Angle
} MTCH6102_GESTURE_DIAGNOSTIC;


/*** REGISTERS (ONES THAT NEED SOME BITFIELD UNIONS) DEFINITION ***/
// TOUCH REGISTERS
typedef union {
	uint8_t Byte;
	struct {
		uint8_t FRAME	:4;		//!< Increments on every touch frame
		uint8_t			:1;
		uint8_t LRG		:1;		//!<Large activation is present
		uint8_t GES		:1;		//!< Gesture is present
		uint8_t TCH		:1;		//!< Touch is present
	} Bits;
} uMTCH_REG__TOUCHSTATE;

typedef union {
	uint8_t Byte;
	struct {
		uint8_t TOUCHX11_4	:8;		//!< coord TOUCHX<11:4>
	} Bits;
} uMTCH_REG__TOUCHX;

typedef union {
	uint8_t Byte;
	struct {
		uint8_t TOUCHY11_4	:8;		//!< coord TOUCHY<11:4>
	} Bits;
} uMTCH_REG__TOUCHY;

typedef union {
	uint8_t Byte;
	struct {
		uint8_t TOUCHX3_0	:4;		//!< coord TOUCHX<3:0>
		uint8_t TOUCHY3_0	:4;		//!< coord TOUCHY<3:0>
	} Bits;
} uMTCH_REG__TOUCHLSB;

// CORE REGISTERS
typedef union {
	uint8_t Byte;
	struct {
		uint8_t NV	:1;
		uint8_t DEF	:1;
		uint8_t CFG	:1;
		uint8_t 	:1;
		uint8_t MFG	:1;
		uint8_t		:2;
		uint8_t BS	:1;
	} Bits;
} uMTCH_REG__CMD;

typedef union {
	uint8_t Byte;
	struct {
		uint8_t				:4;
		MTCH6102_MODE MODE	:4;		//!< Working mode
	} Bits;
} uMTCH_REG__MODE;

typedef union {
	uint8_t Byte;
	struct {
		MTCH6102_TYPE	TYPE	:4;		//!< Type of result
		MTCH6102_CH		CH		:4;		//!< Channel
	} Bits;
} uMTCH_REG__MODE_CON;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for MTCH6102 peripheral
** \weak MTCH6102 Base address may be changed if user implemented
** \warning write rx/tx number if needed other than default (9,6), (prior to calling init function, or by implementing your own init sequence)
** \note set MTCH6102_Set_Centered_Coord(true) if 0,0 point needs to be centered on the pad (prior to calling init function, or by implementing your own init sequence)
** \return FctERR - error code
**/
__WEAK FctERR MTCH6102_Init(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for MTCH6102
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR NONNULL__ MTCH6102_Write(const uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for MTCH6102
**
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ MTCH6102_Read(uint8_t * data, const uint16_t addr, const uint16_t nb);


/****************************************************************/
#include "MTCH6102_ex.h"	// Include extensions
#include "MTCH6102_proc.h"	// Include procedures

#ifdef __cplusplus
	}
#endif

#endif
#endif	/* __MTCH6102_H__ */
/****************************************************************/
