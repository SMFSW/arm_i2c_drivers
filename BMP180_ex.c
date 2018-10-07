/*!\file BMP180_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief BMP180 Driver extensions
** \details BMP180: Digital pressure sensor
**/
/****************************************************************/
#include "BMP180.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_BMP180)
/****************************************************************/


static uint8_t BMP180_OSS_time[4] = { 5, 8, 14, 26 };	//!< BMP180 Oversampling values with +1.5ms than given in datasheet


FctERR BMP180_Start_Conversion(const BMP180_meas meas)
{
	uBMP180_REG__MEAS_CTRL	CTRL;
	FctERR					err;

	if (	(meas != BMP180__MEAS_PRESSURE)
		&&	(meas != BMP180__MEAS_TEMPERATURE))	{ return ERROR_VALUE; }	// Unknown conversion

	CTRL.Byte = 0;
	CTRL.Bits.SCO = 1;
	CTRL.Bits.MEAS_CTRL = meas;

	if (meas == BMP180__MEAS_PRESSURE)	{ CTRL.Bits.OSS = BMP180.cfg.OSS; }

	err = BMP180_Write((uint8_t *) &CTRL.Byte, BMP180__CTRL_MEAS, 1);
	if(err)		{ return err; }

	BMP180.hStartConversion = HAL_GetTick();

	return ERROR_OK;
}


FctERR BMP180_Get_Temperature_Raw(int32_t * tp)
{
	uint16_t	RES;
	FctERR		err;

	err = BMP180_Start_Conversion(BMP180__MEAS_TEMPERATURE);
	if (err)	{ return err; }

	HAL_Delay(5);

	err = BMP180_Read_Word(&RES, BMP180__OUT_MSB);
	if (err)	{ return err; }

	*tp = RES;
	return err;
}


FctERR BMP180_Get_Pressure_Raw(int32_t * pr)
{
	uint8_t		RES[3];
	FctERR		err;

	err = BMP180_Start_Conversion(BMP180__MEAS_PRESSURE);
	if (err)	{ return err; }

	HAL_Delay(BMP180_OSS_time[BMP180.cfg.OSS]);

	err = BMP180_Read(RES, BMP180__OUT_MSB, 3);
	if (err)	{ return err; }

	*pr = (LSHIFT(RES[0], 16) + LSHIFT(RES[1], 8) + RES[2]) >> (8 - BMP180.cfg.OSS);
	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
