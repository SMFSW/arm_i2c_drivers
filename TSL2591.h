/*!\file TSL2591.h
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief TSL2591 Driver
** \details TSL2591: Very-high sensitivity light-to-digital converter
**/
/****************************************************************/
#ifndef __TSL2591_H__
	#define __TSL2591_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/

#ifndef I2C_TSL2591_NB
//! \note Define I2C_TSL2591_NB to enable multiple peripherals of this type
#define I2C_TSL2591_NB	1	//!< Number of TSL2591 peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
#ifndef TSL2591_BASE_ADDR
//! \note Define TSL2591_BASE_ADDR to change default device base address
#define TSL2591_BASE_ADDR		0x29		//!< TSL2591 Base address
#endif


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum TSL2591_reg_map
** \brief Register map enum of TSL2591
**/
typedef enum PACK__ TSL2591_reg_map {
	TSL2591__ENABLE = 0,	//!< Enables states and interrupts
	TSL2591__CONFIG,		//!< ALS gain and integration time configuration
	TSL2591__AILTL = 0x04,	//!< ALS interrupt low threshold low byte
	TSL2591__AILTH,			//!< ALS interrupt low threshold high byte
	TSL2591__AIHTL,			//!< ALS interrupt high threshold low byte
	TSL2591__AIHTH,			//!< ALS interrupt high threshold high byte
	TSL2591__NPAILTL,		//!< No Persist ALS interrupt low threshold low byte
	TSL2591__NPAILTH,		//!< No Persist ALS interrupt low threshold high byte
	TSL2591__NPAIHTL,		//!< No Persist ALS interrupt high threshold low byte
	TSL2591__NPAIHTH,		//!< No Persist ALS interrupt high threshold high byte
	TSL2591__PERSIST,		//!< Interrupt persistence filter
	TSL2591__PID = 0x11,	//!< Package ID
	TSL2591__ID,			//!< Device ID
	TSL2591__STATUS,		//!< Device status
	TSL2591__C0DATAL,		//!< CH0 ADC low data byte
	TSL2591__C0DATAH,		//!< CH0 ADC high data byte
	TSL2591__C1DATAL,		//!< CH1 ADC low data byte
	TSL2591__C1DATAH		//!< CH1 ADC high data byte
} TSL2591_reg;

/*!\enum TSL2591_transaction
** \brief Transaction types of TSL2591
**/
typedef enum PACK__ TSL2591_transaction {
	TSL2591__TRANS_NORMAL_OP = 1,		//!< Normal operation
	TSL2591__TRANS_SPECIAL_FUNC = 3,	//!< Special Function
} TSL2591_transaction;

/*!\enum TSL2591_spec_func
** \brief Special functions of TSL2591
**/
typedef enum PACK__ TSL2591_spec_func {
	TSL2591__SF_FORCE_IT = 4,				//!< Interrupt set (forces an interrupt)
	TSL2591__SF_CLR_ALS_IT = 6,				//!< Clears ALS interrupt
	TSL2591__SF_CLR_ALS_AND_NO_PERS = 7,	//!< Clears ALS and no persist ALS interrupt
	TSL2591__SF_CLR_NO_PERS = 9				//!< Clears no persist ALS interrupt
} TSL2591_spec_func;

/*!\enum TSL2591_gain
** \brief Gain values of TSL2591
**/
typedef enum PACK__ TSL2591_gain {
	TSL2591__LOW_GAIN = 0,	//!< Low gain mode (x1)
	TSL2591__MEDIUM_GAIN,	//!< Medium gain mode (x25)
	TSL2591__HIGH_GAIN,		//!< High gain mode (x428)
	TSL2591__MAXIMUM_GAIN	//!< Maximum gain mode (x9876)
} TSL2591_gain;

/*!\enum TSL2591_integ
** \brief Integration times of TSL2591
**/
typedef enum PACK__ TSL2591_integ {
	TSL2591__INTEG_100MS = 0,	//!< 100ms integration time
	TSL2591__INTEG_200MS,		//!< 200ms integration time
	TSL2591__INTEG_300MS,		//!< 300ms integration time
	TSL2591__INTEG_400MS,		//!< 400ms integration time
	TSL2591__INTEG_500MS,		//!< 500ms integration time
	TSL2591__INTEG_600MS		//!< 600ms integration time
} TSL2591_integ;

/*!\enum TSL2591_it_persist
** \brief Persistence control of TSL2591
**/
typedef enum PACK__ TSL2591_it_persist {
	TSL2591__PERSIST_EVERY_ADC_CYCLE = 0,	//!< Every ADC cycle generates interrupt
	TSL2591__PERSIST_ANY_OUTSIDE_THRESHOLD,	//!< Any value outside of threshold range
	TSL2591__PERSIST_2_IT_PERIODS,			//!< 2 integration time periods out of range
	TSL2591__PERSIST_3_IT_PERIODS,			//!< 3 integration time periods out of range
	TSL2591__PERSIST_5_IT_PERIODS,			//!< 5 integration time periods out of range
	TSL2591__PERSIST_10_IT_PERIODS,			//!< 10 integration time periods out of range
	TSL2591__PERSIST_15_IT_PERIODS,			//!< 15 integration time periods out of range
	TSL2591__PERSIST_20_IT_PERIODS,			//!< 20 integration time periods out of range
	TSL2591__PERSIST_25_IT_PERIODS,			//!< 25 integration time periods out of range
	TSL2591__PERSIST_30_IT_PERIODS,			//!< 30 integration time periods out of range
	TSL2591__PERSIST_35_IT_PERIODS,			//!< 35 integration time periods out of range
	TSL2591__PERSIST_40_IT_PERIODS,			//!< 40 integration time periods out of range
	TSL2591__PERSIST_45_IT_PERIODS,			//!< 45 integration time periods out of range
	TSL2591__PERSIST_50_IT_PERIODS,			//!< 50 integration time periods out of range
	TSL2591__PERSIST_55_IT_PERIODS,			//!< 55 integration time periods out of range
	TSL2591__PERSIST_60_IT_PERIODS,			//!< 60 integration time periods out of range
} TSL2591_it_persist;


/*!\union uTSL2591_CMD
** \brief Union for COMMAND register of TSL2591
**/
typedef union uTSL2591_CMD {
	uint8_t Byte;
	struct PACK__ {
		TSL2591_spec_func	ADDR		:5;		//!< Address field/special function field
		TSL2591_transaction	TRANSACTION	:2;		//!< Select type of transaction to follow in subsequent data transfers
		uint8_t				CMD			:1;		//!< Select command register. Must write as 1 when addressing Command register
	} Bits;
} uTSL2591_CMD;


/*!\union uTSL2591_REG__ENABLE
** \brief Union for ENABLE register of TSL2591
**/
typedef union uTSL2591_REG__ENABLE {
	uint8_t Byte;
	struct PACK__ {
		uint8_t PON		:1;	//!< Power ON. This field activates the internal oscillator to permit the timers and ADC channels to operate
		uint8_t AEN		:1;	//!< ALS Enable. This field activates ALS function. Writing a one activates the ALS. Writing a zero disables the ALS
		uint8_t 		:1;
		uint8_t	AIEN	:1;	//!< ALS Interrupt Enable. When asserted permits ALS interrupts to be generated, subject to the persist filter
		uint8_t 		:1;
		uint8_t SAI		:1;	//!< Sleep after interrupt. When asserted, the device will power down at the end of an ALS cycle if an interrupt has been generated
		uint8_t NPIEN	:1;	//!< No Persist Interrupt Enable. When asserted NP Threshold conditions will generate an interrupt, bypassing the persist filter
	} Bits;
} uTSL2591_REG__ENABLE;


/*!\union uTSL2591_REG__CONFIG
** \brief Union for CONFIG register of TSL2591
**/
typedef union uTSL2591_REG__CONFIG {
	uint8_t Byte;
	struct PACK__ {
		TSL2591_integ	ATIME	:3;	//!< ALS time sets the internal ADC integration time for both photo-diode channels
		uint8_t					:1;
		TSL2591_gain	AGAIN	:2;	//!< ALS gain sets the gain for internal integration amplifiers for both photo-diode channels
		uint8_t					:1;
		uint8_t			SRESET	:1;	//!< System reset. When asserted, the device will reset equivalent to a power-on reset. SRESET is self-clearing
	} Bits;
} uTSL2591_REG__CONFIG;


/*!\union uTSL2591_REG__PERSIST
** \brief Union for PERSIST register of TSL2591
**/
typedef union uTSL2591_REG__PERSIST {
	uint8_t Byte;
	struct PACK__ {
		TSL2591_it_persist	PERSIST	:4;	//!< ALS interrupt persistence filter
		uint8_t 					:4;	//!< Reserved. Write as 0
	} Bits;
} uTSL2591_REG__PERSIST;


/*!\union uTSL2591_REG__STATUS
** \brief Union for STATUS register of TSL2591
**/
typedef union uTSL2591_REG__STATUS {
	uint8_t Byte;
	struct PACK__ {
		uint8_t AVALID	:1;		//!< ALS Valid. Indicates that the ADC channels have completed an integration cycle since the AEN bit was asserted
		uint8_t			:3;
		uint8_t AINT	:1;		//!< ALS Interrupt. Indicates that the device is asserting an ALS interrupt
		uint8_t NPINTR	:1;		//!< No-persist Interrupt. Indicates that the device has encountered a no-persist interrupt condition
		uint8_t 		:2;
	} Bits;
} uTSL2591_REG__STATUS;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for TSL2591 peripheral
** \param[in] idx - TSL2591 index
** \param[in] hi2c - pointer to TSL2591 I2C instance
** \param[in] devAddress - TSL2591 device address
** \return FctERR - error code
**/
FctERR NONNULL__ TSL2591_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress);

/*!\brief Initialization for TSL2591 peripheral
** \warning In case multiple devices (defined by I2C_TSL2591_NB > 1), you shall use TSL2591_Init instead
** \return FctERR - error code
**/
FctERR TSL2591_Init_Single(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for TSL2591
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR NONNULL__ TSL2591_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for TSL2591
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ TSL2591_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Word Write (little endian) function for TSL2591
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \return FctERR - error code
**/
FctERR NONNULL__ TSL2591_Write_Word(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr);


/*!\brief I2C Word Read (little endian) function for TSL2591
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \return FctERR - error code
**/
FctERR NONNULL__ TSL2591_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr);


/*!\brief I2C Special function Write function for TSL2591
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] func - special function to execute
** \return FctERR - error code
**/
FctERR NONNULL__ TSL2591_Write_Special(I2C_slave_t * const pSlave, const TSL2591_spec_func func);


/****************************************************************/
#include "TSL2591_proc.h"	// Include procedures
#include "TSL2591_ex.h"		// Include extensions

#endif

#ifdef __cplusplus
	}
#endif

#endif /* __TSL2591_H__ */
/****************************************************************/
