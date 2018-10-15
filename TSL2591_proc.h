/*!\file TSL2591_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief TSL2591 Driver procedures
** \details TSL2591: Very-high sensitivity light-to-digital converter
**/
/****************************************************************/
#ifndef __TSL2591_PROC_H__
	#define __TSL2591_PROC_H__

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
#define TSL2591_CHIP_ID				0x50	//!< TSL2591 Chip ID to check against


#if !defined(TSL2591_GLASS_TRANSMITIVITY)
#define TSL2591_GLASS_TRANSMITIVITY	100		//!< Glass transmitivity (in percent)
#endif


#define TSL2591_CH0_COUNTS			264.1f	//!< Counts per uW/cm2 for CH0 (Full spectrum)
#define TSL2591_CH1_COUNTS			34.9f	//!< Counts per uW/cm2 for CH1 (IR)


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct TSL2591_t
** \brief TSL2591 user interface struct
**/
typedef struct TSL2591_t {
	uint16_t		Full;				//!< Current Full spectrum raw value
	uint16_t		IR;					//!< Current IR raw value
	uint32_t		Lux;				//!< Current Illuminance (lux)
	uint32_t		MaxLux;				//!< Current Max illuminance in current conditions (lux)
	float			IRF;				//!< Current Current IR ratio
	bool			SaturationRipple;	//!< Current Ripple saturation status (75% of saturation value)
	bool			Saturation;			//!< Current Sensor saturation status
	struct {
	I2C_slave_t *	slave_inst;			//!< Slave structure
	uint32_t		CPkL;				//!< Counts Per kiloLux config
	float			DER;				//!< Accuracy (+/- DER lux) config
	TSL2591_gain	Gain;				//!< Sensor gain config
	TSL2591_integ	Integ;				//!< Sensor integration time config
	bool			AIEN;				//!< Sensor ALS (Ambient Light Sensing) interrupts enabled config
	uint8_t			ID;					//!< Chip ID
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
FctERR NONNULL__ TSL2591_Init_Sequence(TSL2591_t * pCpnt);


/*!\brief Set proper CPL value (Counts per kiloLux)
 * \note Should be called after Configuration change of Integration time or Gain
** \param[in] pCpnt - Pointer to TSL2591 component
** \return FctERR - error code
**/
void NONNULL__ TSL2591_Set_CPL(TSL2591_t * pCpnt);

/*!\brief Get current Illuminance (in lux)
** \param[in] pCpnt - Pointer to TSL2591 component
** \return FctERR - error code
**/
__INLINE uint32_t NONNULL_INLINE__ TSL2591_Get_Lux(TSL2591_t * pCpnt) {
	return pCpnt->Lux; }

/*!\brief Handler for TSL2591 peripheral
** \weak TSL2591 handler may be user implemented to suit custom needs
** \note May be called periodically to handle TSL2591 tasks
** \note Alternately may be called when event occurs on TSL2591 pin
** \param[in] pCpnt - Pointer to TSL2591 component
** \return FctERR - error code
**/
FctERR NONNULL__ TSL2591_handler(TSL2591_t * pCpnt);

/*!\brief Handler for all TSL2591 peripherals
** \note May be called periodically to handle all TSL2591 tasks
**/
__INLINE void INLINE__ TSL2591_handler_all(void) {
	for (TSL2591_t * pCpnt = TSL2591 ; pCpnt < &TSL2591[I2C_TSL2591_NB] ; pCpnt++) {
		TSL2591_handler(pCpnt); }
}


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __TSL2591_PROC_H__ */
/****************************************************************/
