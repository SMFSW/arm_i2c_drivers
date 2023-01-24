/*!\file PCA9956.h
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief PCA9956 Driver
** \details PCA9956: 24-channel Fm+ I2C-bus 57 mA/20 V constant current LED driver
**/
/****************************************************************/
#ifndef __PCA9956_H__
	#define __PCA9956_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#include "PCA9xxx.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/

#ifndef I2C_PCA9956_NB
//! \note Define I2C_PCA9956_NB to enable multiple peripherals of this type
#define I2C_PCA9956_NB	1	//!< Number of PCA9956 peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
#ifndef PCA9956_BASE_ADDR
//! \note Define PCA9956_BASE_ADDR to change default device base address
#define	PCA9956_BASE_ADDR		0x01		//!< PCA9956 Base Address
#endif


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum PCA9956_reg_map
** \brief Register map enum of PCA9956
**/
typedef enum PACK__ PCA9956_reg_map {
	PCA9956__MODE1 = 0,		//!< register MODE1
	PCA9956__MODE2,			//!< register MODE2
	PCA9956__LEDOUT0,		//!< register LED output state 0
	PCA9956__LEDOUT1,		//!< register LED output state 1
	PCA9956__LEDOUT2,		//!< register LED output state 2
	PCA9956__LEDOUT3,		//!< register LED output state 3
	PCA9956__LEDOUT4,		//!< register LED output state 4
	PCA9956__LEDOUT5,		//!< register LED output state 5
	PCA9956__GRPPWM,		//!< register group duty cycle control
	PCA9956__GRPFREQ,		//!< register group frequency
	PCA9956__PWM0,			//!< register brightness control LED0
	PCA9956__PWM1,			//!< register brightness control LED1
	PCA9956__PWM2,			//!< register brightness control LED2
	PCA9956__PWM3,			//!< register brightness control LED3
	PCA9956__PWM4,			//!< register brightness control LED4
	PCA9956__PWM5,			//!< register brightness control LED5
	PCA9956__PWM6,			//!< register brightness control LED6
	PCA9956__PWM7,			//!< register brightness control LED7
	PCA9956__PWM8,			//!< register brightness control LED8
	PCA9956__PWM9,			//!< register brightness control LED9
	PCA9956__PWM10,			//!< register brightness control LED10
	PCA9956__PWM11,			//!< register brightness control LED11
	PCA9956__PWM12,			//!< register brightness control LED12
	PCA9956__PWM13,			//!< register brightness control LED13
	PCA9956__PWM14,			//!< register brightness control LED14
	PCA9956__PWM15,			//!< register brightness control LED15
	PCA9956__PWM16,			//!< register brightness control LED16
	PCA9956__PWM17,			//!< register brightness control LED17
	PCA9956__PWM18,			//!< register brightness control LED18
	PCA9956__PWM19,			//!< register brightness control LED19
	PCA9956__PWM20,			//!< register brightness control LED20
	PCA9956__PWM21,			//!< register brightness control LED21
	PCA9956__PWM22,			//!< register brightness control LED22
	PCA9956__PWM23,			//!< register brightness control LED23
	PCA9956__IREF0,			//!< register output gain control 0
	PCA9956__IREF1,			//!< register output gain control 1
	PCA9956__IREF2,			//!< register output gain control 2
	PCA9956__IREF3,			//!< register output gain control 3
	PCA9956__IREF4,			//!< register output gain control 4
	PCA9956__IREF5,			//!< register output gain control 5
	PCA9956__IREF6,			//!< register output gain control 6
	PCA9956__IREF7,			//!< register output gain control 7
	PCA9956__IREF8,			//!< register output gain control 8
	PCA9956__IREF9,			//!< register output gain control 9
	PCA9956__IREF10,		//!< register output gain control 10
	PCA9956__IREF11,		//!< register output gain control 11
	PCA9956__IREF12,		//!< register output gain control 12
	PCA9956__IREF13,		//!< register output gain control 13
	PCA9956__IREF14,		//!< register output gain control 14
	PCA9956__IREF15,		//!< register output gain control 15
	PCA9956__IREF16,		//!< register output gain control 16
	PCA9956__IREF17,		//!< register output gain control 17
	PCA9956__IREF18,		//!< register output gain control 18
	PCA9956__IREF19,		//!< register output gain control 19
	PCA9956__IREF20,		//!< register output gain control 20
	PCA9956__IREF21,		//!< register output gain control 21
	PCA9956__IREF22,		//!< register output gain control 22
	PCA9956__IREF23,		//!< register output gain control 23
	PCA9956__OFFSET,		//!< register offset / delay on LEDn outputs
	PCA9956__SUBADR1,		//!< register SUBADR1
	PCA9956__SUBADR2,		//!< register SUBADR2
	PCA9956__SUBADR3,		//!< register SUBADR3
	PCA9956__ALLCALLADR,	//!< register ALLCALLADR
	PCA9956__PWMALL,		//!< register brightness control for all LEDn
	PCA9956__IREFALL,		//!< register output gain control for all IREF0 to IREF23
	PCA9956__EFLAG0,		//!< register output error flag 0
	PCA9956__EFLAG1,		//!< register output error flag 1
	PCA9956__EFLAG2,		//!< register output error flag 2
	PCA9956__EFLAG3,		//!< register output error flag 3
	PCA9956__EFLAG4,		//!< register output error flag 4
	PCA9956__EFLAG5			//!< register output error flag 5
} PCA9956_reg;


/*!\enum PCA9956_offset
** \brief Offset enum for PCA9956 peripheral
**/
typedef enum PACK__ PCA9956_offset {
	PCA9956__OFFSET_NONE = 0,		//!< no delay between outputs (all on, all off at the same time)
	PCA9956__OFFSET_1_CLOCK_CYCLE,	//!< delay of 1 clock cycle (125 ns) between successive outputs
	PCA9956__OFFSET_2_CLOCK_CYCLE,	//!< delay of 2 clock cycle (250 ns) between successive outputs
	PCA9956__OFFSET_3_CLOCK_CYCLE,	//!< delay of 3 clock cycle (375 ns) between successive outputs
	PCA9956__OFFSET_4_CLOCK_CYCLE,	//!< delay of 4 clock cycle (500 ns) between successive outputs
	PCA9956__OFFSET_5_CLOCK_CYCLE,	//!< delay of 5 clock cycle (625 ns) between successive outputs
	PCA9956__OFFSET_6_CLOCK_CYCLE,	//!< delay of 6 clock cycle (750 ns) between successive outputs
	PCA9956__OFFSET_7_CLOCK_CYCLE,	//!< delay of 7 clock cycle (875 ns) between successive outputs
	PCA9956__OFFSET_8_CLOCK_CYCLE,	//!< delay of 8 clock cycle (1 us) between successive outputs
	PCA9956__OFFSET_9_CLOCK_CYCLE,	//!< delay of 9 clock cycle (1.125 us) between successive outputs
	PCA9956__OFFSET_10_CLOCK_CYCLE,	//!< delay of 10 clock cycle (1.25 us) between successive outputs
	PCA9956__OFFSET_11_CLOCK_CYCLE,	//!< delay of 11 clock cycle (1.375 us) between successive outputs
} PCA9956_offset;


/*!\union uPCA9956_REG__MODE1
** \brief Union for MODE1 register of PCA9956
**/
typedef union PACK__ uPCA9956_REG__MODE1 {
	uint8_t Byte;
	struct PACK__ {
		uint8_t ALLCALL	:1;		//!< All CALL address acknowledgment
		uint8_t SUB3	:1;		//!< Sub-address 3 acknowledgment
		uint8_t SUB2	:1;		//!< Sub-address 2 acknowledgment
		uint8_t SUB1	:1;		//!< Sub-address 1 acknowledgment
		uint8_t SLEEP	:1;		//!< 0: Normal, 1: Low Power
		uint8_t AI0		:1;		//!< Auto-increment bit 0
		uint8_t AI1		:1;		//!< Auto-increment bit 1
		uint8_t AIF		:1;		//!< Register auto-increment
	} Bits;
} uPCA9956_REG__MODE1;

/*!\union uPCA9956_REG__MODE2
** \brief Union for MODE2 register of PCA9956
**/
typedef union PACK__ uPCA9956_REG__MODE2 {
	uint8_t Byte;
	struct PACK__ {
		uint8_t 					:3;
		PCA9956_latch	OCH			:1;		//!< 0: update on STOP, 1: update on ACK
		uint8_t 		CLRERR		:1;		//!< Write 1 to clear all error status bits in EFLAGn register and ERROR (bit 6)
		PCA9956_dim		DMBLNK		:1;		//!< 0: Dimming, 1: Blinking
		uint8_t			ERROR		:1;		//!< 0: no error, 1: any open or short-circuit detected in error flag registers (EFLAGn)
		uint8_t			OVERTEMP	:1;		//!< 0: OK, 1: overtemperature condition
	} Bits;
} uPCA9956_REG__MODE2;


/*!\union uPCA9956_REG__LEDOUT
** \brief Union for LEDOUT registers of PCA9956
**/
typedef union PACK__ uPCA9956_REG__LEDOUT {
	uint64_t LWord;
	struct PACK__ {
		PCA9956_ledout LDR0		:2;		//!< LED0 output state control
		PCA9956_ledout LDR1		:2;		//!< LED1 output state control
		PCA9956_ledout LDR2		:2;		//!< LED2 output state control
		PCA9956_ledout LDR3		:2;		//!< LED3 output state control
		PCA9956_ledout LDR4		:2;		//!< LED4 output state control
		PCA9956_ledout LDR5		:2;		//!< LED5 output state control
		PCA9956_ledout LDR6		:2;		//!< LED6 output state control
		PCA9956_ledout LDR7		:2;		//!< LED7 output state control
		PCA9956_ledout LDR8		:2;		//!< LED8 output state control
		PCA9956_ledout LDR9		:2;		//!< LED9 output state control
		PCA9956_ledout LDR10	:2;		//!< LED10 output state control
		PCA9956_ledout LDR11	:2;		//!< LED11 output state control
		PCA9956_ledout LDR12	:2;		//!< LED12 output state control
		PCA9956_ledout LDR13	:2;		//!< LED13 output state control
		PCA9956_ledout LDR14	:2;		//!< LED14 output state control
		PCA9956_ledout LDR15	:2;		//!< LED15 output state control
		PCA9956_ledout LDR16	:2;		//!< LED16 output state control
		PCA9956_ledout LDR17	:2;		//!< LED17 output state control
		PCA9956_ledout LDR18	:2;		//!< LED18 output state control
		PCA9956_ledout LDR19	:2;		//!< LED19 output state control
		PCA9956_ledout LDR20	:2;		//!< LED20 output state control
		PCA9956_ledout LDR21	:2;		//!< LED21 output state control
		PCA9956_ledout LDR22	:2;		//!< LED22 output state control
		PCA9956_ledout LDR23	:2;		//!< LED23 output state control
		uint64_t				:16;
	} Bits;
} uPCA9956_REG__LEDOUT;


/*!\union uPCA9956_REG__EFLAG
** \brief Union for EFLAG registers of PCA9956
**/
typedef union PACK__ uPCA9956_REG__EFLAG {
	uint64_t LWord;
	struct PACK__ {
		PCA9956_eflag ERR0		:2;		//!< LED0 error status
		PCA9956_eflag ERR1		:2;		//!< LED1 error status
		PCA9956_eflag ERR2		:2;		//!< LED2 error status
		PCA9956_eflag ERR3		:2;		//!< LED3 error status
		PCA9956_eflag ERR4		:2;		//!< LED4 error status
		PCA9956_eflag ERR5		:2;		//!< LED5 error status
		PCA9956_eflag ERR6		:2;		//!< LED6 error status
		PCA9956_eflag ERR7		:2;		//!< LED7 error status
		PCA9956_eflag ERR8		:2;		//!< LED8 error status
		PCA9956_eflag ERR9		:2;		//!< LED9 error status
		PCA9956_eflag ERR10		:2;		//!< LED10 error status
		PCA9956_eflag ERR11		:2;		//!< LED11 error status
		PCA9956_eflag ERR12		:2;		//!< LED12 error status
		PCA9956_eflag ERR13		:2;		//!< LED13 error status
		PCA9956_eflag ERR14		:2;		//!< LED14 error status
		PCA9956_eflag ERR15		:2;		//!< LED15 error status
		PCA9956_eflag ERR16		:2;		//!< LED16 error status
		PCA9956_eflag ERR17		:2;		//!< LED17 error status
		PCA9956_eflag ERR18		:2;		//!< LED18 error status
		PCA9956_eflag ERR19		:2;		//!< LED19 error status
		PCA9956_eflag ERR20		:2;		//!< LED20 error status
		PCA9956_eflag ERR21		:2;		//!< LED21 error status
		PCA9956_eflag ERR22		:2;		//!< LED22 error status
		PCA9956_eflag ERR23		:2;		//!< LED23 error status
		uint64_t				:16;
	} Bits;
} uPCA9956_REG__EFLAG;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for PCA9956 peripheral
** \param[in] idx - PCA9956 index
** \param[in] hi2c - pointer to PCA9956 I2C instance
** \param[in] devAddress - PCA9956 device address
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9956_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress);

/*!\brief Initialization for PCA9956 peripheral
** \warning In case multiple devices (defined by I2C_PCA9956_NB > 1), you shall use PCA9956_Init instead
** \return FctERR - error code
**/
FctERR PCA9956_Init_Single(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for PCA9956
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9956_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for PCA9956
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9956_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb);


/****************************************************************/
#include "PCA9956_proc.h"	// Include procedures
#include "PCA9956_ex.h"		// Include extensions

#endif

#ifdef __cplusplus
	}
#endif

#endif	/* __PCA9956_H__ */
/****************************************************************/
