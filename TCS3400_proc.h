/*!\file TCS3400_proc.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief TCS3400 Driver procedures declarations
** \details TCS3400: Color Light-to-Digital Converter
**/
/****************************************************************/
#ifndef __TCS3400_PROC_H__
	#define __TCS3400_PROC_H__

#include "sarmfsw.h"
#include "TCS3400.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define TCS34005_CHIP_ID		0x90		//!< TCS34001/TCS34005 Chip ID to check against
#define TCS34007_CHIP_ID		0x93		//!< TCS34003/TCS34007 Chip ID to check against


#define TCS3400_IR_COUNT		13.3f		//!< Counts per uW/cm2 for IR channel
#define TCS3400_CLEAR_COUNT		14.0f		//!< Counts per uW/cm2 for Clear channel
#define TCS3400_RED_COUNT		17.0f		//!< Counts per uW/cm2 for Red channel
#define TCS3400_GREEN_COUNT		14.5f		//!< Counts per uW/cm2 for Green channel
#define TCS3400_BLUE_COUNT		11.8f		//!< Counts per uW/cm2 for Blue channel


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct TCS3400_proc
** \brief TCS3400 user interface struct
**/
typedef struct TCS3400_proc {
	uint16_t		Red;				//!< Current Red conversion
	uint16_t		Green;				//!< Current Green conversion
	uint16_t		Blue;				//!< Current Blue conversion
	uint16_t		Clear;				//!< Current Clear conversion
	uint32_t		Lux;				//!< Current Illuminance (lux)
	uint32_t		Temp;				//!< Current Color temperature (in Kelvin)
	float			IR_Ratio;			//!< Current IR ratio (in percent)
	float			Saturation_Ratio;	//!< Current Saturation ratio (in percent)
	bool			SaturationRipple;	//!< Current Ripple saturation status (75% of saturation value)
	bool			Saturation;			//!< Current Sensor saturation status
	struct {
	TCS3400_gain	Gain;				//!< Sensor gain config
	uint16_t		Integ;				//!< Sensor integration time config
	uint16_t		Wait;				//!< Sensor wait time config
	uint16_t		LowThreshold;		//!< Low Threshold config
	uint16_t		HighThreshold;		//!< High Threshold config
	bool			AIEN;				//!< Sensor ALS (Ambient Light Sensing) interrupts enabled config
	bool			WEN;				//!< Wait between ALS conversions enabled config
	uint8_t			Revision_Id;		//!< TCS3472xx Revision ID
	uint8_t			Device_Id;			//!< TCS3472xx Chip ID
	} cfg;
} TCS3400_proc;

extern TCS3400_proc	TCS3400;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for TCS3400 peripheral
** \weak TCS3400 Init sequence may be user implemented if custom initialization sequence needed
** \return FctERR - error code
**/
__weak FctERR TCS3400_Init_Sequence(void);

/*!\brief Get current Color temperature (in Kelvin)
** \return FctERR - error code
**/
uint32_t TCS3400_Get_Temp(void);

/*!\brief Get current Illuminance (in lux)
** \return FctERR - error code
**/
uint32_t TCS3400_Get_Lux(void);

/*!\brief Handler for TCS3400 peripheral
** \weak TCS3400 handler may be user implemented to suit custom needs
** \note May be called periodically to handle TCS3400 tasks
** \note Alternately may be called when event occurs on TCS3400 pin
** \return FctERR - error code
**/
__weak FctERR TCS3400_handler(void);


/****************************************************************/
#endif
#endif /* __TCS3400_PROC_H__ */
/****************************************************************/
