/*!\file BMP180_proc.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief BMP180 Driver procedures code
** \details BMP180: Digital pressure sensor
**/
/****************************************************************/
#include "BMP180.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_BMP180)
/****************************************************************/
// std libs
#include <math.h>
#include <string.h>
/****************************************************************/

//#define BMP180_TST	//!< Defined to check calculations with datasheet

#if !defined(BMP180_TST)
BMP180_proc BMP180 = { 0.0f, 0.0f, 0.0f, 0.0f, 0, { BMP180__OSS_8_TIME, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0 } };
#else
BMP180_proc BMP180 = { 0.0f, 0.0f, 0.0f, 0.0f, 0, { BMP180__OSS_1_TIME, { 408, -72, -14383, 32741, 32757, 23153, 6190, 4, -32768, -8711, 2868 }, 0 } };
#endif

extern uint8_t BMP180_OSS_time[4];


/****************************************************************/


__weak FctERR BMP180_Init_Sequence(void)
{
	FctERR	err;

	err = BMP180_Get_ChipID(&BMP180.cfg.Id);
	if (err)								{ return err; }
	if (BMP180.cfg.Id != BMP180_CHIP_ID)	{ return ERR_COMMON; }	// Unknown device

	#if !defined(BMP180_TST)
		err = BMP180_Get_Calibration(&BMP180.cfg.Calib);
		if (err)							{ return err; }
	#endif

	return err;
}


/****************************************************************/


/**!\brief Calculates the altitude (in meters) from the specified atmospheric
**	pressure (in hPa), and sea-level pressure (in hPa).
**	\param[in] pressure - Atmospheric pressure in hPa
**/
__STATIC_INLINE float INLINE__ BMP180_Pressure_To_Altitude(float pressure)
{
	// Equation from BMP180 datasheet from page 16
	return (44330.0 * (1.0 - pow(pressure / SEA_LEVEL_PRESSURE, (1 / 5.255))));
}


/**!\brief Calculates the pressure at sea level (in hPa) from the specified altitude
**	(in meters), and atmospheric pressure (in hPa).
**	\param[in] altitude - Altitude in meters
**	\param[in] pressure - Atmospheric pressure in hPa
**/
__STATIC_INLINE float INLINE__ BMP180_seaLevel_Pressure_For_Altitude(float altitude, float pressure)
{
	// Equation from BMP180 datasheet from page 17
	return (pressure / pow(1.0 - (altitude / 44330.0), 5.255));
}


/****************************************************************/


FctERR BMP180_Set_Oversampling(BMP180_oversampling oss)
{
	if (oss > BMP180__OSS_8_TIME)	{ return ERR_VALUE; }	// Unknown Oversampling

	BMP180.cfg.OSS = oss;
	return ERR_OK;
}


FctERR BMP180_Get_Calibration(BMP180_calib * calib)
{
	int16_t *	adr = (int16_t *) calib;
	FctERR		err;

	for (int i = 0 ; i < 11 ; i++)
	{
		err = BMP180_Read_Word((uint16_t *) adr, BMP180__CALIB_AC1_MSB + (i * 2));
		if (err)	{ return err; }
		adr++;
	}

	return err;
}


/**!\brief  Compute B5 coefficient used in temperature & pressure calculations.
**/
static int32_t computeB5(int32_t UT)
{
	// TODO: find what's wrong here
	int32_t X1 = (UT - BMP180.cfg.Calib.AC6) * BMP180.cfg.Calib.AC5 / LSHIFT(1, 15);
	int32_t X2 = BMP180.cfg.Calib.MC * LSHIFT(1, 11) / (X1 + BMP180.cfg.Calib.MD);
	return X1 + X2;
}


FctERR BMP180_Get_Pressure(float * pres)
{
	int32_t		UT = 0, UP = 0, compp = 0;
	int32_t		x1, x2, b5, b6, x3, b3, p;
	uint32_t	b4, b7;
	float		t;
	FctERR		err = ERR_OK;

	/* Get the raw pressure and temperature values */
	#if !defined(BMP180_TST)
		err = BMP180_Get_Temperature_Raw(&UT);
		if (err)	{ return err; }
		err = BMP180_Get_Pressure_Raw(&UP);
		if (err)	{ return err; }
	#else
		UT = 27898;		//!< For test purposes
		UP = 23843;		//!< For test purposes
	#endif

	/* Temperature compensation */
	b5 = computeB5(UT);

	/* Pressure compensation */
	b6 = b5 - 4000;
	x1 = RSHIFT(BMP180.cfg.Calib.B2 * RSHIFT(b6 * b6, 12), 11);
	x2 = RSHIFT(BMP180.cfg.Calib.AC2 * b6, 11);
	x3 = x1 + x2;
	b3 = ((((BMP180.cfg.Calib.AC1 * 4) + x3) << BMP180.cfg.OSS) + 2) / 4;
	x1 = RSHIFT(BMP180.cfg.Calib.AC3 * b6, 13);
	x2 = RSHIFT(BMP180.cfg.Calib.B1 * RSHIFT(b6 * b6, 12), 16);
	x3 = RSHIFT(((x1 + x2) + 2), 2);
	b4 = BMP180.cfg.Calib.AC4 * (x3 + 32768) / LSHIFT(1, 15);
	b7 = (UP - b3) * (50000 >> BMP180.cfg.OSS);

	if (b7 < 0x80000000)	{ p = (b7 * 2) / b4; }
	else					{ p = (b7 / b4) * 2; }

	x1 = RSHIFT(p, 8);
	x1 *= x1;
	x1 = RSHIFT((x1 * 3038), 16);
	x2 = RSHIFT((-7357 * p), 16);
	compp = p + RSHIFT((x1 + x2 + 3791), 4);

	/* Assign compensated pressure value */
	BMP180.Pressure = compp / 100;	// From kPa to hPa

	t = ((float) b5 + 8) / 16;
	BMP180.Temperature = t / 10;	// temperature given in 0.1°C (thus divide by 10 to get °C)

	BMP180.Altitude = BMP180_Pressure_To_Altitude(BMP180.Pressure);
	BMP180.SeaLevelPressure = BMP180_seaLevel_Pressure_For_Altitude(BMP180.Altitude, BMP180.Pressure);

	if (pres)	{ *pres = BMP180.Pressure; }

	return err;
}


FctERR BMP180_Get_Temperature(float * temp)
{
	int32_t	UT = 0, b5;
	float	t;
	FctERR	err = ERR_OK;

	#if !defined(BMP180_TST)
		err = BMP180_Get_Temperature_Raw(&UT);
		if (err)	{ return err; }
	#else
		UT = 27898;		//!< For test purposes
	#endif

	b5 = computeB5(UT);
	t = ((float) b5 + 8) / 16;

	BMP180.Temperature = t / 10;	// temperature given in 0.1°C (thus divide by 10 to get °C)

	if (temp)	{ *temp = BMP180.Temperature; }

	return err;
}


__weak FctERR BMP180_handler(void)
{
	FctERR	err;

	//err = BMP180_Get_Temperature(0);
	//if (err)	{ return err; }

	err = BMP180_Get_Pressure(0);
	if (err)	{ return err; }

	#if defined(VERBOSE)
		printf("BMP180: Pressure %ldhPa, Temperature %d.%02d°C, Alt %ldm, Pressure at sea level %ldhPa\r\n",
				(int32_t) BMP180.Pressure, (int16_t) BMP180.Temperature, get_fp_dec(BMP180.Temperature, 2), (int32_t) BMP180.Altitude, (int32_t) BMP180.SeaLevelPressure);
	#endif

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
