/*!\file PCA9685_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief PCA9685 Driver extensions
** \details PCA9685: 16-channel, 12-bit PWM Fm+ I2C-bus LED controller
**/
/****************************************************************/
#ifndef __PCA9685_EX_H__
	#define __PCA9685_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "PCA9685.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define DefValDelayON			2U			//!< Delay before output rising edge
//! \remark Must be less than 4 to be able to use a 1024 range

#define	DefBitFullOnOff			0x10U		//!< Full ON / OFF bit on xxx_LED_xxx_H


/** 4 registers per channel -> 2 shifts left for next address **/
#define	LED_OFFSET_H(chan)		(uint8_t) (PCA9685__LED0_ON_H + ((chan - 1) << 2))	//!< Macro for address offset computation ON_H for PWM channels
#define	LED_OFFSET_L(chan)		(uint8_t) (PCA9685__LED0_ON_L + ((chan - 1) << 2))	//!< Macro for address offset computation ON_L for PWM channels


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

/*!\brief Reset for PCA9685 peripheral
** \param[in] pCpnt - Pointer to PCA9685 component
**/
FctERR NONNULL__ PCA9685_Reset(PCA9685_t * pCpnt);

/*!\brief General call reset function for PCA9685
** \param[in] hi2c - pointer to general call I2C instance
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9685_Reset_All(const I2C_HandleTypeDef * hi2c);


/*!\brief Reads I2C register from PCA9685
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in] reg - Register address to read from
** \param[in,out] val - Pointer to the data for receive
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9685_ReadRegister(PCA9685_t * pCpnt, const PCA9685_reg reg, uint8_t * val);


/*!\brief Reads I2C lighting values from a LED (4 bytes) and Computes the corresponding duty cycle value (uint8_t)
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in] chan - chan number (1 to 16)
** \param[in,out] duty - Pointer to the DutyCycle data for receive coded on a uint8_t
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9685_ReadVal256(PCA9685_t * pCpnt, const PCA96xx_chan chan, uint8_t * duty);


/*!\brief Reads I2C lighting values from a LED (4 bytes) and Computes the corresponding duty cycle value (uint16_t)
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in] chan - Channel number (1 to 16)
** \param[in,out] duty - Pointer to the DutyCycle data for receive coded on a uint16_t
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9685_ReadVal1024(PCA9685_t * pCpnt, const PCA96xx_chan chan, uint16_t * duty);


/*!\brief Computes and send I2C lighting values to apply to a particular or all channels for PCA9685
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in] chan - Channel number (1 to 16 / PCA9685_ALLCALL can be used to address all channels at the same time)
** \param[in] duty - Duty cycle coded on a uint8_t
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9685_PutVal256(PCA9685_t * pCpnt, const PCA96xx_chan chan, const uint8_t duty);


/*!\brief Computes and send I2C lighting values to apply to a particular or all channels for PCA9685
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in] chan - Channel number (1 to 16 / PCA9685_ALLCALL can be used to address all channels at the same time)
** \param[in] duty - Duty cycle coded on a uint16_t
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9685_PutVal1024(PCA9685_t * pCpnt, const PCA96xx_chan chan, const uint16_t duty);


/*!\brief Sends I2C lighting ON values to apply to a particular or all channels for PCA9685
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in] chan - Channel number (1 to 16 / PCA9685_ALLCALL can be used to address all channels at the same time)
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9685_SetVal(PCA9685_t * pCpnt, const PCA96xx_chan chan);

/*!\brief Sends I2C PWm OFF values to apply to a particular or all channels for PCA9685
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in] chan - Channel number (1 to 16 / PCA9685_ALLCALL can be used to address all channels at the same time)
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9685_ClrVal(PCA9685_t * pCpnt, const PCA96xx_chan chan);

/*
FctERR NONNULL__ PCA9685_ReadVal(PCA9685_t * pCpnt, const PCA96xx_chan chan, uint16_t * duty, const uint8_t downscale);
FctERR NONNULL__ PCA9685_PutVal(PCA9685_t * pCpnt, const PCA96xx_chan chan, const uint16_t duty, const uint8_t upscale);

__INLINE FctERR NONNULL_INLINE__ PCA9685_ReadVal256(PCA9685_t * pCpnt, const PCA96xx_chan chan, uint8_t * duty) {
	return PCA9685_ReadVal(pCpnt, chan, (uint16_t *) duty, 4); }		// Doute sur la transformation du pointeur de 8b en pointeur de 16b pour écrire le résultat

__INLINE FctERR NONNULL_INLINE__ PCA9685_ReadVal1024(PCA9685_t * pCpnt, const PCA96xx_chan chan, uint16_t * duty) {
	return PCA9685_ReadVal(pCpnt, chan, duty, 2); }

__INLINE FctERR NONNULL_INLINE__ PCA9685_ReadVal4096(PCA9685_t * pCpnt, const PCA96xx_chan chan, uint16_t * duty) {
	return PCA9685_ReadVal(pCpnt, chan, duty, 0); }

__INLINE FctERR NONNULL_INLINE__ PCA9685_PutVal256(PCA9685_t * pCpnt, const PCA96xx_chan chan, const uint8_t duty) {
}

__INLINE FctERR NONNULL_INLINE__ PCA9685_PutVal1024(PCA9685_t * pCpnt, const PCA96xx_chan chan, const uint16_t duty) {
}*/


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif	/* __PCA9685_EX_H__ */
/****************************************************************/
