/*!\file PCA9952.h
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief PCA9952 Driver
** \details PCA9952: 16-channel Fm+ I2C-bus 57 mA constant current LED driver
**/
/****************************************************************/
#ifndef __PCA9952_H__
	#define __PCA9952_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#include "PCA9xxx.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/

#ifndef I2C_PCA9952_NB
//! \note Define I2C_PCA9952_NB to enable multiple peripherals of this type
#define I2C_PCA9952_NB	1	//!< Number of PCA9952 peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
#ifndef PCA9952_BASE_ADDR
//! \note Define PCA9952_BASE_ADDR to change default device base address
#define	PCA9952_BASE_ADDR		0x01		//!< PCA9952 Base Address
#endif


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum PCA9952_reg_map
** \brief Register map enum of PCA9952
**/
typedef enum PACK__ PCA9952_reg_map {
	PCA9952__MODE1 = 0,		//!< register MODE1
	PCA9952__MODE2,			//!< register MODE2
	PCA9952__LEDOUT0,		//!< register LED output state 0
	PCA9952__LEDOUT1,		//!< register LED output state 1
	PCA9952__LEDOUT2,		//!< register LED output state 2
	PCA9952__LEDOUT3,		//!< register LED output state 3
	PCA9952__GRPPWM = 0x08,	//!< register group duty cycle control
	PCA9952__GRPFREQ,		//!< register group frequency
	PCA9952__PWM0,			//!< register brightness control LED0
	PCA9952__PWM1,			//!< register brightness control LED1
	PCA9952__PWM2,			//!< register brightness control LED2
	PCA9952__PWM3,			//!< register brightness control LED3
	PCA9952__PWM4,			//!< register brightness control LED4
	PCA9952__PWM5,			//!< register brightness control LED5
	PCA9952__PWM6,			//!< register brightness control LED6
	PCA9952__PWM7,			//!< register brightness control LED7
	PCA9952__PWM8,			//!< register brightness control LED8
	PCA9952__PWM9,			//!< register brightness control LED9
	PCA9952__PWM10,			//!< register brightness control LED10
	PCA9952__PWM11,			//!< register brightness control LED11
	PCA9952__PWM12,			//!< register brightness control LED12
	PCA9952__PWM13,			//!< register brightness control LED13
	PCA9952__PWM14,			//!< register brightness control LED14
	PCA9952__PWM15,			//!< register brightness control LED15
	PCA9952__IREF0 = 0x22,	//!< register output gain control 0
	PCA9952__IREF1,			//!< register output gain control 1
	PCA9952__IREF2,			//!< register output gain control 2
	PCA9952__IREF3,			//!< register output gain control 3
	PCA9952__IREF4,			//!< register output gain control 4
	PCA9952__IREF5,			//!< register output gain control 5
	PCA9952__IREF6,			//!< register output gain control 6
	PCA9952__IREF7,			//!< register output gain control 7
	PCA9952__IREF8,			//!< register output gain control 8
	PCA9952__IREF9,			//!< register output gain control 9
	PCA9952__IREF10,		//!< register output gain control 10
	PCA9952__IREF11,		//!< register output gain control 11
	PCA9952__IREF12,		//!< register output gain control 12
	PCA9952__IREF13,		//!< register output gain control 13
	PCA9952__IREF14,		//!< register output gain control 14
	PCA9952__IREF15,		//!< register output gain control 15
	PCA9952__OFFSET = 0x3A,	//!< register offset / delay on LEDn outputs
	PCA9952__SUBADR1,		//!< register SUBADR1
	PCA9952__SUBADR2,		//!< register SUBADR2
	PCA9952__SUBADR3,		//!< register SUBADR3
	PCA9952__ALLCALLADR,	//!< register ALLCALLADR
	PCA9952__PWMALL = 0x42,	//!< register brightness control for all LEDn
	PCA9952__IREFALL,		//!< register output gain control for all IREF0 to IREF23
	PCA9952__EFLAG0,		//!< register output error flag 0
	PCA9952__EFLAG1,		//!< register output error flag 1
} PCA9952_reg;


/*!\enum PCA9952_offset
** \brief Offset enum for PCA9952 peripheral
**/
typedef enum PACK__ PCA9952_offset {
	PCA9952__OFFSET_NONE = 0,		//!< no delay between outputs (all on, all off at the same time)
	PCA9952__OFFSET_1_CLOCK_CYCLE,	//!< delay of 1 clock cycle (125 ns) between successive outputs
	PCA9952__OFFSET_2_CLOCK_CYCLE,	//!< delay of 2 clock cycle (250 ns) between successive outputs
	PCA9952__OFFSET_3_CLOCK_CYCLE,	//!< delay of 3 clock cycle (375 ns) between successive outputs
	PCA9952__OFFSET_4_CLOCK_CYCLE,	//!< delay of 4 clock cycle (500 ns) between successive outputs
	PCA9952__OFFSET_5_CLOCK_CYCLE,	//!< delay of 5 clock cycle (625 ns) between successive outputs
	PCA9952__OFFSET_6_CLOCK_CYCLE,	//!< delay of 6 clock cycle (750 ns) between successive outputs
	PCA9952__OFFSET_7_CLOCK_CYCLE,	//!< delay of 7 clock cycle (875 ns) between successive outputs
	PCA9952__OFFSET_8_CLOCK_CYCLE,	//!< delay of 8 clock cycle (1 us) between successive outputs
	PCA9952__OFFSET_9_CLOCK_CYCLE,	//!< delay of 9 clock cycle (1.125 us) between successive outputs
	PCA9952__OFFSET_10_CLOCK_CYCLE,	//!< delay of 10 clock cycle (1.25 us) between successive outputs
	PCA9952__OFFSET_11_CLOCK_CYCLE,	//!< delay of 11 clock cycle (1.375 us) between successive outputs
} PCA9952_offset;


/*!\union uPCA9952_REG__MODE1
** \brief Union for MODE1 register of PCA9952
**/
typedef union PACK__ uPCA9952_REG__MODE1 {
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
} uPCA9952_REG__MODE1;

/*!\union uPCA9952_REG__MODE2
** \brief Union for MODE2 register of PCA9952
**/
typedef union PACK__ uPCA9952_REG__MODE2 {
	uint8_t Byte;
	struct PACK__ {
		uint8_t			Res0_1		:1;		//!< Reserved bit 0: reads 1
		uint8_t			Res1_0		:1;		//!< Reserved bit 1: reads 0
		uint8_t			Res2_1		:1;		//!< Reserved bit 2: reads 1
		PCA99xx_latch	OCH			:1;		//!< 0: update on STOP, 1: update on ACK
		uint8_t 		Res4_0		:1;		//!< Reserved bit 4: reads 0
		PCA995x_dim		DMBLNK		:1;		//!< 0: Dimming, 1: Blinking
		uint8_t			FAULTTEST	:1;		//!< 0: LED fault test complete, 1: start fault test
		uint8_t			OVERTEMP	:1;		//!< 0: OK, 1: overtemperature condition
	} Bits;
} uPCA9952_REG__MODE2;


/*!\union uPCA9952_REG__LEDOUT
** \brief Union for LEDOUT registers of PCA9952
**/
typedef union PACK__ uPCA9952_REG__LEDOUT {
	uint32_t DWord;
	struct PACK__ {
		PCA995x_ledout LDR0		:2;		//!< LED0 output state control
		PCA995x_ledout LDR1		:2;		//!< LED1 output state control
		PCA995x_ledout LDR2		:2;		//!< LED2 output state control
		PCA995x_ledout LDR3		:2;		//!< LED3 output state control
		PCA995x_ledout LDR4		:2;		//!< LED4 output state control
		PCA995x_ledout LDR5		:2;		//!< LED5 output state control
		PCA995x_ledout LDR6		:2;		//!< LED6 output state control
		PCA995x_ledout LDR7		:2;		//!< LED7 output state control
		PCA995x_ledout LDR8		:2;		//!< LED8 output state control
		PCA995x_ledout LDR9		:2;		//!< LED9 output state control
		PCA995x_ledout LDR10	:2;		//!< LED10 output state control
		PCA995x_ledout LDR11	:2;		//!< LED11 output state control
		PCA995x_ledout LDR12	:2;		//!< LED12 output state control
		PCA995x_ledout LDR13	:2;		//!< LED13 output state control
		PCA995x_ledout LDR14	:2;		//!< LED14 output state control
		PCA995x_ledout LDR15	:2;		//!< LED15 output state control
	} Bits;
} uPCA9952_REG__LEDOUT;


/*!\union uPCA9952_REG__EFLAG
** \brief Union for EFLAG registers of PCA9952
**/
typedef union PACK__ uPCA9952_REG__EFLAG {
	uint16_t Word;
	struct PACK__ {
		uint16_t ERR0		:1;		//!< LED0 error status
		uint16_t ERR1		:1;		//!< LED1 error status
		uint16_t ERR2		:1;		//!< LED2 error status
		uint16_t ERR3		:1;		//!< LED3 error status
		uint16_t ERR4		:1;		//!< LED4 error status
		uint16_t ERR5		:1;		//!< LED5 error status
		uint16_t ERR6		:1;		//!< LED6 error status
		uint16_t ERR7		:1;		//!< LED7 error status
		uint16_t ERR8		:1;		//!< LED8 error status
		uint16_t ERR9		:1;		//!< LED9 error status
		uint16_t ERR10		:1;		//!< LED10 error status
		uint16_t ERR11		:1;		//!< LED11 error status
		uint16_t ERR12		:1;		//!< LED12 error status
		uint16_t ERR13		:1;		//!< LED13 error status
		uint16_t ERR14		:1;		//!< LED14 error status
		uint16_t ERR15		:1;		//!< LED15 error status
	} Bits;
} uPCA9952_REG__EFLAG;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for PCA9952 peripheral
** \param[in] idx - PCA9952 index
** \param[in] hi2c - pointer to PCA9952 I2C instance
** \param[in] devAddress - PCA9952 device address
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9952_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress);

/*!\brief Initialization for PCA9952 peripheral
** \warning In case multiple devices (defined by I2C_PCA9952_NB > 1), you shall use PCA9952_Init instead
** \return FctERR - error code
**/
FctERR PCA9952_Init_Single(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for PCA9952
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9952_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for PCA9952
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9952_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb);


/****************************************************************/
#include "PCA9952_proc.h"	// Include procedures
#include "PCA9952_ex.h"		// Include extensions

#endif

#ifdef __cplusplus
	}
#endif

#endif	/* __PCA9952_H__ */
/****************************************************************/
