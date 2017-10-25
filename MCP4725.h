/*!\file MCP4725.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief MCP4725 Driver declarations
** \details MCP4725: 12-Bit Digital-to-Analog Converter with EEPROM Memory
**/
/****************************************************************/
#ifndef __MCP4725_H__
	#define __MCP4725_H__

#include "sarmfsw.h"
#include "I2C_component.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


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
extern I2C_slave MCP4725_hal;	//!< MCP4725 Slave instance


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
** \weak MCP4725 Base address may be changed if user implemented
** \return FctERR - error code
**/
__weak FctERR MCP4725_Init(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for MCP4725
** \param[in] data - pointer to write from
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR MCP4725_Write(uint8_t * data, uint16_t nb);


/*!\brief I2C Read function for MCP4725
** \param[in,out] data - pointer to read to
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR MCP4725_Read(uint8_t * data, uint16_t nb);


FctERR MCP4725_General_Call(uint8_t cmd);


/****************************************************************/
#include "MCP4725_ex.h"		// Include extensions
#include "MCP4725_proc.h"	// Include procedures
#endif
#endif /* __MCP4725_H__ */
/****************************************************************/
