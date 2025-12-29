/*!\file DS_GPMS_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief DS-GPM.S Driver procedures
** \details DS-GPM.S: 99 Channel Positioning System (GPS + GLONASS) Shield
**/
/****************************************************************/
#include "DS_GPMS.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_GPMS)
/****************************************************************/


GPMS_t GPMS = { 0 };


/****************************************************************/


__WEAK FctERR GPMS_Init_Sequence(void)
{
	GPMS.cfg.Mode = GPMS__MODE_AUTONOMOUS;

	return ERROR_OK;
}


/****************************************************************/


FctERR GPMS_Get_Date(GPMS_date * const date)
{
	uint8_t	DATE[8];
	FctERR	err;

	err = GPMS_Read(DATE, GPMS__DAY_OF_MONTH_TENS, sizeof(DATE));
	if (err != ERROR_OK)	{ goto ret; }

	GPMS.Date.Day = (DATE[0] * 10U) + DATE[1];
	GPMS.Date.Month = (DATE[2] * 10U) + DATE[3];
	GPMS.Date.Year = (DATE[4] * 1000U) + (DATE[5] * 100U) + (DATE[6] * 10U) + DATE[7];

	if (date != NULL)	{ UNUSED_RET memcpy(date, &GPMS.Date, sizeof(GPMS_date)); }

	ret:
	return err;
}

FctERR GPMS_Get_Hour(GPMS_hour * const hour)
{
	uint8_t	HOUR[6];
	FctERR	err;

	err = GPMS_Read(HOUR, GPMS__HOURS_TENS, sizeof(HOUR));
	if (err != ERROR_OK)	{ goto ret; }

	GPMS.Hour.Hours = (HOUR[0] * 10U) + HOUR[1];
	GPMS.Hour.Minutes = (HOUR[2] * 10U) + HOUR[3];
	GPMS.Hour.Seconds = (HOUR[4] * 10U) + HOUR[5];

	if (hour != NULL)	{ UNUSED_RET memcpy(hour, &GPMS.Hour, sizeof(GPMS_hour)); }

	ret:
	return err;
}


/*!\brief Function to fill coords for latitude and longitude
** \param[in,out] coords - pointer to coordinates data to fill
** \param[in] raw - pointer to raw datas read from sensor
** \return Nothing
**/
static void NONNULL__ GPMS_Fill_coords(GPMS_coord * const coord, const uint8_t * raw)
{
	coord->Degrees = (raw[0] * 10U) + raw[1];
	coord->Minutes = (raw[2] * 10.0f) + raw[3] + (raw[4] * 0.1f) + (raw[5] * 0.01f) + (raw[6] * 0.001f) + (raw[7] * 0.0001f);
	coord->Direction = raw[8];
}

/*!\brief Function to fill speed or heading from raw values
** \param[in,out] out - pointer to speed or heading data to fill
** \param[in] raw - pointer to raw datas read from sensor
** \return Nothing
**/
static void NONNULL__ GPMS_Fill_sp_hd(float * const out, const uint8_t * raw)
{
	*out = (raw[0] * 100.0f) + (raw[1] * 10.0f) + raw[2] + (raw[3] * 0.1f);
}


FctERR GPMS_Get_Latitude(GPMS_coord * const lat)
{
	uint8_t	LAT[9];
	FctERR	err;

	err = GPMS_Read(LAT, GPMS__LATITUDE_DEGREES_TENS, sizeof(LAT));
	if (err != ERROR_OK)	{ goto ret; }

	GPMS_Fill_coords(&GPMS.Latitude, LAT);

	if (lat != NULL)	{ UNUSED_RET memcpy(lat, &GPMS.Latitude, sizeof(GPMS_coord)); }

	ret:
	return err;
}


FctERR GPMS_Get_Longitude(GPMS_coord * const lon)
{
	uint8_t	LON[9];
	FctERR	err;

	err = GPMS_Read(LON, GPMS__LONGITUDE_DEGREES_TENS, sizeof(LON));
	if (err != ERROR_OK)	{ goto ret; }

	GPMS_Fill_coords(&GPMS.Longitude, LON);

	if (lon != NULL)	{ UNUSED_RET memcpy(lon, &GPMS.Longitude, sizeof(GPMS_coord)); }

	ret:
	return err;
}


FctERR GPMS_Get_Heading(float * const heading, const GPMS_north type)
{
	uint8_t	HEADING[4];
	FctERR	err = ERROR_OK;

	if (type > GPMS__MAGNETIC_NORTH)	{ err = ERROR_VALUE; }	// Unknown north type
	if (err != ERROR_OK)				{ goto ret; }

	err = GPMS_Read(HEADING, (type == GPMS__MAGNETIC_NORTH) ? GPMS__HEADING_DEGREES_MAGNETIC_HUNDREDS : GPMS__HEADING_DEGREES_TRUE_HUNDREDS, sizeof(HEADING));
	if (err != ERROR_OK)	{ goto ret; }

	GPMS_Fill_sp_hd((type == GPMS__MAGNETIC_NORTH) ? &GPMS.Heading_Magnetic : &GPMS.Heading_True, HEADING);

	if (heading != NULL)	{ *heading = (type == GPMS__MAGNETIC_NORTH) ? GPMS.Heading_Magnetic : GPMS.Heading_True; }

	ret:
	return err;
}


FctERR GPMS_Get_Speed(float * const speed)
{
	uint8_t	SPEED[4];
	FctERR	err;

	err = GPMS_Read(SPEED, GPMS__SPEED_HUNDREDS, sizeof(SPEED));
	if (err != ERROR_OK)	{ goto ret; }

	GPMS_Fill_sp_hd(&GPMS.Speed, SPEED);

	if (speed != NULL)	{ *speed = GPMS.Speed; }

	ret:
	return err;
}


FctERR GPMS_Get_Altitude(uint16_t * const altitude)
{
	uint8_t	ALT[5];
	FctERR	err;

	err = GPMS_Read(ALT, GPMS__ALTITUDE_TENS_OF_THOUSANDS, sizeof(ALT));
	if (err != ERROR_OK)	{ goto ret; }

	GPMS.Altitude = (ALT[0] * 10000U) + (ALT[1] * 1000U) + (ALT[2] * 100U) + (ALT[3] * 10U) + ALT[4];

	if (altitude)	{ *altitude = GPMS.Altitude; }

	ret:
	return err;
}


__WEAK FctERR GPMS_handler(void)
{
	FctERR	err;

	err = GPMS_Get_Date(0);
	if (err != ERROR_OK)	{ goto ret; }
	HAL_Delay(1);
	err = GPMS_Get_Hour(0);
	if (err != ERROR_OK)	{ goto ret; }
	HAL_Delay(1);
	err = GPMS_Get_Latitude(0);
	if (err != ERROR_OK)	{ goto ret; }
	HAL_Delay(1);
	err = GPMS_Get_Longitude(0);
	if (err != ERROR_OK)	{ goto ret; }
	HAL_Delay(1);
	err = GPMS_Get_Heading(0, GPMS__TRUE_NORTH);
	if (err != ERROR_OK)	{ goto ret; }
	HAL_Delay(1);
	err = GPMS_Get_Heading(0, GPMS__MAGNETIC_NORTH);
	if (err != ERROR_OK)	{ goto ret; }
	HAL_Delay(1);
	err = GPMS_Get_Speed(0);
	if (err != ERROR_OK)	{ goto ret; }
	HAL_Delay(1);
	err = GPMS_Get_Altitude(0);
	if (err != ERROR_OK)	{ goto ret; }

	#if defined(VERBOSE)
		UNUSED_RET printf("GPMS: %d/%d/%d %d:%d:%d\r\n", GPMS.Date.Year, GPMS.Date.Month, GPMS.Date.Day, GPMS.Hour.Hours, GPMS.Hour.Minutes, GPMS.Hour.Seconds);
		UNUSED_RET printf("Lat: %d°%d.%04ld\'%c, Lon: %d°%d.%04ld\'%c\r\n",
							GPMS.Latitude.Degrees, (uint16_t) GPMS.Latitude.Minutes, get_fp_dec(GPMS.Latitude.Minutes, 4), GPMS.Latitude.Direction,
							GPMS.Longitude.Degrees, (uint16_t) GPMS.Longitude.Minutes, get_fp_dec(GPMS.Longitude.Minutes, 4), GPMS.Longitude.Direction);
		UNUSED_RET printf("Heading: True %d.%ld Mag %d.%ld, Speed: %d.%ldkm/h, Altitude: %dm\r\n",
							(uint16_t) GPMS.Heading_True, get_fp_dec(GPMS.Heading_True, 1U),
							(uint16_t) GPMS.Heading_Magnetic, get_fp_dec(GPMS.Heading_Magnetic, 1U),
							(uint16_t) GPMS.Speed, get_fp_dec(GPMS.Speed, 1U),
							GPMS.Altitude);
	#endif

	ret:
	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
