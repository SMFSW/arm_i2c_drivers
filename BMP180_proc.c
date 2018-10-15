/*!\file BMP180_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief BMP180 Driver procedures
** \details BMP180: Digital pressure sensor
**/
/****************************************************************/
#include "BMP180.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_BMP180)
/****************************************************************/
// std libs
#include <math.h>
/****************************************************************/

//#define BMP180_TST	//!< Defined to check calculations with datasheet

BMP180_t BMP180[I2C_BMP180_NB];

extern uint8_t BMP180_OSS_time[4];


/****************************************************************/


__WEAK FctERR NONNULL__ BMP180_Init_Sequence(BMP180_t * pCpnt)
{
	FctERR	err;

	pCpnt->cfg.OSS = BMP180__OSS_8_TIME;

	err = BMP180_Get_ChipID(pCpnt, &pCpnt->cfg.ID);
	if (err)								{ return err; }
	if (pCpnt->cfg.ID != BMP180_CHIP_ID)	{ return ERROR_COMMON; }	// Unknown device

	#if !defined(BMP180_TST)
		err = BMP180_Get_Calibration(pCpnt, &pCpnt->cfg.Calib);
	#else
		pCpnt->cfg.OSS = BMP180__OSS_1_TIME;
		pCpnt->cfg.Calib.AC1 = 408;
		pCpnt->cfg.Calib.AC2 = -72;
		pCpnt->cfg.Calib.AC3 = -14383;
		pCpnt->cfg.Calib.AC4 = 32741;
		pCpnt->cfg.Calib.AC5 = 32757;
		pCpnt->cfg.Calib.AC6 = 23153;
		pCpnt->cfg.Calib.B1 = 6190;
		pCpnt->cfg.Calib.B2 = 4;
		pCpnt->cfg.Calib.MB = -32768;
		pCpnt->cfg.Calib.MC = -8711;
		pCpnt->cfg.Calib.MD = 2868;
	#endif

	return err;
}


/****************************************************************/


/**!\brief Calculates the altitude (in meters) from the specified atmospheric
**	pressure (in hPa), and sea-level pressure (in hPa).
**	\param[in] pressure - Atmospheric pressure in hPa
**/
__STATIC_INLINE float INLINE__ BMP180_Pressure_To_Altitude(const float pressure)
{
	// Equation from BMP180 datasheet from page 16
	return (44330.0f * (1.0f - pow(pressure / SEA_LEVEL_PRESSURE, (1.0f / 5.255f))));
}


/**!\brief Calculates the pressure at sea level (in hPa) from the specified altitude
**	(in meters), and atmospheric pressure (in hPa).
**	\param[in] altitude - Altitude in meters
**	\param[in] pressure - Atmospheric pressure in hPa
**/
__STATIC_INLINE float INLINE__ BMP180_seaLevel_Pressure_For_Altitude(const float altitude, const float pressure)
{
	// Equation from BMP180 datasheet from page 17
	return (pressure / pow(1.0f - (altitude / 44330.0f), 5.255f));
}


/****************************************************************/


FctERR NONNULL__ BMP180_Set_Oversampling(BMP180_t * pCpnt, const BMP180_oversampling oss)
{
	if (oss > BMP180__OSS_8_TIME)	{ return ERROR_VALUE; }	// Unknown Oversampling

	pCpnt->cfg.OSS = oss;
	return ERROR_OK;
}


FctERR NONNULL__ BMP180_Get_Calibration(BMP180_t * pCpnt, BMP180_calib * calib)
{
	int16_t *	addr = (int16_t *) calib;
	FctERR		err = ERROR_OK;

	for (int i = 0 ; i < SZ_OBJ(BMP180_calib, int16_t) ; i++)
	{
		err = BMP180_Read_Word(pCpnt->cfg.slave_inst, (uint16_t *) addr++, BMP180__CALIB_AC1_MSB + (i * 2));
		if (err)	{ return err; }
	}

	return err;
}


/**!\brief  Compute B5 coefficient used in temperature & pressure calculations.
** \param[in] pCpnt - Pointer to BMP180 component
** \param[in] UT - UT value (temperature data)
**/
static int32_t NONNULL__ computeB5(const BMP180_t * pCpnt, const int32_t UT)
{
	int32_t X1 = (UT - pCpnt->cfg.Calib.AC6) * pCpnt->cfg.Calib.AC5 / 32768;
	int32_t X2 = pCpnt->cfg.Calib.MC * 2048 / (X1 + pCpnt->cfg.Calib.MD);
	return X1 + X2;
}


FctERR NONNULLX__(1) BMP180_Get_Pressure(BMP180_t * pCpnt, float * pres)
{
	int32_t		UT = 0, UP = 0, compp = 0;
	int32_t		x1, x2, b5, b6, x3, b3, p;
	uint32_t	b4, b7;
	float		t;
	FctERR		err = ERROR_OK;

	/* Get the raw pressure and temperature values */
	#if !defined(BMP180_TST)
		err = BMP180_Get_Temperature_Raw(pCpnt, &UT);
		if (err)	{ return err; }
		err = BMP180_Get_Pressure_Raw(pCpnt, &UP);
		if (err)	{ return err; }
	#else
		UT = 27898;		//!< For test purposes
		UP = 23843;		//!< For test purposes
	#endif

	/* Temperature compensation */
	b5 = computeB5(pCpnt, UT);

	/* Pressure compensation */
	b6 = b5 - 4000;
	x1 = (pCpnt->cfg.Calib.B2 * ((b6 * b6) / 4096)) / 2048;
	x2 = pCpnt->cfg.Calib.AC2 * b6 / 2048;
	x3 = x1 + x2;
	b3 = ((((pCpnt->cfg.Calib.AC1 * 4) + x3) << pCpnt->cfg.OSS) + 2) / 4;
	x1 = (pCpnt->cfg.Calib.AC3 * b6) / 8192;
	x2 = (pCpnt->cfg.Calib.B1 * ((b6 * b6) / 4096)) / 65536;
	x3 = ((x1 + x2) + 2) / 4;
	b4 = pCpnt->cfg.Calib.AC4 * (x3 + 32768) / 32768;
	b7 = (UP - b3) * (50000 >> pCpnt->cfg.OSS);

	if (b7 < 0x80000000)	{ p = (b7 * 2) / b4; }
	else					{ p = (b7 / b4) * 2; }

	x1 = p / 256;
	x1 *= x1;
	x1 = (x1 * 3038) / 65536;
	x2 = (-7357 * p) / 65536;
	compp = p + ((x1 + x2 + 3791) / 16);

	/* Assign compensated pressure value */
	pCpnt->Pressure = compp / 100;	// From kPa to hPa

	t = ((float) b5 + 8) / 16;
	pCpnt->Temperature = t / 10;	// temperature given in 0.1°C (thus divide by 10 to get °C)

	pCpnt->Altitude = BMP180_Pressure_To_Altitude(pCpnt->Pressure);
	pCpnt->SeaLevelPressure = BMP180_seaLevel_Pressure_For_Altitude(pCpnt->Altitude, pCpnt->Pressure);

	if (pres)	{ *pres = pCpnt->Pressure; }

	return err;
}


FctERR NONNULLX__(1) BMP180_Get_Temperature(BMP180_t * pCpnt, float * temp)
{
	int32_t	UT = 0, b5;
	float	t;
	FctERR	err = ERROR_OK;

	#if !defined(BMP180_TST)
		err = BMP180_Get_Temperature_Raw(pCpnt, &UT);
		if (err)	{ return err; }
	#else
		UT = 27898;		//!< For test purposes
	#endif

	b5 = computeB5(pCpnt, UT);
	t = ((float) b5 + 8) / 16;

	pCpnt->Temperature = t / 10;	// temperature given in 0.1°C (thus divide by 10 to get °C)

	if (temp)	{ *temp = pCpnt->Temperature; }

	return err;
}


__WEAK FctERR NONNULL__ BMP180_handler(BMP180_t * pCpnt)
{
	FctERR	err;

	//err = BMP180_Get_Temperature(pCpnt, NULL);
	//if (err)	{ return err; }

	err = BMP180_Get_Pressure(pCpnt, NULL);
	if (err)	{ return err; }

	#if defined(VERBOSE)
		printf("BMP180: Pressure %ldhPa, Temperature %d.%02d°C, Alt %ldm, Pressure at sea level %ldhPa\r\n",
				(int32_t) pCpnt->Pressure, (int16_t) pCpnt->Temperature, get_fp_dec(pCpnt->Temperature, 2), (int32_t) pCpnt->Altitude, (int32_t) pCpnt->SeaLevelPressure);
	#endif

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
