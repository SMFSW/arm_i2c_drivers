/*!\file TMP1075.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief TMP1075 Driver
** \details TMP1075: Temperature Sensor With I2C and SMBus Interface in Industry Standard LM75 Form Factor and Pinout
**/
/****************************************************************/
#ifndef TMP1075_H__
	#define TMP1075_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


#ifndef I2C_TMP1075_NB
//! \note Define I2C_TMP1075_NB to enable multiple peripherals of this type
#define I2C_TMP1075_NB	1	//!< Number of TMP1075 peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define TMP1075_ADDR		0x48				//!< TMP1075 address

#ifndef TMP1075_BASE_ADDR
//! \note Define TMP1075_BASE_ADDR to change default device base address
#define TMP1075_BASE_ADDR	TMP1075_ADDR		//!< TMP1075 Base address
#endif


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum TMP1075_reg_map
** \brief Register map enum of TMP1075
**/
typedef enum PACK__ TMP1075_reg_map {
	TMP1075__TEMP = 0,		//!< default value for reference
	TMP1075__CFGR,			//!< default value for reference
	TMP1075__LLIM,			//!< default value for reference
	TMP1075__HLIM,			//!< default value for reference
	TMP1075__DIEID = 0x0F,	//!< default value for reference
} TMP1075_reg;


/*!\enum TMP1075_conv_rate
** \brief Conversion rate enum values for TMP1075
**/
typedef enum PACK__ TMP1075_conv_rate {
	TMP1075__CR_27_5MS = 0,	//!< 27.5ms conversion rate
	TMP1075__CR_55MS,		//!< 55ms conversion rate
	TMP1075__CR_110MS,		//!< 110ms conversion rate
	TMP1075__CR_220MS		//!< 220ms conversion rate (250ms TMP1075N)
} TMP1075_conv_rate;


/*!\enum TMP1075_nb_faults
** \brief Number of faults to trigger alert enum values for TMP1075
**/
typedef enum PACK__ TMP1075_nb_faults {
	TMP1075__1_FAULT = 0,	//!< 1 fault
	TMP1075__2_FAULTS,		//!< 2 faults
	TMP1075__3_4_FAULTS,	//!< 3 faults (4 faults TMP1075N)
	TMP1075__4_6_FAULTS		//!< 4 faults (6 faults TMP1075N)
} TMP1075_nb_faults;


/*!\enum TMP1075_alert_pol
** \brief Alert pin polarity enum values for TMP1075
**/
typedef enum PACK__ TMP1075_alert_pol {
	TMP1075__POL_LOW = 0,	//!< Active low ALERT pin
	TMP1075__POL_HIGH		//!< Active high ALERT pin
} TMP1075_alert_pol;


/*!\enum TMP1075_alert_mode
** \brief Alert pin function enum values for TMP1075
**/
typedef enum PACK__ TMP1075_alert_mode {
	TMP1075__MODE_COMP = 0,	//!< ALERT pin functions in comparator mode
	TMP1075__MODE_INT		//!< ALERT pin functions in interrupt mode
} TMP1075_alert_mode;


/*!\enum TMP1075_shutdown_mode
** \brief Shutdown mode enum values for TMP1075
**/
typedef enum PACK__ TMP1075_shutdown_mode {
	TMP1075__MODE_CONTINUOUS = 0,	//!< Device is in continuous conversion
	TMP1075__MODE_SINGLE			//!< Device is in shutdown mode
} TMP1075_shutdown_mode;



/*!\union uTMP1075_REG__CFGR
** \brief Union for CFGR register of TMP1075 (as BYTE)
**/
typedef union uTMP1075_REG__CFGR {
	uint8_t Byte;
	struct PACK__ {
		TMP1075_shutdown_mode	SD	:1;	//!< Sets the device in shutdown mode to conserve power
		TMP1075_alert_mode		TM	:1;	//!< Selects the function of the ALERT pin
		TMP1075_alert_pol		POL	:1;	//!< Polarity of the output pin
		TMP1075_nb_faults		F	:2;	//!< Consecutive fault measurements to trigger the alert function
		TMP1075_conv_rate		R	:2;	//!< Conversion rate setting when device is in continuous conversion mode
		uint8_t					OS	:1;	//!< One shot conversion mode (Writing 1 starts a single temperature conversion. Read returns 0)
	} Bits;
} uTMP1075_REG__CFGR;


/*!\union uTMP1075_REG__CFGR_WORD
** \brief Union for CFGR register of TMP1075 (as WORD)
**/
typedef union uTMP1075_REG__CFGR_WORD {
	uint16_t Word;
	struct PACK__ {
		uint16_t				FFh	:8;	//!< Not used (Reserved: on TMP1075 package, reads 0xFF)
		TMP1075_shutdown_mode	SD	:1;	//!< Sets the device in shutdown mode to conserve power
		TMP1075_alert_mode		TM	:1;	//!< Selects the function of the ALERT pin
		TMP1075_alert_pol		POL	:1;	//!< Polarity of the output pin
		TMP1075_nb_faults		F	:2;	//!< Consecutive fault measurements to trigger the alert function
		TMP1075_conv_rate		R	:2;	//!< Conversion rate setting when device is in continuous conversion mode
		uint16_t				OS	:1;	//!< One shot conversion mode (Writing 1 starts a single temperature conversion. Read returns 0)
	} Bits;
} uTMP1075_REG__CFGR_WORD;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for TMP1075 peripheral
** \param[in] idx - TMP1075 index
** \param[in] hi2c - pointer to TMP1075 I2C instance
** \param[in] devAddress - TMP1075 device address
** \return FctERR - error code
**/
FctERR NONNULL__ TMP1075_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress);

/*!\brief Initialization for TMP1075 peripheral
** \warning In case multiple devices (defined by I2C_TMP1075_NB > 1), you shall use TMP1075_Init instead
** \return FctERR - error code
**/
FctERR TMP1075_Init_Single(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for TMP1075
** \param[in] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \return FctERR - error code
**/
FctERR NONNULL__ TMP1075_Write_Byte(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr);


/*!\brief I2C Read function for TMP1075
** \param[in] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \return FctERR - error code
**/
FctERR NONNULL__ TMP1075_Read_Byte(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr);


/*!\brief I2C Word Write (little endian) function for TMP1075
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \return FctERR - error code
**/
FctERR NONNULL__ TMP1075_Write_Word(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr);


/*!\brief I2C Word Read (little endian) function for TMP1075
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \return FctERR - error code
**/
FctERR NONNULL__ TMP1075_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr);


/****************************************************************/
// cppcheck-suppress-begin misra-c2012-20.1 ; include directives after other declarations
#include "TMP1075_proc.h"	// Include procedures
#include "TMP1075_ex.h"		// Include extensions
// cppcheck-suppress-end misra-c2012-20.1

#endif

#ifdef __cplusplus
	}
#endif

#endif /* TMP1075_H__ */
/****************************************************************/
