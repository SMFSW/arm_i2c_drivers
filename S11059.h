/*!\file S11059.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief S11059 Driver
** \details S11059: I2C interface-compatible color sensor
**/
/****************************************************************/
#ifndef S11059_H__
	#define S11059_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/

#ifndef I2C_S11059_NB
//! \note Define I2C_S11059_NB to enable multiple peripherals of this type
#define I2C_S11059_NB	1U	//!< Number of S11059 peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define S11059_ADDR			0x2AU			//!< S11059 address

#ifndef S11059_BASE_ADDR
//! \note Define S11059_BASE_ADDR to change default device base address
#define S11059_BASE_ADDR	S11059_ADDR		//!< S11059 Base address
#endif


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum _S11059_reg
** \brief Register map enum of S11059
**/
typedef enum PACK__ _S11059_reg {
	S11059__CONTROL = 0U,			//!< Control register
	S11059__MANUAL_TIMING_MSB,		//!< Manual timing register MSB
	S11059__MANUAL_TIMING_LSB,		//!< Manual timing register LSB
	S11059__RED_DATA_MSB,			//!< Sensor data register Red MSB
	S11059__RED_DATA_LSB,			//!< Sensor data register Red LSB
	S11059__GREEN_DATA_MSB,			//!< Sensor data register Green MSB
	S11059__GREEN_DATA_LSB,			//!< Sensor data register Green LSB
	S11059__BLUE_DATA_MSB,			//!< Sensor data register Blue MSB
	S11059__BLUE_DATA_LSB,			//!< Sensor data register Blue LSB
	S11059__IR_DATA_MSB,			//!< Sensor data register Infra-red MSB
	S11059__IR_DATA_LSB,			//!< Sensor data register Infra-red LSB
} S11059_reg;


/*!\enum _S11059_prescaler
** \brief Integration times of S11059
**/
typedef enum PACK__ _S11059_prescaler {
	S11059__INTEG_87_5US = 0U,	//!< 87.5us integration time
	S11059__INTEG_1_4MS,		//!< 1.4ms integration time
	S11059__INTEG_22_4MS,		//!< 22.4ms integration time
	S11059__INTEG_179_2MS,		//!< 179.2ms integration time
} S11059_prescaler;

/*!\enum _S11059_integ
** \brief Integration modes of S11059
**/
typedef enum PACK__ _S11059_integ {
	S11059__FIXED_PERIOD_INTEGRATION = 0U,	//!< Fixed period mode
	S11059__MANUAL_INTEGRATION,				//!< Manual setting mode
} S11059_integ;

/*!\enum _S11059_gain
** \brief Gain values of S11059
**/
typedef enum PACK__ _S11059_gain {
	S11059__LOW_GAIN = 0U,	//!< Low gain mode (x1)
	S11059__HIGH_GAIN,		//!< High gain mode (x10)
} S11059_gain;

/*!\enum _S11059_standby_fct
** \brief Standby function of S11059
**/
typedef enum PACK__ _S11059_standby_fct {
	S11059__OPERATING_MODE = 0U,	//!< Operating mode
	S11059__STANDBY_MODE,			//!< Standby mode
} S11059_standby_fct;

/*!\enum _S11059_adc_reset
** \brief ADC operation mode of S11059
**/
typedef enum PACK__ _S11059_adc_reset {
	S11059__ADC_OPERATION = 0U,	//!< Operation
	S11059__ADC_RESET,			//!< Reset
} S11059_adc_reset;


/*!\union _uS11059_REG__CONTROL
** \brief Union for CONTROL register of S11059
**/
typedef union _uS11059_REG__CONTROL {
	uint8_t Byte;
	struct PACK__ {
		S11059_prescaler	INTEG_PRESCL		:2;		//!< Integration time setting (prescaler)
		S11059_integ		INTEG_MODE			:1;		//!< Integration mode
		S11059_gain			GAIN_SELECTION		:1;		//!< Gain selection
		uint8_t									:1;
		S11059_standby_fct	STANDBY_MONITOR		:1;		//!< Standby function monitor (read only)
		S11059_standby_fct	STANDBY_FUNCTION	:1;		//!< Standby function (0: Operating mode, 1: Standby mode)
		S11059_adc_reset	ADC_RESET			:1;		//!< ADC reset (0: Operation, 1: Reset)
	} Bits;
} uS11059_REG__CONTROL;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for S11059 peripheral
** \param[in] idx - S11059 index
** \param[in] hi2c - pointer to S11059 I2C instance
** \param[in] devAddress - S11059 device address
** \return FctERR - error code
**/
FctERR NONNULL__ S11059_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress);

/*!\brief Initialization for S11059 peripheral
** \warning In case multiple devices (defined by I2C_S11059_NB > 1), you shall use S11059_Init instead
** \return FctERR - error code
**/
FctERR S11059_Init_Single(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for S11059
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR NONNULL__ S11059_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for S11059
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ S11059_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Word Write (big endian) function for S11059
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \return FctERR - error code
**/
FctERR NONNULL__ S11059_Write_Word(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr);


/*!\brief I2C Word Read (big endian) function for S11059
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \return FctERR - error code
**/
FctERR NONNULL__ S11059_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr);


/****************************************************************/
// cppcheck-suppress-begin misra-c2012-20.1 ; include directives after other declarations
#include "S11059_proc.h"	// Include procedures
#include "S11059_ex.h"		// Include extensions
// cppcheck-suppress-end misra-c2012-20.1

#endif

#ifdef __cplusplus
	}
#endif

#endif /* S11059_H__ */
/****************************************************************/
