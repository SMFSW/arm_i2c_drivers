/*!\file BMP180_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief BMP180 Driver extensions
** \details BMP180: Digital pressure sensor
**/
/****************************************************************/
#include "BMP180.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_BMP180)
/****************************************************************/


static uint8_t BMP180_OSS_time[4] = { 5U, 8U, 14U, 26U };	//!< BMP180 Oversampling values with +1.5ms than given in datasheet


FctERR NONNULL__ BMP180_Start_Conversion(BMP180_t * const pCpnt, const BMP180_meas meas)
{
	uBMP180_REG__MEAS_CTRL	CTRL = { 0 };
	FctERR					err = ERROR_OK;

	if (	(meas != BMP180__MEAS_PRESSURE)
		&&	(meas != BMP180__MEAS_TEMPERATURE))	{ err = ERROR_VALUE; }	// Unknown conversion
	if (err != ERROR_OK)						{ goto ret; }

	CTRL.Bits.SCO = 1U;
	CTRL.Bits.MEAS_CTRL = meas;

	if (meas == BMP180__MEAS_PRESSURE)	{ CTRL.Bits.OSS = pCpnt->cfg.OSS; }

	err = BMP180_Write(pCpnt->cfg.slave_inst, (uint8_t *) &CTRL.Byte, BMP180__CTRL_MEAS, 1U);
	pCpnt->hStartConversion = HAL_GetTick();

	ret:
	return err;
}


FctERR NONNULL__ BMP180_Get_Temperature_Raw(BMP180_t * const pCpnt, int32_t * tp)
{
	uint16_t	RES;
	FctERR		err;

	err = BMP180_Start_Conversion(pCpnt, BMP180__MEAS_TEMPERATURE);
	if (err != ERROR_OK)	{ goto ret; }

	HAL_Delay(5);

	err = BMP180_Read_Word(pCpnt->cfg.slave_inst, &RES, BMP180__OUT_MSB);
	if (err != ERROR_OK)	{ goto ret; }

	*tp = RES;

	ret:
	return err;
}


FctERR NONNULL__ BMP180_Get_Pressure_Raw(BMP180_t * const pCpnt, int32_t * pr)
{
	uint8_t		RES[3];
	FctERR		err;

	err = BMP180_Start_Conversion(pCpnt, BMP180__MEAS_PRESSURE);
	if (err != ERROR_OK)	{ goto ret; }

	HAL_Delay(BMP180_OSS_time[pCpnt->cfg.OSS]);

	err = BMP180_Read(pCpnt->cfg.slave_inst, RES, BMP180__OUT_MSB, 3U);
	if (err != ERROR_OK)	{ goto ret; }

	*pr = (LSHIFT(RES[0], 16U) + LSHIFT(RES[1], 8U) + RES[2]) >> (8U - pCpnt->cfg.OSS);

	ret:
	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
