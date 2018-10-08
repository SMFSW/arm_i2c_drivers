/*!\file MCP9808_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief MCP9808 Driver procedures
** \details MCP9808: +/-0.5C Maximum Accuracy Digital Temperature Sensor
**/
/****************************************************************/
#ifndef __MCP9808_PROC_H__
	#define __MCP9808_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "MCP9808.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define MCP9808_MANUFACTURER_ID		0x0054		//!< MCP9808 Manufacturer ID to check against
#define MCP9808_CHIP_ID				0x0400		//!< MCP9808 Chip ID to check against



// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct MCP9808_t
** \brief MCP9808 user interface struct
**/
typedef struct MCP9808_t {
	float			Temperature;		//!< Current temperature
	bool			TUpper;				//!< Upper alert reached
	bool			TLower;				//!< Lower alert reached
	bool			TCrit;				//!< Critical alert reached
	bool			NewData;			//!< New data available
	uint32_t		hLast;				//!< Tick from last data
	struct {
	I2C_slave_t *	slave_inst;			//!< Slave structure
	MCP9808_res		Resolution;			//!< Resolution config
	float			HighAlert;			//!< High alert config
	float			LowAlert;			//!< Low alert config
	float			CriticalAlert;		//!< Critical alert config
	uint16_t		Manufacturer_Id;	//!< Manufacturer ID
	uint16_t		Device_Id;			//!< Device ID
	} cfg;
} MCP9808_t;

extern MCP9808_t	MCP9808;			//!< MCP9808 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence of the MCP9808 peripheral
** \weak MCP9808 handler may be user implemented to suit custom needs
** \return FctERR - error code
**/
FctERR MCP9808_Init_Sequence(void);

/*!\brief Set the high/low/crit Alert temperature
** \param[in,out] temp - temperature (in Celsius degrees)
** \param[in] alt - Alert type
** \return FctERR - error code
**/
FctERR MCP9808_Set_AlertTemp(const float temp, const MCP9808_alert alt);


/*!\brief Get the high/low/crit Alert temperature
** \param[in,out] temp - pointer to temperature to read to (in Celsius degrees)
** \param[in] alt - Alert type
** \return FctERR - error code
**/
FctERR MCP9808_Get_AlertTemp(float * temp, MCP9808_alert alt);

/*!\brief Get the temperature
** \param[in,out] temp - pointer to temperature to read to (in Celsius degrees)
** \return FctERR - error code
**/
FctERR MCP9808_Get_Temperature(float * temp);


/*!\brief Handler for MCP9808 peripheral
** \weak MCP9808 handler may be user implemented to suit custom needs
** \note Should be called periodically to handle BMP180 tasks
** \return FctERR - error code
**/
FctERR MCP9808_handler(void);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __MCP9808_PROC_H__ */
/****************************************************************/
