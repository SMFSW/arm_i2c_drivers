/*!\file TCS3472.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief TCS3472 Driver declarations
** \details TCS3472: Color light-to-digital converter with IR filter
**/
/****************************************************************/
#ifndef __TCS3472_H__
	#define __TCS3472_H__

#include "I2C_component.h"
#include "FctERR.h"

#include "sarmfsw.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define TCS34721_ADDR			0x39			//!< TCS34721 Base address
#define TCS34723_ADDR			0x39			//!< TCS34723 Base address
#define TCS34725_ADDR			0x29			//!< TCS34725 Base address
#define TCS34727_ADDR			0x29			//!< TCS34727 Base address

#define TCS3472_BASE_ADDR		TCS34725_ADDR	//!< TCS3472 Base address


// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern I2C_slave TCS3472_hal;	//!< TCS3472 Slave instance


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum TCS3472_reg_map
** \brief Register map enum of TCS3472
**/
typedef enum PACK__ TCS3472_reg_map {
	TCS3472__ENABLE = 0,		//!< Enables states and interrupts
	TCS3472__ATIME,				//!< RGBC time
	TCS3472__WTIME = 0x03,		//!< Wait time
	TCS3472__AILTL,				//!< Clear interrupt low threshold low byte
	TCS3472__AILTH,				//!< Clear interrupt low threshold high byte
	TCS3472__AIHTL,				//!< Clear interrupt high threshold low byte
	TCS3472__AIHTH,				//!< Clear ALS interrupt high threshold high byte
	TCS3472__PERSIST = 0x0C,	//!< Interrupt persistence filter
	TCS3472__CONFIG,			//!< Configuration
	TCS3472__CONTROL = 0x0F,	//!< Control
	TCS3472__ID = 0x12,			//!< Device ID
	TCS3472__STATUS,			//!< Device status
	TCS3472__CDATAL,			//!< Clear low data byte
	TCS3472__CDATAH,			//!< Clear high data byte
	TCS3472__RDATAL,			//!< Red low data byte
	TCS3472__RDATAH,			//!< Red high data byte
	TCS3472__GDATAL,			//!< Green low data byte
	TCS3472__GDATAH,			//!< Green high data byte
	TCS3472__BDATAL,			//!< Blue low data byte
	TCS3472__BDATAH				//!< Blue high data byte
} TCS3472_reg;

/*!\enum TCS3472_transaction
** \brief Transaction types of TCS3472
**/
typedef enum PACK__ TCS3472_transaction {
	TCS3472__TRANS_NORMAL_OP = 1,		//!< Normal operation
	TCS3472__TRANS_SPECIAL_FUNC = 3,	//!< Special Function
} TCS3472_transaction;

/*!\enum TCS3472_spec_func
** \brief Special functions of TCS3472
**/
typedef enum PACK__ TCS3472_spec_func {
	TCS3472__SF_CLR_IT = 6,				//!< Clear channel interrupt clear
} TCS3472_spec_func;

/*!\enum TCS3472_gain
** \brief Gain values of TCS3472
**/
typedef enum PACK__ TCS3472_gain {
	TCS3472__LOW_GAIN = 0,	//!< Low gain mode (x1)
	TCS3472__MEDIUM_GAIN,	//!< Medium gain mode (x4)
	TCS3472__HIGH_GAIN,		//!< High gain mode (x16)
	TCS3472__MAXIMUM_GAIN	//!< Maximum gain mode (x60)
} TCS3472_gain;


/*!\enum TCS3472_it_persist
** \brief Persistence control of TCS3472
**/
typedef enum PACK__ TCS3472_it_persist {
	TCS3472__PERSIST_EVERY_RGBC_CYCLE = 0,	//!< Every RGBC cycle generates interrupt
	TCS3472__PERSIST_1_IT_PERIODS,			//!< 1 integration time periods out of range
	TCS3472__PERSIST_2_IT_PERIODS,			//!< 2 integration time periods out of range
	TCS3472__PERSIST_3_IT_PERIODS,			//!< 3 integration time periods out of range
	TCS3472__PERSIST_5_IT_PERIODS,			//!< 5 integration time periods out of range
	TCS3472__PERSIST_10_IT_PERIODS,			//!< 10 integration time periods out of range
	TCS3472__PERSIST_15_IT_PERIODS,			//!< 15 integration time periods out of range
	TCS3472__PERSIST_20_IT_PERIODS,			//!< 20 integration time periods out of range
	TCS3472__PERSIST_25_IT_PERIODS,			//!< 25 integration time periods out of range
	TCS3472__PERSIST_30_IT_PERIODS,			//!< 30 integration time periods out of range
	TCS3472__PERSIST_35_IT_PERIODS,			//!< 35 integration time periods out of range
	TCS3472__PERSIST_40_IT_PERIODS,			//!< 40 integration time periods out of range
	TCS3472__PERSIST_45_IT_PERIODS,			//!< 45 integration time periods out of range
	TCS3472__PERSIST_50_IT_PERIODS,			//!< 50 integration time periods out of range
	TCS3472__PERSIST_55_IT_PERIODS,			//!< 55 integration time periods out of range
	TCS3472__PERSIST_60_IT_PERIODS,			//!< 60 integration time periods out of range
} TCS3472_it_persist;


/*!\union uTCS3472_CMD
** \brief Union for COMMAND register of TCS3472
**/
typedef union uTCS3472_CMD {
	uint8_t Byte;
	struct {
		TCS3472_spec_func	ADDR		:5;		//!< Address field/special function field
		TCS3472_transaction	TRANSACTION	:2;		//!< Select type of transaction to follow in subsequent data transfers
		uint8_t				CMD			:1;		//!< Select command register. Must write as 1 when addressing Command register
	} Bits;
} uTCS3472_CMD;


/*!\union uTCS3472_REG__ENABLE
** \brief Union for ENABLE register of TCS3472
**/
typedef union uTCS3472_REG__ENABLE {
	uint8_t Byte;
	struct {
		uint8_t PON		:1;	//!< Power ON. This field activates the internal oscillator to permit the timers and ADC channels to operate
		uint8_t AEN		:1;	//!< RGBC enable. This bit activates the two-channel ADC. Writing a 1 activates the RGBC
		uint8_t 		:1;
		uint8_t WEN		:1;	//!< Wait enable. This bit activates the wait feature. Writing a 1 activates the wait timer
		uint8_t	AIEN	:1;	//!< RGBC interrupt enable. When asserted, permits RGBC interrupts to be generated
		uint8_t 		:3;
	} Bits;
} uTCS3472_REG__ENABLE;


/*!\union uTCS3472_REG__PERSIST
** \brief Union for PERSIST register of TCS3472
**/
typedef union uTCS3472_REG__PERSIST {
	uint8_t Byte;
	struct {
		TCS3472_it_persist	PERSIST	:4;	//!< ALS interrupt persistence filter
		uint8_t 					:4;	//!< Reserved. Write as 0
	} Bits;
} uTCS3472_REG__PERSIST;


/*!\union uTCS3472_REG__CONFIG
** \brief Union for CONFIG register of TCS3472
**/
typedef union uTCS3472_REG__CONFIG {
	uint8_t Byte;
	struct {
		uint8_t			:1;
		uint8_t WLONG	:1;	//!< Wait long. When asserted, the wait cycles are increased by a factor 12x from that programmed in the WTIME register
		uint8_t			:6;
	} Bits;
} uTCS3472_REG__CONFIG;


/*!\union uTCS3472_REG__CONTROL
** \brief Union for CONTROL register of TCS3472
**/
typedef union uTCS3472_REG__CONTROL {
	uint8_t Byte;
	struct {
		TCS3472_gain	AGAIN	:2;	//!< ALS gain sets the gain for internal integration amplifiers for both photo-diode channels
		uint8_t					:6;
	} Bits;
} uTCS3472_REG__CONTROL;


/*!\union uTCS3472_REG__STATUS
** \brief Union for STATUS register of TCS3472
**/
typedef union uTCS3472_REG__STATUS {
	uint8_t Byte;
	struct {
		uint8_t AVALID	:2;		//!< RGBC Valid. Indicates that the RGBC channels have completed an integration cycle
		uint8_t			:3;
		uint8_t AINT	:1;		//!< RGBC clear channel Interrupt
		uint8_t 		:3;
	} Bits;
} uTCS3472_REG__STATUS;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for TCS3472 peripheral
** \return FctERR - error code
**/
FctERR TCS3472_Init(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for TCS3472
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR TCS3472_Write(uint8_t * data, uint16_t addr, uint16_t nb);


/*!\brief I2C Read function for TCS3472
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR TCS3472_Read(uint8_t * data, uint16_t addr, uint16_t nb);


/*!\brief I2C Word Write (low endian) function for TCS3472
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \return FctERR - error code
**/
FctERR TCS3472_Write_Word(uint16_t * data, uint16_t addr);


/*!\brief I2C Word Read (low endian) function for TCS3472
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \return FctERR - error code
**/
FctERR TCS3472_Read_Word(uint16_t * data, uint16_t addr);


/*!\brief I2C Special function Write function for TCS3472
** \param[in] func - special function to execute
** \return FctERR - error code
**/
FctERR TCS3472_Write_Special(TCS3472_spec_func func);


/****************************************************************/
#include "TCS3472_ex.h"		// Include extensions
#include "TCS3472_proc.h"	// Include procedures
#endif
#endif /* __TCS3472_H__ */
/****************************************************************/
