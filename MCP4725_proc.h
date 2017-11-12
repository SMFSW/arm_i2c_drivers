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

#include "sarmfsw.h"
#include "MCP4725.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct MCP4725_proc
** \brief MCP4725 user interface struct
**/
typedef struct MCP4725_proc {
	uint16_t	DAC_4096;	//!< DAC output value (12bits)
	float		DAC_Volts;	//!< DAC output in Volts
	struct {
	MCP4725_cmd	Mode;		//!< Write commands mode config
	MCP4725_pd	PowerDown;	//!< Power down mode config
	float		VRef;		//!< Voltage reference config
	} cfg;
} MCP4725_proc;

extern MCP4725_proc	MCP4725;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence of the MCP4725 peripheral
** \weak MCP4725 Init sequence may be user implemented if custom initialization sequence needed
** \return FctERR - error code
**/
__WEAK FctERR MCP4725_Init_Sequence(void);


/*!\brief Set mode of the MCP4725 peripheral
** \param[in] mode - operating mode of MCP4725
** \return FctERR - error code
**/
FctERR MCP4725_Set_Mode(const MCP4725_cmd mode);

/*!\brief Set Power down mode of the MCP4725 peripheral
** \param[in] pd - Power down mode of MCP4725
** \return FctERR - error code
**/
FctERR MCP4725_Set_PowerDown(const MCP4725_pd pd);

/*!\brief Set VRef for MCP4725 peripheral
** \param[in] vref - Voltage reference of MCP4725
** \return FctERR - error code
**/
FctERR MCP4725_Set_VRef(const float vref);


/*!\brief Write Raw DAC value to MCP4725 peripheral
** \param[in] val - Raw DAC value
** \return FctERR - error code
**/
FctERR MCP4725_Set_Val(const uint16_t val);

/*!\brief Write DAC value (in Volts) to MCP4725 peripheral
** \param[in] volts - Volts DAC value
** \return FctERR - error code
**/
FctERR MCP4725_Set_Volts(const float volts);


/****************************************************************/
#endif
#endif /* __MCP4725_PROC_H__ */
/****************************************************************/
