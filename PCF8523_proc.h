/*!\file PCF8523_proc.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief PCF8523 Driver procedures declarations
** \details PCF8523: Real-Time Clock (RTC) and calendar
**/
/****************************************************************/
#ifndef __PCF8523_PROC_H__
	#define __PCF8523_PROC_H__

#include "PCF8523.h"

#include "sarmfsw.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
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


typedef struct PCF8523_proc {
	PCF8523_date	date;
	PCF8523_time	time;
	struct {
	uint8_t			Src_Clock_A;
	uint8_t			Src_Clock_B;
	bool			Hour_Format;	// 0: 24h; 1: 12h
	} cfg;
} PCF8523_proc;

extern PCF8523_proc	PCF8523;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for PCF8523 peripheral
** \return FctERR - error code
**/
FctERR PCF8523_Init_Sequence(void);


FctERR PCF8523_Set_Date(PCF8523_date date, bool BCD);

FctERR PCF8523_Set_Time(PCF8523_time time, bool BCD);


FctERR PCF8523_Get_Date(PCF8523_date * date, bool BCD);

FctERR PCF8523_Get_Time(PCF8523_time * time, bool BCD);


FctERR PCF8523_handler(void);


/****************************************************************/
#endif
#endif /* __PCF8523_PROC_H__ */
/****************************************************************/
