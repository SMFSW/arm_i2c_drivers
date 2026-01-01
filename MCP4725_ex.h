/*!\file MCP4725_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief MCP4725 Driver extensions
** \details MCP4725: 12-Bit Digital-to-Analog Converter with EEPROM Memory
**/
/****************************************************************/
#ifndef MCP4725_EX_H__
	#define MCP4725_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "MCP4725.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define MCP4725__RESET	0x06U	//!< General call reset command
#define MCP4725__WAKEUP	0x09U	//!< General call wake up command


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Write Command to MCP4725 peripheral
** \param[in] pCpnt - Pointer to MCP4725 component
** \param[in] val - Raw DAC value for MCP4725 to write
** \return FctERR - error code
**/
FctERR NONNULL__ MCP4725_Write_Command(MCP4725_t * const pCpnt, const uint16_t val);

/*!\brief General call reset function for MCP4725
** \param[in] hi2c - pointer to general call I2C instance
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MCP4725_Reset(I2C_HandleTypeDef * const hi2c) {
	return MCP4725_General_Call(hi2c, MCP4725__RESET); }

/*!\brief General call wake up function for MCP4725
** \param[in] hi2c - pointer to general call I2C instance
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MCP4725_WakeUp(I2C_HandleTypeDef * const hi2c) {
	return MCP4725_General_Call(hi2c, MCP4725__WAKEUP); }

/*!\brief Read DAC value
** \param[in] pCpnt - Pointer to MCP4725 component
** \param[in] val - pointer to DAC value to read to
** \return FctERR - error code
**/
FctERR NONNULL__ MCP4725_Read_DAC(MCP4725_t * const pCpnt, uint16_t * const val);

/*!\brief Read DAC state
** \param[in] pCpnt - Pointer to MCP4725 component
** \param[in] state - pointer to state to read to (0 Ready, 1 Busy)
** \return FctERR - error code
**/
FctERR NONNULL__ MCP4725_Read_State(MCP4725_t * const pCpnt, bool * const state);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* MCP4725_EX_H__ */
/****************************************************************/
