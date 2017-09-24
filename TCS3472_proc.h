/*!\file TCS3472_proc.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief TCS3472 Driver procedures declarations
** \details TCS3472: Color light-to-digital converter with IR filter
**/
/****************************************************************/
#ifndef __TCS3472_PROC_H__
	#define __TCS3472_PROC_H__

#include "TCS3472.h"

#include "sarmfsw.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define TCS34725_CHIP_ID		0x44		//!< TCS34721/TCS34725 Chip ID to check against
#define TCS34727_CHIP_ID		0x4D		//!< TCS34723/TCS34727 Chip ID to check against


#define TCS3472_IR_COUNT		0.0f		//!< Counts per uW/cm2 for IR channel
#define TCS3472_CLEAR_COUNT		0.0f		//!< Counts per uW/cm2 for Clear channel
#define TCS3472_RED_COUNT		0.0f		//!< Counts per uW/cm2 for Red channel
#define TCS3472_GREEN_COUNT		0.0f		//!< Counts per uW/cm2 for Green channel
#define TCS3472_BLUE_COUNT		0.0f		//!< Counts per uW/cm2 for Blue channel


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct TCS3472_proc
** \brief TCS3472 user interface struct
**/
typedef struct TCS3472_proc {
	uint16_t		Red;				//!< Current Red conversion
	uint16_t		Green;				//!< Current Green conversion
	uint16_t		Blue;				//!< Current Blue conversion
	uint16_t		Clear;				//!< Current Clear conversion
	uint32_t		Lux;				//!< Current Illuminance (lux)
	uint32_t		Temp;				//!< Current Color temperature (in Kelvin)
	float			IR_Ratio;			//!< Current IR Ratio (in percent)
	float			Saturation_Ratio;	//!< Current Saturation ration(in percent)
	bool			SaturationRipple;	//!< Current Ripple saturation status (75% of saturation value)
	bool			Saturation;			//!< Current Sensor saturation status
	struct {
	TCS3472_gain	Gain;				//!< Sensor gain config
	uint16_t		Integ;				//!< Sensor integration time config
	uint16_t		Wait;				//!< Sensor wait time config
	uint16_t		LowThreshold;		//!< Low Threshold config
	uint16_t		HighThreshold;		//!< High Threshold config
	bool			AIEN;				//!< Sensor ALS (Ambient Light Sensing) interrupts enabled config
	bool			WEN;				//!< Wait between ALS conversions enabled config
	uint8_t			Id;					//!< TCS34xx Chip ID
	} cfg;
} TCS3472_proc;

extern TCS3472_proc TCS3472;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for TCS3472 peripheral
** \return FctERR - error code
**/
FctERR TCS3472_Init_Sequence(void);

/*!\brief Get current Color temperature (in Kelvin)
** \return FctERR - error code
**/
uint32_t TCS3472_Get_Temp(void);

/*!\brief Get current Illuminance (in lux)
** \return FctERR - error code
**/
uint32_t TCS3472_Get_Lux(void);

/*!\brief Handler for TCS3472 peripheral
** \note May be called periodically to handle TCS3472 tasks
** \note Alternately may be called when event occurs on TCS3472 pin
** \return FctERR - error code
**/
FctERR TCS3472_handler(void);


/****************************************************************/
#endif
#endif /* __TCS3472_PROC_H__ */
/****************************************************************/
