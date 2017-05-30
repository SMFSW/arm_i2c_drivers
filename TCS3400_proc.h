/*!\file TCS3400_proc.h
** \author SMFSW
** \version v0.3
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief TCS3400 Driver procedures declarations
** \details TCS3400: Color Light-to-Digital Converter
**/
/****************************************************************/
#ifndef __TCS3400_PROC_H__
	#define __TCS3400_PROC_H__

#include "TCS3400.h"

#include "sarmfsw.h"
#include CMSIS_INC
#include CMSIS_CFG

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/
// TODO: doxygen


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
typedef struct TCS3400_proc {
	uint16_t		Red;
	uint16_t		Green;
	uint16_t		Blue;
	uint16_t		Clear;
	uint32_t		Lux;				//!< Illuminance (lux)
	uint32_t		Temp;				//!< Color temperature (in Kelvin)
	float			IR_Ratio;			//!< (in percent)
	float			Saturation_Ratio;	//!< (in percent)
	bool			SaturationRipple;	//!< Ripple saturation reached (75% of saturation value)
	bool			Saturation;			//!< Sensor saturation reached
	struct {
	TCS3400_gain	Gain;				//!< Sensor gain
	uint16_t		Integ;				//!< Sensor integration time
	uint16_t		Wait;				//!< Sensor wait time
	uint16_t		LowThreshold;
	uint16_t		HighThreshold;
	bool			AIEN;				//!< Sensor ALS (Ambient Light Sensing) interrupts enabled
	bool			WEN;				//!< Wait between ALS conversions enabled
	uint8_t			Revision_Id;		//!< Revision ID
	uint8_t			Device_Id;			//!< Chip ID
	} cfg;
} TCS3400_proc;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for TCS3400 peripheral
** \return FctERR - error code
**/
FctERR TCS3400_Init_Sequence(void);

uint32_t TCS3400_Get_Temp(void);
uint32_t TCS3400_Get_Lux(void);

FctERR TCS3400_handler(void);


/****************************************************************/
#endif
#endif /* __TCS3400_PROC_H__ */
/****************************************************************/
