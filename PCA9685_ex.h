/*!\file PCA9685_ex.h
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief PCA9685 Driver declarations (16 channels 16b PWM driver)
**/
/****************************************************************/
#ifndef __PCA9685_EX_H__
	#define __PCA9685_EX_H__

#include "PCA9685.h"

#include "sarmfsw.h"
#include CMSIS_INC
#include CMSIS_CFG

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define PCA9685_FREQ			500//82		//!< Frequency to affect to PCA9685 register (in Hz, range from 26 to 1700Hz)
//\warning Outputs frequency in Hz may range from 26 to 1700Hz

#define	PCA9685_FREQ_HZ_MIN		0x1AU		//!< Lower bound of interval for method PCA9685_Get_PWM_Prescaler
#define	PCA9685_FREQ_HZ_MAX		0x06A4U		//!< Upper bound of interval for method PCA9685_Get_PWM_Prescaler



#define DefValDelayON			2U			//!< Delay before output rising edge
//! \remark Must be less than 4 to be able to use a 1024 range

#define	DefBitFullOnOff			0x10U		//!< Full ON / OFF bit on xxx_LED_xxx_H


/** 4 registers per channel -> 2 shifts left for next address **/
#define	LED_OFFSET_H(chan)		(uint8_t) (PCA9685__LED0_ON_H + ((chan - 1) << 2))	//!< Macro for address offset computation ON_H for PWM channels
#define	LED_OFFSET_L(chan)		(uint8_t) (PCA9685__LED0_ON_L + ((chan - 1) << 2))	//!< Macro for address offset computation ON_L for PWM channels


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// High level functions
uint8_t PCA9685_Get_PWM_Prescaler(uint16_t fr);

FctERR PCA9685_Reset(bool all);
FctERR PCA9685_ReadRegister(PCA9685_reg Reg, uint8_t * Value);
FctERR PCA9685_ReadVal256(PCA96xx_chan Chan, uint8_t * DutyCycle);
FctERR PCA9685_ReadVal1024(PCA96xx_chan Chan, uint16_t * DutyCycle);
FctERR PCA9685_PutVal256(PCA96xx_chan Chan, uint8_t DutyCycle);
FctERR PCA9685_PutVal1024(PCA96xx_chan Chan, uint16_t DutyCycle);
FctERR PCA9685_SetVal(PCA96xx_chan Chan);
FctERR PCA9685_ClrVal(PCA96xx_chan Chan);

/*
FctERR PCA9685_ReadVal(PCA96xx_chan Chan, uint16_t * DutyCycle, uint8_t downscaler);
FctERR PCA9685_PutVal(PCA96xx_chan Chan, uint16_t DutyCycle, uint8_t upscaler);

__INLINE FctERR INLINE__ PCA9685_ReadVal256(PCA96xx_chan Chan, uint8_t * DutyCycle) {
	return PCA9685_ReadVal(Chan, (uint16_t *) DutyCycle, 4); }		// Doute sur la transformation du pointeur de 8b en pointeur de 16b pour écrire le résultat

__INLINE FctERR INLINE__ PCA9685_ReadVal1024(PCA96xx_chan Chan, uint16_t * DutyCycle) {
	return PCA9685_ReadVal(Chan, DutyCycle, 2); }

__INLINE FctERR INLINE__ PCA9685_ReadVal4096(PCA96xx_chan Chan, uint16_t * DutyCycle) {
	return PCA9685_ReadVal(Chan, DutyCycle, 0); }

__INLINE FctERR INLINE__ PCA9685_PutVal256(PCA96xx_chan Chan, uint8_t DutyCycle) {
}

__INLINE FctERR INLINE__ PCA9685_PutVal1024(PCA96xx_chan Chan, uint16_t DutyCycle) {
}*/


/****************************************************************/
#endif
#endif	/* __PCA9685_EX_H__ */
/****************************************************************/
