/*!\file PCF8523_ex.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief PCF8523 Driver extensions code
** \details PCF8523: Real-Time Clock (RTC) and calendar
**/
/****************************************************************/
#include "PCF8523.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCF8523)
/****************************************************************/


static float PCF8523_Clock_Timer_A_B[8] = { 4096.0f, 64.0f, 1.0f, (1.0f/60.0f), (1.0f/3600.0f), 0.0f, (1.0f/3600.0f), (1.0f/3600.0f) };

static float PCF8523_Low_Pulse_Width_B[8] = { 46.875f, 62.5f, 78.125f, 93.750f, 125.0f, 156.250f, 187.5f, 218.75f };


FctERR PCF8523_Enable_Minute_Alarm(uint8_t min)
{
	uPCF8523_REG__MINUTES ALARM;
	
	ALARM.Bits.AEN_M = 1;
	ALARM.Bits.MINUTES = min;
	return PCF8523_Write(&ALARM.Byte, PCF8523__MINUTE_ALARM, 1);
}

FctERR PCF8523_Enable_Hour_Alarm(uint8_t hour)
{
	uPCF8523_REG__HOURS ALARM;
	
	ALARM.Bits.TWENTYFOUR.AEN_H = 1;
	ALARM.Bits.TWENTYFOUR.HOURS = hour;
	return PCF8523_Write(&ALARM.Byte, PCF8523__HOUR_ALARM, 1);
}

FctERR PCF8523_Enable_Day_Alarm(uint8_t day)
{
	uPCF8523_REG__DAYS ALARM;
	
	ALARM.Bits.AEN_D = 1;
	ALARM.Bits.DAYS = day;
	return PCF8523_Write(&ALARM.Byte, PCF8523__DAY_ALARM, 1);
}

FctERR PCF8523_Enable_Weekday_Alarm(uint8_t weekday)
{
	uPCF8523_REG__WEEKDAYS ALARM;
	
	ALARM.Bits.AEN_W = 1;
	ALARM.Bits.WEEKDAYS = weekday;
	return PCF8523_Write(&ALARM.Byte, PCF8523__WEEKDAY_ALARM, 1);
}


FctERR PCF8523_Set_12_24(bool twelve)
{
	uPCF8523_REG__CTRL1	CTRL1;
	FctERR				err;
	
	err = PCF8523_Read(&CTRL1.Byte, PCF8523__CONTROL_1, 1);
	if (err)	{ return err; }
	
	CTRL1.Bits.H12_24 = twelve;
	return PCF8523_Write(&CTRL1.Byte, PCF8523__CONTROL_1, 1);
}


FctERR PCF8523_Get_Countdown(uint8_t * ctdw, uint16_t period, uint8_t timer)
{
	uint16_t	T_x;
	float		Clk_Timer = PCF8523_Clock_Timer_A_B[(timer ? PCF8523.cfg.Src_Clock_B : PCF8523.cfg.Src_Clock_A)];
	
	if (timer > 1)	{ return ERR_VALUE; }	// Unknown timer
	
	T_x = period * Clk_Timer;
	
	if (T_x > 0xFF)	{ return ERR_RANGE; }	// Period too high
	
	*ctdw = (uint8_t) T_x;
	return ERR_OK;
}


FctERR PCF8523_Check_Clock_Integrity(bool * integrity)
{
	uPCF8523_REG__SECONDS	SEC;
	FctERR					err;

	err = PCF8523_Read(&SEC.Byte, PCF8523__SECONDS, 1);
	if (err)	{ return err; }

	*integrity = SEC.Bits.OS;
	return ERR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
