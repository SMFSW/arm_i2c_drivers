/*!\file MCP4725_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
** \brief MCP4725 Driver procedures
** \details MCP4725: 12-Bit Digital-to-Analog Converter with EEPROM Memory
**/
/****************************************************************/
#ifndef __MCP4725_PROC_H__
	#define __MCP4725_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "MCP4725.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct MCP4725_t
** \brief MCP4725 user interface struct
**/
typedef struct MCP4725_t {
	uint16_t		DAC_4096;		//!< DAC output value (12bits)
	float			DAC_Volts;		//!< DAC output in Volts
	struct {
	I2C_slave_t *	slave_inst;		//!< Slave structure
	MCP4725_cmd		Mode;			//!< Write commands mode config
	MCP4725_pd		PowerDown;		//!< Power down mode config
	float			VRef;			//!< Voltage reference config
	} cfg;
} MCP4725_t;

extern MCP4725_t	MCP4725[I2C_MCP4725_NB];	//!< MCP4725 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence of the MCP4725 peripheral
** \weak MCP4725 Init sequence may be user implemented if custom initialization sequence needed
** \param[in] pCpnt - Pointer to MCP4725 component
** \return FctERR - error code
**/
FctERR NONNULL__ MCP4725_Init_Sequence(MCP4725_t * const pCpnt);


/*!\brief Set mode of the MCP4725 peripheral
** \param[in] pCpnt - Pointer to MCP4725 component
** \param[in] mode - operating mode of MCP4725
** \return FctERR - error code
**/
FctERR NONNULL__ MCP4725_Set_Mode(MCP4725_t * const pCpnt, const MCP4725_cmd mode);

/*!\brief Set Power down mode of the MCP4725 peripheral
** \param[in] pCpnt - Pointer to MCP4725 component
** \param[in] pd - Power down mode of MCP4725
** \return FctERR - error code
**/
FctERR NONNULL__ MCP4725_Set_PowerDown(MCP4725_t * const pCpnt, const MCP4725_pd pd);

/*!\brief Set VRef for MCP4725 peripheral
** \param[in] pCpnt - Pointer to MCP4725 component
** \param[in] vref - Voltage reference of MCP4725
** \return FctERR - error code
**/
FctERR NONNULL__ MCP4725_Set_VRef(MCP4725_t * const pCpnt, const float vref);


/*!\brief Write Raw DAC value to MCP4725 peripheral
** \param[in] pCpnt - Pointer to MCP4725 component
** \param[in] val - Raw DAC value
** \return FctERR - error code
**/
FctERR NONNULL__ MCP4725_Set_Val(MCP4725_t * const pCpnt, const uint16_t val);

/*!\brief Write DAC value (in Volts) to MCP4725 peripheral
** \param[in] pCpnt - Pointer to MCP4725 component
** \param[in] volts - Volts DAC value
** \return FctERR - error code
**/
FctERR NONNULL__ MCP4725_Set_Volts(MCP4725_t * const pCpnt, const float volts);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* __MCP4725_PROC_H__ */
/****************************************************************/
