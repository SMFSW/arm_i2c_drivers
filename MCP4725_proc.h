/*!\file MCP4725_proc.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief MCP4725 Driver procedures declarations
** \details MCP4725: 12-Bit Digital-to-Analog Converter with EEPROM Memory
**/
/****************************************************************/
#ifndef __MCP4725_PROC_H__
	#define __MCP4725_PROC_H__

#include "MCP4725.h"

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


// *****************************************************************************
// Section: Types
// *****************************************************************************
typedef struct MCP4725_proc {
	uint16_t	DAC_4096;	//!< DAC output value (12bits)
	float		DAC_Volts;	//!< DAC output in Volts
	struct {
	MCP4725_cmd	Mode;		//!< Write commands mode
	MCP4725_pd	PowerDown;	//!< Power down mode
	float		VRef;		//!< Voltage reference
	} cfg;
} MCP4725_proc;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence of the MCP4725 peripheral
** \return FctERR - error code
**/
FctERR MCP4725_Init_Sequence(void);


FctERR MCP4725_Set_Mode(MCP4725_cmd mode);

FctERR MCP4725_Set_PowerDown(MCP4725_pd pd);

FctERR MCP4725_Set_VRef(float vref);


FctERR MCP4725_Set_Val(uint16_t val);

FctERR MCP4725_Set_Volts(float volts);


/****************************************************************/
#endif
#endif /* __MCP4725_PROC_H__ */
/****************************************************************/
