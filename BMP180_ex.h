/*!\file BMP180_ex.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief BMP180 Driver extensions declarations
** \details BMP180: Digital pressure sensor
**/
/****************************************************************/
#ifndef __BMP180_EX_H__
	#define __BMP180_EX_H__

#include "BMP180.h"

#include "sarmfsw.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define	BMP180_RESET_VAL	0xB6	//!< Reset value for "soft reset" register


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

/*!\brief Start BMP180 Temperature/Pressure conversion
** \param[in] meas - Temperature / Pressure
** \return FctERR - error code
**/
FctERR BMP180_Start_Conversion(BMP180_meas meas);


/*!\brief Get BMP180 chip ID
** \param[in,out] id - pointer to chip ID result
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ BMP180_Get_ChipID(uint8_t * id) {
	return BMP180_Read(id, BMP180__ID, 1); }

/*!\brief Reset BMP180 chip
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ BMP180_Reset(void) {
	uint8_t rst = BMP180_RESET_VAL;
	return BMP180_Write(&rst, BMP180__SOFT_RESET, 1); }


/*!\brief Get Raw Temperature
** \param[in,out] tp - pointer to raw temperature result
** \return FctERR - error code
**/
FctERR BMP180_Get_Temperature_Raw(uint32_t * tp);

/*!\brief Get Raw Pressure
** \param[in,out] pr - pointer to raw pressure result
** \return FctERR - error code
**/
FctERR BMP180_Get_Pressure_Raw(uint32_t * pr);


/****************************************************************/
#endif
#endif /* __BMP180_EX_H__ */
/****************************************************************/
