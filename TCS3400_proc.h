/*!\file TCS3400_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
** \brief TCS3400 Driver procedures
** \details TCS3400: Color Light-to-Digital Converter
**/
/****************************************************************/
#ifndef __TCS3400_PROC_H__
	#define __TCS3400_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "TCS3400.h"

#include "shared_CLS.h"

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
/*!\struct TCS3400_t
** \brief TCS3400 user interface struct
**/
typedef struct TCS3400_t {
	uint16_t		Red;				//!< Current Red conversion
	uint16_t		Green;				//!< Current Green conversion
	uint16_t		Blue;				//!< Current Blue conversion
	uint16_t		Clear;				//!< Current Clear conversion
	uint32_t		Lux;				//!< Current Illuminance (lux)
	uint32_t		Temp;				//!< Current Color temperature (in Kelvin)
	float			xy[2];				//!< Current CIE1931 chromacity coordinates
	float			IR_Ratio;			//!< Current IR ratio (in percent)
	float			Saturation_Ratio;	//!< Current Saturation ratio (in percent)
	bool			SaturationRipple;	//!< Current Ripple saturation status (75% of saturation value)
	bool			Saturation;			//!< Current Sensor saturation status
	struct {
	I2C_slave_t *	slave_inst;			//!< Slave structure
	float			mat[3][3];			//!< RGB to XYZ matrix
	TCS3400_gain	Gain;				//!< Sensor gain config
	uint8_t			Integ_reg;			//!< Sensor integration time config (value written to register)
	uint16_t		Integ;				//!< Sensor integration time config
	uint16_t		Wait;				//!< Sensor wait time config
	uint16_t		LowThreshold;		//!< Low Threshold config
	uint16_t		HighThreshold;		//!< High Threshold config
	bool			AIEN;				//!< Sensor ALS (Ambient Light Sensing) interrupts enabled config
	bool			WEN;				//!< Wait between ALS conversions enabled config
	uint8_t			Revision_ID;		//!< Revision ID
	uint8_t			Device_ID;			//!< Chip ID
	} cfg;
} TCS3400_t;

extern TCS3400_t	TCS3400[I2C_TCS3400_NB];	//!< TCS34000 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for TCS3400 peripheral
** \weak TCS3400 Init sequence may be user implemented if custom initialization sequence needed
** \param[in] pCpnt - Pointer to TCS3400 component
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3400_Init_Sequence(TCS3400_t * pCpnt);

/*!\brief Get current Color temperature (in Kelvin)
** \param[in] pCpnt - Pointer to TCS3400 component
** \return FctERR - error code
**/
__INLINE uint32_t NONNULL_INLINE__ TCS3400_Get_Temp(TCS3400_t * pCpnt) {
	return pCpnt->Temp; }

/*!\brief Get current Illuminance (in lux)
** \param[in] pCpnt - Pointer to TCS3400 component
** \return FctERR - error code
**/
__INLINE uint32_t NONNULL_INLINE__ TCS3400_Get_Lux(TCS3400_t * pCpnt) {
	return pCpnt->Lux; }

/*!\brief Handler for TCS3400 peripheral
** \weak TCS3400 handler may be user implemented to suit custom needs
** \note May be called periodically to handle TCS3400 tasks
** \note Alternately may be called when event occurs on TCS3400 pin
** \param[in] pCpnt - Pointer to TCS3400 component
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3400_handler(TCS3400_t * pCpnt);

/*!\brief Handler for all TCS3400 peripherals
** \note May be called periodically to handle all TCS3400 tasks
**/
__INLINE void INLINE__ TCS3400_handler_all(void) {
	for (TCS3400_t * pCpnt = TCS3400 ; pCpnt < &TCS3400[I2C_TCS3400_NB] ; pCpnt++) {
		TCS3400_handler(pCpnt); }
}


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __TCS3400_PROC_H__ */
/****************************************************************/
