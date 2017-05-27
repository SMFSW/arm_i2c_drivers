/*!\file BMP180_ex.h
** \author SMFSW
** \version v0.2
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
#include CMSIS_INC
#include CMSIS_CFG

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

FctERR BMP180_Start_Conversion(BMP180_meas meas);


__INLINE FctERR INLINE__ BMP180_Get_ChipID(uint8_t * id) {
	return BMP180_Read(id, BMP180__ID, 1); }

__INLINE FctERR INLINE__ BMP180_Reset(void) {
	uint8_t rst = BMP180_RESET_VAL;
	return BMP180_Write(&rst, BMP180__SOFT_RESET, 1); }


FctERR BMP180_Get_Temperature_Raw(uint32_t * temp);
FctERR BMP180_Get_Pressure_Raw(uint32_t * temp);


/****************************************************************/
#endif
#endif /* __BMP180_EX_H__ */
/****************************************************************/
