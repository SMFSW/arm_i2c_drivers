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
#include CMSIS_INC
#include CMSIS_CFG

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/
// TODO: doxygen


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define MCP9808_MANUFACTURER_ID		0x0054		//!< MCP9808 Manufacturer ID to check against
#define MCP9808_CHIP_ID				0x0400		//!< MCP9808 Chip ID to check against



// *****************************************************************************
// Section: Types
// *****************************************************************************
typedef struct MCP9808_proc {
	float		Temperature;
	bool		TUpper;
	bool		TLower;
	bool		TCrit;
	bool		NewData;
	uint32_t	hLast;
	struct {
	MCP9808_res	Resolution;
	float		HighAlert;
	float		LowAlert;
	float		CriticalAlert;
	uint16_t	Manufacturer_Id;
	uint16_t	Device_Id;
	} cfg;
} MCP9808_proc;


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

FctERR MCP9808_Set_AlertTemp(float temp, MCP9808_alert alt);


FctERR MCP9808_Get_AlertTemp(float * temp, MCP9808_alert alt);

FctERR MCP9808_Get_Temperature(float * temp);


FctERR MCP9808_handler(void);


/****************************************************************/
#endif
#endif /* __MCP9808_PROC_H__ */
/****************************************************************/
