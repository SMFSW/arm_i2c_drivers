/*!\file BMP180_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief BMP180 Driver procedures
** \details BMP180: Digital pressure sensor
**/
/****************************************************************/
#ifndef __BMP180_PROC_H__
	#define __BMP180_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

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

/*!\struct BMP180_t
** \brief BMP180 user interface struct
**/
typedef struct BMP180_t {
	float					Pressure;			//!< Current atmospheric pressure
	float					Temperature;		//!< Current temperature
	float					Altitude;			//!< Current altitude
	float					SeaLevelPressure;	//!< Current atmospheric pressure at sea level
	uint32_t				hStartConversion;	//!< Last conversion start tick
	struct {
	I2C_slave_t *			slave_inst;			//!< Slave structure
	BMP180_oversampling		OSS;				//!< Oversampling
	BMP180_calib			Calib;				//!< Calibration values
	uint8_t					ID;					//!< Chip ID
	} cfg;
} BMP180_t;

extern BMP180_t BMP180[I2C_BMP180_NB];			//!< BMP180 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for BMP180 peripheral
** \weak BMP180 Init sequence may be user implemented if custom initialization sequence needed
** \param[in] pCpnt - Pointer to BMP180 component
** \return FctERR - error code
**/
FctERR NONNULL__ BMP180_Init_Sequence(BMP180_t * pCpnt);


/*!\brief Set oversampling for BMP180 peripheral
** \param[in] pCpnt - Pointer to BMP180 component
** \param[in,out] oss - oversampling value
** \return FctERR - error code
**/
FctERR NONNULL__ BMP180_Set_Oversampling(BMP180_t * pCpnt, const BMP180_oversampling oss);


/*!\brief Get calibration parameters from BMP180 peripheral
** \param[in] pCpnt - Pointer to BMP180 component
** \param[in,out] calib - pointer to calibration structure to read to
** \return FctERR - error code
**/
FctERR NONNULL__ BMP180_Get_Calibration(BMP180_t * pCpnt, BMP180_calib * calib);

/*!\brief Gets the compensated pressure level
** \param[in] pCpnt - Pointer to BMP180 component
** \param[in,out] pres - pointer to atmospheric pressure level to read to (in hPa)
** \return FctERR - error code
**/
FctERR NONNULLX__(1) BMP180_Get_Pressure(BMP180_t * pCpnt, float * pres);

/*!\brief Get the temperature
** \param[in] pCpnt - Pointer to BMP180 component
** \param[in,out] temp - pointer to temperature to read to (in Celsius degrees)
** \return FctERR - error code
**/
FctERR NONNULLX__(1) BMP180_Get_Temperature(BMP180_t * pCpnt, float * temp);


/*!\brief Handler for BMP180 peripheral
** \weak BMP180 handler may be user implemented to suit custom needs
** \note Should be called periodically to handle BMP180 tasks
** \param[in] pCpnt - Pointer to BMP180 component
** \return FctERR - error code
**/
FctERR NONNULL__ BMP180_handler(BMP180_t * pCpnt);


/*!\brief Handler for all BMP180 peripherals
** \note May be called periodically to handle all BMP180 tasks
**/
__INLINE void INLINE__ BMP180_handler_all(void) {
	for (BMP180_t * pCpnt = BMP180 ; pCpnt < &BMP180[I2C_BMP180_NB] ; pCpnt++) {
		BMP180_handler(pCpnt); }
}


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __BMP180_PROC_H__ */
/****************************************************************/
