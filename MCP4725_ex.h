/*!\file MCP4725_ex.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief MCP4725 Driver extensions declarations
** \details MCP4725: 12-Bit Digital-to-Analog Converter with EEPROM Memory
**/
/****************************************************************/
#ifndef __MCP4725_EX_H__
	#define __MCP4725_EX_H__

#include "sarmfsw.h"
#include "MCP4725.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define MCP4725__RESET	0x06	//!< General call reset command
#define MCP4725__WAKEUP	0x09	//!< General call wake up command


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Write Command to MCP4725 peripheral
** \param[in] val - Raw DAC value for MCP4725 to write
** \return FctERR - error code
**/
FctERR MCP4725_Write_Command(uint16_t val);

/*!\brief General call reset function for MCP4725
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ MCP4725_Reset(void) {
	return MCP4725_General_Call(MCP4725__RESET); }

/*!\brief General call wake up function for MCP4725
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ MCP4725_WakeUp(void) {
	return MCP4725_General_Call(MCP4725__WAKEUP); }

/*!\brief Read DAC value
** \param[in] val - pointer to DAC value to read to
** \return FctERR - error code
**/
FctERR MCP4725_Read_DAC(uint16_t * val);

/*!\brief Read DAC state
** \param[in] state - pointer to state to read to (0 Ready, 1 Busy)
** \return FctERR - error code
**/
FctERR MCP4725_Read_State(bool * state);


/****************************************************************/
#endif
#endif /* __MCP4725_EX_H__ */
/****************************************************************/
