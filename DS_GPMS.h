/*!\file DS_GPMS.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief DS-GPM.S Driver
** \details DS-GPM.S: 99 Channel Positioning System (GPS + GLONASS) Shield
**/
/****************************************************************/
#ifndef __DS_GPMS_H__
	#define __DS_GPMS_H__

#include "sarmfsw.h"
#include "I2C_component.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define GPMS_ADDR			0x68								//!< DS-GPM.S address

#define GPMS_JUMPER_A0_A1	0									//!< DS-GPM.S index with jumper A0 & A1
#define GPMS_JUMPER_A1		1									//!< DS-GPM.S index with jumper A1
#define GPMS_JUMPER_A0		2									//!< DS-GPM.S index with jumper A0
#define GPMS_JUMPER_NONE	3									//!< DS-GPM.S index with no jumper

#define GPMS_BASE_ADDR		(GPMS_ADDR + GPMS_JUMPER_A0_A1)		//!< DS-GPM.S Base address


// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern I2C_slave GPMS_hal;	//!< DS-GPM.S Slave structure


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum GPMS_wr_reg_map
** \brief Write register map enum of DS-GPM.S
**/
typedef enum PACK__ GPMS_wr_reg_map {
	GPMS__IO_DIRECTION = 0,						//!< Local IO port direction register
	GPMS__IO_INPUT_TYPE,						//!< Local IO port input type register
	GPMS__IO_PORT_OUTPUT,						//!< Local IO port output data register
} GPMS_wr_reg;

/*!\enum GPMS_rd_reg_map
** \brief Read register map enum of DS-GPM.S
**/
typedef enum PACK__ GPMS_rd_reg_map {
	GPMS__HOURS_TENS = 0,						//!< Hours tens register
	GPMS__HOURS_UNITS,							//!< Hours units register
	GPMS__MINUTES_TENS,							//!< Minutes tens register
	GPMS__MINUTES_UNITS,						//!< Minutes units register
	GPMS__SECONDS_TENS,							//!< Seconds tens register
	GPMS__SECONDS_UNITS,						//!< Seconds units register
	GPMS__DAY_OF_MONTH_TENS,					//!< Day of month tens register
	GPMS__DAY_OF_MONTH_UNITS,					//!< Day of month units register
	GPMS__MONTH_TENS,							//!< Month tens register
	GPMS__MONTH_UNITS,							//!< Month units register
	GPMS__YEARS_THOUSANDS,						//!< Years thousands register
	GPMS__YEARS_HUNDREDS,						//!< Years hundreds register
	GPMS__YEARS_TENS,							//!< Years tens register
	GPMS__YEARS_UNITS,							//!< Years units register
	GPMS__LATITUDE_DEGREES_TENS,				//!< Latitude degree tens register
	GPMS__LATITUDE_DEGREES_UNITS,				//!< Latitude degree units register
	GPMS__LATITUDE_MINUTES_TENS,				//!< Latitude minutes tens register
	GPMS__LATITUDE_MINUTES_UNITS,				//!< Latitude minutes units register
	GPMS__LATITUDE_MINUTES_TENTHS,				//!< Latitude minutes tenths register
	GPMS__LATITUDE_MINUTES_HUNDREDTHS,			//!< Latitude minutes hundredths register
	GPMS__LATITUDE_MINUTES_THOUSANDS,			//!< Latitude minutes thousands register
	GPMS__LATITUDE_MINUTES_TEN_THOUSANDS,		//!< Latitude minutes ten thousands register
	GPMS__LATITUDE_DIRECTION_CHARACTER,			//!< Latitude direction character register
	GPMS__LONGITUDE_DEGREES_HUNDREDTHS,			//!< Longitude degrees hundredths register
	GPMS__LONGITUDE_DEGREES_TENS,				//!< Longitude degrees tens register
	GPMS__LONGITUDE_DEGREES_UNITS,				//!< Longitude degrees units register
	GPMS__LONGITUDE_MINUTES_TENS,				//!< Longitude minutes tens register
	GPMS__LONGITUDE_MINUTES_UNITS,				//!< Longitude minutes units register
	GPMS__LONGITUDE_MINUTES_TENTHS,				//!< Longitude minutes tenths register
	GPMS__LONGITUDE_MINUTES_HUNDREDTHS,			//!< Longitude minutes hundredths register
	GPMS__LONGITUDE_MINUTES_THOUSANDS,			//!< Longitude minutes thousands register
	GPMS__LONGITUDE_MINUTES_TEN_THOUSANDS,		//!< Longitude minutes ten thousands register
	GPMS__LONGITUDE_DIRECTION_CHARACTER,		//!< Longitude direction character register
	GPMS__GPS_QUALITY_INDICATOR,				//!< GPS quality indicator
	GPMS__SATELLITES_IN_USE_TENS,				//!< Satellites in use tens register
	GPMS__SATELLITES_IN_USE_UNITS,				//!< Satellites in use units register
	GPMS__HDOP_TENS,							//!< HDOP tens register
	GPMS__HDOP_UNITS,							//!< HDOP units register
	GPMS__HDOP_TENTHS,							//!< HDOP tenths register
	GPMS__ALTITUDE_TENS_OF_THOUSANDS,			//!< Altitude (m) tens of thousands register
	GPMS__ALTITUDE_THOUSANDS,					//!< Altitude (m) thousands register
	GPMS__ALTITUDE_HUNDREDS,					//!< Altitude (m) hundreds register
	GPMS__ALTITUDE_TENS,						//!< Altitude (m) tens register
	GPMS__ALTITUDE_UNITS,						//!< Altitude (m) units register
	GPMS__HEADING_DEGREES_TRUE_HUNDREDS,		//!< Heading degrees (true North) hundreds register
	GPMS__HEADING_DEGREES_TRUE_TENS,			//!< Heading degrees (true North) tens register
	GPMS__HEADING_DEGREES_TRUE_UNITS,			//!< Heading degrees (true North) units register
	GPMS__HEADING_DEGREES_TRUE_TENTHS,			//!< Heading degrees (true North) tenths register
	GPMS__HEADING_DEGREES_MAGNETIC_HUNDREDS,	//!< Heading degrees (magnetic North) hundreds register
	GPMS__HEADING_DEGREES_MAGNETIC_TENS,		//!< Heading degrees (magnetic North) tens register
	GPMS__HEADING_DEGREES_MAGNETIC_UNITS,		//!< Heading degrees (magnetic North) units register
	GPMS__HEADING_DEGREES_MAGNETIC_TENTHS,		//!< Heading degrees (magnetic North) tenths register
	GPMS__SPEED_HUNDREDS,						//!< Speed hundreds register
	GPMS__SPEED_TENS,							//!< Speed tens register
	GPMS__SPEED_UNITS,							//!< Speed units register
	GPMS__SPEED_TENTHS,							//!< Speed tenths register
	GPMS__GPS_MODE,								//!< GPS mode character register
	GPMS__AN0_VALUE,							//!< Local analogic input AN0 value register
	GPMS__AN1_VALUE,							//!< Local analogic input AN1 value register
	GPMS__AN2_VALUE,							//!< Local analogic input AN2 value register
	GPMS__AN3_VALUE,							//!< Local analogic input AN3 value register
	GPMS__IOS_VALUE,							//!< Local I/O port input value
	GPMS__STATUS								//!< DS-GPM.S Status register
} GPMS_rd_reg;


/*!\enum GPMS_direction
** \brief DS-GPM.S direction enum
**/
typedef enum PACK__ GPMS_direction {
	GPMS__DIR_EAST = 'E',		//!< Direction East
	GPMS__DIR_NORTH = 'N',		//!< Direction North
	GPMS__DIR_SOUTH = 'S',		//!< Direction South
	GPMS__DIR_WEST = 'W'		//!< Direction West
} GPMS_direction;


/*!\enum GPMS_quality
** \brief DS-GPM.S signal quality
**/
typedef enum PACK__ GPMS_quality {
	GPMS__NO_GPS = 0,	//!< No GPS
	GPMS__GPS,			//!< GPS
	GPMS__DGPS			//!< DGPS
} GPMS_quality;


/*!\enum GPMS_mode
** \brief DS-GPM.S modes enum
**/
typedef enum PACK__ GPMS_mode {
	GPMS__MODE_AUTONOMOUS = 'A',		//!< Autonomous mode
	GPMS__MODE_DIFFERENTIAL = 'D',		//!< Differential mode
	GPMS__MODE_ESTIMATED = 'E',			//!< Estimated mode
	GPMS__MODE_MANUAL = 'M',			//!< Manual input mode
	GPMS__MODE_SIMULATED = 'S',			//!< Simulated mode
	GPMS__MODE_DATA_NOT_VALID = 'N'		//!< Data not valid
} GPMS_mode;

/*!\enum GPMS_north
** \brief DS-GPM.S north type
**/
typedef enum PACK__ GPMS_north {
	GPMS__TRUE_NORTH = 0,		//!< True north
	GPMS__MAGNETIC_NORTH		//!< Magnetic north
} GPMS_north;


/*!\union uGPMS_REG__STATUS
** \brief Union for STATUS register of DS-GPM.S
**/
typedef union uGPMS_REG__STATUS {
	uint8_t Byte;
	struct {
		uint8_t FW_VERSION_MINOR	:4;		//!< Firmware version minor (0-15)
		uint8_t FW_VERSION_MAJOR	:3;		//!< Firmware version major (0-3)
		uint8_t POS_FOUND			:1;		//!< Position found bit (0 = Not found, 1 = Found) 
		uint8_t MOTION				:1;		//!< Motion bit (0 = Standstill, 1 = Moving) 
	} Bits;
} uGPMS_REG__STATUS;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for GPMS peripheral
** \weak GPMS Base address may be changed if user implemented
** \return FctERR - error code
**/
__WEAK FctERR GPMS_Init(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for GPMS
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR GPMS_Write(const uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for GPMS
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR GPMS_Read(uint8_t * data, const uint16_t addr, const uint16_t nb);


/****************************************************************/
#include "DS_GPMS_ex.h"		// Include extensions
#include "DS_GPMS_proc.h"	// Include procedures
#endif
#endif /* __DS_GPMS_H__ */
/****************************************************************/
