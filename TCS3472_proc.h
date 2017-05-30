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
#include CMSIS_INC
#include CMSIS_CFG

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/
// TODO: doxygen


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
typedef struct TCS3472_proc {
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
	TCS3472_gain	Gain;				//!< Sensor gain
	uint16_t		Integ;				//!< Sensor integration time
	uint16_t		Wait;				//!< Sensor wait time
	uint16_t		LowThreshold;
	uint16_t		HighThreshold;
	bool			AIEN;				//!< Sensor ALS (Ambient Light Sensing) interrupts enabled
	bool			WEN;				//!< Wait between ALS conversions enabled
	uint8_t			Id;					//!< Chip ID
	} cfg;
} TCS3472_proc;


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

uint32_t TCS3472_Get_Temp(void);
uint32_t TCS3472_Get_Lux(void);

FctERR TCS3472_handler(void);


/****************************************************************/
#endif
#endif /* __TCS3472_PROC_H__ */
/****************************************************************/
