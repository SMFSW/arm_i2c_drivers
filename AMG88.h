/*!\file AMG88.h
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
** \brief AMG88 Driver
** \details AMG88: Infrared Array Sensor (Grid-EYE)
**/
/****************************************************************/
#ifndef __AMG88_H__
	#define __AMG88_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


#ifndef I2C_AMG88_NB
//! \note Define I2C_AMG88_NB to enable multiple peripherals of this type
#define I2C_AMG88_NB	1	//!< Number of AMG88 peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define AMG88_ADDR			0x68			//!< AMG88 address

#ifndef AMG88_BASE_ADDR
//! \note Define AMG88_BASE_ADDR to change default device base address
#define AMG88_BASE_ADDR		AMG88_ADDR		//!< AMG88 Base address
#endif


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum AMG88_reg_map
** \brief Register map enum of AMG88
**/
typedef enum PACK__ AMG88_reg_map {
	AMG88__PCTL = 0,	//!< Set operating mode(Normal, Sleep etc.)
	AMG88__RST,			//!< Software Reset
	AMG88__FPSC,		//!< Frame rate
	AMG88__INTC,		//!< Interrupt Function
	AMG88__STAT,		//!< Interrupt Flag, low voltage Flag
	AMG88__SCLR,		//!< Interrupt Flag Clear
	AMG88__AVE = 0x07,	//!< Moving Average Output Mode
	AMG88__INTHL,		//!< Interrupt upper value(Lower level)
	AMG88__INTHH,		//!< Interrupt upper value(Upper level)
	AMG88__INTLL,		//!< Interrupt lower value(Lower level)
	AMG88__INTLH,		//!< Interrupt lower value(Upper level)
	AMG88__IHYSL,		//!< Interrupt hysteresis value(Lower level)
	AMG88__IHYSH,		//!< Interrupt hysteresis value(Upper level)
	AMG88__TTHL,		//!< Thermistor Output Value(Lower level)
	AMG88__TTHH,		//!< Thermistor Output Value(Upper level)
	AMG88__INT0,		//!< Pixel 1~8 Interrupt Result
	AMG88__INT1,		//!< Pixel 9~16 Interrupt Result
	AMG88__INT2,		//!< Pixel 17~24 Interrupt Result
	AMG88__INT3,		//!< Pixel 25~32 Interrupt Result
	AMG88__INT4,		//!< Pixel 33~40 Interrupt Result
	AMG88__INT5,		//!< Pixel 41~48 Interrupt Result
	AMG88__INT6,		//!< Pixel 49~56 Interrupt Result
	AMG88__INT7,		//!< Pixel 57~64 Interrupt Result
	AMG88__T01L = 0x80,	//!< Pixel 01 Output Value (Lower Level)
	AMG88__T01H,		//!< Pixel 01 Output Value (Upper Level)
	AMG88__T02L,		//!< Pixel 02 Output Value (Lower Level)
	AMG88__T02H,		//!< Pixel 02 Output Value (Upper Level)
	AMG88__T03L,		//!< Pixel 03 Output Value (Lower Level)
	AMG88__T03H,		//!< Pixel 03 Output Value (Upper Level)
	AMG88__T04L,		//!< Pixel 04 Output Value (Lower Level)
	AMG88__T04H,		//!< Pixel 04 Output Value (Upper Level)
	AMG88__T05L,		//!< Pixel 05 Output Value (Lower Level)
	AMG88__T05H,		//!< Pixel 05 Output Value (Upper Level)
	AMG88__T06L,		//!< Pixel 06 Output Value (Lower Level)
	AMG88__T06H,		//!< Pixel 06 Output Value (Upper Level)
	AMG88__T07L,		//!< Pixel 07 Output Value (Lower Level)
	AMG88__T07H,		//!< Pixel 07 Output Value (Upper Level)
	AMG88__T08L,		//!< Pixel 08 Output Value (Lower Level)
	AMG88__T08H,		//!< Pixel 08 Output Value (Upper Level)
	AMG88__T09L,		//!< Pixel 09 Output Value (Lower Level)
	AMG88__T09H,		//!< Pixel 09 Output Value (Upper Level)
	AMG88__T10L,		//!< Pixel 10 Output Value (Lower Level)
	AMG88__T10H,		//!< Pixel 10 Output Value (Upper Level)
	AMG88__T11L,		//!< Pixel 11 Output Value (Lower Level)
	AMG88__T11H,		//!< Pixel 11 Output Value (Upper Level)
	AMG88__T12L,		//!< Pixel 12 Output Value (Lower Level)
	AMG88__T12H,		//!< Pixel 12 Output Value (Upper Level)
	AMG88__T13L,		//!< Pixel 13 Output Value (Lower Level)
	AMG88__T13H,		//!< Pixel 13 Output Value (Upper Level)
	AMG88__T14L,		//!< Pixel 14 Output Value (Lower Level)
	AMG88__T14H,		//!< Pixel 14 Output Value (Upper Level)
	AMG88__T15L,		//!< Pixel 15 Output Value (Lower Level)
	AMG88__T15H,		//!< Pixel 15 Output Value (Upper Level)
	AMG88__T16L,		//!< Pixel 16 Output Value (Lower Level)
	AMG88__T16H,		//!< Pixel 16 Output Value (Upper Level)
	AMG88__T17L,		//!< Pixel 17 Output Value (Lower Level)
	AMG88__T17H,		//!< Pixel 17 Output Value (Upper Level)
	AMG88__T18L,		//!< Pixel 18 Output Value (Lower Level)
	AMG88__T18H,		//!< Pixel 18 Output Value (Upper Level)
	AMG88__T19L,		//!< Pixel 19 Output Value (Lower Level)
	AMG88__T19H,		//!< Pixel 19 Output Value (Upper Level)
	AMG88__T20L,		//!< Pixel 20 Output Value (Lower Level)
	AMG88__T20H,		//!< Pixel 20 Output Value (Upper Level)
	AMG88__T21L,		//!< Pixel 21 Output Value (Lower Level)
	AMG88__T21H,		//!< Pixel 21 Output Value (Upper Level)
	AMG88__T22L,		//!< Pixel 22 Output Value (Lower Level)
	AMG88__T22H,		//!< Pixel 22 Output Value (Upper Level)
	AMG88__T23L,		//!< Pixel 23 Output Value (Lower Level)
	AMG88__T23H,		//!< Pixel 23 Output Value (Upper Level)
	AMG88__T24L,		//!< Pixel 24 Output Value (Lower Level)
	AMG88__T24H,		//!< Pixel 24 Output Value (Upper Level)
	AMG88__T25L,		//!< Pixel 25 Output Value (Lower Level)
	AMG88__T25H,		//!< Pixel 25 Output Value (Upper Level)
	AMG88__T26L,		//!< Pixel 26 Output Value (Lower Level)
	AMG88__T26H,		//!< Pixel 26 Output Value (Upper Level)
	AMG88__T27L,		//!< Pixel 27 Output Value (Lower Level)
	AMG88__T27H,		//!< Pixel 27 Output Value (Upper Level)
	AMG88__T28L,		//!< Pixel 28 Output Value (Lower Level)
	AMG88__T28H,		//!< Pixel 28 Output Value (Upper Level)
	AMG88__T29L,		//!< Pixel 29 Output Value (Lower Level)
	AMG88__T29H,		//!< Pixel 29 Output Value (Upper Level)
	AMG88__T30L,		//!< Pixel 30 Output Value (Lower Level)
	AMG88__T30H,		//!< Pixel 30 Output Value (Upper Level)
	AMG88__T31L,		//!< Pixel 31 Output Value (Lower Level)
	AMG88__T31H,		//!< Pixel 31 Output Value (Upper Level)
	AMG88__T32L,		//!< Pixel 32 Output Value (Lower Level)
	AMG88__T32H,		//!< Pixel 32 Output Value (Upper Level)
	AMG88__T33L,		//!< Pixel 33 Output Value (Lower Level)
	AMG88__T33H,		//!< Pixel 33 Output Value (Upper Level)
	AMG88__T34L,		//!< Pixel 34 Output Value (Lower Level)
	AMG88__T34H,		//!< Pixel 34 Output Value (Upper Level)
	AMG88__T35L,		//!< Pixel 35 Output Value (Lower Level)
	AMG88__T35H,		//!< Pixel 35 Output Value (Upper Level)
	AMG88__T36L,		//!< Pixel 36 Output Value (Lower Level)
	AMG88__T36H,		//!< Pixel 36 Output Value (Upper Level)
	AMG88__T37L,		//!< Pixel 37 Output Value (Lower Level)
	AMG88__T37H,		//!< Pixel 37 Output Value (Upper Level)
	AMG88__T38L,		//!< Pixel 38 Output Value (Lower Level)
	AMG88__T38H,		//!< Pixel 38 Output Value (Upper Level)
	AMG88__T39L,		//!< Pixel 39 Output Value (Lower Level)
	AMG88__T39H,		//!< Pixel 39 Output Value (Upper Level)
	AMG88__T40L,		//!< Pixel 40 Output Value (Lower Level)
	AMG88__T40H,		//!< Pixel 40 Output Value (Upper Level)
	AMG88__T41L,		//!< Pixel 41 Output Value (Lower Level)
	AMG88__T41H,		//!< Pixel 41 Output Value (Upper Level)
	AMG88__T42L,		//!< Pixel 42 Output Value (Lower Level)
	AMG88__T42H,		//!< Pixel 42 Output Value (Upper Level)
	AMG88__T43L,		//!< Pixel 43 Output Value (Lower Level)
	AMG88__T43H,		//!< Pixel 43 Output Value (Upper Level)
	AMG88__T44L,		//!< Pixel 44 Output Value (Lower Level)
	AMG88__T44H,		//!< Pixel 44 Output Value (Upper Level)
	AMG88__T45L,		//!< Pixel 45 Output Value (Lower Level)
	AMG88__T45H,		//!< Pixel 45 Output Value (Upper Level)
	AMG88__T46L,		//!< Pixel 46 Output Value (Lower Level)
	AMG88__T46H,		//!< Pixel 46 Output Value (Upper Level)
	AMG88__T47L,		//!< Pixel 47 Output Value (Lower Level)
	AMG88__T47H,		//!< Pixel 47 Output Value (Upper Level)
	AMG88__T48L,		//!< Pixel 48 Output Value (Lower Level)
	AMG88__T48H,		//!< Pixel 48 Output Value (Upper Level)
	AMG88__T49L,		//!< Pixel 49 Output Value (Lower Level)
	AMG88__T49H,		//!< Pixel 49 Output Value (Upper Level)
	AMG88__T50L,		//!< Pixel 50 Output Value (Lower Level)
	AMG88__T50H,		//!< Pixel 50 Output Value (Upper Level)
	AMG88__T51L,		//!< Pixel 51 Output Value (Lower Level)
	AMG88__T51H,		//!< Pixel 51 Output Value (Upper Level)
	AMG88__T52L,		//!< Pixel 52 Output Value (Lower Level)
	AMG88__T52H,		//!< Pixel 52 Output Value (Upper Level)
	AMG88__T53L,		//!< Pixel 53 Output Value (Lower Level)
	AMG88__T53H,		//!< Pixel 53 Output Value (Upper Level)
	AMG88__T54L,		//!< Pixel 54 Output Value (Lower Level)
	AMG88__T54H,		//!< Pixel 54 Output Value (Upper Level)
	AMG88__T55L,		//!< Pixel 55 Output Value (Lower Level)
	AMG88__T55H,		//!< Pixel 55 Output Value (Upper Level)
	AMG88__T56L,		//!< Pixel 56 Output Value (Lower Level)
	AMG88__T56H,		//!< Pixel 56 Output Value (Upper Level)
	AMG88__T57L,		//!< Pixel 57 Output Value (Lower Level)
	AMG88__T57H,		//!< Pixel 57 Output Value (Upper Level)
	AMG88__T58L,		//!< Pixel 58 Output Value (Lower Level)
	AMG88__T58H,		//!< Pixel 58 Output Value (Upper Level)
	AMG88__T59L,		//!< Pixel 59 Output Value (Lower Level)
	AMG88__T59H,		//!< Pixel 59 Output Value (Upper Level)
	AMG88__T60L,		//!< Pixel 60 Output Value (Lower Level)
	AMG88__T60H,		//!< Pixel 60 Output Value (Upper Level)
	AMG88__T61L,		//!< Pixel 61 Output Value (Lower Level)
	AMG88__T61H,		//!< Pixel 61 Output Value (Upper Level)
	AMG88__T62L,		//!< Pixel 62 Output Value (Lower Level)
	AMG88__T62H,		//!< Pixel 62 Output Value (Upper Level)
	AMG88__T63L,		//!< Pixel 63 Output Value (Lower Level)
	AMG88__T63H,		//!< Pixel 63 Output Value (Upper Level)
	AMG88__T64L,		//!< Pixel 64 Output Value (Lower Level)
	AMG88__T64H			//!< Pixel 64 Output Value (Upper Level)
} AMG88_reg;


/*!\enum AMG88_reset
** \brief Reset types for AMG88
**/
typedef enum PACK__ AMG88_reset {
	AMG88__FLAG_RESET = 0x30,		//!< Flag Reset can all clear the Status Register(0x04),Interrupt Flag, and Interrupt Table(0x10~0x17).
	AMG88__INITIAL_RESET = 0x3F,	//!< Initial Reset brings Flag reset and returns to initial setting.
} AMG88_reset;


/*!\enum AMG88_mode
** \brief Operating modes for AMG88
**/
typedef enum PACK__ AMG88_mode {
	AMG88__NORMAL = 0,			//!< Normal mode
	AMG88__SLEEP = 0x10,		//!< Sleep mode
	AMG88__STANBY_60 = 0x20,	//!< Stand-by mode (60s intermittence)
	AMG88__STANBY_10 = 0x21,	//!< Stand-by mode (10s intermittence)
} AMG88_mode;


/*!\union uAMG88_REG__FRAME_RATE
** \brief Union for frame rate register of AMG88
**/
typedef union uAMG88_REG__FRAME_RATE {
	uint8_t Byte;
	struct PACK__ {
		uint8_t FPS		:1;	//!< Frame mode (1: 1FPS ; 0: 10FPS)
		uint8_t			:7;
	} Bits;
} uAMG88_REG__FRAME_RATE;


/*!\union uAMG88_REG__INT_CONTROL
** \brief Union for interrupt control register of AMG88
**/
typedef union uAMG88_REG__INT_CONTROL {
	uint8_t Byte;
	struct PACK__ {
		uint8_t INTEN	:1;	//!< INT Output (0: reactive (Hi-Z) ; 1: active)
		uint8_t INTMOD	:1;	//!< Interrupt Mode (0: Difference ; 1: Absolute)
		uint8_t			:6;
	} Bits;
} uAMG88_REG__INT_CONTROL;


/*!\union uAMG88_REG__STATUS
** \brief Union for status register of AMG88
**/
typedef union uAMG88_REG__STATUS {
	uint8_t Byte;
	struct PACK__ {
		uint8_t			:1;
		uint8_t INTF	:1;	//!< Interrupt Outbreak
		uint8_t OVF_IRS	:1;	//!< Temperature Output Overflow
		uint8_t OVF_THS	:1;	//!< Thermistor Output Overflow
		uint8_t			:4;
	} Bits;
} uAMG88_REG__STATUS;


/*!\union uAMG88_REG__STATUS_CLEAR
** \brief Union for status clear register of AMG88
**/
typedef union uAMG88_REG__STATUS_CLEAR {
	uint8_t Byte;
	struct PACK__ {
		uint8_t			:1;
		uint8_t INTCLR	:1;	//!< Interrupt Flag Clear
		uint8_t OVS_CLR	:1;	//!< Temperature Output Overflow Flag Clear
		uint8_t OVT_CLR	:1;	//!< Thermistor Temperature Output Overflow Flag Clear
		uint8_t			:4;
	} Bits;
} uAMG88_REG__STATUS_CLEAR;


/*!\union uAMG88_REG__AVERAGE
** \brief Union for average register of AMG88
**/
typedef union uAMG88_REG__AVERAGE {
	uint8_t Byte;
	struct PACK__ {
		uint8_t			:5;
		uint8_t MAMOD	:1;	//!< Moving average Output Mode (0: single ; 1: twice)
		uint8_t			:2;
	} Bits;
} uAMG88_REG__AVERAGE;


/*!\union uAMG88_REG__TEMP
** \brief Union for any temperature register of AMG88
**/
typedef union uAMG88_REG__TEMP {
	uint16_t Word;
	struct PACK__ {
		//! \warning temp is coded as absolute (thermistor), or as twos complement (pixels and thresholds)
		uint16_t temp			:11;	//!< Temperature value (absolute)
		uint16_t sign			:1;		//!< Temperature sign
		uint16_t				:4;
	} Bits;
} uAMG88_REG__TEMP;


/*!\union uAMG88_REG__TEMP
** \brief Union for any temperature register of AMG88
**/
typedef union uAMG88_REG__INT {
	uint64_t LWord;
	uint8_t Bytes[8];
	struct PACK__ {
		uint64_t	PIX01	:1;		//!< Pixel 01 interrupt
		uint64_t	PIX02	:1;		//!< Pixel 02 interrupt
		uint64_t	PIX03	:1;		//!< Pixel 03 interrupt
		uint64_t	PIX04	:1;		//!< Pixel 04 interrupt
		uint64_t	PIX05	:1;		//!< Pixel 05 interrupt
		uint64_t	PIX06	:1;		//!< Pixel 06 interrupt
		uint64_t	PIX07	:1;		//!< Pixel 07 interrupt
		uint64_t	PIX08	:1;		//!< Pixel 08 interrupt
		uint64_t	PIX09	:1;		//!< Pixel 09 interrupt
		uint64_t	PIX10	:1;		//!< Pixel 10 interrupt
		uint64_t	PIX11	:1;		//!< Pixel 11 interrupt
		uint64_t	PIX12	:1;		//!< Pixel 12 interrupt
		uint64_t	PIX13	:1;		//!< Pixel 13 interrupt
		uint64_t	PIX14	:1;		//!< Pixel 14 interrupt
		uint64_t	PIX15	:1;		//!< Pixel 15 interrupt
		uint64_t	PIX16	:1;		//!< Pixel 16 interrupt
		uint64_t	PIX17	:1;		//!< Pixel 17 interrupt
		uint64_t	PIX18	:1;		//!< Pixel 18 interrupt
		uint64_t	PIX19	:1;		//!< Pixel 19 interrupt
		uint64_t	PIX20	:1;		//!< Pixel 20 interrupt
		uint64_t	PIX21	:1;		//!< Pixel 21 interrupt
		uint64_t	PIX22	:1;		//!< Pixel 22 interrupt
		uint64_t	PIX23	:1;		//!< Pixel 23 interrupt
		uint64_t	PIX24	:1;		//!< Pixel 24 interrupt
		uint64_t	PIX25	:1;		//!< Pixel 25 interrupt
		uint64_t	PIX26	:1;		//!< Pixel 26 interrupt
		uint64_t	PIX27	:1;		//!< Pixel 27 interrupt
		uint64_t	PIX28	:1;		//!< Pixel 28 interrupt
		uint64_t	PIX29	:1;		//!< Pixel 29 interrupt
		uint64_t	PIX30	:1;		//!< Pixel 30 interrupt
		uint64_t	PIX31	:1;		//!< Pixel 31 interrupt
		uint64_t	PIX32	:1;		//!< Pixel 32 interrupt
		uint64_t	PIX33	:1;		//!< Pixel 33 interrupt
		uint64_t	PIX34	:1;		//!< Pixel 34 interrupt
		uint64_t	PIX35	:1;		//!< Pixel 35 interrupt
		uint64_t	PIX36	:1;		//!< Pixel 36 interrupt
		uint64_t	PIX37	:1;		//!< Pixel 37 interrupt
		uint64_t	PIX38	:1;		//!< Pixel 38 interrupt
		uint64_t	PIX39	:1;		//!< Pixel 39 interrupt
		uint64_t	PIX40	:1;		//!< Pixel 40 interrupt
		uint64_t	PIX41	:1;		//!< Pixel 41 interrupt
		uint64_t	PIX42	:1;		//!< Pixel 42 interrupt
		uint64_t	PIX43	:1;		//!< Pixel 43 interrupt
		uint64_t	PIX44	:1;		//!< Pixel 44 interrupt
		uint64_t	PIX45	:1;		//!< Pixel 45 interrupt
		uint64_t	PIX46	:1;		//!< Pixel 46 interrupt
		uint64_t	PIX47	:1;		//!< Pixel 47 interrupt
		uint64_t	PIX48	:1;		//!< Pixel 48 interrupt
		uint64_t	PIX49	:1;		//!< Pixel 49 interrupt
		uint64_t	PIX50	:1;		//!< Pixel 50 interrupt
		uint64_t	PIX51	:1;		//!< Pixel 51 interrupt
		uint64_t	PIX52	:1;		//!< Pixel 52 interrupt
		uint64_t	PIX53	:1;		//!< Pixel 53 interrupt
		uint64_t	PIX54	:1;		//!< Pixel 54 interrupt
		uint64_t	PIX55	:1;		//!< Pixel 55 interrupt
		uint64_t	PIX56	:1;		//!< Pixel 56 interrupt
		uint64_t	PIX57	:1;		//!< Pixel 57 interrupt
		uint64_t	PIX58	:1;		//!< Pixel 58 interrupt
		uint64_t	PIX59	:1;		//!< Pixel 59 interrupt
		uint64_t	PIX60	:1;		//!< Pixel 60 interrupt
		uint64_t	PIX61	:1;		//!< Pixel 61 interrupt
		uint64_t	PIX62	:1;		//!< Pixel 62 interrupt
		uint64_t	PIX63	:1;		//!< Pixel 63 interrupt
		uint64_t	PIX64	:1;		//!< Pixel 64 interrupt
	} Bits;
} uAMG88_REG__INT;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for AMG88 peripheral
** \param[in] idx - AMG88 index
** \param[in] hi2c - pointer to AMG88 I2C instance
** \param[in] devAddress - AMG88 device address
** \return FctERR - error code
**/
FctERR NONNULL__ AMG88_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress);

/*!\brief Initialization for AMG88 peripheral
** \warning In case multiple devices (defined by I2C_AMG88_NB > 1), you shall use AMG88_Init instead
** \return FctERR - error code
**/
FctERR AMG88_Init_Single(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for AMG88
** \param[in] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR NONNULL__ AMG88_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for AMG88
** \param[in] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ AMG88_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Word Write (little endian) function for AMG88
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \return FctERR - error code
**/
FctERR NONNULL__ AMG88_Write_Word(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr);


/*!\brief I2C Word Read (little endian) function for AMG88
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \return FctERR - error code
**/
FctERR NONNULL__ AMG88_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr);


/****************************************************************/
#include "AMG88_proc.h"	// Include procedures
#include "AMG88_ex.h"		// Include extensions

#endif

#ifdef __cplusplus
	}
#endif

#endif /* __AMG88_H__ */
/****************************************************************/
