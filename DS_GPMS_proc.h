/*!\file DS_GPMS_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief DS-GPM.S Driver procedures
** \details DS-GPM.S: 99 Channel Positioning System (GPS + GLONASS) Shield
**/
/****************************************************************/
#ifndef __DS_GPMS_PROC_H__
	#define __DS_GPMS_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "DS_GPMS.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/
// TODO: doxygen


// *****************************************************************************
// Section: Constants
// *****************************************************************************


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct GPMS_date
** \brief DS-GPM.S Date struct
**/
typedef struct GPMS_date {
	uint16_t	Year;	//!< Year
	uint8_t		Month;	//!< Month
	uint8_t		Day;	//!< Day
} GPMS_date;

/*!\struct GPMS_hour
** \brief DS-GPM.S Time of day struct
**/
typedef struct GPMS_hour {
	uint8_t	Hours;		//!< Hours
	uint8_t	Minutes;	//!< Minutes
	uint8_t	Seconds;	//!< Seconds
} GPMS_hour;

/*!\struct GPMS_coord
** \brief DS-GPM.S coordinate struct
**/
typedef struct GPMS_coord {
	uint8_t			Degrees;	//!< Degrees coordinate
	float			Minutes;	//!< Minutes coordinate
	GPMS_direction	Direction;	//!< Direction coordinate
} GPMS_coord;


typedef struct GPMS_t {
	GPMS_date		Date;
	GPMS_hour		Hour;
	GPMS_coord		Latitude;
	GPMS_coord		Longitude;
	uint16_t		Altitude;
	float			Speed;
	float			Heading_True;
	float			Heading_Magnetic;
	struct {
	I2C_slave_t *	slave_inst;			//!< Slave structure
	GPMS_mode		Mode;
	} cfg;
} GPMS_t;

extern GPMS_t	GPMS;					//!< GPMS User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for GPMS peripheral
** \weak GPMS Init sequence may be user implemented if custom initialization sequence needed
** \return FctERR - error code
**/
__WEAK FctERR GPMS_Init_Sequence(void);

FctERR GPMS_Get_Date(GPMS_date * date);

FctERR GPMS_Get_Hour(GPMS_hour * hour);

FctERR GPMS_Get_Latitude(GPMS_coord * lat);

FctERR GPMS_Get_Longitude(GPMS_coord * lon);

FctERR GPMS_Get_Heading(float * heading, const GPMS_north type);

FctERR GPMS_Get_Speed(float * speed);

FctERR GPMS_Get_Altitude(uint16_t * altitude);

/*!\brief Handler for GPMS peripheral
** \weak GPMS handler may be user implemented to suit custom needs
** \note May be called periodically to handle GPMS tasks
** \return FctERR - error code
**/
__WEAK FctERR GPMS_handler(void);

/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __DS_GPMS_PROC_H__ */
/****************************************************************/
