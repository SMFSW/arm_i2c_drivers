/*!\file PCF8523.h
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief PCF8523 Driver
** \details PCF8523: Real-Time Clock (RTC) and calendar
**/
/****************************************************************/
#ifndef __PCF8523_H__
	#define __PCF8523_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/
// TODO: doxygen


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#ifndef PCF8523_BASE_ADDR
//! \note Define PCF8523_BASE_ADDR to change default device base address
#define PCF8523_BASE_ADDR	0x68		//!< PCF8523 Base address
#endif


// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern I2C_slave_t PCF8523_hal;	//!< PCF8523 Slave structure


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum PCF8523_reg_map
** \brief Register map enum for PCF8523 peripheral peripheral
**/
typedef enum PACK__ PCF8523_reg_map {
	// Control registers
	PCF8523__CONTROL_1 = 0,		//!< Control and status register 1
	PCF8523__CONTROL_2,			//!< Control and status register 2
	PCF8523__CONTROL_3,			//!< Control and status register 3
	// Time and Date registers
	PCF8523__SECONDS,			//!< Seconds and clock integrity status register
	PCF8523__MINUTES,			//!< Minutes register
	PCF8523__HOURS,				//!< Hours register
	PCF8523__DAYS,				//!< Days register
	PCF8523__WEEKDAYS,			//!< Weekdays register
	PCF8523__MONTHS,			//!< Months register
	PCF8523__YEARS,				//!< Years register
	// Alarm registers
	PCF8523__MINUTE_ALARM,		//!< Minute alarm register
	PCF8523__HOUR_ALARM,		//!< Hour alarm register
	PCF8523__DAY_ALARM,			//!< Day alarm register
	PCF8523__WEEKDAY_ALARM,		//!< Weekday alarm register
	// Offset register
	PCF8523__OFFSET,			//!< Offset register
	// Clock and Timer registers
	PCF8523__TMR_CLKOUT_CTRL,	//!< Timer and CLKOUT control register
	PCF8523__TMR_A_FREQ_CTRL,	//!< Timer A frequency control register
	PCF8523__TMR_A_REG,			//!< Timer A value register
	PCF8523__TMR_B_FREQ_CTRL,	//!< Timer B frequency control register
	PCF8523__TMR_B_REG			//!< Timer B value register
} PCF8523_reg;

/*!\enum PCF8523_weekdays
** \brief Weekdays enum for PCF8523 peripheral
** \note For reference only, Weekdays can start from any day for week
**/
typedef enum PACK__ PCF8523_weekdays {
	PCF8523__SUNDAY = 0,	//!< Sunday
	PCF8523__MONDAY,		//!< Monday
	PCF8523__TUESDAY,		//!< Tuesday
	PCF8523__WEDNESDAY,		//!< Wednesday
	PCF8523__THURSDAY,		//!< Thursday
	PCF8523__FRIDAY,		//!< Friday
	PCF8523__SATURDAY		//!< Saturday
} PCF8523_weekdays;


/*!\enum PCF8523_months
** \note BCD coded values
** \brief Weekdays enum for PCF8523 peripheral
**/
typedef enum PACK__ PCF8523_months {
	PCF8523__JANUARY = 0x01,	//!< January
	PCF8523__FEBRUARY,			//!< February
	PCF8523__MARCH,				//!< March
	PCF8523__APRIL,				//!< April
	PCF8523__MAY,				//!< May
	PCF8523__JUNE,				//!< June
	PCF8523__JULY,				//!< July
	PCF8523__AUGUST,			//!< August
	PCF8523__SEPTEMBER,			//!< September
	PCF8523__OCTOBER = 0x10,	//!< October
	PCF8523__NOVEMBER,			//!< November
	PCF8523__DECEMBER,			//!< December
} PCF8523_months;


/*!\enum PCF8523_power
** \brief Power Management enum for PCF8523 peripheral
**/
typedef enum PACK__ PCF8523_power {
	PCF8523__SO_EN_STANDARD = 0,			//!< battery switch-over function is enabled in standard mode; battery low detection function is enabled
	PCF8523__SO_EN_DIRECT_SWITCHING,		//!< battery switch-over function is enabled in direct switching mode; battery low detection function is enabled
	PCF8523__SO_DI_POWER_VDD,				//!< battery switch-over function is disabled - only one power supply (VDD); battery low detection function is enabled
	//PCF8523__SO_DI_POWER_VDD,				//!< battery switch-over function is disabled - only one power supply (VDD); battery low detection function is enabled
	PCF8523__SO_EN_STANDARD_NO_BL = 4,		//!< battery switch-over function is enabled in standard mode; battery low detection function is disabled
	PCF8523__SO_EN_DIRECT_SWITCHING_NO_BL,	//!< battery switch-over function is enabled in direct switching mode; battery low detection function is disabled
	//PCF8523__NOT_ALLOWED,					//!< not allowed
	PCF8523__SO_DI_POWER_VDD_NO_BL = 7		//!< battery switch-over function is disabled - only one power supply (VDD); battery low detection function is disabled
} PCF8523_power;


/*!\union uPCF8523_REG__CTRL1
** \brief Union for Control_1 register for PCF8523 peripheral
**/
typedef union uPCF8523_REG__CTRL1 {
	uint8_t Byte;
	struct PACK__ {
		uint8_t CIE			:1;		//!< 0: no correction interrupt generated, 1: interrupt pulses are generated at every correction cycle
		uint8_t AIE			:1;		//!< Alarm interrupt enable
		uint8_t SIE			:1;		//!< Second interrupt enable
		uint8_t H12_24		:1;		//!< 12/24 hour mode (0: 24h, 1: 12h)
		uint8_t SR			:1;		//!< Software reset
		uint8_t STOP		:1;		//!< 0: RTC time circuits running, 1: RTC time circuits frozen
		uint8_t T			:1;		//!< unused
		uint8_t CAP_SEL		:1;		//!< internal oscillator capacitor selection for quartz crystals with a corresponding load capacitance (0: 7pF, 1: 12.5pF)
	} Bits;
} uPCF8523_REG__CTRL1;


/*!\union uPCF8523_REG__CTRL2
** \brief Union for Control_2 register for PCF8523 peripheral
**/
typedef union uPCF8523_REG__CTRL2 {
	uint8_t Byte;
	struct PACK__ {
		uint8_t CTBIE	:1;		//!< Countdown timer B enable
		uint8_t CTAIE	:1;		//!< Countdown timer A enable
		uint8_t WTAIE	:1;		//!< Watchdog timer A enable
		uint8_t AF		:1;		//!< Alarm interrupt (flag set when alarm triggered; flag must be cleared to clear interrupt)
		uint8_t SF		:1;		//!< Second interrupt (flag set when second interrupt generated; flag must be cleared to clear interrupt)
		uint8_t CTBF	:1;		//!< Countdown timer B (flag set when countdown timer B interrupt generated; flag must be cleared to clear interrupt)
		uint8_t CTAF	:1;		//!< Countdown timer A (flag set when countdown timer A interrupt generated; flag must be cleared to clear interrupt)
		uint8_t WTAF	:1;		//!< Watchdog timer A (flag set when watchdog timer A interrupt generated; flag is read-only and cleared by reading register Control_2)
	} Bits;
} uPCF8523_REG__CTRL2;


/*!\union uPCF8523_REG__CTRL3
** \brief Union for Control_3 register for PCF8523 peripheral
**/
typedef union uPCF8523_REG__CTRL3 {
	uint8_t Byte;
	struct PACK__ {
		uint8_t			BLIE	:1;		//!< Battery Low (1: interrupt generated when BLF is set)
		uint8_t			BSIE	:1;		//!< Battery Switch-over (1: interrupt generated when BSF is set)
		uint8_t			BLF		:1;		//!< Battery status (0: battery status Ok; 1: Battery low)
		uint8_t			BSF		:1;		//!< Battery Switch-over enable (flag set when battery switch-over occurs; flag must be cleared to clear interrupt)
		uint8_t					:1;
		PCF8523_power	PM		:3;		//!< Battery switch-over and battery low detection control
	} Bits;
} uPCF8523_REG__CTRL3;


/*!\union uPCF8523_REG__SECONDS
** \brief Union for Seconds register for PCF8523 peripheral
**/
typedef union uPCF8523_REG__SECONDS {
	uint8_t Byte;
	struct PACK__ {
		uint8_t SECONDS	:7;		//!< Seconds
		uint8_t OS		:1;		//!< Clock integrity
	} Bits;
} uPCF8523_REG__SECONDS;


/*!\union uPCF8523_REG__MINUTES
** \brief Union for Minutes and Minute Alarm when in AM/PM mode register for PCF8523 peripheral
**/
typedef union uPCF8523_REG__MINUTES {
	uint8_t Byte;
	struct PACK__ {
		uint8_t MINUTES	:7;		//!< Minutes
		// AEN_M: Only for Minute Alarm
		uint8_t AEN_M	:1;		//!< Alarm enabled
	} Bits;
} uPCF8523_REG__MINUTES;


/*!\union uPCF8523_REG__HOURS
** \brief Union for Hours and Hour Alarm when in AM/PM mode register for PCF8523 peripheral peripheral
**/
typedef union uPCF8523_REG__HOURS {
	uint8_t Byte;
	struct PACK__ {
		union {
			struct PACK__ {
				uint8_t HOURS	:5;		//!< Hours (12h format)
				uint8_t AMPM	:1;		//!< AM / PM
				uint8_t			:1;
				// AEN_H: Only for Hour Alarm
				uint8_t AEN_H	:1;		//!< Alarm enabled
			} TWELVE;
			struct PACK__ {
				uint8_t HOURS	:6;		//!< Hours (24h format)
				uint8_t			:1;
				// AEN_H: Only for Hour Alarm
				uint8_t AEN_H	:1;		//!< Alarm enabled
			} TWENTYFOUR;
		};
	} Bits;
} uPCF8523_REG__HOURS;


/*!\union uPCF8523_REG__DAYS
** \brief Union for Days and Day Alarm register for PCF8523 peripheral peripheral
**/
typedef union uPCF8523_REG__DAYS {
	uint8_t Byte;
	struct PACK__ {
		uint8_t DAYS	:6;		//!< Days
		uint8_t			:1;
		// AEN_D: Only for Day Alarm
		uint8_t AEN_D	:1;		//!< Alarm enabled
	} Bits;
} uPCF8523_REG__DAYS;


/*!\union uPCF8523_REG__WEEKDAYS
** \brief Union for Weekdays and Weekday Alarm register for PCF8523 peripheral peripheral
**/
typedef union uPCF8523_REG__WEEKDAYS {
	uint8_t Byte;
	struct PACK__ {
		PCF8523_weekdays WEEKDAYS	:3;		//!< Weekdays
		uint8_t						:4;
		// AEN_D: only for Weekday Alarm
		uint8_t AEN_W				:1;		//!< Alarm enabled
	} Bits;
} uPCF8523_REG__WEEKDAYS;


/*!\union uPCF8523_REG__OFFSET
** \brief Union for Offset for PCF8523 peripheral peripheral
**/
typedef union uPCF8523_REG__OFFSET {
	uint8_t Byte;
	struct PACK__ {
		uint8_t OFFSET	:7;		//!< offset value (+63 to -64) MODE = 0: offset step 4.340ppm, MODE = 1: offset step 4.069ppm
		int8_t MODE		:1;		//!< 0: offset is made once every two hours, 1: offset is made once every minute
	} Bits;
} uPCF8523_REG__OFFSET;


/*!\union uPCF8523_REG__CLKOUT_CTRL
** \brief Union for Tmr_CLKOUT_ctrl for PCF8523 peripheral peripheral
**/
typedef union uPCF8523_REG__CLKOUT_CTRL {
	uint8_t Byte;
	struct PACK__ {
		uint8_t TBC		:1;		//!< timer B is enabled (if CTBIE (register Control_2) is set logic 1, the interrupt is activated when the countdown timed out)
		uint8_t TAC		:2;		//!< b00: disabled, b01: timer A countdown timer, b10: timer A watchdog timer
		uint8_t COF		:3;		//!< CLKOUT frequency selection
		uint8_t TBM		:1;		//!< 0: permanent active interrupt, 1: pulsed interrupt -> for timer B
		uint8_t TAM		:1;		//!< 0: permanent active interrupt, 1: pulsed interrupt -> for timer A and for the second interrupt timer
	} Bits;
} uPCF8523_REG__CLKOUT_CTRL;


/*!\union uPCF8523_REG__B_FREQ_CTRL
** \brief Union for Tmr_B_freq_ctrl for PCF8523 peripheral peripheral
**/
typedef union uPCF8523_REG__B_FREQ_CTRL {
	uint8_t Byte;
	struct PACK__ {
		uint8_t TBQ		:3;		//!< source clock for timer B
		uint8_t			:1;
		uint8_t TBW		:3;		//!< low pulse width for pulsed timer B interrupt
		uint8_t			:1;
	} Bits;
} uPCF8523_REG__B_FREQ_CTRL;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for PCF8523 peripheral peripheral
** \weak PCF8523 Base address may be changed if user implemented
** \return FctERR - error code
**/
FctERR PCF8523_Init(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for PCF8523 peripheral
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR NONNULL__ PCF8523_Write(const uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for PCF8523 peripheral
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ PCF8523_Read(uint8_t * data, const uint16_t addr, const uint16_t nb);


/****************************************************************/
#include "PCF8523_ex.h"		// Include extensions
#include "PCF8523_proc.h"	// Include procedures

#endif

#ifdef __cplusplus
	}
#endif

#endif /* __PCF8523_H__ */
/****************************************************************/
