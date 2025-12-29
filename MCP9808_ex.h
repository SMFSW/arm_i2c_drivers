/*!\file MCP9808_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief MCP9808 Driver extensions
** \details MCP9808: +/-0.5C Maximum Accuracy Digital Temperature Sensor
**/
/****************************************************************/
#ifndef MCP9808_EX_H__
	#define MCP9808_EX_H__

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
/*!\union _uMCP9808_REG_MAP
** \brief Union of MCP9808 registry map
**/
typedef union _uMCP9808_REG_MAP {
	uint16_t Words[9];
	struct PACK__ {
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
** \param[in] pCpnt - Pointer to MCP9808 component
** \param[in] cfg - Configuration value
** \return FctERR - error code
**/
FctERR NONNULL__ MCP9808_Write_Config(MCP9808_t * const pCpnt, const uint16_t cfg);

/*!\brief Write MCP9808 configuration
** \param[in] pCpnt - Pointer to MCP9808 component
** \param[in] shutdown - 0 Normal operation, 1 Shutdown
** \return FctERR - error code
**/
FctERR NONNULL__ MCP9808_Shutdown(MCP9808_t * const pCpnt, const bool shutdown);

/*!\brief Set MCP9808 alert hysteresis
** \param[in] pCpnt - Pointer to MCP9808 component
** \param[in] hys - Alert hysteresis
** \return FctERR - error code
**/
FctERR NONNULL__ MCP9808_Set_AlertHysteresis(MCP9808_t * const pCpnt, const MCP9808_hyst hys);

/*!\brief Set MCP9808 alert type
** \param[in] pCpnt - Pointer to MCP9808 component
** \param[in] comparator - 0 ???, 1 Comparator
** \return FctERR - error code
**/
FctERR NONNULL__ MCP9808_Set_AlertType(MCP9808_t * const pCpnt, const bool comparator);

/*!\brief Set MCP9808 alert enable
** \param[in] pCpnt - Pointer to MCP9808 component
** \param[in] en - 0 Disabled, 1 Enabled
** \param[in] alt - 0 Low, 1 High
** \return FctERR - error code
**/
FctERR NONNULL__ MCP9808_Set_AlertOutput(MCP9808_t * const pCpnt, const bool en, const bool alt);

/*!\brief Set MCP9808 alert lock
** \param[in] pCpnt - Pointer to MCP9808 component
** \param[in] alt - Alert type
** \param[in] lock - 0 Unlocked, 1 Locked
** \return FctERR - error code
**/
FctERR NONNULL__ MCP9808_Set_AlertLock(MCP9808_t * const pCpnt, const MCP9808_alert alt, const bool lock);

/*!\brief Set MCP9808 Resolution
** \param[in] pCpnt - Pointer to MCP9808 component
** \param[in] res - Resolution for MCP9808
** \return FctERR - error code
**/
FctERR NONNULL__ MCP9808_Set_Resolution(MCP9808_t * const pCpnt, const MCP9808_res res);


/*!\brief Get MCP9808 Configuration
** \param[in] pCpnt - Pointer to MCP9808 component
** \param[in,out] cfg - pointer to Configuration value to read to
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MCP9808_Read_Config(MCP9808_t * const pCpnt, uint16_t * const cfg) {
	return MCP9808_Read(pCpnt->cfg.slave_inst, cfg, MCP9808__CONFIGURATION, 1U); }

/*!\brief Get MCP9808 Configuration
** \param[in] pCpnt - Pointer to MCP9808 component
** \param[in,out] temp - pointer to Temperature value to read to
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MCP9808_Get_Temperature_Raw(MCP9808_t * const pCpnt, uint16_t * const temp) {
	return MCP9808_Read(pCpnt->cfg.slave_inst, temp, MCP9808__TEMPERATURE, 1U); }

/*!\brief Get MCP9808 Resolution
** \param[in] pCpnt - Pointer to MCP9808 component
** \param[in,out] res - pointer to Resolution to read to
** \return FctERR - error code
**/
FctERR NONNULL__ MCP9808_Get_Resolution(MCP9808_t * const pCpnt, MCP9808_res * const res);


/*!\brief Get MCP9808 Manufacturer ID
** \param[in] pCpnt - Pointer to MCP9808 component
** \param[in,out] id - pointer to Manufacturer ID result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MCP9808_Get_ManufacturerID(MCP9808_t * const pCpnt, uint16_t * const id) {
	return MCP9808_Read(pCpnt->cfg.slave_inst, id, MCP9808__MANUFACTURER_ID, 1U); }

/*!\brief Get MCP9808 chip ID
** \param[in] pCpnt - Pointer to MCP9808 component
** \param[in,out] id - pointer to chip ID result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ MCP9808_Get_ChipID(MCP9808_t * const pCpnt, uint16_t * const id) {
	return MCP9808_Read(pCpnt->cfg.slave_inst, id, MCP9808__DEVICE_ID, 1U); }


/*******************/
/*** GPIO access ***/
/*******************/

/*!\brief Alert GPIO pin init for MCP9808
** \weak MCP9808 Alert GPIO pin init may be user implemented if needed
** \param[in] pCpnt - Pointer to MCP9808 component
** \param[in] GPIOx - Alert port
** \param[in] GPIO_Pin - Alert pin
** \param[in] GPIO_Active: Alert pin active state
**/
void NONNULL__ MCP9808_Alert_GPIO_Init(MCP9808_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active);

/*!\brief Change GPIO pin getter for MCP9808
** \weak MCP9808 Change GPIO pin getter may be user implemented if needed
** \param[in] pCpnt - Pointer to MCP9808 component
** \return Alert pin state value (0: inactive, 1: active)
**/
bool NONNULL__ MCP9808_Alert_GPIO_Get(const MCP9808_t * const pCpnt);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* MCP9808_EX_H__ */
/****************************************************************/
