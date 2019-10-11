/*!\file PCF8523_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
** \brief PCF8523 Driver extensions
** \details PCF8523: Real-Time Clock (RTC) and calendar
**/
/****************************************************************/
#ifndef __PCF8523_EX_H__
	#define __PCF8523_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "PCF8523.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\union uPCF8523_REG_MAP
** \brief Union of PCF8523 registry map
**/
typedef union uPCF8523_REG_MAP {
	uint8_t Bytes[20];
	struct {
		uPCF8523_REG__CTRL1			Control_1;
		uPCF8523_REG__CTRL2			Control_2;
		uPCF8523_REG__CTRL3			Control_3;
		uPCF8523_REG__SECONDS		Seconds;
		uPCF8523_REG__MINUTES		Minutes;
		uPCF8523_REG__HOURS			Hours;
		uPCF8523_REG__DAYS			Days;
		uPCF8523_REG__WEEKDAYS		Weekdays;
		PCF8523_months				Months;
		uint8_t						Years;
		uPCF8523_REG__MINUTES		Minute_alarm;
		uPCF8523_REG__HOURS			Hour_alarm;
		uPCF8523_REG__DAYS			Day_alarm;
		uPCF8523_REG__WEEKDAYS		Weekday_alarm;
		uPCF8523_REG__OFFSET		Offset;
		uPCF8523_REG__CLKOUT_CTRL	Tmr_CLKOUT_ctrl;
		uint8_t						Tmr_A_freq_ctrl;
		uint8_t						Tmr_A_reg;
		uPCF8523_REG__B_FREQ_CTRL	Tmr_B_freq_ctrl;
		uint8_t						Tmr_B_reg;
	} Reg;
} uPCF8523_REG_MAP;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/
// TODO: add more functions/inlines handling interrupts & clock output

__INLINE FctERR INLINE__ PCF8523_Reset(void) {
	uint8_t RST = 0x58;
	return PCF8523_Write(&RST, PCF8523__CONTROL_1, 1); }


__INLINE FctERR INLINE__ PCF8523_Disable_Alarm(void) {
	uint8_t ALARM[4] = { 0, 0, 0, 0 };
	return PCF8523_Write(ALARM, PCF8523__MINUTE_ALARM, 4); }


__INLINE FctERR INLINE__ PCF8523_Disable_Minute_Alarm(void) {
	uint8_t ALARM = 0;
	return PCF8523_Write(&ALARM, PCF8523__MINUTE_ALARM, 1); }

__INLINE FctERR INLINE__ PCF8523_Disable_Hour_Alarm(void) {
	uint8_t ALARM = 0;
	return PCF8523_Write(&ALARM, PCF8523__HOUR_ALARM, 1); }

__INLINE FctERR INLINE__ PCF8523_Disable_Day_Alarm(void) {
	uint8_t ALARM = 0;
	return PCF8523_Write(&ALARM, PCF8523__DAY_ALARM, 1); }

__INLINE FctERR INLINE__ PCF8523_Disable_Weekday_Alarm(void) {
	uint8_t ALARM = 0;
	return PCF8523_Write(&ALARM, PCF8523__WEEKDAY_ALARM, 1); }


FctERR PCF8523_Enable_Minute_Alarm(const uint8_t min);
FctERR PCF8523_Enable_Hour_Alarm(const uint8_t hour);
FctERR PCF8523_Enable_Day_Alarm(const uint8_t day);
FctERR PCF8523_Enable_Weekday_Alarm(const uint8_t weekday);

FctERR PCF8523_Set_12_24(const bool twelve);


__INLINE FctERR NONNULL_INLINE__ PCF8523_Set_Date_Raw(const uint8_t date[4]) {
	return PCF8523_Write((uint8_t *) date, PCF8523__DAYS, 4); }

__INLINE FctERR NONNULL_INLINE__ PCF8523_Set_Time_Raw(const uint8_t time[3]) {
	return PCF8523_Write((uint8_t *) time, PCF8523__SECONDS, 3); }


/*!\brief Countdown calculation for PCF8523 peripheral
** \return Error code
**/
FctERR NONNULL__ PCF8523_Get_Countdown(uint8_t * ctdw, const uint16_t period, const uint8_t timer);

__INLINE FctERR NONNULL_INLINE__ PCF8523_Get_Date_Raw(uint8_t * date) {
	return PCF8523_Read(date, PCF8523__DAYS, 4); }

__INLINE FctERR NONNULL_INLINE__ PCF8523_Get_Time_Raw(uint8_t * time) {
	return PCF8523_Read(time, PCF8523__SECONDS, 3); }

FctERR NONNULL__ PCF8523_Check_Clock_Integrity(bool * integrity);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __PCF8523_EX_H__ */
/****************************************************************/