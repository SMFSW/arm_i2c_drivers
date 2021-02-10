/*!\file PCA9624.h
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief PCA9624 Driver
** \details PCA9624: 8-bit Fm+ I2C-bus 100mA 40V LED driver
**/
/****************************************************************/
#ifndef __PCA9624_H__
	#define __PCA9624_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#include "PCA9xxx.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/

#ifndef I2C_PCA9624_NB
//! \note Define I2C_PCA9624_NB to enable multiple peripherals of this type
#define I2C_PCA9624_NB	1	//!< Number of PCA9624 peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
#ifndef PCA9624_BASE_ADDR
//! \note Define PCA9624_BASE_ADDR to change default device base address
//! \warning no real base default address
#define	PCA9624_BASE_ADDR			0x00		//!< PCA9624 Base Address
#endif

// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern I2C_slave_t PCA9624_hal[I2C_PCA9624_NB];	//!< PCA9624 Slave structure


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum PCA9624_reg_map
** \brief Register map enum of PCA9624
**/
typedef enum PACK__ PCA9624_reg_map {
	PCA9624__MODE1 = 0,		//!< register MODE1
	PCA9624__MODE2,			//!< register MODE2
	PCA9624__PWM0,			//!< register PWM0
	PCA9624__PWM1,			//!< register PWM1
	PCA9624__PWM2,			//!< register PWM2
	PCA9624__PWM3,			//!< register PWM3
	PCA9624__PWM4,			//!< register PWM4
	PCA9624__PWM5,			//!< register PWM5
	PCA9624__PWM6,			//!< register PWM6
	PCA9624__PWM7,			//!< register PWM7
	PCA9624__GRPPWM,		//!< register Group Duty Cycle Control
	PCA9624__GRPFREQ,		//!< register Group Frequency
	PCA9624__LEDOUT0,		//!< register LED output state 0
	PCA9624__LEDOUT1,		//!< register LED output state 1
	PCA9624__SUBADR1,		//!< register SUBADR1
	PCA9624__SUBADR2,		//!< register SUBADR2
	PCA9624__SUBADR3,		//!< register SUBADR3
	PCA9624__ALLCALLADR,	//!< register ALLCALLADR
} PCA9624_reg;


/*!\union uPCA9624_REG__MODE1
** \brief Union for MODE1 register of PCA9624
**/
typedef union PACK__ uPCA9624_REG__MODE1 {
	uint8_t Byte;
	struct PACK__ {
		uint8_t ALLCALL	:1;		//!< All CALL address acknowledgment
		uint8_t SUB3	:1;		//!< Sub-address 3 acknowledgment
		uint8_t SUB2	:1;		//!< Sub-address 2 acknowledgment
		uint8_t SUB1	:1;		//!< Sub-address 1 acknowledgment
		uint8_t SLEEP	:1;		//!< 0: Normal, 1: Low Power
		uint8_t AI0		:1;		//!< Auto-increment bit 0
		uint8_t AI1		:1;		//!< Auto-increment bit 1
		uint8_t AI2		:1;		//!< Register auto-increment
	} Bits;
} uPCA9624_REG__MODE1;

/*!\union uPCA9624_REG__MODE2
** \brief Union for MODE2 register of PCA9624
**/
typedef union PACK__ uPCA9624_REG__MODE2 {
	uint8_t Byte;
	struct PACK__ {
		uint8_t 				:3;
		PCA96xx_latch	OCH		:1;		//!< 0: update on STOP, 1: update on ACK
		uint8_t			INVRT	:1;		//!< Reserved, Write must always be a logic 0
		PCA962x_dim		DMBLNK	:1;		//!< 0: Dimming, 1: Blinking
		uint8_t 				:2;
	} Bits;
} uPCA9624_REG__MODE2;


/*!\union uPCA9624_REG__LEDOUT
** \brief Union for LEDOUT registers of PCA9624
**/
typedef union PACK__ uPCA9624_REG__LEDOUT {
	uint16_t Word;
	struct PACK__ {
		PCA962x_ledout LDR0	:2;		//!< LED0 output state control
		PCA962x_ledout LDR1	:2;		//!< LED1 output state control
		PCA962x_ledout LDR2	:2;		//!< LED2 output state control
		PCA962x_ledout LDR3	:2;		//!< LED3 output state control
		PCA962x_ledout LDR4	:2;		//!< LED4 output state control
		PCA962x_ledout LDR5	:2;		//!< LED5 output state control
		PCA962x_ledout LDR6	:2;		//!< LED6 output state control
		PCA962x_ledout LDR7	:2;		//!< LED7 output state control
	} Bits;
} uPCA9624_REG__LEDOUT;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for PCA9624 peripheral
** \param[in] idx - PCA9624 index
** \param[in] hi2c - pointer to PCA9624 I2C instance
** \param[in] devAddress - PCA9624 device address
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9624_Init(const uint8_t idx, const I2C_HandleTypeDef * hi2c, const uint16_t devAddress);

/*!\brief Initialization for PCA9624 peripheral
** \warning In case multiple devices (defined by I2C_PCA9624_NB > 1), you shall use PCA9624_Init instead
** \return FctERR - error code
**/
FctERR PCA9624_Init_Single(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for PCA9624
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9624_Write(I2C_slave_t * pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for PCA9624
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9624_Read(I2C_slave_t * pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb);


/****************************************************************/
#include "PCA9624_proc.h"	// Include procedures
#include "PCA9624_ex.h"		// Include extensions

#endif

#ifdef __cplusplus
	}
#endif

#endif	/* __PCA9624_H__ */
/****************************************************************/
