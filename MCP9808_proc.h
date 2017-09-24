/*!\file MCP9808_proc.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief MCP9808 Driver procedures declarations
** \details MCP9808: +/-0.5C Maximum Accuracy Digital Temperature Sensor
**/
/****************************************************************/
#ifndef __MCP9808_PROC_H__
	#define __MCP9808_PROC_H__

#include "MCP9808.h"

#include "sarmfsw.h"

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
/*!\struct MCP9808_proc
** \brief MCP9808 user interface struct
**/
typedef struct MCP9808_proc {
	float		Temperature;		//!< Current temperature
	bool		TUpper;				//!< Upper alert reached
	bool		TLower;				//!< Lower alert reached
	bool		TCrit;				//!< Critical alert reached
	bool		NewData;			//!< New data available
	uint32_t	hLast;				//!< Tick from last data
	struct {
	MCP9808_res	Resolution;			//!< Resolution config
	float		HighAlert;			//!< High alert config
	float		LowAlert;			//!< Low alert config
	float		CriticalAlert;		//!< Critical alert config
	uint16_t	Manufacturer_Id;	//!< MCP9808 Manufacturer ID
	uint16_t	Device_Id;			//!< MCP9808 Device ID
	} cfg;
} MCP9808_proc;

extern MCP9808_proc	MCP9808;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence of the MCP9808 peripheral
** \return FctERR - error code
**/
FctERR MCP9808_Init_Sequence(void);

/*!\brief Set the high/low/crit Alert temperature
** \param[in,out] temp - temperature (in Celsius degrees)
** \param[in] alt - Alert type
** \return FctERR - error code
**/
FctERR MCP9808_Set_AlertTemp(float temp, MCP9808_alert alt);


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
** \note Should be called periodically to handle BMP180 tasks
** \return FctERR - error code
**/
FctERR MCP9808_handler(void);


/****************************************************************/
#endif
#endif /* __MCP9808_PROC_H__ */
/****************************************************************/
