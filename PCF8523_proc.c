/*!\file PCF8523_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief PCF8523 Driver procedures
** \details PCF8523: Real-Time Clock (RTC) and calendar
**/
/****************************************************************/
#include "PCF8523.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCF8523)
/****************************************************************/


PCF8523_t PCF8523 = { { 0, 0, 0, 0}, { 0, 0, 0}, { &PCF8523_hal, 0, 0, false } };


/****************************************************************/


__WEAK FctERR PCF8523_Init_Sequence(void)
{
	FctERR err = ERROR_OK;

	err = PCF8523_Get_Date(0, false);
	if (err)	{ return err; }
	err = PCF8523_Get_Time(0, false);
	if (err)	{ return err; }

	return ERROR_OK;
}


/****************************************************************/


/*!\brief Converts uint8_t from hexadecimal to BCD
**
** \param [in,out] bcd - pointer to BCD variable
** \param [in] hex - Hexadecimal value to convert
** \return FctERR - error code
**/
static FctERR hex2bcd(uint8_t * bcd, const uint8_t hex)
{
	*bcd = (uint8_t) (LSHIFT((hex / 10), 4) | (hex % 10));
	return ERROR_OK;
}

/*!\brief Converts uint8_t from BCD to hexadecimal
**
** \param [in,out] hex - pointer to hexadecimal variable
** \param [in] bcd - BCD value to convert
** \return FctERR - error code
**/
static FctERR bcd2hex(uint8_t * hex, const uint8_t bcd)
{
	uint8_t	ms = RSHIFT(bcd & 0xF0, 4);
	uint8_t	ls = bcd & 0x0F;

	if ((ms > 9) || (ls > 9))	{ return ERROR_VALUE; }

	*hex = (uint8_t) ((ms * 10) + ls);
	return ERROR_OK;
}


/****************************************************************/


FctERR PCF8523_Set_Date(const PCF8523_date date, const bool BCD)
{
	uint8_t	DATE[4];
	FctERR	err;

	if (!BCD)
	{
		// Consistency check
		if ((date.Day > 31) || (date.Weekday > PCF8523__SATURDAY) || (date.Month > 12) || (date.Year - 2000 > 99))	{ return ERROR_RANGE; }	// Date outside range

		// Conversion to BCD for PCF8523
		hex2bcd(&DATE[0], date.Day);
		DATE[1] = date.Weekday;
		hex2bcd(&DATE[2], date.Month);
		hex2bcd(&DATE[3], date.Year - 2000);
	}
	else
	{
		// Conversion to hexadecimal for consistency check
		err = bcd2hex(&DATE[0], date.Day);
		if (err)	{ return err; }
		err = bcd2hex(&DATE[2], date.Month);
		if (err)	{ return err; }
		err = bcd2hex(&DATE[3], date.Year - 0x2000);
		if (err)	{ return err; }

		if ((DATE[0] > 31) || (date.Weekday > PCF8523__SATURDAY) || (DATE[2] > 12) || (DATE[3] > 99))	{ return ERROR_RANGE; }	// Time outside range

		// Year is uint16_t and shall already be between 0 & 99
		memcpy(DATE, (uint8_t *) &date, 3);
		DATE[3] = date.Year - 0x2000;	// Sub year 2000 in BCD
	}

	return PCF8523_Set_Date_Raw(DATE);
}


FctERR PCF8523_Set_Time(const PCF8523_time time, const bool BCD)
{
	uint8_t	TIME[3];
	FctERR	err;

	if (!BCD)
	{
		if ((time.Seconds >= 60) || (time.Minutes >= 60) || (time.Hours >= 24))		{ return ERROR_RANGE; }	// Time outside range

		// Conversion to BCD for PCF8523
		hex2bcd(&TIME[0], time.Seconds);
		hex2bcd(&TIME[1], time.Minutes);

		if (PCF8523.cfg.Hour_Format)	{ /* TODO: handle 12 hours mode too */ }
		else							{ hex2bcd(&TIME[2], time.Hours); }
	}
	else
	{
		// Conversion to hexadecimal for consistency check
		err = bcd2hex(&TIME[0], time.Seconds);
		if (err)	{ return err; }
		err = bcd2hex(&TIME[1], time.Minutes);
		if (err)	{ return err; }

		if (PCF8523.cfg.Hour_Format)	{ /* TODO: handle 12 hours mode too */ }
		else							{ err = bcd2hex(&TIME[2], time.Hours); }
		if (err)	{ return err; }

		if ((TIME[0] >= 60) || (TIME[1] >= 60) || (TIME[2] >= 24))	{ return ERROR_RANGE; }	// Time outside range

		memcpy(TIME, (uint8_t *) &time, sizeof(PCF8523_time));
	}

	return PCF8523_Set_Time_Raw(TIME);
}



FctERR PCF8523_Get_Date(PCF8523_date * date, const bool BCD)
{
	uint8_t		DATE[4];
	uint8_t		year = 0;
	FctERR		err;

	err = PCF8523_Get_Date_Raw((uint8_t *) &DATE);
	if (err)	{ return err; }

	err = bcd2hex(&PCF8523.date.Day, DATE[0]);
	PCF8523.date.Weekday = DATE[1];
	err |= bcd2hex(&PCF8523.date.Month, DATE[2]);
	err |= bcd2hex(&year, DATE[3]);
	PCF8523.date.Year = 2000 + year;	// Add year 2000

	if (date)
	{
		if (BCD)
		{
			// Year is uint16_t and shall be between 0 & 99
			memcpy(date, DATE, 3);
			date->Year = DATE[3];
			date->Year += 0x2000;	// Add year 2000 in BCD
		}
		else
		{
			memcpy(date, &PCF8523.date, sizeof(PCF8523_date));
		}
	}

	return err;
}


FctERR PCF8523_Get_Time(PCF8523_time * time, const bool BCD)
{
	PCF8523_time	TIME;
	FctERR			err;

	err = PCF8523_Get_Time_Raw((uint8_t *) &TIME);
	if (err)	{ return err; }

	TIME.Seconds &= 0x7F;	// Remove OS bit from Seconds
	TIME.Minutes &= 0x7F;	// Remove OS bit from Minutes
	TIME.Hours &= 0x7F;		// Remove OS bit from Hours
	err = bcd2hex(&PCF8523.time.Seconds, TIME.Seconds);
	err |= bcd2hex(&PCF8523.time.Minutes, TIME.Minutes);
	err |= bcd2hex(&PCF8523.time.Hours, TIME.Hours);		// TODO: make it work for 12h format too

	if (time)	{ memcpy(time, (uint8_t *) (BCD ? &TIME : &PCF8523.time), sizeof(PCF8523_time)); }

	return err;
}


__WEAK FctERR PCF8523_handler(void)
{
	FctERR	err = ERROR_OK;

	err = PCF8523_Get_Date(0, false);
	if (err)	{ return err; }
	err = PCF8523_Get_Time(0, false);
	if (err)	{ return err; }

	#if defined(VERBOSE)
		printf("Date: %d/%d/%d - Time: %d:%d:%d\r\n", PCF8523.date.Month, PCF8523.date.Day, PCF8523.date.Year, PCF8523.time.Hours, PCF8523.time.Minutes, PCF8523.time.Seconds);
	#endif

	return ERROR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
