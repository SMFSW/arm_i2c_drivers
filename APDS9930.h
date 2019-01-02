/*!\file APDS9930.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief APDS9930 Driver
** \details APDS9930: Digital Proximity and Ambient Light Sensor
**/
/****************************************************************/
#ifndef __APDS9930_H__
	#define __APDS9930_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "globals.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/

#ifndef I2C_APDS9930_NB
//! \note Define I2C_APDS9930_NB in globals.h or at project to enable multiple peripherals of this type
#define I2C_APDS9930_NB	1	//!< Number of APDS9930 peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define APDS9930_ADDR		0x39				//!< APDS9930 address

#define APDS9930_BASE_ADDR	APDS9930_ADDR		//!< APDS9930 Base address


// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern I2C_slave_t APDS9930_hal[I2C_APDS9930_NB];	//!< APDS9930 Slave structure


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum APDS9930_reg_map
** \brief Register map enum of APDS9930
**/
typedef enum PACK__ APDS9930_reg_map {
	APDS9930__ENABLE = 0,	//!< Enables states and interrupts
	APDS9930__ATIME,		//!< ALS ADC time
	APDS9930__PTIME,		//!< Proximity ADC time
	APDS9930__WTIME,		//!< Wait time
	APDS9930__AILTL,		//!< ALS interrupt low threshold low byte
	APDS9930__AILTH,		//!< ALS interrupt low threshold high byte
	APDS9930__AIHTL,		//!< ALS interrupt high threshold low byte
	APDS9930__AIHTH,		//!< ALS interrupt high threshold high byte
	APDS9930__PILTL,		//!< Proximity interrupt low threshold low byte
	APDS9930__PILTH,		//!< Proximity interrupt low threshold high byte
	APDS9930__PIHTL,		//!< Proximity interrupt high threshold low byte
	APDS9930__PIHTH,		//!< Proximity interrupt high threshold high byte
	APDS9930__PERS,			//!< Interrupt persistence filter
	APDS9930__CONFIG,		//!< Configuration
	APDS9930__PPULSE,		//!< Proximity pulse count
	APDS9930__CONTROL,		//!< Gain control register
	APDS9930__ID = 0x12,	//!< Device ID
	APDS9930__STATUS,		//!< Device status
	APDS9930__CH0DATAL,		//!< CH0 ADC low data register
	APDS9930__CH0DATAH,		//!< CH0 ADC high data register
	APDS9930__CH1DATAL,		//!< CH1 ADC low data register
	APDS9930__CH1DATAH,		//!< CH1 ADC high data register
	APDS9930__PDATAL,		//!< Proximity ADC low data register
	APDS9930__PDATAH,		//!< Proximity ADC high data register
	APDS9930__POFFSET,		//!< Proximity offset register
} APDS9930_reg;

/*!\enum APDS9930_transaction
** \brief Transaction types of APDS9930
**/
typedef enum PACK__ APDS9930_transaction {
	APDS9930__TRANS_REPEATED_BYTE = 0,	//!< Repeated Byte protocol transaction
	APDS9930__TRANS_AUTO_INCREMENT = 1,	//!< Auto-Increment protocol transaction
	APDS9930__TRANS_SPECIAL_FUNC = 3,	//!< Special Function
} APDS9930_transaction;

/*!\enum APDS9930_spec_func
** \brief Special functions of APDS9930
**/
typedef enum PACK__ APDS9930_spec_func {
	APDS9930__SF_CLR_PROX_IT = 5,		//!< Proximity interrupt clear
	APDS9930__SF_CLR_ALS_IT = 6,		//!< ALS interrupt clear
	APDS9930__SF_CLR_PROX_ALS_IT = 7	//!< Proximity and ALS interrupt clear
} APDS9930_spec_func;

/*!\enum APDS9930_als_gain
** \brief Gain values of APDS9930 ALS
**/
typedef enum PACK__ APDS9930_als_gain {
	APDS9930__ALS_1X_GAIN = 0,	//!< ALS 1X Gain
	APDS9930__ALS_8X_GAIN,		//!< ALS 8X Gain
	APDS9930__ALS_16X_GAIN,		//!< ALS 16X Gain
	APDS9930__ALS_120X_GAIN		//!< ALS 120X Gain
} APDS9930_als_gain;

/*!\enum APDS9930_prox_gain
** \brief Gain values of APDS9930 proximity
**/
typedef enum PACK__ APDS9930_prox_gain {
	APDS9930__PROX_1X_GAIN = 0,	//!< Proximity 1X Gain
	APDS9930__PROX_2X_GAIN,		//!< Proximity 2X Gain
	APDS9930__PROX_4X_GAIN,		//!< Proximity 4X Gain
	APDS9930__PROX_8X_GAIN		//!< Proximity 8X Gain
} APDS9930_prox_gain;

/*!\enum APDS9930_prox_diode
** \brief Proximity Diode Select values
**/
typedef enum PACK__ APDS9930_prox_diode {
	APDS9930__PDIODE_RESERVED1 = 0,	//!< Reserved
	APDS9930__PDIODE_RESERVED2,		//!< Reserved
	APDS9930__PDIODE_CH1,			//!< Proximity uses the Ch1 diode
	APDS9930__PDIODE_RESERVED3		//!< Reserved
} APDS9930_prox_diode;

/*!\enum APDS9930_drive_strength
** \brief Proximity LED Drive strength values
**/
typedef enum PACK__ APDS9930_drive_strength {
	APDS9930__STRENGTH_MAX = 0,	//!< PDL=0 : 100mA ; PDL=1 : 11.1mA
	APDS9930__STRENGTH_HIGH,	//!< PDL=0 : 50mA ; PDL=1 : 5.6mA
	APDS9930__STRENGTH_MEDIUM,	//!< PDL=0 : 25mA ; PDL=1 : 2.8mA
	APDS9930__STRENGTH_LOW,		//!< PDL=0 : 12.5mA ; PDL=1 : 1.4mA
} APDS9930_drive_strength;

/*!\enum APDS9930_als_it_persist
** \brief Persistence control of APDS9930 ALS
**/
typedef enum PACK__ APDS9930_als_it_persist {
	APDS9930__PERSIST_EVERY_ALS_CYCLE = 0,	//!< Every ALS cycle generates interrupt
	APDS9930__PERSIST_1_ALS_PERIODS,		//!< 1 consecutive Ch0 channel values out of range
	APDS9930__PERSIST_2_ALS_PERIODS,		//!< 2 consecutive Ch0 channel values out of range
	APDS9930__PERSIST_3_ALS_PERIODS,		//!< 3 consecutive Ch0 channel values out of range
	APDS9930__PERSIST_5_ALS_PERIODS,		//!< 5 consecutive Ch0 channel values out of range
	APDS9930__PERSIST_10_ALS_PERIODS,		//!< 10 consecutive Ch0 channel values out of range
	APDS9930__PERSIST_15_ALS_PERIODS,		//!< 15 consecutive Ch0 channel values out of range
	APDS9930__PERSIST_20_ALS_PERIODS,		//!< 20 consecutive Ch0 channel values out of range
	APDS9930__PERSIST_25_ALS_PERIODS,		//!< 25 consecutive Ch0 channel values out of range
	APDS9930__PERSIST_30_ALS_PERIODS,		//!< 30 consecutive Ch0 channel values out of range
	APDS9930__PERSIST_35_ALS_PERIODS,		//!< 35 consecutive Ch0 channel values out of range
	APDS9930__PERSIST_40_ALS_PERIODS,		//!< 40 consecutive Ch0 channel values out of range
	APDS9930__PERSIST_45_ALS_PERIODS,		//!< 45 consecutive Ch0 channel values out of range
	APDS9930__PERSIST_50_ALS_PERIODS,		//!< 50 consecutive Ch0 channel values out of range
	APDS9930__PERSIST_55_ALS_PERIODS,		//!< 55 consecutive Ch0 channel values out of range
	APDS9930__PERSIST_60_ALS_PERIODS,		//!< 60 consecutive Ch0 channel values out of range
} APDS9930_als_it_persist;

/*!\enum APDS9930_prox_it_persist
** \brief Persistence control of APDS9930 Proximity
**/
typedef enum PACK__ APDS9930_prox_it_persist {
	APDS9930__PERSIST_EVERY_PROX_CYCLE = 0,	//!< Every proximity cycle generates an interrupt
	APDS9930__PERSIST_1_PROX_PERIODS,		//!< 1 consecutive proximity values out of range
	APDS9930__PERSIST_2_PROX_PERIODS,		//!< 2 consecutive proximity values out of range
	APDS9930__PERSIST_3_PROX_PERIODS,		//!< 3 consecutive proximity values out of range
	APDS9930__PERSIST_4_PROX_PERIODS,		//!< 4 consecutive proximity values out of range
	APDS9930__PERSIST_5_PROX_PERIODS,		//!< 5 consecutive proximity values out of range
	APDS9930__PERSIST_6_PROX_PERIODS,		//!< 6 consecutive proximity values out of range
	APDS9930__PERSIST_7_PROX_PERIODS,		//!< 7 consecutive proximity values out of range
	APDS9930__PERSIST_8_PROX_PERIODS,		//!< 8 consecutive proximity values out of range
	APDS9930__PERSIST_9_PROX_PERIODS,		//!< 9 consecutive proximity values out of range
	APDS9930__PERSIST_10_PROX_PERIODS,		//!< 10 consecutive proximity values out of range
	APDS9930__PERSIST_11_PROX_PERIODS,		//!< 11 consecutive proximity values out of range
	APDS9930__PERSIST_12_PROX_PERIODS,		//!< 12 consecutive proximity values out of range
	APDS9930__PERSIST_13_PROX_PERIODS,		//!< 13 consecutive proximity values out of range
	APDS9930__PERSIST_14_PROX_PERIODS,		//!< 14 consecutive proximity values out of range
	APDS9930__PERSIST_15_PROX_PERIODS,		//!< 15 consecutive proximity values out of range
} APDS9930_prox_it_persist;


/*!\union uAPDS9930_CMD
** \brief Union for COMMAND register of APDS9930
**/
typedef union uAPDS9930_CMD {
	uint8_t Byte;
	struct {
		APDS9930_spec_func		ADDR		:5;		//!< Address field/special function field
		APDS9930_transaction	TRANSACTION	:2;		//!< Select type of transaction to follow in subsequent data transfers
		uint8_t					CMD			:1;		//!< Select command register. Must write as 1 when addressing Command register
	} Bits;
} uAPDS9930_CMD;


/*!\union uAPDS9930_REG__ENABLE
** \brief Union for ENABLE register of APDS9930
**/
typedef union uAPDS9930_REG__ENABLE {
	uint8_t Byte;
	struct {
		uint8_t PON		:1;	//!< Power ON. This field activates the internal oscillator to permit the timers and ADC channels to operate
		uint8_t AEN		:1;	//!< ALS Enable. This field activates ALS function. Writing a one activates the ALS. Writing a zero disables the ALS
		uint8_t PEN		:1;	//!< Proximity Enable. This bit activates the proximity function. Writing a 1 enables proximity. Writing a 0 disables proximity.
		uint8_t WEN		:1;	//!< Wait Enable. This bit activates the wait feature. Writing a 1 activates the wait timer. Writing a 0 disables the wait timer.
		uint8_t AIEN	:1;	//!< ALS Interrupt Mask. When asserted, permits ALS interrupt to be generated.
		uint8_t PIEN	:1;	//!< Proximity Interrupt Mask. When asserted, permits proximity interrupts to be generated.
		uint8_t SAI		:1;	//!< Sleep after interrupt. When asserted, the device will power down at the end of an ALS cycle if an interrupt has been generated
		uint8_t			:1;	//!< Reserved
	} Bits;
} uAPDS9930_REG__ENABLE;


/*!\union uAPDS9930_REG__CONFIG
** \brief Union for CONFIG register of APDS9930
**/
typedef union uAPDS9930_REG__CONFIG {
	uint8_t Byte;
	struct {
		uint8_t	PDL		:1;	//!< Proximity drive level. When asserted, the proximity LDR drive current is reduced by 9
		uint8_t	WLONG	:1;	//!< Wait Long. When asserted, the wait cycles are increased by a factor 12x from that programmed in the WTIME register.
		uint8_t	AGL		:1;	//!< ALS gain level. When asserted, the 1× and 8× ALS gain (AGAIN) modes are scaled by 0.16. Otherwise, AGAIN is scaled by 1. Do not use with AGAIN greater than 8×.
		uint8_t			:5;
	} Bits;
} uAPDS9930_REG__CONFIG;


/*!\union uAPDS9930_REG__CONTROL
** \brief Union for CONTROL register of APDS9930
**/
typedef union uAPDS9930_REG__CONTROL {
	uint8_t Byte;
	struct {
		APDS9930_als_gain		AGAIN	:2;	//!< ALS Gain Control.
		APDS9930_prox_gain		PGAIN	:2;	//!< Proximity Gain Control.
		APDS9930_prox_diode		PDIODE	:2;	//!< Proximity Diode Select.
		APDS9930_drive_strength	PDRIVE	:2;	//!< LED Drive Strength.
	} Bits;
} uAPDS9930_REG__CONTROL;


/*!\union uAPDS9930_REG__PERSIST
** \brief Union for PERSIST register of APDS9930
**/
typedef union uAPDS9930_REG__PERSIST {
	uint8_t Byte;
	struct {
		APDS9930_als_it_persist		APERS	:4;	//!< Interrupt persistence. Controls rate of interrupt to the host processor.
		APDS9930_prox_it_persist	PPERS	:4;	//!< Proximity interrupt persistence. Controls rate of proximity interrupt to the host processor.
	} Bits;
} uAPDS9930_REG__PERSIST;


/*!\union uAPDS9930_REG__STATUS
** \brief Union for STATUS register of APDS9930
**/
typedef union uAPDS9930_REG__STATUS {
	uint8_t Byte;
	struct {
		uint8_t AVALID	:1;		//!< ALS Valid. Indicates that the ALS Ch0/Ch1 channels have completed an integration cycle.
		uint8_t PVALID	:1;		//!< PS Valid. Indicates that the PS has completed an integration cycle.
		uint8_t			:2;
		uint8_t AINT	:1;		//!< ALS Interrupt. Indicates that the device is asserting an ALS interrupt.
		uint8_t PINT	:1;		//!< Proximity Interrupt. Indicates that the device is asserting a proximity interrupt.
		uint8_t PSAT	:1;		//!< Proximity Saturation. Indicates that the proximity measurement is saturated
		uint8_t 		:1;
	} Bits;
} uAPDS9930_REG__STATUS;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for APDS9930 peripheral
** \param[in] idx - APDS9930 index
** \param[in] hi2c - pointer to APDS9930 I2C instance
** \param[in] devAddress - APDS9930 device address
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Init(const uint8_t idx, const I2C_HandleTypeDef * hi2c, const uint16_t devAddress);

/*!\brief Initialization for APDS9930 peripheral
** \warning In case multiple devices (defined by I2C_APDS9930_NB > 1), you shall use APDS9930_Init instead
** \return FctERR - error code
**/
FctERR APDS9930_Init_Single(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for APDS9930
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Write(I2C_slave_t * pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for APDS9930
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Read(I2C_slave_t * pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Word Write (low endian) function for APDS9930
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Write_Word(I2C_slave_t * pSlave, const uint16_t * data, const uint16_t addr);


/*!\brief I2C Word Read (low endian) function for APDS9930
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Read_Word(I2C_slave_t * pSlave, uint16_t * data, const uint16_t addr);


/*!\brief I2C Special function Write function for APDS9930
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] func - special function to execute
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Write_Special(I2C_slave_t * pSlave, const APDS9930_spec_func func);


/****************************************************************/
#include "APDS9930_proc.h"	// Include procedures
#include "APDS9930_ex.h"	// Include extensions

#ifdef __cplusplus
	}
#endif

#endif
#endif /* __APDS9930_H__ */
/****************************************************************/
