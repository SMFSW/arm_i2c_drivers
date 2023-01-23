/*!\file BMP180_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief BMP180 Driver extensions
** \details BMP180: Digital pressure sensor
**/
/****************************************************************/
#include "BMP180.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_BMP180)
/****************************************************************/


static uint8_t BMP180_OSS_time[4] = { 5, 8, 14, 26 };	//!< BMP180 Oversampling values with +1.5ms than given in datasheet


FctERR NONNULL__ BMP180_Start_Conversion(BMP180_t * const pCpnt, const BMP180_meas meas)
{
	uBMP180_REG__MEAS_CTRL	CTRL = { 0 };
	FctERR					err;

	if (	(meas != BMP180__MEAS_PRESSURE)
		&&	(meas != BMP180__MEAS_TEMPERATURE))	{ return ERROR_VALUE; }	// Unknown conversion

	CTRL.Bits.SCO = 1;
	CTRL.Bits.MEAS_CTRL = meas;

	if (meas == BMP180__MEAS_PRESSURE)	{ CTRL.Bits.OSS = pCpnt->cfg.OSS; }

	err = BMP180_Write(pCpnt->cfg.slave_inst, (uint8_t *) &CTRL.Byte, BMP180__CTRL_MEAS, 1);
	pCpnt->hStartConversion = HAL_GetTick();

	return err;
}


FctERR NONNULL__ BMP180_Get_Temperature_Raw(BMP180_t * const pCpnt, int32_t * tp)
{
	uint16_t	RES;
	FctERR		err;

	err = BMP180_Start_Conversion(pCpnt, BMP180__MEAS_TEMPERATURE);
	if (err)	{ return err; }

	HAL_Delay(5);

	err = BMP180_Read_Word(pCpnt->cfg.slave_inst, &RES, BMP180__OUT_MSB);
	if (err)	{ return err; }

	*tp = RES;
	return err;
}


FctERR NONNULL__ BMP180_Get_Pressure_Raw(BMP180_t * const pCpnt, int32_t * pr)
{
	uint8_t		RES[3];
	FctERR		err;

	err = BMP180_Start_Conversion(pCpnt, BMP180__MEAS_PRESSURE);
	if (err)	{ return err; }

	HAL_Delay(BMP180_OSS_time[pCpnt->cfg.OSS]);

	err = BMP180_Read(pCpnt->cfg.slave_inst, RES, BMP180__OUT_MSB, 3);
	if (err)	{ return err; }

	*pr = (LSHIFT(RES[0], 16) + LSHIFT(RES[1], 8) + RES[2]) >> (8 - pCpnt->cfg.OSS);
	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
