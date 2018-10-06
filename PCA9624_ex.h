/*!\file PCA9624_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief PCA9624 Driver extensions
** \details PCA9624: 8-bit Fm+ I2C-bus 100mA 40V LED driver
**/
/****************************************************************/
#ifndef __PCA9624_EX_H__
	#define __PCA9624_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "PCA9624.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Types
// *****************************************************************************
typedef union uPCA9624_REG_MAP {
	uint8_t Bytes[0x11];
	struct {
		uPCA9624_REG__MODE1		MODE1;
		uPCA9624_REG__MODE2		MODE2;
		uint8_t					PWM0;
		uint8_t					PWM1;
		uint8_t					PWM2;
		uint8_t					PWM3;
		uint8_t					PWM4;
		uint8_t					PWM5;
		uint8_t					PWM6;
		uint8_t					PWM7;
		uint8_t					GRPPWM;
		uint8_t					GRPFREQ;
		uPCA9624_REG__LEDOUT0	LEDOUT0;
		uPCA9624_REG__LEDOUT1	LEDOUT1;
		uint8_t					SUBADR1;
		uint8_t					SUBADR2;
		uint8_t					SUBADR3;
		uint8_t					ALLCALLADR;
	} Reg;
} uPCA9624_REG_MAP;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

FctERR PCA9624_Set_Latch(const PCA96xx_latch latch);

FctERR PCA9624_Set_Mode_LED(const PCA96xx_chan chan, const PCA9624_ledout mode);
FctERR PCA9624_Set_Mode_LEDs(const uint8_t chans, const PCA9624_ledout mode);


/*!\brief Reset for PCA9624 peripheral
** \param [in] all - Reset all devices
**/
FctERR PCA9624_Reset(const bool all);


/*!\brief Reads I2C register from PCA9624
** \param [in] reg - Register address to read from
** \param [in,out] val - Pointer to the data for receive
** \return FctERR - ErrorCode
**/
FctERR PCA9624_ReadRegister(const PCA9624_reg reg, uint8_t * val);


/*!\brief Reads I2C lighting values from a LED (4 bytes) and Computes the corresponding duty cycle value (uint8_t)
** \param [in] chan - channel number
** \param [in,out] duty - Pointer to the DutyCycle data for receive coded on a uint8_t
** \return FctERR - ErrorCode
**/
FctERR PCA9624_ReadVal(const PCA96xx_chan chan, uint8_t * duty);


/*!\brief Computes and send I2C lighting values to apply to a particular channel for PCA9624
** \param [in] chan - channel number
** \param [in] duty - Duty cycle coded on a uint8_t
** \return FctERR - ErrorCode
**/
FctERR PCA9624_PutVal(const PCA96xx_chan chan, uint8_t duty);


/*!\brief Sends I2C PWM ON values to apply to a particular channel for PCA9624
** \param [in] chan - channel number
** \return FctERR - ErrorCode
**/
FctERR PCA9624_SetVal(const PCA96xx_chan chan);


/*!\brief Sends I2C PWM OFF values to apply to a particular channel for PCA9624
** \param [in] chan - channel number
** \return FctERR - ErrorCode
**/
FctERR PCA9624_ClrVal(const PCA96xx_chan chan);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif	/* __PCA9624_EX_H__ */
/****************************************************************/
