/*!\file MCP4725.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief MCP4725 Driver
** \details MCP4725: 12-Bit Digital-to-Analog Converter with EEPROM Memory
**/
/****************************************************************/
#ifndef __MCP4725_H__
	#define __MCP4725_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "globals.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/

#ifndef I2C_MCP4725_NB
//! \note Define I2C_MCP4725_NB in globals.h or at project to enable multiple peripherals of this type
#define I2C_MCP4725_NB	1	//!< Number of MCP4725 peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
// For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR pin tied to VDD)
// For MCP4725A0 the address is 0x60 or 0x61  // Microchip EvalBoard
// For MCP4725A2 the address is 0x64 or 0x65
#define MCP4725A0_ADDR		0x60			//!< MCP4725 address
#define MCP4725A1_ADDR		0x62			//!< MCP4725 with A1 address
#define MCP4725A2_ADDR		0x64			//!< MCP4725 with A2 address

#define MCP4725_BASE_ADDR	MCP4725A1_ADDR	//!< MCP4725 Base address


// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern I2C_slave_t MCP4725_hal[I2C_MCP4725_NB];	//!< MCP4725 Slave structure


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum MCP4725_cmd
** \brief Register map enum of MCP4725
**/
typedef enum PACK__ MCP4725_cmd {
	MCP4725__FAST_MODE = 0,		//!< This command is used to change the DAC register. EEPROM is not affected
	MCP4725__WRITE_DAC = 2,		//!< Load configuration bits and data code to the DAC Register
	MCP4725__WRITE_DAC_EEP,		//!< Load configuration bits and data code to the DAC Register and also write the EEPROM
	MCP4725__RESERVED_1,		//!< Reserved for future use
	MCP4725__RESERVED_2,		//!< Reserved for future use
	MCP4725__RESERVED_3,		//!< Reserved for future use
	MCP4725__RESERVED_4,		//!< Reserved for future use
} MCP4725_cmd;


/*!\enum MCP4725_pd
** \brief Power down enum of MCP4725
**/
typedef enum PACK__ MCP4725_pd {
	MCP4725__PWR_NORMAL = 0,	//!< Normal mode
	MCP4725__PWR_DOWN_1K,		//!< 1K Ohm resistor to ground
	MCP4725__PWR_DOWN_100K,		//!< 100K Ohm resistor to ground
	MCP4725__PWR_DOWN_500K		//!< 500K Ohm resistor to ground
} MCP4725_pd;


/*!\union uMCP4725_REG__CMD
** \brief Union for CMD of MCP4725
**/
typedef union uMCP4725_REG__CMD {
	uint8_t Byte;
	struct {
		uint8_t				:1;
		MCP4725_pd	PD		:2;		//!< Power down mode
		uint8_t				:2;
		MCP4725_cmd	C		:3;		//!< Write command type
	} Bits;
} uMCP4725_REG__CMD;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization of the MCP4725 peripheral
** \param[in] idx - MCP4725 index
** \param[in] hi2c - pointer to MCP4725 I2C instance
** \param[in] devAddress - MCP4725 device address
** \return FctERR - error code
**/
FctERR NONNULL__ MCP4725_Init(const uint8_t idx, const I2C_HandleTypeDef * hi2c, const uint16_t devAddress);

/*!\brief Initialization for MCP4725 peripheral
** \warning In case multiple devices (defined by I2C_MCP4725_NB > 1), you shall use MCP4725_Init instead
** \return FctERR - error code
**/
FctERR MCP4725_Init_Single(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C general call for MCP4725
** \param[in] hi2c - pointer to general call I2C instance
** \param[in] cmd - Command to send
** \return FctERR - error code
**/
FctERR NONNULL__ MCP4725_General_Call(const I2C_HandleTypeDef * hi2c, const uint8_t cmd);


/*!\brief I2C Write function for MCP4725
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR NONNULL__ MCP4725_Write(I2C_slave_t * pSlave, const uint8_t * data, const uint16_t nb);


/*!\brief I2C Read function for MCP4725
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ MCP4725_Read(I2C_slave_t * pSlave, uint8_t * data, const uint16_t nb);


/****************************************************************/
#include "MCP4725_proc.h"	// Include procedures
#include "MCP4725_ex.h"		// Include extensions

#ifdef __cplusplus
	}
#endif

#endif
#endif /* __MCP4725_H__ */
/****************************************************************/
