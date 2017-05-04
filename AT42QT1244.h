/*!\file AT42QT1244.h
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Atmel Capacitive Driver declarations
**/
/****************************************************************/
#ifndef __AT42QT1244_H__
	#define __AT42QT1244_H__

#include "I2C_component.h"
#include "FctERR.h"

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
#define AT42QT1244_ADDR			(57 << 1)			//!< AT42QT1244 address (A0 & A1 tied to GND)
#define AT42QT1244_ADDR_A0		(7 << 1)			//!< AT42QT1244 address (A0 tied to PWR & A1 tied to GND)
#define AT42QT1244_ADDR_A1		(17 << 1)			//!< AT42QT1244 address (A0 tied to GND & A1 tied to PWR)
#define AT42QT1244_ADDR_A01		(117 << 1)			//!< AT42QT1244 address (A0 & A1 tied to PWR)

#define AT42QT1244_BASE_ADDR	AT42QT1244_ADDR		//!< AT42QT1244 Base address


// *****************************************************************************
// Section: Types
// *****************************************************************************
typedef enum PACK__ AT42QT_reg_map {
	//AT42QT__RES,	//!< Reserved
	//AT42QT__RES,	//!< Reserved
	AT42QT__CNT_100MS				= 2,	//!<
	AT42QT__CNT_SIGNAL_FAIL			= 3,	//!<
	AT42QT__CNT_MTX_SCAN			= 4,	//!<
	AT42QT__DEVICE_STATUS			= 5,	//!<
	AT42QT__DETECT_STATUS_1			= 6,	//!<
	AT42QT__DETECT_STATUS_2			= 7,	//!<
	AT42QT__DETECT_STATUS_3			= 8,	//!<
	//AT42QT__RES,	//!< Reserved
	AT42QT__CURRENT_FREQUENCY		= 10,	//!<
	AT42QT__CURRENT_PULSE_SPACING	= 11,	//!<
	AT42QT__DATA_KEY_0				= 12,	//!<
	AT42QT__DATA_KEY_1				= 16,	//!<
	AT42QT__DATA_KEY_2				= 20,	//!<
	AT42QT__DATA_KEY_3				= 24,	//!<
	AT42QT__DATA_KEY_4				= 28,	//!<
	AT42QT__DATA_KEY_5				= 32,	//!<
	AT42QT__DATA_KEY_6				= 36,	//!<
	AT42QT__DATA_KEY_7				= 40,	//!<
	AT42QT__DATA_KEY_8				= 44,	//!<
	AT42QT__DATA_KEY_9				= 48,	//!<
	AT42QT__DATA_KEY_10				= 52,	//!<
	AT42QT__DATA_KEY_11				= 56,	//!<
	AT42QT__DATA_KEY_12				= 60,	//!<
	AT42QT__DATA_KEY_13				= 64,	//!<
	AT42QT__DATA_KEY_14				= 68,	//!<
	AT42QT__DATA_KEY_15				= 72,	//!<
	AT42QT__DATA_KEY_16				= 76,	//!<
	AT42QT__DATA_KEY_17				= 80,	//!<
	AT42QT__DATA_KEY_18				= 84,	//!<
	AT42QT__DATA_KEY_19				= 88,	//!<
	AT42QT__DATA_KEY_20				= 92,	//!<
	AT42QT__DATA_KEY_21				= 96,	//!<
	AT42QT__DATA_KEY_22				= 100,	//!<
	AT42QT__DATA_KEY_23				= 104,	//!<
	AT42QT__CONTROL_COMMAND			= 140,	//!<
	//AT42QT__SETUPS							= 141	//!<
	AT42QT__SETUP_KEYS_THRESHOLD_0				= 141,	//!<
	AT42QT__SETUP_KEYS_THRESHOLD_1,		AT42QT__SETUP_KEYS_THRESHOLD_2,		AT42QT__SETUP_KEYS_THRESHOLD_3,		AT42QT__SETUP_KEYS_THRESHOLD_4,
	AT42QT__SETUP_KEYS_THRESHOLD_5,		AT42QT__SETUP_KEYS_THRESHOLD_6,		AT42QT__SETUP_KEYS_THRESHOLD_7,		AT42QT__SETUP_KEYS_THRESHOLD_8,
	AT42QT__SETUP_KEYS_THRESHOLD_9,		AT42QT__SETUP_KEYS_THRESHOLD_10,	AT42QT__SETUP_KEYS_THRESHOLD_11,	AT42QT__SETUP_KEYS_THRESHOLD_12,
	AT42QT__SETUP_KEYS_THRESHOLD_13,	AT42QT__SETUP_KEYS_THRESHOLD_14,	AT42QT__SETUP_KEYS_THRESHOLD_15,	AT42QT__SETUP_KEYS_THRESHOLD_16,
	AT42QT__SETUP_KEYS_THRESHOLD_17,	AT42QT__SETUP_KEYS_THRESHOLD_18,	AT42QT__SETUP_KEYS_THRESHOLD_19,	AT42QT__SETUP_KEYS_THRESHOLD_20,
	AT42QT__SETUP_KEYS_THRESHOLD_21,	AT42QT__SETUP_KEYS_THRESHOLD_22,	AT42QT__SETUP_KEYS_THRESHOLD_23,
	AT42QT__SETUP_KEYS_MODE_0					= 165,	//!<
	AT42QT__SETUP_KEYS_MODE_1,	AT42QT__SETUP_KEYS_MODE_2,	AT42QT__SETUP_KEYS_MODE_3,	AT42QT__SETUP_KEYS_MODE_4,	AT42QT__SETUP_KEYS_MODE_5,
	AT42QT__SETUP_KEYS_MODE_6,	AT42QT__SETUP_KEYS_MODE_7,	AT42QT__SETUP_KEYS_MODE_8,	AT42QT__SETUP_KEYS_MODE_9,	AT42QT__SETUP_KEYS_MODE_10,
	AT42QT__SETUP_KEYS_MODE_11,	AT42QT__SETUP_KEYS_MODE_12,	AT42QT__SETUP_KEYS_MODE_13,	AT42QT__SETUP_KEYS_MODE_14,	AT42QT__SETUP_KEYS_MODE_15,
	AT42QT__SETUP_KEYS_MODE_16,	AT42QT__SETUP_KEYS_MODE_17,	AT42QT__SETUP_KEYS_MODE_18,	AT42QT__SETUP_KEYS_MODE_19,	AT42QT__SETUP_KEYS_MODE_20,
	AT42QT__SETUP_KEYS_MODE_21,	AT42QT__SETUP_KEYS_MODE_22,	AT42QT__SETUP_KEYS_MODE_23,
	AT42QT__SETUP_CFO_1_0						= 189,	//!<
	AT42QT__SETUP_CFO_1_1,	AT42QT__SETUP_CFO_1_2,	AT42QT__SETUP_CFO_1_3,	AT42QT__SETUP_CFO_1_4,	AT42QT__SETUP_CFO_1_5,
	AT42QT__SETUP_CFO_1_6,	AT42QT__SETUP_CFO_1_7,	AT42QT__SETUP_CFO_1_8,	AT42QT__SETUP_CFO_1_9,	AT42QT__SETUP_CFO_1_10,
	AT42QT__SETUP_CFO_1_11,	AT42QT__SETUP_CFO_1_12,	AT42QT__SETUP_CFO_1_13,	AT42QT__SETUP_CFO_1_14,	AT42QT__SETUP_CFO_1_15,
	AT42QT__SETUP_CFO_1_16,	AT42QT__SETUP_CFO_1_17,	AT42QT__SETUP_CFO_1_18,	AT42QT__SETUP_CFO_1_19,	AT42QT__SETUP_CFO_1_20,
	AT42QT__SETUP_CFO_1_21,	AT42QT__SETUP_CFO_1_22,	AT42QT__SETUP_CFO_1_23,
	AT42QT__SETUP_CFO_2_0						= 213,	//!<
	AT42QT__SETUP_CFO_2_1,	AT42QT__SETUP_CFO_2_2,	AT42QT__SETUP_CFO_2_3,	AT42QT__SETUP_CFO_2_4,	AT42QT__SETUP_CFO_2_5,
	AT42QT__SETUP_CFO_2_6,	AT42QT__SETUP_CFO_2_7,	AT42QT__SETUP_CFO_2_8,	AT42QT__SETUP_CFO_2_9,	AT42QT__SETUP_CFO_2_10,
	AT42QT__SETUP_CFO_2_11,	AT42QT__SETUP_CFO_2_12,	AT42QT__SETUP_CFO_2_13,	AT42QT__SETUP_CFO_2_14,	AT42QT__SETUP_CFO_2_15,
	AT42QT__SETUP_CFO_2_16,	AT42QT__SETUP_CFO_2_17,	AT42QT__SETUP_CFO_2_18,	AT42QT__SETUP_CFO_2_19,	AT42QT__SETUP_CFO_2_20,
	AT42QT__SETUP_CFO_2_21,	AT42QT__SETUP_CFO_2_22,	AT42QT__SETUP_CFO_2_23,
	AT42QT__SETUP_NEGATIVE_RECALL_DELAY			= 237,	//!<
	AT42QT__SETUP_DEBUG_MSYNC					= 238,	//!<
	AT42QT__SETUP_AWAKE_TIMEOUT					= 239,	//!<
	AT42QT__SETUP_DRIFT_HOLD_TIME				= 240,	//!<
	AT42QT__SETUP_DRIFT							= 241,	//!<
	AT42QT__SETUP_LOWER_SIGNAL_LIMIT_1			= 242,	//!<
	AT42QT__SETUP_LOWER_SIGNAL_LIMIT_2			= 243,	//!<
	AT42QT__SETUP_FREQ_HOPING_DWELL				= 244,	//!<
	AT42QT__SETUP_FREQ0							= 245,	//!<
	AT42QT__SETUP_FREQ1							= 246,	//!<
	AT42QT__SETUP_FREQ2							= 247,	//!<
	AT42QT__SETUP_NOISE							= 248,	//!<
	AT42QT__SETUP_HOST_CRC_LSB					= 249,	//!<
	AT42QT__SETUP_HOST_CRC_MSB					= 250	//!<
} AT42QT_reg;


typedef enum PACK__ AT42QT_ctl_cmd {
	AT42QT__CALIBRATE_KEY_0			= 0,	//!< Calibration: Key 0
	AT42QT__CALIBRATE_KEY_1,				//!< Calibration: Key 1
	AT42QT__CALIBRATE_KEY_2,				//!< Calibration: Key 2
	AT42QT__CALIBRATE_KEY_3,				//!< Calibration: Key 3
	AT42QT__CALIBRATE_KEY_4,				//!< Calibration: Key 4
	AT42QT__CALIBRATE_KEY_5,				//!< Calibration: Key 5
	AT42QT__CALIBRATE_KEY_6,				//!< Calibration: Key 6
	AT42QT__CALIBRATE_KEY_7,				//!< Calibration: Key 7
	AT42QT__CALIBRATE_KEY_8,				//!< Calibration: Key 8
	AT42QT__CALIBRATE_KEY_9,				//!< Calibration: Key 9
	AT42QT__CALIBRATE_KEY_10,				//!< Calibration: Key 10
	AT42QT__CALIBRATE_KEY_11,				//!< Calibration: Key 11
	AT42QT__CALIBRATE_KEY_12,				//!< Calibration: Key 12
	AT42QT__CALIBRATE_KEY_13,				//!< Calibration: Key 13
	AT42QT__CALIBRATE_KEY_14,				//!< Calibration: Key 14
	AT42QT__CALIBRATE_KEY_15,				//!< Calibration: Key 15
	AT42QT__CALIBRATE_KEY_16,				//!< Calibration: Key 16
	AT42QT__CALIBRATE_KEY_17,				//!< Calibration: Key 17
	AT42QT__CALIBRATE_KEY_18,				//!< Calibration: Key 18
	AT42QT__CALIBRATE_KEY_19,				//!< Calibration: Key 19
	AT42QT__CALIBRATE_KEY_20,				//!< Calibration: Key 20
	AT42QT__CALIBRATE_KEY_21,				//!< Calibration: Key 21
	AT42QT__CALIBRATE_KEY_22,				//!< Calibration: Key 22
	AT42QT__CALIBRATE_KEY_23,				//!< Calibration: Key 23
	AT42QT__RESET_DEVICE			= 0x18,	//!< Reset device
	AT42QT__LOW_LEVEL_CALIBRATION	= 0xFD,	//!< Calibration: Low level and frequency hoping
	AT42QT__WRITE_SETUPS			= 0xFE,	//!< Write 0xFD immediately before writing setups
	AT42QT__CALIBRATE_ALL_KEYS		= 0xFF,	//!< Calibration: all keys
} AT42QT_cmd;

typedef enum PACK__ AT42QT_fhm_mode {
	AT42QT__FHM_OFF = 0,					//!< Frequency Hoping Mode OFF
	AT42QT__FHM_CALIB_AFTER_HOP,			//!< Calibrate all keys after hop
	AT42QT__FHM_ADJ_KEYS_REF_DURING_HOP,	//!< Adjust each keys reference during hop
	AT42QT__FHM_FREQUENCY_SWEEP,			//!< Frequency sweep
} AT42QT_FHM;


typedef union uAT42QT_REG__KEY_DATA {
	uint32_t Raw;
	struct {
		uint32_t Normal_Detector_Integrator	:4;		//!< Normal detector integrator
		uint32_t Signal						:12;	//!< Signal
		uint32_t 							:2;
		uint32_t FMEA_Failure				:1;		//!< FMEA failure
		uint32_t LSL_Failure				:1;		//!< LSL failure
		uint32_t Reference					:12;	//!< Reference
	} Data;
} uAT42QT_REG__KEY_DATA;

typedef union uAT42QT_REG__DEVICE_STATUS {
	uint8_t Byte;
	struct {
		uint8_t Reserved			:3;		//!< Reserved bits
		uint8_t FMEA_Failure		:1;		//!< FMEA failure
		uint8_t LSL_Failure			:1;		//!< LSL failure
		uint8_t Key_In_Calib		:1;		//!< Key in calibration
		uint8_t Mains_Sync_Error	:1;		//!< Mains sync error
		uint8_t CRC_Incorrect		:1;		//!< Setups CRC does not match HCRC
	} Bits;
} uAT42QT_REG__DEVICE_STATUS;


typedef union uAT42QT_REG__SETUP_141_164 {
	uint8_t Byte;
	struct {
		uint8_t BL				:2;		//!< Burst length
		uint8_t NDRIFT			:3;		//!< Drift compensation
		uint8_t NTHR_PTHR		:2;		//!< Threshold
	} Bits;
} uAT42QT_REG__SETUP_141_164;

typedef union uAT42QT_REG__SETUP_165_188 {
	uint8_t Byte;
	struct {
		uint8_t WAKE			:1;		//!< Wake on touch
		uint8_t AKS				:1;		//!< Adjacent Key suppression technology
		uint8_t FDIL			:3;		//!< False detection
		uint8_t NDIL			:3;		//!< No detection
	} Bits;
} uAT42QT_REG__SETUP_165_188;

typedef union uAT42QT_REG__SETUP_238 {
	uint8_t Byte;
	struct {
		uint8_t DEBUG			:1;		//!< Debug output
		uint8_t NHYST			:2;		//!< Negative hysteresis
		uint8_t MSYNC			:1;		//!< Main sync
		uint8_t SLEEP			:3;		//!< Sleep duration
	} Bits;
} uAT42QT_REG__SETUP_238;

typedef union uAT42QT_REG__SETUP_241 {
	uint8_t Byte;
	struct {
		uint8_t SSYNC			:5;		//!< Oscilloscope sync
		uint8_t PDRIFT			:3;		//!< Positive drift compensation
	} Bits;
} uAT42QT_REG__SETUP_241;

typedef union uAT42QT_REG__SETUP_242 {
	uint8_t Byte;
	struct {
		uint8_t LSL				:8;		//!< Lower signal limit (LSB)
	} Bits;
} uAT42QT_REG__SETUP_242;

typedef union uAT42QT_REG__SETUP_243 {
	uint8_t Byte;
	struct {
		uint8_t KGTT			:5;		//!< Key gain test threshold
		uint8_t LSL				:3;		//!< Lower signal limit (MSB)
	} Bits;
} uAT42QT_REG__SETUP_243;

typedef union uAT42QT_REG__SETUP_244 {
	uint8_t Byte;
	struct {
		AT42QT_FHM FHM			:2;		//!< Frequency hoping mode
		uint8_t THRM			:2;		//!< Threshold multiplier
		uint8_t RIB				:1;		//!< Restart interrupted burst
		uint8_t DWELL			:3;		//!< Dwell time
	} Bits;
} uAT42QT_REG__SETUP_244;

typedef union uAT42QT_REG__SETUP_248 {
	uint8_t Byte;
	struct {
		uint8_t NIL				:4;		//!< Noise integrator limit
		uint8_t NSTHR			:4;		//!< Noise threshold
	} Bits;
} uAT42QT_REG__SETUP_248;



typedef union uAT42QT_REG_MAP {
	uint8_t Bytes[251];
	struct {
		uint8_t						Res_1;
		uint8_t						Res_2;
		uint8_t						Counter_100ms;
		uint8_t						Counter_Signal_Fail;
		uint8_t						Counter_Matrix_Scan;
		uAT42QT_REG__DEVICE_STATUS	Device_Status;
		uint8_t						Detect_Status_0_7;
		uint8_t						Detect_Status_8_15;
		uint8_t						Detect_Status_16_23;
		uint8_t						Res_3;
		uint8_t						Current_Frequency;
		uint8_t						Current_Pulse_Spacing;
		uAT42QT_REG__KEY_DATA		KeyData[24];
		uint8_t						Res_4[31];
		uint8_t						Control_Command;
		uAT42QT_REG__SETUP_141_164	Setup_Key_Thr[24];
		uAT42QT_REG__SETUP_165_188	Setup_Key_Mode[24];
		uint8_t						Setup_Calib_Frequency_Offset_1[24];
		uint8_t						Setup_Calib_Frequency_Offset_2[24];
		uint8_t						Setup_Negative_Recall_Delay;
		uAT42QT_REG__SETUP_238		Setup_Debug_MSync;
		uint8_t						Setup_Awake_Timeout;
		uint8_t						Setup_Drift_Hold_Time;
		uAT42QT_REG__SETUP_241		Setup_Drift;
		uAT42QT_REG__SETUP_242		Setup_Lower_Signal_Limit_1;
		uAT42QT_REG__SETUP_243		Setup_Lower_Signal_Limit_2;
		uAT42QT_REG__SETUP_244		Setup_Freq_Hoping_Dwell;
		uint8_t						Setup_Frequency_0;
		uint8_t						Setup_Frequency_1;
		uint8_t						Setup_Frequency_2;
		uAT42QT_REG__SETUP_248		Setup_Noise;
		uint8_t						Setup_Host_CRC[2];	//!< Host CRC (LSB at addr 249, MSB at addr 250)
		uint8_t						Setup_Positive_Recalibration_Delay;
	} Reg;
} uAT42QT_REG_MAP;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// Slave init
FctERR AT42QT1244_Init(void);

// Low level access
FctERR AT42QT1244_Write(uint8_t * Buffer, uint16_t Addr, uint16_t nb);
FctERR AT42QT1244_Read(uint8_t * Buffer, uint16_t Addr, uint16_t nb);


/****************************************************************/
#include "AT42QT1244_ex.h"
#endif
#endif /* __AT42QT1244_H__ */
/****************************************************************/
