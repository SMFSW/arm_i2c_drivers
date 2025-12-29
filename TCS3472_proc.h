/*!\file TCS3472_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief TCS3472 Driver procedures
** \details TCS3472: Color light-to-digital converter with IR filter
**/
/****************************************************************/
#ifndef TCS3472_PROC_H__
	#define TCS3472_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "TCS3472.h"

#include "shared_CLS.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define TCS34725_CHIP_ID		0x44U		//!< TCS34721/TCS34725 Chip ID to check against
#define TCS34727_CHIP_ID		0x4DU		//!< TCS34723/TCS34727 Chip ID to check against


#define TCS3472_IR_COUNT		0.0f		//!< Counts per uW/cm2 for IR channel
#define TCS3472_CLEAR_COUNT		0.0f		//!< Counts per uW/cm2 for Clear channel
#define TCS3472_RED_COUNT		0.0f		//!< Counts per uW/cm2 for Red channel
#define TCS3472_GREEN_COUNT		0.0f		//!< Counts per uW/cm2 for Green channel
#define TCS3472_BLUE_COUNT		0.0f		//!< Counts per uW/cm2 for Blue channel


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct _TCS3472_t
** \brief TCS3472 user interface struct
**/
typedef struct _TCS3472_t {
	uint16_t			Red;				//!< Current Red conversion
	uint16_t			Green;				//!< Current Green conversion
	uint16_t			Blue;				//!< Current Blue conversion
	uint16_t			Clear;				//!< Current Clear conversion
	uint32_t			Lux;				//!< Current Illuminance (lux)
	uint32_t			Temp;				//!< Current Color temperature (in Kelvin)
	float				xy[2];				//!< Current CIE1931 chromacity coordinates
	float				IR_Ratio;			//!< Current IR Ratio (in percent)
	float				Saturation_Ratio;	//!< Current Saturation ration(in percent)
	bool				SaturationRipple;	//!< Current Ripple saturation status (75% of saturation value)
	bool				Saturation;			//!< Current Sensor saturation status
	struct {
	I2C_slave_t *		slave_inst;			//!< Slave structure
	PeripheralGPIO_t	INT_GPIO;			//!< Interrupt GPIO struct
	float				mat[3][3];			//!< RGB to XYZ matrix
	TCS3472_gain		Gain;				//!< Sensor gain config
	uint8_t				Integ_reg;			//!< Sensor integration time config (value written to register)
	uint16_t			Integ;				//!< Sensor integration time config
	uint16_t			Wait;				//!< Sensor wait time config
	uint16_t			LowThreshold;		//!< Low Threshold config
	uint16_t			HighThreshold;		//!< High Threshold config
	bool				AIEN;				//!< Sensor ALS (Ambient Light Sensing) interrupts enabled config
	bool				WEN;				//!< Wait between ALS conversions enabled config
	uint8_t				ChipID;				//!< Chip ID
	} cfg;
} TCS3472_t;

extern TCS3472_t TCS3472[I2C_TCS3472_NB];	//!< TCSL3472 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for TCS3472 peripheral
** \weak TCS3472 Init sequence may be user implemented if custom initialization sequence needed
** \param[in] pCpnt - Pointer to TCS3472 component
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3472_Init_Sequence(TCS3472_t * const pCpnt);

/*!\brief Get current Color temperature (in Kelvin)
** \param[in] pCpnt - Pointer to TCS3472 component
** \return FctERR - error code
**/
__INLINE uint32_t NONNULL_INLINE__ TCS3472_Get_Temp(const TCS3472_t * const pCpnt) {
	return pCpnt->Temp; }

/*!\brief Get current Illuminance (in lux)
** \param[in] pCpnt - Pointer to TCS3472 component
** \return FctERR - error code
**/
__INLINE uint32_t NONNULL_INLINE__ TCS3472_Get_Lux(const TCS3472_t * const pCpnt) {
	return pCpnt->Lux; }


/*!\brief Handler for TCS3472 peripheral
** \weak TCS3472 handler may be user implemented to suit custom needs
** \note May be called periodically to handle TCS3472 tasks
** \note Alternately may be called when event occurs on TCS3472 pin (or by calling \ref TCS3472_handler_it instead)
** \param[in] pCpnt - Pointer to TCS3472 component
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3472_handler(TCS3472_t * const pCpnt);

/*!\brief Handler for TCS3472 peripheral GPIO interrupt
** \note \ref TCS3472_INT_GPIO_Init has to be called at init before using interrupt handler function
** \weak TCS3472 GPIO interrupt handler may be user implemented to suit custom needs
** \note May be called periodically to handle TCS3472 tasks through interrupts
** \param[in] pCpnt - Pointer to TCS3472 component
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3472_handler_it(TCS3472_t * const pCpnt);

/*!\brief Handler for all TCS3472 peripherals
** \note May be called periodically to handle all TCS3472 tasks
** \return FctERR - error code
**/
FctERR TCS3472_handler_all(void);

/*!\brief Handler for all TCS3472 peripherals GPIO interrupt
** \note \ref TCS3472_INT_GPIO_Init has to be called at init before using interrupt handler function
** \note May be called periodically to handle all TCS3472 tasks
** \return FctERR - error code
**/
FctERR TCS3472_handler_it_all(void);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* TCS3472_PROC_H__ */
/****************************************************************/
