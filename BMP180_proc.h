/*!\file BMP180_proc.h
** \author SMFSW
** \version v0.2
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief BMP180 Driver procedures declarations
** \details BMP180: Digital pressure sensor
**/
/****************************************************************/
#ifndef __BMP180_PROC_H__
	#define __BMP180_PROC_H__

#include "BMP180.h"

#include "sarmfsw.h"
#include CMSIS_INC
#include CMSIS_CFG

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define BMP180_CHIP_ID		0x55			//!< BMP180 Chip ID to check against

#define SEA_LEVEL_PRESSURE	1013.25f		//!< Sea pressure level (hPa)


// *****************************************************************************
// Section: Types
// *****************************************************************************
typedef struct BMP180_calibration {
	int16_t		AC1;
	int16_t		AC2;
	int16_t		AC3;
	uint16_t	AC4;
	uint16_t	AC5;
	uint16_t	AC6;
	int16_t		B1;
	int16_t		B2;
	int16_t		MB;
	int16_t		MC;
	int16_t		MD;
} BMP180_calib;

typedef struct BMP180_proc {
	float				Pressure;
	float				Temperature;
	float				Altitude;
	float				SeaLevelPressure;
	uint32_t			hStartConversion;
	struct {
	BMP180_oversampling	OSS;
	BMP180_calib		Calib;
	uint8_t				Id;
	} cfg;
} BMP180_proc;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for BMP180 peripheral
** \return FctERR - error code
**/
FctERR BMP180_Init_Sequence(void);


FctERR BMP180_Set_Oversampling(BMP180_oversampling oss);


FctERR BMP180_Get_Calibration(BMP180_calib * calib);

/**!\brief  Gets the compensated pressure level in hPa
**/
FctERR BMP180_Get_Pressure(float * pres);

/**!\brief  Reads the temperatures in degrees Celsius
**/
FctERR BMP180_Get_Temperature(float * temp);


FctERR BMP180_handler(void);


/****************************************************************/
#endif
#endif /* __BMP180_PROC_H__ */
/****************************************************************/
