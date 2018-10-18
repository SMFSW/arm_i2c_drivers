/*!\file TCS3472_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief TCS3472 Driver procedures
** \details TCS3472: Color light-to-digital converter with IR filter
**/
/****************************************************************/
#ifndef __TCS3472_PROC_H__
	#define __TCS3472_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "TCS3472.h"

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
/*!\struct TCS3472_t
** \brief TCS3472 user interface struct
**/
typedef struct TCS3472_t {
	uint16_t		Red;				//!< Current Red conversion
	uint16_t		Green;				//!< Current Green conversion
	uint16_t		Blue;				//!< Current Blue conversion
	uint16_t		Clear;				//!< Current Clear conversion
	uint32_t		Lux;				//!< Current Illuminance (lux)
	uint32_t		Temp;				//!< Current Color temperature (in Kelvin)
	float			xy[2];				//!< Current CIE1931 chromacity coordinates
	float			IR_Ratio;			//!< Current IR Ratio (in percent)
	float			Saturation_Ratio;	//!< Current Saturation ration(in percent)
	bool			SaturationRipple;	//!< Current Ripple saturation status (75% of saturation value)
	bool			Saturation;			//!< Current Sensor saturation status
	struct {
	I2C_slave_t *	slave_inst;			//!< Slave structure
	double			mat[3][3];			//!< RGB to XYZ matrix
	TCS3472_gain	Gain;				//!< Sensor gain config
	uint16_t		Integ;				//!< Sensor integration time config
	uint16_t		Wait;				//!< Sensor wait time config
	uint16_t		LowThreshold;		//!< Low Threshold config
	uint16_t		HighThreshold;		//!< High Threshold config
	bool			AIEN;				//!< Sensor ALS (Ambient Light Sensing) interrupts enabled config
	bool			WEN;				//!< Wait between ALS conversions enabled config
	uint8_t			ID;					//!< Chip ID
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
FctERR NONNULL__ TCS3472_Init_Sequence(TCS3472_t * pCpnt);

/*!\brief Get current Color temperature (in Kelvin)
** \param[in] pCpnt - Pointer to TCS3472 component
** \return FctERR - error code
**/
__INLINE uint32_t NONNULL_INLINE__ TCS3472_Get_Temp(TCS3472_t * pCpnt) {
	return pCpnt->Temp; }

/*!\brief Get current Illuminance (in lux)
** \param[in] pCpnt - Pointer to TCS3472 component
** \return FctERR - error code
**/
__INLINE uint32_t NONNULL_INLINE__ TCS3472_Get_Lux(TCS3472_t * pCpnt) {
	return pCpnt->Lux; }

/*!\brief Handler for TCS3472 peripheral
** \weak TCS3472 handler may be user implemented to suit custom needs
** \note May be called periodically to handle TCS3472 tasks
** \note Alternately may be called when event occurs on TCS3472 pin
** \param[in] pCpnt - Pointer to TCS3472 component
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3472_handler(TCS3472_t * pCpnt);

/*!\brief Handler for all TCS3472 peripherals
** \note May be called periodically to handle all TCS3472 tasks
**/
__INLINE void INLINE__ TCS3472_handler_all(void) {
	for (TCS3472_t * pCpnt = TCS3472 ; pCpnt < &TCS3472[I2C_TCS3472_NB] ; pCpnt++) {
		TCS3472_handler(pCpnt); }
}


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __TCS3472_PROC_H__ */
/****************************************************************/
