/*!\file TSL2591_proc.h
** \author SMFSW
** \version v0.3
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief TSL2591 Driver procedures declarations
** \details TSL2591: Very-high sensitivity light-to-digital converter
**/
/****************************************************************/
#ifndef __TSL2591_PROC_H__
	#define __TSL2591_PROC_H__

#include "TSL2591.h"

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
#define TSL2591_CHIP_ID				0x50	//!< TSL2591 Chip ID to check against


#if !defined(TSL2591_GLASS_TRANSMITIVITY)
#define TSL2591_GLASS_TRANSMITIVITY	100		//!< Glass transmitivity (in percent)
#endif


#define TSL2591_CH0_COUNTS			264.1f	//!< Counts per uW/cm2 for CH0 (Full spectrum)
#define TSL2591_CH1_COUNTS			34.9f	//!< Counts per uW/cm2 for CH1 (IR)


// *****************************************************************************
// Section: Types
// *****************************************************************************
typedef struct TSL2591_proc {
	uint16_t		Full;				//!< Full spectrum raw value
	uint16_t		IR;					//!< IR raw value
	uint32_t		Lux;				//!< Illuminance (lux)
	uint32_t		MaxLux;				//!< Max illuminance in current conditions (lux)
	float			IRF;				//!< Infra Red Factor
	bool			SaturationRipple;	//!< Ripple saturation reached (75% of saturation value)
	bool			Saturation;			//!< Sensor saturation reached
	struct {
	uint32_t		CPkL;				//!< Counts Per kiloLux
	float			DER;				//!< Accuracy (+/- DER lux)
	TSL2591_gain	Gain;				//!< Sensor gain
	TSL2591_integ	Integ;				//!< Sensor integration time
	bool			AIEN;				//!< Sensor ALS (Ambient Light Sensing) interrupts enabled
	uint8_t			Id;					//!< Chip ID
	} cfg;
} TSL2591_proc;

// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for TSL2591 peripheral
** \return FctERR - error code
**/
FctERR TSL2591_Init_Sequence(void);


void TSL2591_Set_CPL(void);

uint32_t TSL2591_Get_Lux(void);

FctERR TSL2591_handler(void);


/****************************************************************/
#endif
#endif /* __TSL2591_PROC_H__ */
/****************************************************************/
