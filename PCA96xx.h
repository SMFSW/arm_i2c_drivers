/*!\file PCA96xx.h
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
** \brief PCA96xx shared declarations
**/
/****************************************************************/
#ifndef __PCA96XX_H__
	#define __PCA96XX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
// PCA96xx general addresses
#define	PCA96xx_GENERAL_CALL_ADDR	0x00	//!< PCA96xx General CALL Address
#define	PCA96xx_ALL_CALL_ADDR		0xE0	//!< PCA96xx ALL CALL Address (for all channels)
#define	PCA96xx_SUB1_CALL_ADDR		0xE2	//!< PCA96xx SUB1 CALL Address
#define	PCA96xx_SUB2_CALL_ADDR		0xE4	//!< PCA96xx SUB2 CALL Address
#define	PCA96xx_SUB3_CALL_ADDR		0xE8	//!< PCA96xx SUB3 CALL Address


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*********************/
/*** PCA96xx enums ***/
/*********************/

/*!\enum PCA96xx_chan
** \brief Channels for PCA96xx
**/
typedef enum PACK__ PCA96xx_chan {
	PCA96xx__PWM1 = 0,		//!< PCA96xx Channel 1 PWM
	PCA96xx__PWM2,			//!< PCA96xx Channel 2 PWM
	PCA96xx__PWM3,			//!< PCA96xx Channel 3 PWM
	PCA96xx__PWM4,			//!< PCA96xx Channel 4 PWM
	PCA96xx__PWM5,			//!< PCA96xx Channel 5 PWM
	PCA96xx__PWM6,			//!< PCA96xx Channel 6 PWM
	PCA96xx__PWM7,			//!< PCA96xx Channel 7 PWM
	PCA96xx__PWM8,			//!< PCA96xx Channel 8 PWM
	PCA96xx__PWM9,			//!< PCA96xx Channel 9 PWM
	PCA96xx__PWM10,			//!< PCA96xx Channel 10 PWM
	PCA96xx__PWM11,			//!< PCA96xx Channel 11 PWM
	PCA96xx__PWM12,			//!< PCA96xx Channel 12 PWM
	PCA96xx__PWM13,			//!< PCA96xx Channel 13 PWM
	PCA96xx__PWM14,			//!< PCA96xx Channel 14 PWM
	PCA96xx__PWM15,			//!< PCA96xx Channel 15 PWM
	PCA96xx__PWM16,			//!< PCA96xx Channel 16 PWM
	PCA96xx__PWM17,			//!< PCA96xx Channel 17 PWM
	PCA96xx__PWM18,			//!< PCA96xx Channel 18 PWM
	PCA96xx__PWM19,			//!< PCA96xx Channel 19 PWM
	PCA96xx__PWM20,			//!< PCA96xx Channel 20 PWM
	PCA96xx__PWM21,			//!< PCA96xx Channel 21 PWM
	PCA96xx__PWM22,			//!< PCA96xx Channel 22 PWM
	PCA96xx__PWM23,			//!< PCA96xx Channel 23 PWM
	PCA96xx__PWM24,			//!< PCA96xx Channel 24 PWM
	PCA96xx__ALL = 0xFF		//!< PCA96xx All PWM Channels
} PCA96xx_chan;


/*!\enum PCA96xx_latch
** \brief Latch type enum for PCA96xx
**/
typedef enum PACK__ PCA96xx_latch {
	PCA96xx__LATCH_ON_STOP = 0,	//!< Duty cycle update on I2C stop
	PCA96xx__LATCH_ON_ACK		//!< Duty cycle update on I2C acknowledge
} PCA96xx_latch;


/*********************/
/*** PCA962x enums ***/
/*********************/

/*!\enum PCA962x_reg_inc
** \brief Registers increment options enum of PCA962x
**/
typedef enum PACK__ PCA962x_reg_inc {
	PCA962x__AUTO_INC_NONE = 0x00,				//!< no Auto-Increment
	PCA962x__AUTO_INC_ALL = 0x80,				//!< Auto-Increment for all registers
	PCA962x__AUTO_INC_BRIGHT = 0xA0,			//!< Auto-Increment for individual brightness registers only
	PCA962x__AUTO_INC_GLOBAL = 0xC0,			//!< Auto-Increment for global control registers only
	PCA962x__AUTO_INC_BRIGHT_GLOBAL = 0xE0,		//!< Auto-Increment for individual and global control registers only
} PCA962x_reg_inc;


/*!\enum PCA962x_ledout
** \brief enum for PCA962x output drive
**/
typedef enum PACK__ PCA962x_ledout {
	PCA962x__LED_DRV_OFF = 0,	//!< Off
	PCA962x__LED_DRV_FULL_ON,	//!< On
	PCA962x__INDIV_BRIGHT,		//!< Individual Dimming
	PCA962x__GROUP_BRIGHT		//!< Group Dimming
} PCA962x_ledout;

/*!\enum PCA962x_dim
** \brief enum for PCA962x dimming type
**/
typedef enum PACK__ PCA962x_dim {
	PCA962x__DIMMING = 0,	//!< Dimming
	PCA962x__BLINKING		//!< Blinking
} PCA962x_dim;


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif	/* __PCA96XX_H__ */
/****************************************************************/
