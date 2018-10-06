/*!\file BMP180_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief BMP180 Driver extensions
** \details BMP180: Digital pressure sensor
**/
/****************************************************************/
#ifndef __BMP180_EX_H__
	#define __BMP180_EX_H__

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
FctERR BMP180_Start_Conversion(const BMP180_meas meas);


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
FctERR BMP180_Get_Temperature_Raw(int32_t * tp);

/*!\brief Get Raw Pressure
** \param[in,out] pr - pointer to raw pressure result
** \return FctERR - error code
**/
FctERR BMP180_Get_Pressure_Raw(int32_t * pr);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __BMP180_EX_H__ */
/****************************************************************/
