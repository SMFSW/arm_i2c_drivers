/*!\file BMP180_proc.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief BMP180 Driver procedures declarations
** \details BMP180: Digital pressure sensor
**/
/****************************************************************/
#ifndef __BMP180_PROC_H__
	#define __BMP180_PROC_H__

#include "sarmfsw.h"
#include "BMP180.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define BMP180_CHIP_ID		0x55			//!< BMP180 Chip ID to check against

#define SEA_LEVEL_PRESSURE	1013.25f		//!< Sea pressure level (hPa)


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct BMP180_calibration
** \brief BMP180 calibration parameters structure
**/
typedef struct BMP180_calibration {
	int16_t		AC1;	//!< ac1 calibration value
	int16_t		AC2;	//!< ac2 calibration value
	int16_t		AC3;	//!< ac3 calibration value
	uint16_t	AC4;	//!< ac4 calibration value
	uint16_t	AC5;	//!< ac5 calibration value
	uint16_t	AC6;	//!< ac6 calibration value
	int16_t		B1;		//!< b1 calibration value
	int16_t		B2;		//!< b2 calibration value
	int16_t		MB;		//!< mb calibration value
	int16_t		MC;		//!< mc calibration value
	int16_t		MD;		//!< md calibration value
} BMP180_calib;

/*!\struct BMP180_proc
** \brief BMP180 user interface struct
**/
typedef struct BMP180_proc {
	float				Pressure;			//!< Current atmospheric pressure
	float				Temperature;		//!< Current temperature
	float				Altitude;			//!< Current altitude
	float				SeaLevelPressure;	//!< Current atmospheric pressure at sea level
	uint32_t			hStartConversion;	//!< Last conversion start tick
	struct {
	BMP180_oversampling	OSS;				//!< BMP180 Oversampling
	BMP180_calib		Calib;				//!< BMP180 calibration values
	uint8_t				Id;					//!< BMP180 chip ID
	} cfg;
} BMP180_proc;

extern BMP180_proc BMP180;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for BMP180 peripheral
** \weak BMP180 Init sequence may be user implemented if custom initialization sequence needed
** \return FctERR - error code
**/
__weak FctERR BMP180_Init_Sequence(void);


/*!\brief Set oversampling for BMP180 peripheral
** \param[in,out] oss - oversampling value
** \return FctERR - error code
**/
FctERR BMP180_Set_Oversampling(BMP180_oversampling oss);


/*!\brief Get calibration parameters from BMP180 peripheral
** \param[in,out] calib - pointer to calibration structure to read to
** \return FctERR - error code
**/
FctERR BMP180_Get_Calibration(BMP180_calib * calib);

/*!\brief Gets the compensated pressure level
** \param[in,out] pres - pointer to atmospheric pressure level to read to (in hPa)
** \return FctERR - error code
**/
FctERR BMP180_Get_Pressure(float * pres);

/*!\brief Get the temperature
** \param[in,out] temp - pointer to temperature to read to (in Celsius degrees)
** \return FctERR - error code
**/
FctERR BMP180_Get_Temperature(float * temp);


/*!\brief Handler for BMP180 peripheral
** \weak BMP180 handler may be user implemented to suit custom needs
** \note Should be called periodically to handle BMP180 tasks
** \return FctERR - error code
**/
__weak FctERR BMP180_handler(void);


/****************************************************************/
#endif
#endif /* __BMP180_PROC_H__ */
/****************************************************************/
