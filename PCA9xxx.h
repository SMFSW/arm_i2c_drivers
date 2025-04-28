/*!\file PCA9xxx.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief PCA9xxx shared declarations
**/
/****************************************************************/
#ifndef PCA9XXX_H__
	#define PCA9XXX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
// PCA9xxx general addresses
#define	PCA9xxx_DEFAULT_ALL_CALL_ADDR		(0xE0 >> 1)		//!< PCA9xxx ALL CALL Address (for all channels)


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
	PCA9xxx__ALL = 0xFF		//!< PCA9xxx All PWM Channels
} PCA9xxx_chan;


/*!\enum PCA9xxx_latch
** \brief Latch type enum for PCA9xxx
**/
typedef enum PACK__ PCA9xxx_latch {
	PCA9xxx__LATCH_ON_STOP = 0,	//!< Duty cycle update on I2C stop
	PCA9xxx__LATCH_ON_ACK		//!< Duty cycle update on I2C acknowledge
} PCA9xxx_latch;

typedef PCA9xxx_latch		PCA96xx_latch;	//!< Latch type enum for PCA96xx
typedef PCA9xxx_latch		PCA99xx_latch;	//!< Latch type enum for PCA99xx


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

/*!\enum PCA9xxx_reg_inc
** \brief Registers increment options enum of PCA9xxx
**/
typedef enum PACK__ PCA9xxx_reg_inc {
	PCA9xxx__AUTO_INC_NONE = 0x00,				//!< no Auto-Increment
	PCA9xxx__AUTO_INC_ALL = 0x80,				//!< Auto-Increment for all registers
	PCA9xxx__AUTO_INC_BRIGHT = 0xA0,			//!< Auto-Increment for individual brightness registers only
	PCA9xxx__AUTO_INC_GLOBAL = 0xC0,			//!< Auto-Increment for global control registers only
	PCA9xxx__AUTO_INC_BRIGHT_GLOBAL = 0xE0,		//!< Auto-Increment for individual and global control registers only
} PCA9xxx_reg_inc;

typedef PCA9xxx_reg_inc		PCA962x_reg_inc;	//!< Registers increment options enum of PCA962x
typedef PCA9xxx_reg_inc		PCA995x_reg_inc;	//!< Registers increment options enum of PCA995x

/*!\enum PCA95xx_ledsel
** \brief enum for PCA95xx output drive
**/
typedef enum PACK__ PCA95xx_ledsel {
	PCA95xx__LED_OFF = 0,	//!< Output is set high-impedance (LED off; default)
	PCA95xx__LED_ON,		//!< Output is set LOW (LED on)
	PCA95xx__LED_PWM0,		//!< Output blinks at PWM0 rate
	PCA95xx__LED_PWM1		//!< Output blinks at PWM1 rate
} PCA95xx_ledsel;

/*!\enum PCA9xxx_dim
** \brief enum for PCA9xxx dimming type
**/
typedef enum PACK__ PCA9xxx_dim {
	PCA9xxx__DIMMING = 0,	//!< Dimming
	PCA9xxx__BLINKING		//!< Blinking
} PCA9xxx_dim;

typedef PCA9xxx_dim		PCA962x_dim;	//!< enum for PCA962x dimming type
typedef PCA9xxx_dim		PCA995x_dim;	//!< enum for PCA995x dimming type


/*!\enum PCA9xxx_ledout
** \brief enum for PCA9xxx output drive
**/
typedef enum PACK__ PCA9xxx_ledout {
	PCA9xxx__LED_DRV_OFF = 0,	//!< Off
	PCA9xxx__LED_DRV_FULL_ON,	//!< On
	PCA9xxx__INDIV_BRIGHT,		//!< Individual Dimming
	PCA9xxx__GROUP_BRIGHT		//!< Group Dimming
} PCA9xxx_ledout;

typedef PCA9xxx_ledout		PCA962x_ledout;	//!< enum for PCA962x output drive
typedef PCA9xxx_ledout		PCA995x_ledout;	//!< enum for PCA995x output drive


/*!\union uPCA9xxx_REG__LEDOUTx
** \brief Union for LEDOUTx register of PCA9xxx
**/
typedef union PACK__ uPCA9xxx_REG__LEDOUTx {
	uint8_t Byte;
	struct PACK__ {
		PCA9xxx_ledout LDRx0	:2;		//!< LEDx + 0 output state control
		PCA9xxx_ledout LDRx1	:2;		//!< LEDx + 1 output state control
		PCA9xxx_ledout LDRx2	:2;		//!< LEDx + 2 output state control
		PCA9xxx_ledout LDRx3	:2;		//!< LEDx + 3 output state control
	} Bits;
} uPCA9xxx_REG__LEDOUTx;

typedef uPCA9xxx_REG__LEDOUTx		uPCA962x_REG__LEDOUTx;	//!< Union for LEDOUTx register of PCA962x
typedef uPCA9xxx_REG__LEDOUTx		uPCA995x_REG__LEDOUTx;	//!< Union for LEDOUTx register of PCA995x


/*!\union uPCA9xxx_REG__LEDOUT0
** \brief Union for LEDOUT0 register of PCA9xxx
**/
typedef union PACK__ uPCA9xxx_REG__LEDOUT0 {
	uint8_t Byte;
	struct PACK__ {
		PCA9xxx_ledout LDR0		:2;		//!< LED0 output state control
		PCA9xxx_ledout LDR1		:2;		//!< LED1 output state control
		PCA9xxx_ledout LDR2		:2;		//!< LED2 output state control
		PCA9xxx_ledout LDR3		:2;		//!< LED3 output state control
	} Bits;
} uPCA9xxx_REG__LEDOUT0;

typedef uPCA9xxx_REG__LEDOUT0		uPCA962x_REG__LEDOUT0;	//!< Union for LEDOUT0 register of PCA962x
typedef uPCA9xxx_REG__LEDOUT0		uPCA995x_REG__LEDOUT0;	//!< Union for LEDOUT0 register of PCA995x


/*!\union uPCA9xxx_REG__LEDOUT1
** \brief Union for LEDOUT1 register of PCA9xxx
**/
typedef union PACK__ uPCA9xxx_REG__LEDOUT1 {
	uint8_t Byte;
	struct PACK__ {
		PCA9xxx_ledout LDR4		:2;		//!< LED4 output state control
		PCA9xxx_ledout LDR5		:2;		//!< LED5 output state control
		PCA9xxx_ledout LDR6		:2;		//!< LED6 output state control
		PCA9xxx_ledout LDR7		:2;		//!< LED7 output state control
	} Bits;
} uPCA9xxx_REG__LEDOUT1;

typedef uPCA9xxx_REG__LEDOUT1		uPCA962x_REG__LEDOUT1;	//!< Union for LEDOUT1 register of PCA962x
typedef uPCA9xxx_REG__LEDOUT1		uPCA995x_REG__LEDOUT1;	//!< Union for LEDOUT1 register of PCA995x


/*!\union uPCA9xxx_REG__LEDOUT2
** \brief Union for LEDOUT2 register of PCA9xxx
**/
typedef union PACK__ uPCA9xxx_REG__LEDOUT2 {
	uint8_t Byte;
	struct PACK__ {
		PCA9xxx_ledout LDR8		:2;		//!< LED8 output state control
		PCA9xxx_ledout LDR9		:2;		//!< LED9 output state control
		PCA9xxx_ledout LDR10	:2;		//!< LED10 output state control
		PCA9xxx_ledout LDR11	:2;		//!< LED11 output state control
	} Bits;
} uPCA9xxx_REG__LEDOUT2;

typedef uPCA9xxx_REG__LEDOUT2		uPCA962x_REG__LEDOUT2;	//!< Union for LEDOUT2 register of PCA962x
typedef uPCA9xxx_REG__LEDOUT2		uPCA995x_REG__LEDOUT2;	//!< Union for LEDOUT2 register of PCA995x


/*!\union uPCA9xxx_REG__LEDOUT3
** \brief Union for LEDOUT3 register of PCA9xxx
**/
typedef union PACK__ uPCA9xxx_REG__LEDOUT3 {
	uint8_t Byte;
	struct PACK__ {
		PCA9xxx_ledout LDR12	:2;		//!< LED12 output state control
		PCA9xxx_ledout LDR13	:2;		//!< LED13 output state control
		PCA9xxx_ledout LDR14	:2;		//!< LED14 output state control
		PCA9xxx_ledout LDR15	:2;		//!< LED15 output state control
	} Bits;
} uPCA9xxx_REG__LEDOUT3;

typedef uPCA9xxx_REG__LEDOUT3		uPCA962x_REG__LEDOUT3;	//!< Union for LEDOUT3 register of PCA962x
typedef uPCA9xxx_REG__LEDOUT3		uPCA995x_REG__LEDOUT3;	//!< Union for LEDOUT3 register of PCA995x


/*!\union uPCA9xxx_REG__LEDOUT4
** \brief Union for LEDOUT4 register of PCA9xxx
**/
typedef union PACK__ uPCA9xxx_REG__LEDOUT4 {
	uint8_t Byte;
	struct PACK__ {
		PCA9xxx_ledout LDR16	:2;		//!< LED16 output state control
		PCA9xxx_ledout LDR17	:2;		//!< LED17 output state control
		PCA9xxx_ledout LDR18	:2;		//!< LED18 output state control
		PCA9xxx_ledout LDR19	:2;		//!< LED19 output state control
	} Bits;
} uPCA9xxx_REG__LEDOUT4;

typedef uPCA9xxx_REG__LEDOUT4		uPCA995x_REG__LEDOUT4;	//!< Union for LEDOUT4 register of PCA995x


/*!\union uPCA9xxx_REG__LEDOUT5
** \brief Union for LEDOUT5 register of PCA9xxx
**/
typedef union PACK__ uPCA9xxx_REG__LEDOUT5 {
	uint8_t Byte;
	struct PACK__ {
		PCA9xxx_ledout LDR20	:2;		//!< LED20 output state control
		PCA9xxx_ledout LDR21	:2;		//!< LED21 output state control
		PCA9xxx_ledout LDR22	:2;		//!< LED22 output state control
		PCA9xxx_ledout LDR23	:2;		//!< LED23 output state control
	} Bits;
} uPCA9xxx_REG__LEDOUT5;

typedef uPCA9xxx_REG__LEDOUT5		uPCA995x_REG__LEDOUT5;	//!< Union for LEDOUT5 register of PCA995x


/*!\enum PCA9xxx_eflag
** \brief enum for PCA9xxx error status
**/
typedef enum PACK__ PCA9xxx_eflag {
	PCA9xxx__ERR_OK = 0,	//!< In normal operation and no error
	PCA9xxx__ERR_SHORT,		//!< Detected LED short-circuit condition
	PCA9xxx__ERR_OPEN,		//!< Detected LED open-circuit condition
	PCA9xxx__ERR_DNE		//!< This condition does not exist
} PCA9xxx_eflag;

typedef PCA9xxx_eflag		PCA9956_eflag;	//!< enum for PCA9965 error status


/*!\union uPCA9xxx_REG__EFLAGx
** \brief Union for EFLAGx register of PCA9xxx
**/
typedef union PACK__ uPCA9xxx_REG__EFLAGx {
	uint8_t Byte;
	struct PACK__ {
		PCA9xxx_eflag ERRx0	:2;		//!< LEDx + 0 error status
		PCA9xxx_eflag ERRx1	:2;		//!< LEDx + 1 error status
		PCA9xxx_eflag ERRx2	:2;		//!< LEDx + 2 error status
		PCA9xxx_eflag ERRx3	:2;		//!< LEDx + 3 error status
	} Bits;
} uPCA9xxx_REG__EFLAGx;

typedef uPCA9xxx_REG__EFLAGx		uPCA995x_REG__EFLAGx;	//!< Union for EFLAGx register of PCA995x


/*!\union uPCA9xxx_REG__EFLAG0
** \brief Union for EFLAG0 register of PCA9xxx
**/
typedef union PACK__ uPCA9xxx_REG__EFLAG0 {
	uint8_t Byte;
	struct PACK__ {
		PCA9xxx_eflag ERR0		:2;		//!< LED0 error status
		PCA9xxx_eflag ERR1		:2;		//!< LED1 error status
		PCA9xxx_eflag ERR2		:2;		//!< LED2 error status
		PCA9xxx_eflag ERR3		:2;		//!< LED3 error status
	} Bits;
} uPCA9xxx_REG__EFLAG0;

typedef uPCA9xxx_REG__EFLAG0		uPCA995x_REG__EFLAG0;	//!< Union for EFLAG0 register of PCA995x


/*!\union uPCA9xxx_REG__EFLAG1
** \brief Union for EFLAG1 register of PCA9xxx
**/
typedef union PACK__ uPCA9xxx_REG__EFLAG1 {
	uint8_t Byte;
	struct PACK__ {
		PCA9xxx_eflag ERR4		:2;		//!< LED4 error status
		PCA9xxx_eflag ERR5		:2;		//!< LED5 error status
		PCA9xxx_eflag ERR6		:2;		//!< LED6 error status
		PCA9xxx_eflag ERR7		:2;		//!< LED7 error status
	} Bits;
} uPCA9xxx_REG__EFLAG1;

typedef uPCA9xxx_REG__EFLAG1		uPCA995x_REG__EFLAG1;	//!< Union for EFLAG1 register of PCA995x


/*!\union uPCA9xxx_REG__EFLAG2
** \brief Union for EFLAG2 register of PCA9xxx
**/
typedef union PACK__ uPCA9xxx_REG__EFLAG2 {
	uint8_t Byte;
	struct PACK__ {
		PCA9xxx_eflag ERR8		:2;		//!< LED8 error status
		PCA9xxx_eflag ERR9		:2;		//!< LED9 error status
		PCA9xxx_eflag ERR10		:2;		//!< LED10 error status
		PCA9xxx_eflag ERR11		:2;		//!< LED11 error status
	} Bits;
} uPCA9xxx_REG__EFLAG2;

typedef uPCA9xxx_REG__EFLAG2		uPCA995x_REG__EFLAG2;	//!< Union for EFLAG2 register of PCA995x


/*!\union uPCA9xxx_REG__EFLAG3
** \brief Union for EFLAG3 register of PCA9xxx
**/
typedef union PACK__ uPCA9xxx_REG__EFLAG3 {
	uint8_t Byte;
	struct PACK__ {
		PCA9xxx_eflag ERR12		:2;		//!< LED12 error status
		PCA9xxx_eflag ERR13		:2;		//!< LED13 error status
		PCA9xxx_eflag ERR14		:2;		//!< LED14 error status
		PCA9xxx_eflag ERR15		:2;		//!< LED15 error status
	} Bits;
} uPCA9xxx_REG__EFLAG3;

typedef uPCA9xxx_REG__EFLAG3		uPCA995x_REG__EFLAG3;	//!< Union for EFLAG3 register of PCA995x


/*!\union uPCA9xxx_REG__EFLAG4
** \brief Union for EFLAG4 register of PCA9xxx
**/
typedef union PACK__ uPCA9xxx_REG__EFLAG4 {
	uint8_t Byte;
	struct PACK__ {
		PCA9xxx_eflag ERR16		:2;		//!< LED16 error status
		PCA9xxx_eflag ERR17		:2;		//!< LED17 error status
		PCA9xxx_eflag ERR18		:2;		//!< LED18 error status
		PCA9xxx_eflag ERR19		:2;		//!< LED19 error status
	} Bits;
} uPCA9xxx_REG__EFLAG4;

typedef uPCA9xxx_REG__EFLAG4		uPCA995x_REG__EFLAG4;	//!< Union for EFLAG4 register of PCA995x


/*!\union uPCA9xxx_REG__EFLAG5
** \brief Union for EFLAG5 register of PCA9xxx
**/
typedef union PACK__ uPCA9xxx_REG__EFLAG5 {
	uint8_t Byte;
	struct PACK__ {
		PCA9xxx_eflag ERR20		:2;		//!< LED20 error status
		PCA9xxx_eflag ERR21		:2;		//!< LED21 error status
		PCA9xxx_eflag ERR22		:2;		//!< LED22 error status
		PCA9xxx_eflag ERR23		:2;		//!< LED23 error status
	} Bits;
} uPCA9xxx_REG__EFLAG5;

typedef uPCA9xxx_REG__EFLAG5		uPCA995x_REG__EFLAG5;	//!< Union for EFLAG5 register of PCA995x


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif	/* PCA9XXX_H__ */
/****************************************************************/
