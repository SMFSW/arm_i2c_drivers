/*!\file S11059.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief S11059 Driver
** \details S11059: I2C interface-compatible color sensor
**/
/****************************************************************/
#ifndef __S11059_H__
	#define __S11059_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "globals.h"

#include "I2C_component.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define S11059_ADDR			0x2A			//!< S11059 address

#define S11059_BASE_ADDR	S11059_ADDR		//!< S11059 Base address


// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern I2C_slave_t S11059_hal;	//!< S11059 Slave structure


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum S11059_reg_map
** \brief Register map enum of S11059
**/
typedef enum PACK__ S11059_reg_map {
	S11059__CONTROL = 0,			//!< Control register
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


/*!\enum S11059_prescaler
** \brief Integration times of S11059
**/
typedef enum PACK__ S11059_integ {
	S11059__INTEG_87_5US = 0,	//!< 87.5us integration time
	S11059__INTEG_1_4MS,		//!< 1.4ms integration time
	S11059__INTEG_22_4MS,		//!< 22.4ms integration time
	S11059__INTEG_179_2MS,		//!< 179.2ms integration time
} S11059_prescaler;

/*!\enum S11059_integ
** \brief Integration modes of S11059
**/
typedef enum PACK__ S11059_integ_mode {
	S11059__FIXED_PERIOD_INTEGRATION = 0,	//!< Fixed period mode
	S11059__MANUAL_INTEGRATION,				//!< Manual setting mode
} S11059_integ;

/*!\enum S11059_gain
** \brief Gain values of S11059
**/
typedef enum PACK__ S11059_gain {
	S11059__LOW_GAIN = 0,	//!< Low gain mode (x1)
	S11059__HIGH_GAIN,		//!< High gain mode (x10)
} S11059_gain;

/*!\enum S11059_standby_fct
** \brief Standby function of S11059
**/
typedef enum PACK__ S11059_standby_fct {
	S11059__OPERATING_MODE = 0,	//!< Operating mode
	S11059__STANDBY_MODE,		//!< Standby mode
} S11059_standby_fct;

/*!\enum S11059_adc_reset
** \brief ADC operation mode of S11059
**/
typedef enum PACK__ S11059_adc_reset {
	S11059__ADC_OPERATION = 0,	//!< Operation
	S11059__ADC_RESET,			//!< Reset
} S11059_adc_reset;


/*!\union uS11059_REG__CONTROL
** \brief Union for CONTROL register of S11059
**/
typedef union uS11059_REG__CONTROL {
	uint8_t Byte;
	struct {
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
** \weak S11059 Base address may be changed if user implemented
** \return FctERR - error code
**/
__WEAK FctERR S11059_Init(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for S11059
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR NONNULL__ S11059_Write(const uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for S11059
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ S11059_Read(uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Word Write (big endian) function for S11059
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \return FctERR - error code
**/
FctERR NONNULL__ S11059_Write_Word(const uint16_t * data, const uint16_t addr);


/*!\brief I2C Word Read (big endian) function for S11059
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \return FctERR - error code
**/
FctERR NONNULL__ S11059_Read_Word(uint16_t * data, const uint16_t addr);


/****************************************************************/
#include "S11059_ex.h"		// Include extensions
#include "S11059_proc.h"	// Include procedures

#ifdef __cplusplus
	}
#endif

#endif
#endif /* __S11059_H__ */
/****************************************************************/
