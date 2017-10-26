/*!\file TCS3400.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief TCS3400 Driver declarations
** \details TCS3400: Color Light-to-Digital Converter
**/
/****************************************************************/
#ifndef __TCS3400_H__
	#define __TCS3400_H__

#include "sarmfsw.h"
#include "I2C_component.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define TCS34001_ADDR			0x39			//!< TCS34001 Base address
#define TCS34003_ADDR			0x39			//!< TCS34003 Base address
#define TCS34005_ADDR			0x29			//!< TCS34005 Base address
#define TCS34007_ADDR			0x29			//!< TCS34007 Base address

#define TCS3400_BASE_ADDR		TCS34005_ADDR	//!< TCS3400 Base address


// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern I2C_slave TCS3400_hal;	//!< TCS3400 Slave instance


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum TCS3400_reg_map
** \brief Register map enum of TCS3400
**/
typedef enum PACK__ TCS3400_reg_map {
	TCS3400__ENABLE = 0x80,		//!< Enables states and interrupts
	TCS3400__ATIME,				//!< RGBC time
	TCS3400__WTIME = 0x83,		//!< Wait time
	TCS3400__AILTL,				//!< Clear interrupt low threshold low byte
	TCS3400__AILTH,				//!< Clear interrupt low threshold high byte
	TCS3400__AIHTL,				//!< Clear interrupt high threshold low byte
	TCS3400__AIHTH,				//!< Clear ALS interrupt high threshold high byte
	TCS3400__PERSIST = 0x8C,	//!< Interrupt persistence filter
	TCS3400__CONFIG,			//!< Configuration
	TCS3400__CONTROL = 0x8F,	//!< Control
	TCS3400__AUX,				//!< Auxiliary control
	TCS3400__REVID,				//!< Revision ID
	TCS3400__ID,				//!< Device ID
	TCS3400__STATUS,			//!< Device status
	TCS3400__CDATAL,			//!< Clear / IR channel low data byte
	TCS3400__CDATAH,			//!< Clear / IR channel high data byte
	TCS3400__RDATAL,			//!< Red low data byte
	TCS3400__RDATAH,			//!< Red high data byte
	TCS3400__GDATAL,			//!< Green low data byte
	TCS3400__GDATAH,			//!< Green high data byte
	TCS3400__BDATAL,			//!< Blue low data byte
	TCS3400__BDATAH,			//!< Blue high data byte
	TCS3400__IR = 0xC0,			//!< Access IR channel
	TCS3400__IFORCE = 0xE4,		//!< Force interrupt
	TCS3400__CICLEAR = 0xE6,	//!< Clear channel interrupt clear
	TCS3400__AICLEAR = 0xE7		//!< Clear all interrupts
} TCS3400_reg;


/*!\enum TCS3400_gain
** \brief Gain values of TCS3400
**/
typedef enum PACK__ TCS3400_gain {
	TCS3400__LOW_GAIN = 0,	//!< Low gain mode (x1)
	TCS3400__MEDIUM_GAIN,	//!< Medium gain mode (x4)
	TCS3400__HIGH_GAIN,		//!< High gain mode (x16)
	TCS3400__MAXIMUM_GAIN	//!< Maximum gain mode (x64)
} TCS3400_gain;


/*!\enum TCS3400_it_persist
** \brief Persistence control of TCS3400
**/
typedef enum PACK__ TCS3400_it_persist {
	TCS3400__PERSIST_EVERY_RGBC_CYCLE = 0,	//!< Every RGBC cycle generates interrupt
	TCS3400__PERSIST_ANY_OUTSIDE_THRESHOLD,	//!< Any value outside of threshold range
	TCS3400__PERSIST_2_IT_PERIODS,			//!< 2 consecutive values out of range
	TCS3400__PERSIST_3_IT_PERIODS,			//!< 3 consecutive values out of range
	TCS3400__PERSIST_5_IT_PERIODS,			//!< 5 consecutive values out of range
	TCS3400__PERSIST_10_IT_PERIODS,			//!< 10 consecutive values out of range
	TCS3400__PERSIST_15_IT_PERIODS,			//!< 15 consecutive values out of range
	TCS3400__PERSIST_20_IT_PERIODS,			//!< 20 consecutive values out of range
	TCS3400__PERSIST_25_IT_PERIODS,			//!< 25 consecutive values out of range
	TCS3400__PERSIST_30_IT_PERIODS,			//!< 30 consecutive values out of range
	TCS3400__PERSIST_35_IT_PERIODS,			//!< 35 consecutive values out of range
	TCS3400__PERSIST_40_IT_PERIODS,			//!< 40 consecutive values out of range
	TCS3400__PERSIST_45_IT_PERIODS,			//!< 45 consecutive values out of range
	TCS3400__PERSIST_50_IT_PERIODS,			//!< 50 consecutive values out of range
	TCS3400__PERSIST_55_IT_PERIODS,			//!< 55 consecutive values out of range
	TCS3400__PERSIST_60_IT_PERIODS,			//!< 60 consecutive values out of range
} TCS3400_it_persist;


/*!\union uTCS3400_REG__ENABLE
** \brief Union for ENABLE register of TCS3400
**/
typedef union uTCS3400_REG__ENABLE {
	uint8_t Byte;
	struct {
		uint8_t PON		:1;	//!< Power ON. This field activates the internal oscillator to permit the timers and ADC channels to operate
		uint8_t AEN		:1;	//!< ADC enable. This bit activates the four-channel (RGBC) ADC. Writing a 1 enables the ADC
		uint8_t 		:1;
		uint8_t WEN		:1;	//!< Wait enable. This bit activates the wait feature. Writing a 1 activates the wait timer
		uint8_t	AIEN	:1;	//!< ALS interrupt enable. When asserted, permits ALS interrupts to be generated
		uint8_t 		:1;
		uint8_t	SAI		:1;	//!< Sleep After Interrupt. When asserted, the device will power down at the end of a RGBC cycle if an interrupt is generated
		uint8_t 		:1;
	} Bits;
} uTCS3400_REG__ENABLE;


/*!\union uTCS3400_REG__PERSIST
** \brief Union for PERSIST register of TCS3400
**/
typedef union uTCS3400_REG__PERSIST {
	uint8_t Byte;
	struct {
		TCS3400_it_persist	PERSIST	:4;	//!< ALS interrupt persistence filter
		uint8_t 					:4;	//!< Reserved. Write as 0
	} Bits;
} uTCS3400_REG__PERSIST;


/*!\union uTCS3400_REG__CONFIG
** \brief Union for CONFIG register of TCS3400
**/
typedef union uTCS3400_REG__CONFIG {
	uint8_t Byte;
	struct {
		uint8_t			:1;
		uint8_t WLONG	:1;	//!< Wait long. When asserted, the wait cycles are increased by a factor 12x from that programmed in the WTIME register
		uint8_t			:6;
	} Bits;
} uTCS3400_REG__CONFIG;


/*!\union uTCS3400_REG__CONTROL
** \brief Union for CONTROL register of TCS3400
**/
typedef union uTCS3400_REG__CONTROL {
	uint8_t Byte;
	struct {
		TCS3400_gain	AGAIN	:2;	//!< ALS gain sets the gain for internal integration amplifiers for both photo-diode channels
		uint8_t					:6;
	} Bits;
} uTCS3400_REG__CONTROL;


/*!\union uTCS3400_REG__AUX
** \brief Union for AUX register of TCS3400
**/
typedef union uTCS3400_REG__AUX {
	uint8_t Byte;
	struct {
		uint8_t			:5;
		uint8_t	ASIEN	:1;	//!< 0 disables, 1 enables ALS Saturation Interrupt
		uint8_t			:2;
	} Bits;
} uTCS3400_REG__AUX;


/*!\union uTCS3400_REG__STATUS
** \brief Union for STATUS register of TCS3400
**/
typedef union uTCS3400_REG__STATUS {
	uint8_t Byte;
	struct {
		uint8_t AVALID	:2;		//!< RGBC Valid. Indicates that the RGBC channels have completed an integration cycle
		uint8_t			:3;
		uint8_t AINT	:1;		//!< RGBC clear channel Interrupt
		uint8_t 		:2;
		uint8_t ASAT	:1;		//!< ALS Saturation. When asserted, the analog sensor was at the upper end of its dynamic range
	} Bits;
} uTCS3400_REG__STATUS;


/*!\union uTCS3400_REG__IR
** \brief Union for IR sensor access register of TCS3400
**/
typedef union uTCS3400_REG__IR {
	uint8_t Byte;
	struct {
		uint8_t		:7;
		uint8_t	IR	:1;	//!< IR Sensor access. If this bit is set the clear channel reports the measurement from the IR sensor (center diode)
	} Bits;
} uTCS3400_REG__IR;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for TCS3400 peripheral
** \weak TCS3400 Base address may be changed if user implemented
** \return FctERR - error code
**/
__WEAK FctERR TCS3400_Init(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for TCS3400
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR TCS3400_Write(uint8_t * data, uint16_t addr, uint16_t nb);


/*!\brief I2C Read function for TCS3400
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR TCS3400_Read(uint8_t * data, uint16_t addr, uint16_t nb);


/*!\brief I2C Word Write (low endian) function for TCS3400
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \return FctERR - error code
**/
FctERR TCS3400_Write_Word(uint16_t * data, uint16_t addr);


/*!\brief I2C Word Read (low endian) function for TCS3400
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \return FctERR - error code
**/
FctERR TCS3400_Read_Word(uint16_t * data, uint16_t addr);


/****************************************************************/
#include "TCS3400_ex.h"		// Include extensions
#include "TCS3400_proc.h"	// Include procedures
#endif
#endif /* __TCS3400_H__ */
/****************************************************************/
