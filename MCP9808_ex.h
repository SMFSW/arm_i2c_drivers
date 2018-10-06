/*!\file MCP9808_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief MCP9808 Driver extensions
** \details MCP9808: +/-0.5C Maximum Accuracy Digital Temperature Sensor
**/
/****************************************************************/
#ifndef __MCP9808_EX_H__
	#define __MCP9808_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "MCP9808.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


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
		uint16_t				RFU;				//!< RFU, Reserved for Future Use (Read-Only register)
		uMCP9808_REG__CFG		CONFIG;				//!< Configuration register (CONFIG)
		uMCP9808_REG__TEMP_LIM	TUpper;				//!< Alert Temperature Upper Boundary Trip register (T_upper)
		uMCP9808_REG__TEMP_LIM	TLower;				//!< Alert Temperature Lower Boundary Trip register (T_lower)
		uMCP9808_REG__TEMP_LIM	TCrit;				//!< Critical Temperature Trip register (T_crit)
		uMCP9808_REG__TEMP_AMB	TA;					//!< Temperature register (T_A)
		uint16_t				ManufacturerID;		//!< Manufacturer ID register
		uint16_t				DeviceID;			//!< Device ID/Revision register
		uint16_t				Resolution;			//!< Resolution register (is a 8b register in reality)
	} Reg;
} uMCP9808_REG_MAP;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/
/*!\brief Write MCP9808 configuration
** \param[in] cfg - Configuration value
** \return FctERR - error code
**/
FctERR MCP9808_Write_Config(const uint16_t cfg);

/*!\brief Write MCP9808 configuration
** \param[in] shutdown - 0 Normal operation, 1 Shutdown
** \return FctERR - error code
**/
FctERR MCP9808_Shutdown(const bool shutdown);

/*!\brief Set MCP9808 alert hysteresis
** \param[in] hys - Alert hysteresis
** \return FctERR - error code
**/
FctERR MCP9808_Set_AlertHysteresis(const MCP9808_hyst hys);

/*!\brief Set MCP9808 alert type
** \param[in] comparator - 0 ???, 1 Comparator
** \return FctERR - error code
**/
FctERR MCP9808_Set_AlertType(const bool comparator);

/*!\brief Set MCP9808 alert enable
** \param[in] en - 0 Disabled, 1 Enabled
** \param[in] alt - 0 Low, 1 High
** \return FctERR - error code
**/
FctERR MCP9808_Set_AlertOutput(const bool en, const bool alt);

/*!\brief Set MCP9808 alert lock
** \param[in] alt - Alert type
** \param[in] lock - 0 Unlocked, 1 Locked
** \return FctERR - error code
**/
FctERR MCP9808_Set_AlertLock(const MCP9808_alert alt, const bool lock);

/*!\brief Set MCP9808 Resolution
** \param[in] res - Resolution for MCP9808
** \return FctERR - error code
**/
FctERR MCP9808_Set_Resolution(const MCP9808_res res);


/*!\brief Get MCP9808 Configuration
** \param[in,out] cfg - pointer to Configuration value to read to
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ MCP9808_Read_Config(uint16_t * cfg) {
	return MCP9808_Read(cfg, MCP9808__CONFIGURATION, 1); }

/*!\brief Get MCP9808 Configuration
** \param[in,out] temp - pointer to Temperature value to read to
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ MCP9808_Get_Temperature_Raw(uint16_t * temp) {
	return MCP9808_Read(temp, MCP9808__TEMPERATURE, 1); }

/*!\brief Get MCP9808 Resolution
** \param[in,out] res - pointer to Resolution to read to
** \return FctERR - error code
**/
FctERR MCP9808_Get_Resolution(MCP9808_res * res);


/*!\brief Get MCP9808 Manufacturer ID
** \param[in,out] id - pointer to Manufacturer ID result
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ MCP9808_Get_ManufacturerID(uint16_t * id) {
	return MCP9808_Read(id, MCP9808__MANUFACTURER_ID, 1); }

/*!\brief Get MCP9808 chip ID
** \param[in,out] id - pointer to chip ID result
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ MCP9808_Get_ChipID(uint16_t * id) {
	return MCP9808_Read(id, MCP9808__DEVICE_ID, 1); }


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __MCP9808_EX_H__ */
/****************************************************************/
