/*!\file PCA9624.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
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
#include "globals.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#include "PCA96xx.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/
// TODO: doxygen

#ifndef I2C_PCA9624_NB
//! \note Define I2C_PCA9624_NB in globals.h or at project to enable multiple peripherals of this type
#define I2C_PCA9624_NB	1	//!< Number of PCA9624 peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define	PCA9624_BASE_ADDR			0x00		//!< PCA9624 Base Address
//\warning no real base default address


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
	PCA9624__MODE1		= 0x00U,	//!< register MODE1
	PCA9624__MODE2		= 0x01U,	//!< register MODE2
	PCA9624__PWM0		= 0x02U,	//!< register PWM0
	PCA9624__PWM1		= 0x03U,	//!< register PWM1
	PCA9624__PWM2		= 0x04U,	//!< register PWM2
	PCA9624__PWM3		= 0x05U,	//!< register PWM3
	PCA9624__PWM4		= 0x06U,	//!< register PWM4
	PCA9624__PWM5		= 0x07U,	//!< register PWM5
	PCA9624__PWM6		= 0x08U,	//!< register PWM6
	PCA9624__PWM7		= 0x09U,	//!< register PWM7
	PCA9624__GRPPWM		= 0x0AU,	//!< register Group Duty Cycle Control
	PCA9624__GRPFREQ	= 0x0BU,	//!< register Group Frequency
	PCA9624__LEDOUT0	= 0x0CU,	//!< register LED output state 0
	PCA9624__LEDOUT1	= 0x0DU,	//!< register LED output state 1
	PCA9624__SUBADR1	= 0x0EU,	//!< register SUBADR1
	PCA9624__SUBADR2	= 0x0FU,	//!< register SUBADR2
	PCA9624__SUBADR3	= 0x10U,	//!< register SUBADR3
	PCA9624__ALLCALLADR	= 0x11U,	//!< register ALLCALLADR
} PCA9624_reg;

typedef enum PACK__ PCA9624_LEDOUT {
	PCA9624__LED_DRV_OFF = 0,
	PCA9624__LED_DRV_FULL_ON,
	PCA9624__INDIV_BRIGHT,
	PCA9624__GROUP_BRIGHT
} PCA9624_ledout;

typedef enum PACK__ PCA9624_DIM {
	PCA9624__DIMMING = 0,
	PCA9624__BLINKING
} PCA9624_dim;

typedef union uPCA9624_REG__MODE1 {
	uint8_t Byte;
	struct {
		uint8_t ALLCALL		:1;		//!< All CALL address acknowledgment
		uint8_t SUB3		:1;		//!< Sub-address 3 acknowledgment
		uint8_t SUB2		:1;		//!< Sub-address 2 acknowledgment
		uint8_t SUB1		:1;		//!< Sub-address 1 acknowledgment
		uint8_t SLEEP		:1;		//!< 0: Normal, 1: Low Power
		uint8_t AI0			:1;		//!< Auto-increment bit 0
		uint8_t AI1			:1;		//!< Auto-increment bit 1
		uint8_t AI2			:1;		//!< Register auto-increment
	} Bits;
} uPCA9624_REG__MODE1;

typedef union uPCA9624_REG__MODE2 {
	uint8_t Byte;
	struct {
		uint8_t 				:3;
		PCA96xx_latch	OCH		:1;		//!< 0: update on STOP, 1: update on ACK
		uint8_t			INVRT	:1;		//!< Reserved, Write must always be a logic 0
		PCA9624_dim		DMBLNK	:1;		//!< 0: Dimming, 1: Blinking
		uint8_t 				:2;
	} Bits;
} uPCA9624_REG__MODE2;

typedef union uPCA9624_REG__LEDOUT0 {
	uint8_t Byte;
	struct {
		PCA9624_ledout LDR0		:2;		//!< LED0 output state control
		PCA9624_ledout LDR1		:2;		//!< LED1 output state control
		PCA9624_ledout LDR2		:2;		//!< LED2 output state control
		PCA9624_ledout LDR3		:2;		//!< LED3 output state control
	} Bits;
} uPCA9624_REG__LEDOUT0;

typedef union uPCA9624_REG__LEDOUT1 {
	uint8_t Byte;
	struct {
		PCA9624_ledout LDR4		:2;		//!< LED4 output state control
		PCA9624_ledout LDR5		:2;		//!< LED5 output state control
		PCA9624_ledout LDR6		:2;		//!< LED6 output state control
		PCA9624_ledout LDR7		:2;		//!< LED7 output state control
	} Bits;
} uPCA9624_REG__LEDOUT1;


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

#ifdef __cplusplus
	}
#endif

#endif
#endif	/* __PCA9624_H__ */
/****************************************************************/
