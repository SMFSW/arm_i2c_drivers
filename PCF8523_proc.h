/*!\file PCF8523_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief PCF8523 Driver procedures
** \details PCF8523: Real-Time Clock (RTC) and calendar
**/
/****************************************************************/
#ifndef __PCF8523_PROC_H__
	#define __PCF8523_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "PCF8523.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct PCF8523_date
** \brief PCF8523 Date struct
**/
typedef struct PCF8523_date {
	uint8_t		Day;		//!< Day
	uint8_t		Weekday;	//!< Weekday
	uint8_t		Month;		//!< Month
	uint16_t	Year;		//!< Year
} PCF8523_date;


/*!\struct PCF8523_time
** \brief PCF8523 Time of day struct
**/
typedef struct PCF8523_time {
	uint8_t		Seconds;	//!< Seconds
	uint8_t		Minutes;	//!< Minutes
	uint8_t		Hours;		//!< Hours
} PCF8523_time;


typedef struct PCF8523_t {
	PCF8523_date	date;
	PCF8523_time	time;
	struct {
	I2C_slave_t *	slave_inst;		//!< Slave structure
	uint8_t			Src_Clock_A;
	uint8_t			Src_Clock_B;
	bool			Hour_Format;	//!< 0: 24h; 1: 12h
	} cfg;
} PCF8523_t;

extern PCF8523_t	PCF8523;		//!< PCF8523 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for PCF8523 peripheral
** \weak PCF8523 Init sequence may be user implemented if custom initialization sequence needed
** \return FctERR - error code
**/
FctERR PCF8523_Init_Sequence(void);


FctERR PCF8523_Set_Date(const PCF8523_date date, const bool BCD);

FctERR PCF8523_Set_Time(const PCF8523_time time, const bool BCD);


FctERR PCF8523_Get_Date(PCF8523_date * date, const bool BCD);

FctERR PCF8523_Get_Time(PCF8523_time * time, const bool BCD);


/*!\brief Handler for PCF8523 peripheral
** \weak PCF8523 handler may be user implemented to suit custom needs
** \note May be called periodically to handle PCF8523 tasks
** \note Alternately may be called when event occurs on PCF8523 pin
** \return FctERR - error code
**/
FctERR PCF8523_handler(void);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* __PCF8523_PROC_H__ */
/****************************************************************/
