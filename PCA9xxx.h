/*!\file PCA9xxx.h
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
** \brief PCA9xxx shared declarations
**/
/****************************************************************/
#ifndef __PCA9XXX_H__
	#define __PCA9XXX_H__

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

/*!\enum PCA9xxx_chan
** \brief Channels for PCA9xxx
**/
typedef enum PACK__ PCA9xxx_chan {
	PCA9xxx__PWM1 = 0,		//!< PCA9xxx Channel 1 PWM
	PCA9xxx__PWM2,			//!< PCA9xxx Channel 2 PWM
	PCA9xxx__PWM3,			//!< PCA9xxx Channel 3 PWM
	PCA9xxx__PWM4,			//!< PCA9xxx Channel 4 PWM
	PCA9xxx__PWM5,			//!< PCA9xxx Channel 5 PWM
	PCA9xxx__PWM6,			//!< PCA9xxx Channel 6 PWM
	PCA9xxx__PWM7,			//!< PCA9xxx Channel 7 PWM
	PCA9xxx__PWM8,			//!< PCA9xxx Channel 8 PWM
	PCA9xxx__PWM9,			//!< PCA9xxx Channel 9 PWM
	PCA9xxx__PWM10,			//!< PCA9xxx Channel 10 PWM
	PCA9xxx__PWM11,			//!< PCA9xxx Channel 11 PWM
	PCA9xxx__PWM12,			//!< PCA9xxx Channel 12 PWM
	PCA9xxx__PWM13,			//!< PCA9xxx Channel 13 PWM
	PCA9xxx__PWM14,			//!< PCA9xxx Channel 14 PWM
	PCA9xxx__PWM15,			//!< PCA9xxx Channel 15 PWM
	PCA9xxx__PWM16,			//!< PCA9xxx Channel 16 PWM
	PCA9xxx__PWM17,			//!< PCA9xxx Channel 17 PWM
	PCA9xxx__PWM18,			//!< PCA9xxx Channel 18 PWM
	PCA9xxx__PWM19,			//!< PCA9xxx Channel 19 PWM
	PCA9xxx__PWM20,			//!< PCA9xxx Channel 20 PWM
	PCA9xxx__PWM21,			//!< PCA9xxx Channel 21 PWM
	PCA9xxx__PWM22,			//!< PCA9xxx Channel 22 PWM
	PCA9xxx__PWM23,			//!< PCA9xxx Channel 23 PWM
	PCA9xxx__PWM24,			//!< PCA9xxx Channel 24 PWM
	PCA96xx__ALL = 0xFF		//!< PCA9xxx All PWM Channels
} PCA9xxx_chan;


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

/*!\enum PCA95xx_reg_inc
** \brief Registers increment options enum of PCA95xx
**/
typedef enum PACK__ PCA95xx_reg_inc {
	PCA95xx__AUTO_INC_NONE = 0x00,		//!< no Auto-Increment
	PCA95xx__AUTO_INC_ALL = 0x10,		//!< Auto-Increment for all registers
} PCA95xx_reg_inc;

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


/*!\enum PCA95xx_ledsel
** \brief enum for PCA95xx output drive
**/
typedef enum PACK__ PCA95xx_ledsel {
	PCA95xx__LED_OFF = 0,	//!< Output is set high-impedance (LED off; default)
	PCA95xx__LED_ON,		//!< Output is set LOW (LED on)
	PCA95xx__LED_PWM0,		//!< Output blinks at PWM0 rate
	PCA95xx__LED_PWM1		//!< Output blinks at PWM1 rate
} PCA95xx_ledsel;

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
#endif	/* __PCA9XXX_H__ */
/****************************************************************/
