/*!\file PCF8523_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief PCF8523 Driver extensions
** \details PCF8523: Real-Time Clock (RTC) and calendar
**/
/****************************************************************/
#include "PCF8523.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCF8523)
/****************************************************************/


static float PCF8523_Clock_Timer_A_B[8] = { 4096.0f, 64.0f, 1.0f, (1.0f/60.0f), (1.0f/3600.0f), 0.0f, (1.0f/3600.0f), (1.0f/3600.0f) };

static float PCF8523_Low_Pulse_Width_B[8] = { 46.875f, 62.5f, 78.125f, 93.750f, 125.0f, 156.250f, 187.5f, 218.75f };


FctERR PCF8523_Enable_Minute_Alarm(const uint8_t min)
{
	uPCF8523_REG__MINUTES ALARM;

	ALARM.Bits.AEN_M = 1;
	ALARM.Bits.MINUTES = min;
	return PCF8523_Write(&ALARM.Byte, PCF8523__MINUTE_ALARM, 1);
}

FctERR PCF8523_Enable_Hour_Alarm(const uint8_t hour)
{
	uPCF8523_REG__HOURS ALARM;

	ALARM.Bits.TWENTYFOUR.AEN_H = 1;
	ALARM.Bits.TWENTYFOUR.HOURS = hour;
	return PCF8523_Write(&ALARM.Byte, PCF8523__HOUR_ALARM, 1);
}

FctERR PCF8523_Enable_Day_Alarm(const uint8_t day)
{
	uPCF8523_REG__DAYS ALARM;

	ALARM.Bits.AEN_D = 1;
	ALARM.Bits.DAYS = day;
	return PCF8523_Write(&ALARM.Byte, PCF8523__DAY_ALARM, 1);
}

FctERR PCF8523_Enable_Weekday_Alarm(const uint8_t weekday)
{
	uPCF8523_REG__WEEKDAYS ALARM;

	ALARM.Bits.AEN_W = 1;
	ALARM.Bits.WEEKDAYS = weekday;
	return PCF8523_Write(&ALARM.Byte, PCF8523__WEEKDAY_ALARM, 1);
}


FctERR PCF8523_Set_12_24(const bool twelve)
{
	uPCF8523_REG__CTRL1	CTRL1;
	FctERR				err;

	err = PCF8523_Read(&CTRL1.Byte, PCF8523__CONTROL_1, 1);
	if (err)	{ return err; }

	CTRL1.Bits.H12_24 = twelve;
	return PCF8523_Write(&CTRL1.Byte, PCF8523__CONTROL_1, 1);
}


FctERR NONNULL__ PCF8523_Get_Countdown(uint8_t * ctdw, const uint16_t period, const uint8_t timer)
{
	uint16_t	T_x;
	float		Clk_Timer = PCF8523_Clock_Timer_A_B[(timer ? PCF8523.cfg.Src_Clock_B : PCF8523.cfg.Src_Clock_A)];

	if (timer > 1)	{ return ERROR_VALUE; }	// Unknown timer

	T_x = period * Clk_Timer;

	if (T_x > 0xFF)	{ return ERROR_RANGE; }	// Period too high

	*ctdw = (uint8_t) T_x;
	return ERROR_OK;
}


FctERR NONNULL__ PCF8523_Check_Clock_Integrity(bool * integrity)
{
	uPCF8523_REG__SECONDS	SEC;
	FctERR					err;

	err = PCF8523_Read(&SEC.Byte, PCF8523__SECONDS, 1);
	if (err)	{ return err; }

	*integrity = SEC.Bits.OS;
	return ERROR_OK;
}


/****************************************************************/


__WEAK FctERR NONNULL__ PCF8523_INT1_GPIO_Init(GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	return I2C_peripheral_GPIO_init(&PCF8523.cfg.INT1_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK FctERR NONNULL__ PCF8523_INT1_GPIO_Get(bool * const pState) {
	return I2C_peripheral_GPIO_get(&PCF8523.cfg.INT1_GPIO, pState); }


__WEAK FctERR NONNULL__ PCF8523_INT2_GPIO_Init(GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	return I2C_peripheral_GPIO_init(&PCF8523.cfg.INT2_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK FctERR NONNULL__ PCF8523_INT2_GPIO_Get(bool * const pState) {
	return I2C_peripheral_GPIO_get(&PCF8523.cfg.INT2_GPIO, pState); }


/****************************************************************/
#endif
#endif
/****************************************************************/
