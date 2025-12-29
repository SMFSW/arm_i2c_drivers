/*!\file AT42QT1244.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief AT42QT1244 Driver
** \details AT42QT1244: 24-key QMatrix FMEA IEC/EN/UL60730 Touch Sensor
**/
/****************************************************************/
#ifndef AT42QT1244_H__
	#define AT42QT1244_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/
// TODO: doxygen

#ifndef I2C_AT42QT1244_NB
//! \note Define I2C_AT42QT1244_NB to enable multiple peripherals of this type
#define I2C_AT42QT1244_NB	1U	//!< Number of AT42QT1244 peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define AT42QT1244_ADDR			57U					//!< AT42QT1244 address (A0 & A1 tied to GND)
#define AT42QT1244_ADDR_A0		7U					//!< AT42QT1244 address (A0 tied to PWR & A1 tied to GND)
#define AT42QT1244_ADDR_A1		17U					//!< AT42QT1244 address (A0 tied to GND & A1 tied to PWR)
#define AT42QT1244_ADDR_A01		117U				//!< AT42QT1244 address (A0 & A1 tied to PWR)

#ifndef AT42QT1244_BASE_ADDR
//! \note Define AT42QT1244_BASE_ADDR to change default device base address
#define AT42QT1244_BASE_ADDR	AT42QT1244_ADDR		//!< AT42QT1244 Base address
#endif

#ifndef AT42QT1244_CHECK_CRC
//! \note Define AT42QT1244_CHECK_CRC to change CRC check behavior
#define AT42QT1244_CHECK_CRC	1					//!< AT42QT1244 read transactions CRC check
#endif

#define AT42QT1244_MAX_KEYS		24U					//!< AT42QT1244 peripheral maximum number of keys


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum _AT42QT_reg
** \brief Register map enum of AT42QT
**/
typedef enum PACK__ _AT42QT_reg {
	//AT42QT__RES,	//!< Reserved
	//AT42QT__RES,	//!< Reserved
	AT42QT__CNT_100MS				= 2U,	//!<
	AT42QT__CNT_SIGNAL_FAIL			= 3U,	//!<
	AT42QT__CNT_MTX_SCAN			= 4U,	//!<
	AT42QT__DEVICE_STATUS			= 5U,	//!<
	AT42QT__DETECT_STATUS_1			= 6U,	//!<
	AT42QT__DETECT_STATUS_2			= 7U,	//!<
	AT42QT__DETECT_STATUS_3			= 8U,	//!<
	//AT42QT__RES,	//!< Reserved
	AT42QT__CURRENT_FREQUENCY		= 10U,	//!<
	AT42QT__CURRENT_PULSE_SPACING	= 11U,	//!<
	AT42QT__DATA_KEY_0				= 12U,	//!<
	AT42QT__DATA_KEY_1				= 16U,	//!<
	AT42QT__DATA_KEY_2				= 20U,	//!<
	AT42QT__DATA_KEY_3				= 24U,	//!<
	AT42QT__DATA_KEY_4				= 28U,	//!<
	AT42QT__DATA_KEY_5				= 32U,	//!<
	AT42QT__DATA_KEY_6				= 36U,	//!<
	AT42QT__DATA_KEY_7				= 40U,	//!<
	AT42QT__DATA_KEY_8				= 44U,	//!<
	AT42QT__DATA_KEY_9				= 48U,	//!<
	AT42QT__DATA_KEY_10				= 52U,	//!<
	AT42QT__DATA_KEY_11				= 56U,	//!<
	AT42QT__DATA_KEY_12				= 60U,	//!<
	AT42QT__DATA_KEY_13				= 64U,	//!<
	AT42QT__DATA_KEY_14				= 68U,	//!<
	AT42QT__DATA_KEY_15				= 72U,	//!<
	AT42QT__DATA_KEY_16				= 76U,	//!<
	AT42QT__DATA_KEY_17				= 80U,	//!<
	AT42QT__DATA_KEY_18				= 84U,	//!<
	AT42QT__DATA_KEY_19				= 88U,	//!<
	AT42QT__DATA_KEY_20				= 92U,	//!<
	AT42QT__DATA_KEY_21				= 96U,	//!<
	AT42QT__DATA_KEY_22				= 100U,	//!<
	AT42QT__DATA_KEY_23				= 104U,	//!<
	AT42QT__CONTROL_COMMAND			= 140U,	//!<
	//AT42QT__SETUPS							= 141U	//!<
	AT42QT__SETUP_KEYS_THRESHOLD_0				= 141U,	//!<
	AT42QT__SETUP_KEYS_THRESHOLD_1,		AT42QT__SETUP_KEYS_THRESHOLD_2,		AT42QT__SETUP_KEYS_THRESHOLD_3,		AT42QT__SETUP_KEYS_THRESHOLD_4,
	AT42QT__SETUP_KEYS_THRESHOLD_5,		AT42QT__SETUP_KEYS_THRESHOLD_6,		AT42QT__SETUP_KEYS_THRESHOLD_7,		AT42QT__SETUP_KEYS_THRESHOLD_8,
	AT42QT__SETUP_KEYS_THRESHOLD_9,		AT42QT__SETUP_KEYS_THRESHOLD_10,	AT42QT__SETUP_KEYS_THRESHOLD_11,	AT42QT__SETUP_KEYS_THRESHOLD_12,
	AT42QT__SETUP_KEYS_THRESHOLD_13,	AT42QT__SETUP_KEYS_THRESHOLD_14,	AT42QT__SETUP_KEYS_THRESHOLD_15,	AT42QT__SETUP_KEYS_THRESHOLD_16,
	AT42QT__SETUP_KEYS_THRESHOLD_17,	AT42QT__SETUP_KEYS_THRESHOLD_18,	AT42QT__SETUP_KEYS_THRESHOLD_19,	AT42QT__SETUP_KEYS_THRESHOLD_20,
	AT42QT__SETUP_KEYS_THRESHOLD_21,	AT42QT__SETUP_KEYS_THRESHOLD_22,	AT42QT__SETUP_KEYS_THRESHOLD_23,
	AT42QT__SETUP_KEYS_MODE_0					= 165U,	//!<
	AT42QT__SETUP_KEYS_MODE_1,	AT42QT__SETUP_KEYS_MODE_2,	AT42QT__SETUP_KEYS_MODE_3,	AT42QT__SETUP_KEYS_MODE_4,	AT42QT__SETUP_KEYS_MODE_5,
	AT42QT__SETUP_KEYS_MODE_6,	AT42QT__SETUP_KEYS_MODE_7,	AT42QT__SETUP_KEYS_MODE_8,	AT42QT__SETUP_KEYS_MODE_9,	AT42QT__SETUP_KEYS_MODE_10,
	AT42QT__SETUP_KEYS_MODE_11,	AT42QT__SETUP_KEYS_MODE_12,	AT42QT__SETUP_KEYS_MODE_13,	AT42QT__SETUP_KEYS_MODE_14,	AT42QT__SETUP_KEYS_MODE_15,
	AT42QT__SETUP_KEYS_MODE_16,	AT42QT__SETUP_KEYS_MODE_17,	AT42QT__SETUP_KEYS_MODE_18,	AT42QT__SETUP_KEYS_MODE_19,	AT42QT__SETUP_KEYS_MODE_20,
	AT42QT__SETUP_KEYS_MODE_21,	AT42QT__SETUP_KEYS_MODE_22,	AT42QT__SETUP_KEYS_MODE_23,
	AT42QT__SETUP_CFO_1_0						= 189U,	//!<
	AT42QT__SETUP_CFO_1_1,	AT42QT__SETUP_CFO_1_2,	AT42QT__SETUP_CFO_1_3,	AT42QT__SETUP_CFO_1_4,	AT42QT__SETUP_CFO_1_5,
	AT42QT__SETUP_CFO_1_6,	AT42QT__SETUP_CFO_1_7,	AT42QT__SETUP_CFO_1_8,	AT42QT__SETUP_CFO_1_9,	AT42QT__SETUP_CFO_1_10,
	AT42QT__SETUP_CFO_1_11,	AT42QT__SETUP_CFO_1_12,	AT42QT__SETUP_CFO_1_13,	AT42QT__SETUP_CFO_1_14,	AT42QT__SETUP_CFO_1_15,
	AT42QT__SETUP_CFO_1_16,	AT42QT__SETUP_CFO_1_17,	AT42QT__SETUP_CFO_1_18,	AT42QT__SETUP_CFO_1_19,	AT42QT__SETUP_CFO_1_20,
	AT42QT__SETUP_CFO_1_21,	AT42QT__SETUP_CFO_1_22,	AT42QT__SETUP_CFO_1_23,
	AT42QT__SETUP_CFO_2_0						= 213U,	//!<
	AT42QT__SETUP_CFO_2_1,	AT42QT__SETUP_CFO_2_2,	AT42QT__SETUP_CFO_2_3,	AT42QT__SETUP_CFO_2_4,	AT42QT__SETUP_CFO_2_5,
	AT42QT__SETUP_CFO_2_6,	AT42QT__SETUP_CFO_2_7,	AT42QT__SETUP_CFO_2_8,	AT42QT__SETUP_CFO_2_9,	AT42QT__SETUP_CFO_2_10,
	AT42QT__SETUP_CFO_2_11,	AT42QT__SETUP_CFO_2_12,	AT42QT__SETUP_CFO_2_13,	AT42QT__SETUP_CFO_2_14,	AT42QT__SETUP_CFO_2_15,
	AT42QT__SETUP_CFO_2_16,	AT42QT__SETUP_CFO_2_17,	AT42QT__SETUP_CFO_2_18,	AT42QT__SETUP_CFO_2_19,	AT42QT__SETUP_CFO_2_20,
	AT42QT__SETUP_CFO_2_21,	AT42QT__SETUP_CFO_2_22,	AT42QT__SETUP_CFO_2_23,
	AT42QT__SETUP_NEGATIVE_RECALL_DELAY			= 237U,	//!<
	AT42QT__SETUP_DEBUG_MSYNC					= 238U,	//!<
	AT42QT__SETUP_AWAKE_TIMEOUT					= 239U,	//!<
	AT42QT__SETUP_DRIFT_HOLD_TIME				= 240U,	//!<
	AT42QT__SETUP_DRIFT							= 241U,	//!<
	AT42QT__SETUP_LOWER_SIGNAL_LIMIT_1			= 242U,	//!<
	AT42QT__SETUP_LOWER_SIGNAL_LIMIT_2			= 243U,	//!<
	AT42QT__SETUP_FREQ_HOPING_DWELL				= 244U,	//!<
	AT42QT__SETUP_FREQ0							= 245U,	//!<
	AT42QT__SETUP_FREQ1							= 246U,	//!<
	AT42QT__SETUP_FREQ2							= 247U,	//!<
	AT42QT__SETUP_NOISE							= 248U,	//!<
	AT42QT__SETUP_HOST_CRC_LSB					= 249U,	//!<
	AT42QT__SETUP_HOST_CRC_MSB					= 250U	//!<
} AT42QT_reg;


typedef enum PACK__ _AT42QT_cmd {
	AT42QT__CALIBRATE_KEY_0			= 0U,		//!< Calibration: Key 0
	AT42QT__CALIBRATE_KEY_1,					//!< Calibration: Key 1
	AT42QT__CALIBRATE_KEY_2,					//!< Calibration: Key 2
	AT42QT__CALIBRATE_KEY_3,					//!< Calibration: Key 3
	AT42QT__CALIBRATE_KEY_4,					//!< Calibration: Key 4
	AT42QT__CALIBRATE_KEY_5,					//!< Calibration: Key 5
	AT42QT__CALIBRATE_KEY_6,					//!< Calibration: Key 6
	AT42QT__CALIBRATE_KEY_7,					//!< Calibration: Key 7
	AT42QT__CALIBRATE_KEY_8,					//!< Calibration: Key 8
	AT42QT__CALIBRATE_KEY_9,					//!< Calibration: Key 9
	AT42QT__CALIBRATE_KEY_10,					//!< Calibration: Key 10
	AT42QT__CALIBRATE_KEY_11,					//!< Calibration: Key 11
	AT42QT__CALIBRATE_KEY_12,					//!< Calibration: Key 12
	AT42QT__CALIBRATE_KEY_13,					//!< Calibration: Key 13
	AT42QT__CALIBRATE_KEY_14,					//!< Calibration: Key 14
	AT42QT__CALIBRATE_KEY_15,					//!< Calibration: Key 15
	AT42QT__CALIBRATE_KEY_16,					//!< Calibration: Key 16
	AT42QT__CALIBRATE_KEY_17,					//!< Calibration: Key 17
	AT42QT__CALIBRATE_KEY_18,					//!< Calibration: Key 18
	AT42QT__CALIBRATE_KEY_19,					//!< Calibration: Key 19
	AT42QT__CALIBRATE_KEY_20,					//!< Calibration: Key 20
	AT42QT__CALIBRATE_KEY_21,					//!< Calibration: Key 21
	AT42QT__CALIBRATE_KEY_22,					//!< Calibration: Key 22
	AT42QT__CALIBRATE_KEY_23,					//!< Calibration: Key 23
	AT42QT__RESET_DEVICE			= 0x18U,	//!< Reset device
	AT42QT__LOW_LEVEL_CALIBRATION	= 0xFDU,	//!< Calibration: Low level and frequency hoping
	AT42QT__WRITE_SETUPS			= 0xFEU,	//!< Write 0xFD immediately before writing setups
	AT42QT__CALIBRATE_ALL_KEYS		= 0xFFU,	//!< Calibration: all keys
} AT42QT_cmd;

typedef enum PACK__ _AT42QT_FHM {
	AT42QT__FHM_OFF = 0U,					//!< Frequency Hoping Mode OFF
	AT42QT__FHM_CALIB_AFTER_HOP,			//!< Calibrate all keys after hop
	AT42QT__FHM_ADJ_KEYS_REF_DURING_HOP,	//!< Adjust each keys reference during hop
	AT42QT__FHM_FREQUENCY_SWEEP,			//!< Frequency sweep
} AT42QT_FHM;


typedef union _uAT42QT_REG__KEY_DATA {
	uint8_t	Bytes[4];
	uint32_t Raw;
	struct PACK__ {
		uint32_t Signal						:12;	//!< Signal
		uint32_t Normal_Detector_Integrator	:4;		//!< Normal detector integrator
		uint32_t Reference					:12;	//!< Reference
		uint32_t LSL_Failure				:1;		//!< LSL failure
		uint32_t FMEA_Failure				:1;		//!< FMEA failure
		uint32_t 							:2;
	} Data;
} uAT42QT_REG__KEY_DATA;

typedef union _uAT42QT_REG__DEVICE_STATUS {
	uint8_t Byte;
	struct PACK__ {
		uint8_t CRC_Incorrect		:1;		//!< Setups CRC does not match HCRC
		uint8_t Mains_Sync_Error	:1;		//!< Mains sync error
		uint8_t Key_In_Calib		:1;		//!< Key in calibration
		uint8_t LSL_Failure			:1;		//!< LSL failure
		uint8_t FMEA_Failure		:1;		//!< FMEA failure
		uint8_t Reserved			:3;		//!< Reserved bits
	} Bits;
} uAT42QT_REG__DEVICE_STATUS;


typedef union _uAT42QT_REG__SETUP_141_164 {
	uint8_t Byte;
	struct PACK__ {
		uint8_t NTHR_PTHR	:2;		//!< Threshold
		uint8_t NDRIFT		:3;		//!< Drift compensation
		uint8_t BL			:2;		//!< Burst length
	} Bits;
} uAT42QT_REG__SETUP_141_164;

typedef union _uAT42QT_REG__SETUP_165_188 {
	uint8_t Byte;
	struct PACK__ {
		uint8_t NDIL	:3;		//!< No detection
		uint8_t FDIL	:3;		//!< False detection
		uint8_t AKS		:1;		//!< Adjacent Key suppression technology
		uint8_t WAKE	:1;		//!< Wake on touch
	} Bits;
} uAT42QT_REG__SETUP_165_188;

typedef union _uAT42QT_REG__SETUP_238 {
	uint8_t Byte;
	struct PACK__ {
		uint8_t SLEEP	:3;		//!< Sleep duration
		uint8_t MSYNC	:1;		//!< Main sync
		uint8_t NHYST	:2;		//!< Negative hysteresis
		uint8_t DBUG	:1;		//!< Debug output (E removed not to be asserted as DEBUG macro)
	} Bits;
} uAT42QT_REG__SETUP_238;

typedef union _uAT42QT_REG__SETUP_241 {
	uint8_t Byte;
	struct PACK__ {
		uint8_t PDRIFT	:3;		//!< Positive drift compensation
		uint8_t SSYNC	:5;		//!< Oscilloscope sync
	} Bits;
} uAT42QT_REG__SETUP_241;

typedef union _uAT42QT_REG__SETUP_242 {
	uint8_t Byte;
	struct PACK__ {
		uint8_t LSL		:8;		//!< Lower signal limit (LSB)
	} Bits;
} uAT42QT_REG__SETUP_242;

typedef union _uAT42QT_REG__SETUP_243 {
	uint8_t Byte;
	struct PACK__ {
		uint8_t LSL		:3;		//!< Lower signal limit (MSB)
		uint8_t KGTT	:5;		//!< Key gain test threshold
	} Bits;
} uAT42QT_REG__SETUP_243;

typedef union _uAT42QT_REG__SETUP_244 {
	uint8_t Byte;
	struct PACK__ {
		uint8_t		DWELL	:3;		//!< Dwell time
		uint8_t		RIB		:1;		//!< Restart interrupted burst
		uint8_t		THRM	:2;		//!< Threshold multiplier
		AT42QT_FHM	FHM		:2;		//!< Frequency hoping mode
	} Bits;
} uAT42QT_REG__SETUP_244;

typedef union _uAT42QT_REG__SETUP_248 {
	uint8_t Byte;
	struct PACK__ {
		uint8_t NSTHR	:4;		//!< Noise threshold
		uint8_t NIL		:4;		//!< Noise integrator limit
	} Bits;
} uAT42QT_REG__SETUP_248;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for AT42QT1244 peripheral
** \param[in] idx - AT42QT1244 index
** \param[in] hi2c - pointer to AT42QT1244 I2C instance
** \param[in] devAddress - AT42QT1244 device address
** \return FctERR - error code
**/
FctERR NONNULL__ AT42QT1244_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress);

/*!\brief Initialization for AT42QT1244 peripheral
** \warning In case multiple devices (defined by I2C_AT42QT1244_NB > 1), you shall use AT42QT1244_Init instead
** \return FctERR - error code
**/
FctERR AT42QT1244_Init_Single(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for AT42QT1244
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR NONNULL__ AT42QT1244_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for AT42QT1244
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ AT42QT1244_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb);


/****************************************************************/
// cppcheck-suppress-begin misra-c2012-20.1 ; include directives after other declarations
#include "AT42QT1244_proc.h"	// Include procedures
#include "AT42QT1244_ex.h"		// Include extensions
// cppcheck-suppress-end misra-c2012-20.1

#endif

#ifdef __cplusplus
	}
#endif

#endif /* AT42QT1244_H__ */
/****************************************************************/
