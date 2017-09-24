/*!\file PCA96xx.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief PCA96xx declarations (shared)
**/
/****************************************************************/
#ifndef __PCA96XX_H__
	#define __PCA96XX_H__

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
/*!\enum PCA96xx_channels
** \brief Channels for PCA96xx
**/
typedef enum PCA96xx_channels {
	PCA96xx__PWM1	= 1U,
	PCA96xx__PWM2	= 2U,
	PCA96xx__PWM3	= 3U,
	PCA96xx__PWM4	= 4U,
	PCA96xx__PWM5	= 5U,
	PCA96xx__PWM6	= 6U,
	PCA96xx__PWM7	= 7U,
	PCA96xx__PWM8	= 8U,
	PCA96xx__PWM9	= 9U,
	PCA96xx__PWM10	= 10U,
	PCA96xx__PWM11	= 11U,
	PCA96xx__PWM12	= 12U,
	PCA96xx__PWM13	= 13U,
	PCA96xx__PWM14	= 14U,
	PCA96xx__PWM15	= 15U,
	PCA96xx__PWM16	= 16U,
	PCA96xx__ALL	= 0xFF
} PCA96xx_chan;


/*!\enum PCA96xx_latchs
** \brief Latch type enum for PCA96xx
**/
typedef enum PACK__ PCA96xx_latchs {
	PCA96xx__LATCH_ON_STOP = 0,
	PCA96xx__LATCH_ON_ACK
} PCA96xx_latch;


/****************************************************************/
#endif
#endif	/* __PCA96XX_H__ */
/****************************************************************/
