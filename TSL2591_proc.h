/*!\file TSL2591_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief TSL2591 Driver procedures
** \details TSL2591: Very-high sensitivity light-to-digital converter
**/
/****************************************************************/
#ifndef TSL2591_PROC_H__
	#define TSL2591_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "TSL2591.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define TSL2591_CHIP_ID				0x50U	//!< TSL2591 Chip ID to check against


#define TSL2591_DEVICE_FACTOR		53U		//!< TSL2591 Device factor

#define TSL2591_CH0_COUNTS			264.1f	//!< Counts per uW/cm2 for CH0 (Full spectrum)
#define TSL2591_CH1_COUNTS			34.9f	//!< Counts per uW/cm2 for CH1 (IR)


#if !defined(TSL2591_GLASS_TRANSMISSIVITY)
#define TSL2591_GLASS_TRANSMISSIVITY	100U	//!< Glass transmissivity (in percent)
#endif


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct TSL2591_t
** \brief TSL2591 user interface struct
**/
typedef struct TSL2591_t {
	uint16_t			Full;				//!< Current Full spectrum raw value
	uint16_t			IR;					//!< Current IR raw value
	uint32_t			Lux;				//!< Current Illuminance (lux)
	float				IRF;				//!< Current Current IR ratio
	bool				SaturationRipple;	//!< Current Ripple saturation status (75% of saturation value)
	bool				Saturation;			//!< Current Sensor saturation status
	struct {
	I2C_slave_t *		slave_inst;			//!< Slave structure
	PeripheralGPIO_t	INT_GPIO;			//!< Interrupt GPIO struct
	uint32_t			CPkL;				//!< Counts Per kiloLux config
	float				DER;				//!< Accuracy (+/- DER lux) config
	TSL2591_gain		Gain;				//!< Sensor gain config
	TSL2591_integ		Integ;				//!< Sensor integration time config
	uint16_t			LowThreshold;		//!< Low Threshold config
	uint16_t			HighThreshold;		//!< High Threshold config
	bool				AIEN;				//!< Sensor ALS (Ambient Light Sensing) interrupts enabled config
	uint8_t				ID;					//!< Chip ID
	} cfg;
} TSL2591_t;

extern TSL2591_t	TSL2591[I2C_TSL2591_NB];	//!< TSL2591 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for TSL2591 peripheral
** \weak TSL2591 Init sequence may be user implemented if custom initialization sequence needed
** \param[in] pCpnt - Pointer to TSL2591 component
** \return FctERR - error code
**/
FctERR NONNULL__ TSL2591_Init_Sequence(TSL2591_t * const pCpnt);


/*!\brief Set proper CPL value (Counts per kiloLux)
 * \note Should be called after Configuration change of Integration time or Gain
** \param[in] pCpnt - Pointer to TSL2591 component
**/
void NONNULL__ TSL2591_Set_CPL(TSL2591_t * const pCpnt);

/*!\brief Get current Illuminance (in lux)
** \param[in] pCpnt - Pointer to TSL2591 component
** \return FctERR - error code
**/
__INLINE uint32_t NONNULL_INLINE__ TSL2591_Get_Lux(TSL2591_t * const pCpnt) {
	return pCpnt->Lux; }


/*!\brief Handler for TSL2591 peripheral
** \weak TSL2591 handler may be user implemented to suit custom needs
** \note May be called periodically to handle TSL2591 tasks
** \note Alternately may be called when event occurs on TSL2591 pin (or by calling \ref TSL2591_handler_it instead)
** \param[in] pCpnt - Pointer to TSL2591 component
** \return FctERR - error code
**/
FctERR NONNULL__ TSL2591_handler(TSL2591_t * const pCpnt);

/*!\brief Handler for TSL2591 peripheral GPIO interrupt
** \note \ref TSL2591_INT_GPIO_Init has to be called at init before using interrupt handler function
** \weak TSL2591 GPIO interrupt handler may be user implemented to suit custom needs
** \note May be called periodically to handle TSL2591 tasks through interrupts
** \param[in] pCpnt - Pointer to TSL2591 component
** \return FctERR - error code
**/
FctERR NONNULL__ TSL2591_handler_it(TSL2591_t * const pCpnt);

/*!\brief Handler for all TSL2591 peripherals
** \note May be called periodically to handle all TSL2591 tasks
** \return FctERR - error code
**/
FctERR TSL2591_handler_all(void);

/*!\brief Handler for all TSL2591 peripherals GPIO interrupt
** \note \ref TSL2591_INT_GPIO_Init has to be called at init before using interrupt handler function
** \note May be called periodically to handle all TSL2591 tasks
** \return FctERR - error code
**/
FctERR TSL2591_handler_it_all(void);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* TSL2591_PROC_H__ */
/****************************************************************/
