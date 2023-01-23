/*!\file MCP9808.h
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief MCP9808 Driver
** \details MCP9808: +/-0.5C Maximum Accuracy Digital Temperature Sensor
**/
/****************************************************************/
#ifndef __MCP9808_H__
	#define __MCP9808_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/

#ifndef I2C_MCP9808_NB
//! \note Define I2C_MCP9808_NB to enable multiple peripherals of this type
#define I2C_MCP9808_NB	1	//!< Number of MCP9808 peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define MCP9808_ADDR		0x18			//!< MCP9808 address

#ifndef MCP9808_BASE_ADDR
//! \note Define MCP9808_BASE_ADDR to change default device base address
#define MCP9808_BASE_ADDR	MCP9808_ADDR	//!< MCP9808 Base address
#endif


// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern I2C_slave_t MCP9808_hal[I2C_MCP9808_NB];	//!< MCP9808 Slave structure


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum MCP9808_reg_map
** \brief Register map enum of MCP9808
**/
typedef enum PACK__ MCP9808_reg_map {
	MCP9808__RFU = 0,				//!< RFU, Reserved for Future Use (Read-Only register)
	MCP9808__CONFIGURATION,			//!< Configuration register (CONFIG)
	MCP9808__ALERT_UPPER,			//!< Alert Temperature Upper Boundary Trip register (T UPPER)
	MCP9808__ALERT_LOWER,			//!< Alert Temperature Lower Boundary Trip register (T LOWER)
	MCP9808__CRITICAL_TEMP_TRIP,	//!< Critical Temperature Trip register (T CRIT)
	MCP9808__TEMPERATURE,			//!< Temperature register (T A)
	MCP9808__MANUFACTURER_ID,		//!< Manufacturer ID register
	MCP9808__DEVICE_ID,				//!< Device ID/Revision register
	MCP9808__RESOLUTION				//!< Resolution register
} MCP9808_reg;


/*!\enum MCP9808_res
** \brief Resolution enum of MCP9808
**/
typedef enum PACK__ MCP9808_res {
	MCP9808__RES_0_5 = 0,			//!< Resolution +0.5 Celsius degrees
	MCP9808__RES_0_25,				//!< Resolution +0.25 Celsius degrees
	MCP9808__RES_0_125,				//!< Resolution +0.125 Celsius degrees
	MCP9808__RES_0_0625				//!< Resolution +0.0625 Celsius degrees
} MCP9808_res;


/*!\enum MCP9808_hyst
** \brief Resolution enum of MCP9808
**/
typedef enum PACK__ MCP9808_hyst {
	MCP9808__HYST_0 = 0,			//!< Hysteresis +0.0 Celsius degrees
	MCP9808__HYST_1_5,				//!< Hysteresis +1.5 Celsius degrees
	MCP9808__HYST_3_0,				//!< Hysteresis +3.0 Celsius degrees
	MCP9808__HYST_6_0				//!< Hysteresis +6.0 Celsius degrees
} MCP9808_hyst;


/*!\union uMCP9808_REG__CFG
** \brief Union for CFG of MCP9808
**/
typedef union uMCP9808_REG__CFG {
	uint16_t Word;
	struct PACK__ {
		uint16_t		AlertMod	:1;		//!< Alert output mode bit (0: Comparator output, 1: Interrupt output)
		uint16_t		AlertPol	:1;		//!< Alert output polarity bit (0: active low, 1: active high)
		uint16_t		AlertSel	:1;		//!< Alert output select bit (0: alert output for TUpper TLower & TCrit, 1: TA > TCrit only)
		uint16_t		AlertCnt	:1;		//!< Alert output control bit (0: Disabled, 1: Enabled)
		uint16_t		AlertStat	:1;		//!< Alert output status bit (0: alert output not asserted by device, 1: alert output asserted as comparator/interrupt or critical temperature output)
		uint16_t		IntClr		:1;		//!< Interrupt clear bit (0: No effect, 1: Clear interrupt output), always reads 0
		uint16_t		WinLoc		:1;		//!< TUpper & TLower window lock bit (0: unlocked registers can be written, 1: locked registers cannot be written)
		uint16_t		CrtLoc		:1;		//!< TCrit lock bit (0: unlocked register can be written, 1: locked register cannot be written)
		uint16_t		SHDN		:1;		//!< Shutdown mode bit (0: continuous conversion, 1: shutdown)
		MCP9808_hyst	Hysteresis	:2;		//!< TUpper & TLower limit hysteresis bits
		uint16_t 					:5;
	} Bits;
} uMCP9808_REG__CFG;


/*!\union uMCP9808_REG__TEMP_LIM
** \brief Union for Temperature Limit registers of MCP9808
**/
typedef union uMCP9808_REG__TEMP_LIM {
	uint16_t Word;
	struct PACK__ {
		uint16_t			:2;
		uint16_t Decimal	:2;		//!< Temperature boundary limits (fractional part)
		uint16_t Integer	:8;		//!< Temperature boundary limits (integer part)
		uint16_t Sign		:1;		//!< Sign (0: Positive, 1: Negative)
		uint16_t			:3;
	} Bits;
} uMCP9808_REG__TEMP_LIM;


/*!\union uMCP9808_REG__TEMP_AMB
** \brief Union for Ambient temperature register of MCP9808
**/
typedef union uMCP9808_REG__TEMP_AMB {
	uint16_t Word;
	struct PACK__ {
		uint16_t Decimal	:4;		//!< Ambient temperature (fractional part)
		uint16_t Integer	:8;		//!< Ambient temperature (integer part)
		uint16_t Sign		:1;		//!< Sign (0: Positive, 1: Negative)
		uint16_t VsTLow		:1;		//!< 0: TA >= TLower, 1: TA < TLower
		uint16_t VsTHigh	:1;		//!< 0: TA >= TUpper, 1: TA < TUpper
		uint16_t VsTCrit	:1;		//!< 0: TA >= TCrit, 1: TA < TCrit
	} Bits;
} uMCP9808_REG__TEMP_AMB;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization of the MCP9808 peripheral
** \param[in] idx - MCP9808 index
** \param[in] hi2c - pointer to MCP9808 I2C instance
** \param[in] devAddress - MCP9808 device address
** \return FctERR - error code
**/
FctERR NONNULL__ MCP9808_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress);

/*!\brief Initialization for MCP9808 peripheral
** \warning In case multiple devices (defined by I2C_MCP9808_NB > 1), you shall use MCP9808_Init instead
** \return FctERR - error code
**/
FctERR MCP9808_Init_Single(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for MCP9808
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from (16b type pointer)
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR NONNULL__ MCP9808_Write(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for MCP9808
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to (16b type pointer)
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ MCP9808_Read(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr, const uint16_t nb);


/****************************************************************/
#include "MCP9808_proc.h"	// Include procedures
#include "MCP9808_ex.h"		// Include extensions

#endif

#ifdef __cplusplus
	}
#endif

#endif /* __MCP9808_H__ */
/****************************************************************/
