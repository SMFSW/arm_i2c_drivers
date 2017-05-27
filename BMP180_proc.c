/*!\file BMP180_proc.c
** \author SMFSW
** \version v0.2
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief BMP180 Driver procedures code
** \details BMP180: Digital pressure sensor
**/
/****************************************************************/
#include "BMP180.h"
#include "globals.h"

//#if defined(HAL_IWDG_MODULE_ENABLED)
//#include "iwdg.h"
//#endif

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_BMP180)
/****************************************************************/
// std libs
#include <math.h>
#include <string.h>
/****************************************************************/


BMP180_proc BMP180 = { 0.0f, 0.0f, 0.0f, 0.0f, 0, { BMP180__OSS_8_TIME, { 408, -72, -14383, 32741, 32757, 23153, 6190, 4, -32768, -8711, 2868 }, 0 } };

extern uint8_t BMP180_OSS_time[4];


/****************************************************************/


FctERR BMP180_Init_Sequence(void)
{
	FctERR	err;

	err = BMP180_Get_ChipID(&BMP180.cfg.Id);
	if (err)								{ return err; }
	if (BMP180.cfg.Id != BMP180_CHIP_ID)	{ return ERR_COMMON; }	// Unknown device

	err = BMP180_Get_Calibration(&BMP180.cfg.Calib);
	if (err)								{ return err; }

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
	int32_t X1 = (UT - (int32_t) BMP180.cfg.Calib.AC6) * ((int32_t) BMP180.cfg.Calib.AC5) / 0x8000;
	int32_t X2 = ((int32_t) BMP180.cfg.Calib.MC * 0x800) / (X1 + (int32_t) BMP180.cfg.Calib.MD);
	return X1 + X2;
}


FctERR BMP180_Get_Pressure(float * pres)
{
	int32_t		UT = 0, UP = 0, compp = 0;
	int32_t		x1, x2, b5, b6, x3, b3, p;
	uint32_t	b4, b7;
	float		t;
	FctERR		err;

	/* Get the raw pressure and temperature values */
	err = BMP180_Get_Temperature_Raw((uint32_t *) &UT);
	if (err)	{ return err; }
	err = BMP180_Get_Pressure_Raw((uint32_t *) &UP);
	if (err)	{ return err; }

	/* Temperature compensation */
	b5 = computeB5(UT);

	/* Pressure compensation */
	b6 = b5 - 4000;
	x1 = (BMP180.cfg.Calib.B2 * ((b6 * b6) / 0x1000)) / 0x800;
	x2 = (BMP180.cfg.Calib.AC2 * b6) / 0x800;
	x3 = x1 + x2;
	b3 = (((((int32_t) BMP180.cfg.Calib.AC1) * 4 + x3) << BMP180.cfg.OSS) + 2) / 4;
	x1 = (BMP180.cfg.Calib.AC3 * b6) / 0x2000;
	x2 = (BMP180.cfg.Calib.B1 * ((b6 * b6) / 0x1000)) / 0x10000;
	x3 = ((x1 + x2) + 2) / 4;
	b4 = (BMP180.cfg.Calib.AC4 * (uint32_t) (x3 + 32768)) / 0x8000;
	b7 = ((uint32_t) (UP - b3) * (50000 >> BMP180.cfg.OSS));

	if (b7 < 0x80000000)	{ p = (b7 << 1) / b4; }
	else					{ p = (b7 / b4) << 1; }

	x1 = (p / 0x100) * (p / 0x100);
	x1 = (x1 * 3038) / 0x10000;
	x2 = (-7357 * p) / 0x10000;
	compp = p + ((x1 + x2 + 3791) / 16);

	/* Assign compensated pressure value */
	BMP180.Pressure = compp / 100.0f;	// From kPa to hPa

	t = (b5 + 8.0f) / 16.0f;
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
	FctERR	err;

	err = BMP180_Get_Temperature_Raw((uint32_t *) &UT);
	if (err)	{ return err; }

	b5 = computeB5(UT);
	t = (b5 + 8) / 16;

	BMP180.Temperature = t / 10;	// temperature given in 0.1°C (thus divide by 10 to get °C)

	if (temp)	{ *temp = BMP180.Temperature; }

	return err;
}


FctERR BMP180_handler(void)
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
