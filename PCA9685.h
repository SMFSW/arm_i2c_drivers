/*!\file PCA9685.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief PCA9685 Driver declarations
** \details PCA9685: 16-channel, 12-bit PWM Fm+ I2C-bus LED controller
**/
/****************************************************************/
#ifndef __PCA9685_H__
	#define __PCA9685_H__

#include "I2C_component.h"
#include "PCA96xx.h"
#include "FctERR.h"

#include "sarmfsw.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define	PCA9685_BASE_ADDR		0x40		//!< PCA9685 Base Address


// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern I2C_slave PCA9685_hal;	//!< PCA9685 Slave instance


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum PCA9685_reg_map
** \brief Register map enum of PCA9685
**/
typedef enum PACK__ PCA9685_reg_map {
	PCA9685__MODE1			= 0x00U,	//!< register MODE1
	PCA9685__MODE2			= 0x01U,	//!< register MODE2
	PCA9685__SUBADR1		= 0x02U,	//!< register SUBADR1
	PCA9685__SUBADR2		= 0x03U,	//!< register SUBADR2
	PCA9685__SUBADR3		= 0x04U,	//!< register SUBADR3
	PCA9685__ALLCALLADR		= 0x05U,	//!< register ALLCALLADR
	PCA9685__LED0_ON_L		= 0x06U,	//!< register LED 0 ON Low
	PCA9685__LED0_ON_H		= 0x07U,	//!< register LED 0 ON High
	PCA9685__LED0_OFF_L		= 0x08U,	//!< register LED 0 OFF Low
	PCA9685__LED0_OFF_H		= 0x09U,	//!< register LED 0 OFF High
	PCA9685__LED1_ON_L		= 0x0AU,	//!< register LED 1 ON Low
	PCA9685__LED1_ON_H		= 0x0BU,	//!< register LED 1 ON High
	PCA9685__LED1_OFF_L		= 0x0CU,	//!< register LED 1 OFF Low
	PCA9685__LED1_OFF_H		= 0x0DU,	//!< register LED 1 OFF High
	PCA9685__LED2_ON_L		= 0x0EU,	//!< register LED 2 ON Low
	PCA9685__LED2_ON_H		= 0x0FU,	//!< register LED 2 ON High
	PCA9685__LED2_OFF_L		= 0x10U,	//!< register LED 2 OFF Low
	PCA9685__LED2_OFF_H		= 0x11U,	//!< register LED 2 OFF High
	PCA9685__LED3_ON_L		= 0x12U,	//!< register LED 3 ON Low
	PCA9685__LED3_ON_H		= 0x13U,	//!< register LED 3 ON High
	PCA9685__LED3_OFF_L		= 0x14U,	//!< register LED 3 OFF Low
	PCA9685__LED3_OFF_H		= 0x15U,	//!< register LED 3 OFF High
	PCA9685__LED4_ON_L		= 0x16U,	//!< register LED 4 ON Low
	PCA9685__LED4_ON_H		= 0x17U,	//!< register LED 4 ON High
	PCA9685__LED4_OFF_L		= 0x18U,	//!< register LED 4 OFF Low
	PCA9685__LED4_OFF_H		= 0x19U,	//!< register LED 4 OFF High
	PCA9685__LED5_ON_L		= 0x1AU,	//!< register LED 5 ON Low
	PCA9685__LED5_ON_H		= 0x1BU,	//!< register LED 5 ON High
	PCA9685__LED5_OFF_L		= 0x1CU,	//!< register LED 5 OFF Low
	PCA9685__LED5_OFF_H		= 0x1DU,	//!< register LED 5 OFF High
	PCA9685__LED6_ON_L		= 0x1EU,	//!< register LED 6 ON Low
	PCA9685__LED6_ON_H		= 0x1FU,	//!< register LED 6 ON High
	PCA9685__LED6_OFF_L		= 0x20U,	//!< register LED 6 OFF Low
	PCA9685__LED6_OFF_H		= 0x21U,	//!< register LED 6 OFF High
	PCA9685__LED7_ON_L		= 0x22U,	//!< register LED 7 ON Low
	PCA9685__LED7_ON_H		= 0x23U,	//!< register LED 7 ON High
	PCA9685__LED7_OFF_L		= 0x24U,	//!< register LED 7 OFF Low
	PCA9685__LED7_OFF_H		= 0x25U,	//!< register LED 7 OFF High
	PCA9685__LED8_ON_L		= 0x26U,	//!< register LED 8 ON Low
	PCA9685__LED8_ON_H		= 0x27U,	//!< register LED 8 ON High
	PCA9685__LED8_OFF_L		= 0x28U,	//!< register LED 8 OFF Low
	PCA9685__LED8_OFF_H		= 0x29U,	//!< register LED 8 OFF High
	PCA9685__LED9_ON_L		= 0x2AU,	//!< register LED 9 ON Low
	PCA9685__LED9_ON_H		= 0x2BU,	//!< register LED 9 ON High
	PCA9685__LED9_OFF_L		= 0x2CU,	//!< register LED 9 OFF Low
	PCA9685__LED9_OFF_H		= 0x2DU,	//!< register LED 9 OFF High
	PCA9685__LED10_ON_L		= 0x2EU,	//!< register LED 10 ON Low
	PCA9685__LED10_ON_H		= 0x2FU,	//!< register LED 10 ON High
	PCA9685__LED10_OFF_L	= 0x30U,	//!< register LED 10 OFF Low
	PCA9685__LED10_OFF_H	= 0x31U,	//!< register LED 10 OFF High
	PCA9685__LED11_ON_L		= 0x32U,	//!< register LED 11 ON Low
	PCA9685__LED11_ON_H		= 0x33U,	//!< register LED 11 ON High
	PCA9685__LED11_OFF_L	= 0x34U,	//!< register LED 11 OFF Low
	PCA9685__LED11_OFF_H	= 0x35U,	//!< register LED 11 OFF High
	PCA9685__LED12_ON_L		= 0x36U,	//!< register LED 12 ON Low
	PCA9685__LED12_ON_H		= 0x37U,	//!< register LED 12 ON High
	PCA9685__LED12_OFF_L	= 0x38U,	//!< register LED 12 OFF Low
	PCA9685__LED12_OFF_H	= 0x39U,	//!< register LED 12 OFF High
	PCA9685__LED13_ON_L		= 0x3AU,	//!< register LED 13 ON Low
	PCA9685__LED13_ON_H		= 0x3BU,	//!< register LED 13 ON High
	PCA9685__LED13_OFF_L	= 0x3CU,	//!< register LED 13 OFF Low
	PCA9685__LED13_OFF_H	= 0x3DU,	//!< register LED 13 OFF High
	PCA9685__LED14_ON_L		= 0x3EU,	//!< register LED 14 ON Low
	PCA9685__LED14_ON_H		= 0x3FU,	//!< register LED 14 ON High
	PCA9685__LED14_OFF_L	= 0x40U,	//!< register LED 14 OFF Low
	PCA9685__LED14_OFF_H	= 0x41U,	//!< register LED 14 OFF High
	PCA9685__LED15_ON_L		= 0x42U,	//!< register LED 15 ON Low
	PCA9685__LED15_ON_H		= 0x43U,	//!< register LED 15 ON High
	PCA9685__LED15_OFF_L	= 0x44U,	//!< register LED 15 OFF Low
	PCA9685__LED15_OFF_H	= 0x45U,	//!< register LED 15 OFF High
	PCA9685__ALL_LED_ON_L	= 0xFAU,	//!< register ALL LED ON Low
	PCA9685__ALL_LED_ON_H	= 0xFBU,	//!< register ALL LED ON High
	PCA9685__ALL_LED_OFF_L	= 0xFCU,	//!< register ALL LED OFF Low
	PCA9685__ALL_LED_OFF_H	= 0xFDU,	//!< register ALL LED OFF High
	PCA9685__PRE_SCALE		= 0xFEU,	//!< register PRE_SCALE
	PCA9685__TestMode		= 0xFFU		//!< register TestMode
} PCA9685_reg;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for PCA9685 peripheral
** \return FctERR - error code
**/
FctERR PCA9685_Init(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for PCA9685
**
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR PCA9685_Write(uint8_t * data, uint16_t addr, uint16_t nb);


/*!\brief I2C Read function for PCA9685
**
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR PCA9685_Read(uint8_t * data, uint16_t addr, uint16_t nb);


/****************************************************************/
#include "PCA9685_ex.h"		// Include extensions
#include "PCA9685_proc.h"	// Include procedures
#endif
#endif	/* __PCA9685_H__ */
/****************************************************************/
