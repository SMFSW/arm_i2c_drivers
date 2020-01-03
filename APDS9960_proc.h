/*!\file APDS9960_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief APDS9960 Driver procedures
** \details APDS9960: Digital Proximity, Ambient Light, RGB and Gesture Sensor
**/
/****************************************************************/
#ifndef __APDS9960_PROC_H__
	#define __APDS9960_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "APDS9960.h"

#include "shared_ALS.h"
#include "shared_CLS.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define APDS9960_CHIP_ID		0xAB		//!< APDS9960 Chip ID to check against



// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct APDS9960_t
** \brief APDS9960 user interface struct
**/
typedef struct APDS9960_t {
	uint16_t					Red;				//!< Current Red conversion
	uint16_t					Green;				//!< Current Green conversion
	uint16_t					Blue;				//!< Current Blue conversion
	uint16_t					Clear;				//!< Current Clear conversion
	uint8_t						Prox;				//!< Current Proximity raw value
	uint32_t					Lux;				//!< Current Illuminance (lux)
	uint32_t					Temp;				//!< Current Color temperature (in Kelvin)
	float						xy[2];				//!< Current CIE1931 chromacity coordinates
	float						IR_Ratio;			//!< Current IR ratio (in percent)
	float						Saturation_Ratio;	//!< Current Saturation ratio (in percent)
	bool						SaturationRipple;	//!< Current Ripple saturation status (75% of saturation value)
	bool						Saturation;			//!< Current Sensor saturation status
	struct {
	I2C_slave_t *				slave_inst;			//!< Slave structure
	float						mat[3][3];			//!< RGB to XYZ matrix
	APDS9960_als_gain			ADC_Gain;			//!< ADC (ALS & Color) sensor gain config
	APDS9960_prox_gain			Prox_Gain;			//!< Proximity sensor gain config
	APDS9960_pulse_length		Prox_Length;			//!< Proximity LED pulse length
	APDS9960_drive_strength		Prox_Strength;		//!< Proximity LED drive strength
	uint8_t						Prox_Pulses;		//!< Proximity pulses sent in a cycle
	uint8_t						ADC_Integ_reg;		//!< ADC (ALS & Color) sensor integration time config (value written to register)
	uint16_t					ADC_Integ;			//!< ADC (ALS & Color) sensor integration time config
	uint16_t					Wait;				//!< Sensor wait time config
	uint16_t					ADC_LowThreshold;	//!< ADC (ALS & Color) Low Threshold config
	uint16_t					ADC_HighThreshold;	//!< ADC (ALS & Color) High Threshold config
	uint8_t						Prox_LowThreshold;	//!< Proximity Low Threshold config
	uint8_t						Prox_HighThreshold;	//!< Proximity High Threshold config
	bool						AIEN;				//!< Sensor ALS (Ambient Light Sensing) interrupts enabled config
	bool						PIEN;				//!< Sensor Proximity interrupts enabled config
	bool						GIEN;				//!< Gestures enabled config
	bool						WEN;				//!< Wait between conversions enabled config
	uint8_t						Id;					//!< APDS9960 chip ID
	} cfg;
} APDS9960_t;

extern APDS9960_t APDS9960[I2C_APDS9960_NB];		//!< APDS9960 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for APDS9960 peripheral
** \weak APDS9960 Init sequence may be user implemented if custom initialization sequence needed
** \param[in] pCpnt - Pointer to APDS9960 component
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Init_Sequence(APDS9960_t * pCpnt);


/*!\brief Handler for APDS9960 peripheral
** \weak APDS9960 handler may be user implemented to suit custom needs
** \note May be called periodically to handle APDS9960 tasks
** \note Alternately may be called when event occurs on APDS9960 pin
** \param[in] pCpnt - Pointer to APDS9960 component
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_handler(APDS9960_t * pCpnt);


/*!\brief Handler for all APDS9960 peripherals
** \note May be called periodically to handle all APDS9960 tasks
**/
__INLINE void INLINE__ APDS9960_handler_all(void) {
	for (APDS9960_t * pCpnt = APDS9960 ; pCpnt < &APDS9960[I2C_APDS9960_NB] ; pCpnt++) {
		APDS9960_handler(pCpnt); }
}


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __APDS9960_PROC_H__ */
/****************************************************************/
