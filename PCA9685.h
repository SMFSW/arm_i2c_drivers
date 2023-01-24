/*!\file PCA9685.h
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief PCA9685 Driver
** \details PCA9685: 16-channel, 12-bit PWM Fm+ I2C-bus LED controller
**/
/****************************************************************/
#ifndef __PCA9685_H__
	#define __PCA9685_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#include "PCA9xxx.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/

#ifndef I2C_PCA9685_NB
//! \note Define I2C_PCA9685_NB to enable multiple peripherals of this type
#define I2C_PCA9685_NB	1	//!< Number of PCA9685 peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
#ifndef PCA9685_BASE_ADDR
//! \note Define PCA9685_BASE_ADDR to change default device base address
#define	PCA9685_BASE_ADDR		0x40		//!< PCA9685 Base Address
#endif


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum PCA9685_reg_map
** \brief Register map enum of PCA9685
**/
typedef enum PACK__ PCA9685_reg_map {
	PCA9685__MODE1 = 0,				//!< register MODE1
	PCA9685__MODE2,					//!< register MODE2
	PCA9685__SUBADR1,				//!< register SUBADR1
	PCA9685__SUBADR2,				//!< register SUBADR2
	PCA9685__SUBADR3,				//!< register SUBADR3
	PCA9685__ALLCALLADR,			//!< register ALLCALLADR
	PCA9685__LED0_ON_L,				//!< register LED 0 ON Low
	PCA9685__LED0_ON_H,				//!< register LED 0 ON High
	PCA9685__LED0_OFF_L,			//!< register LED 0 OFF Low
	PCA9685__LED0_OFF_H,			//!< register LED 0 OFF High
	PCA9685__LED1_ON_L,				//!< register LED 1 ON Low
	PCA9685__LED1_ON_H,				//!< register LED 1 ON High
	PCA9685__LED1_OFF_L,			//!< register LED 1 OFF Low
	PCA9685__LED1_OFF_H,			//!< register LED 1 OFF High
	PCA9685__LED2_ON_L,				//!< register LED 2 ON Low
	PCA9685__LED2_ON_H,				//!< register LED 2 ON High
	PCA9685__LED2_OFF_L,			//!< register LED 2 OFF Low
	PCA9685__LED2_OFF_H,			//!< register LED 2 OFF High
	PCA9685__LED3_ON_L,				//!< register LED 3 ON Low
	PCA9685__LED3_ON_H,				//!< register LED 3 ON High
	PCA9685__LED3_OFF_L,			//!< register LED 3 OFF Low
	PCA9685__LED3_OFF_H,			//!< register LED 3 OFF High
	PCA9685__LED4_ON_L,				//!< register LED 4 ON Low
	PCA9685__LED4_ON_H,				//!< register LED 4 ON High
	PCA9685__LED4_OFF_L,			//!< register LED 4 OFF Low
	PCA9685__LED4_OFF_H,			//!< register LED 4 OFF High
	PCA9685__LED5_ON_L,				//!< register LED 5 ON Low
	PCA9685__LED5_ON_H,				//!< register LED 5 ON High
	PCA9685__LED5_OFF_L,			//!< register LED 5 OFF Low
	PCA9685__LED5_OFF_H,			//!< register LED 5 OFF High
	PCA9685__LED6_ON_L,				//!< register LED 6 ON Low
	PCA9685__LED6_ON_H,				//!< register LED 6 ON High
	PCA9685__LED6_OFF_L,			//!< register LED 6 OFF Low
	PCA9685__LED6_OFF_H,			//!< register LED 6 OFF High
	PCA9685__LED7_ON_L,				//!< register LED 7 ON Low
	PCA9685__LED7_ON_H,				//!< register LED 7 ON High
	PCA9685__LED7_OFF_L,			//!< register LED 7 OFF Low
	PCA9685__LED7_OFF_H,			//!< register LED 7 OFF High
	PCA9685__LED8_ON_L,				//!< register LED 8 ON Low
	PCA9685__LED8_ON_H,				//!< register LED 8 ON High
	PCA9685__LED8_OFF_L,			//!< register LED 8 OFF Low
	PCA9685__LED8_OFF_H,			//!< register LED 8 OFF High
	PCA9685__LED9_ON_L,				//!< register LED 9 ON Low
	PCA9685__LED9_ON_H,				//!< register LED 9 ON High
	PCA9685__LED9_OFF_L,			//!< register LED 9 OFF Low
	PCA9685__LED9_OFF_H,			//!< register LED 9 OFF High
	PCA9685__LED10_ON_L,			//!< register LED 10 ON Low
	PCA9685__LED10_ON_H,			//!< register LED 10 ON High
	PCA9685__LED10_OFF_L,			//!< register LED 10 OFF Low
	PCA9685__LED10_OFF_H,			//!< register LED 10 OFF High
	PCA9685__LED11_ON_L,			//!< register LED 11 ON Low
	PCA9685__LED11_ON_H,			//!< register LED 11 ON High
	PCA9685__LED11_OFF_L,			//!< register LED 11 OFF Low
	PCA9685__LED11_OFF_H,			//!< register LED 11 OFF High
	PCA9685__LED12_ON_L,			//!< register LED 12 ON Low
	PCA9685__LED12_ON_H,			//!< register LED 12 ON High
	PCA9685__LED12_OFF_L,			//!< register LED 12 OFF Low
	PCA9685__LED12_OFF_H,			//!< register LED 12 OFF High
	PCA9685__LED13_ON_L,			//!< register LED 13 ON Low
	PCA9685__LED13_ON_H,			//!< register LED 13 ON High
	PCA9685__LED13_OFF_L,			//!< register LED 13 OFF Low
	PCA9685__LED13_OFF_H,			//!< register LED 13 OFF High
	PCA9685__LED14_ON_L,			//!< register LED 14 ON Low
	PCA9685__LED14_ON_H,			//!< register LED 14 ON High
	PCA9685__LED14_OFF_L,			//!< register LED 14 OFF Low
	PCA9685__LED14_OFF_H,			//!< register LED 14 OFF High
	PCA9685__LED15_ON_L,			//!< register LED 15 ON Low
	PCA9685__LED15_ON_H,			//!< register LED 15 ON High
	PCA9685__LED15_OFF_L,			//!< register LED 15 OFF Low
	PCA9685__LED15_OFF_H,			//!< register LED 15 OFF High
	PCA9685__ALL_LED_ON_L = 0xFA,	//!< register ALL LED ON Low
	PCA9685__ALL_LED_ON_H,			//!< register ALL LED ON High
	PCA9685__ALL_LED_OFF_L,			//!< register ALL LED OFF Low
	PCA9685__ALL_LED_OFF_H,			//!< register ALL LED OFF High
	PCA9685__PRE_SCALE,				//!< register PRE_SCALE
	PCA9685__TestMode				//!< register TestMode
} PCA9685_reg;


/*!\union uPCA9685_REG__MODE1
** \brief Union for MODE1 register of PCA9685
**/
typedef union PACK__ uPCA9685_REG__MODE1 {
	uint8_t Byte;
	struct PACK__ {
		uint8_t ALLCALL	:1;		//!< All CALL address acknowledgment
		uint8_t SUB3	:1;		//!< Sub-address 3 acknowledgment
		uint8_t SUB2	:1;		//!< Sub-address 2 acknowledgment
		uint8_t SUB1	:1;		//!< Sub-address 1 acknowledgment
		uint8_t SLEEP	:1;		//!< 0: Normal, 1: Low Power
		uint8_t AI		:1;		//!< Auto-increment bit (0: Disabled, 1: Enabled)
		uint8_t EXTCLK	:1;		//!< External Clock (0: Internal clock, 1: External clock)
		//!\warning This bit is a 'sticky bit', that is, it cannot be cleared by writing a logic 0 to it. The EXTCLK bit can only be cleared by a power cycle or software reset.
		uint8_t RESTART	:1;		//!< 0: Restart disabled, 1: Restart enabled
		//!\warning User writes logic 1 to this bit to clear it to logic 0. A user write of logic 0 will have no effect.
	} Bits;
} uPCA9685_REG__MODE1;

/*!\union uPCA9685_REG__MODE2
** \brief Union for MODE2 register of PCA9685
**/
typedef union PACK__ uPCA9685_REG__MODE2 {
	uint8_t Byte;
	struct PACK__ {
		uint8_t 		OUTNE	:2;
		uint8_t			OUTDRV	:1;		//!< 0: Open drain, 1: Totem Pole
		PCA96xx_latch	OCH		:1;		//!< 0: update on STOP, 1: update on ACK
		uint8_t			INVRT	:1;		//!< 0: Output logic not inverted, 1: Output logic inverted
		uint8_t 				:3;
	} Bits;
} uPCA9685_REG__MODE2;


/*!\union uPCA9685_REG__DUTY
** \brief Union for Duty Cycle registers of PCA9624
**/
typedef union PACK__ uPCA9685_REG__DUTY {
	uint16_t Word;
	struct PACK__ {
		uint8_t LSB;				//!< Less significant byte
		uint8_t MSB;				//!< Most significant byte
	} Bytes;
	struct PACK__ {
		uint16_t 	VAL		:12;	//!< Value
		uint16_t	FULL	:1;		//!< Full bit
		uint16_t 			:3;
	} Bits;
} uPCA9685_REG__DUTY;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for PCA9685 peripheral
** \param[in] idx - PCA9685 index
** \param[in] hi2c - pointer to PCA9685 I2C instance
** \param[in] devAddress - PCA9685 device address
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9685_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress);

/*!\brief Initialization for PCA9685 peripheral
** \warning In case multiple devices (defined by I2C_PCA9685_NB > 1), you shall use PCA9685_Init instead
** \return FctERR - error code
**/
FctERR PCA9685_Init_Single(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for PCA9685
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9685_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for PCA9685
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9685_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb);


/****************************************************************/
#include "PCA9685_proc.h"	// Include procedures
#include "PCA9685_ex.h"		// Include extensions

#endif

#ifdef __cplusplus
	}
#endif

#endif	/* __PCA9685_H__ */
/****************************************************************/
