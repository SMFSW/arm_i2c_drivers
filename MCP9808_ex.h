/*!\file MCP9808_ex.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief MCP9808 Driver extensions declarations
** \details MCP9808: +/-0.5C Maximum Accuracy Digital Temperature Sensor
**/
/****************************************************************/
#ifndef __MCP9808_EX_H__
	#define __MCP9808_EX_H__

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


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum MCP9808_alert
** \brief Alerts enum of MCP9808
**/
typedef enum PACK__ MCP9808_alert {
	MCP9808__ALERT_HIGH = 0,	//!< High temperature alert
	MCP9808__ALERT_LOW,			//!< Low temperature alert
	MCP9808__ALERT_CRIT			//!< Critical temperature alert
} MCP9808_alert;


/*!\union uMCP9808_REG_MAP
** \brief Union of MCP9808 registry map
**/
typedef union uMCP9808_REG_MAP {
	uint16_t Words[9];
	struct {
		uint16_t				RFU;
		uMCP9808_REG__CFG		CONFIG;
		uMCP9808_REG__TEMP_LIM	TUpper;
		uMCP9808_REG__TEMP_LIM	TLower;
		uMCP9808_REG__TEMP_LIM	TCrit;
		uMCP9808_REG__TEMP_AMB	TA;
		uint16_t				ManufacturerID;
		uint16_t				DeviceID;
		uint16_t				Resolution;		// is a 8b register in reality
	} Reg;
} uMCP9808_REG_MAP;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/
FctERR MCP9808_Write_Config(uint16_t cfg);
//	return  MCP9808_Write(&cfg, MCP9808__CONFIGURATION, 1); }

FctERR MCP9808_Shutdown(bool shutdown);

FctERR MCP9808_Set_AlertHysteresis(MCP9808_hyst hys);

FctERR MCP9808_Set_AlertType(bool comparator);

FctERR MCP9808_Set_AlertOutput(bool en, bool alt);

FctERR MCP9808_Set_AlertLock(MCP9808_alert alt, bool lock);

FctERR MCP9808_Set_Resolution(MCP9808_res res);


__INLINE FctERR INLINE__ MCP9808_Read_Config(uint16_t * cfg) {
	return MCP9808_Read(cfg, MCP9808__CONFIGURATION, 1); }

__INLINE FctERR INLINE__ MCP9808_Get_Temperature_Raw(uint16_t * temp) {
	return MCP9808_Read(temp, MCP9808__TEMPERATURE, 1); }

FctERR MCP9808_Get_Resolution(MCP9808_res * res);


__INLINE FctERR INLINE__ MCP9808_Get_ManufacturerID(uint16_t * id) {
	return MCP9808_Read(id, MCP9808__MANUFACTURER_ID, 1); }

__INLINE FctERR INLINE__ MCP9808_Get_ChipID(uint16_t * id) {
	return MCP9808_Read(id, MCP9808__DEVICE_ID, 1); }


/****************************************************************/
#endif
#endif /* __MCP9808_EX_H__ */
/****************************************************************/
