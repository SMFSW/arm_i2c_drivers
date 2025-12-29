/*!\file PCA9685_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief PCA9685 Driver extensions
** \details PCA9685: 16-channel, 12-bit PWM Fm+ I2C-bus LED controller
**/
/****************************************************************/
#ifndef PCA9685_EX_H__
	#define PCA9685_EX_H__

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
#define	PCA9685_FREQ_HZ_MIN		24U			//!< Lower admissible frequency (when internal clock used)
#define	PCA9685_FREQ_HZ_MAX		1526U		//!< Upper admissible frequency (when internal clock used)

#define PCA9685_CLOCK_FREQ		25000000UL	//!< PCA9685 Internal clock frequency

#define	DefBitFullOnOff			0x10U		//!< Full ON / OFF bit on LEDxx_xxx_H

/** 4 registers per channel -> 2 shifts left for next address **/
#define	LED_OFFSET_H(chan)		((uint8_t) (PCA9685__LED0_ON_H + LSHIFT(chan, 2U)))	//!< Macro for address offset computation ON_H for PWM channels
#define	LED_OFFSET_L(chan)		((uint8_t) (PCA9685__LED0_ON_L + LSHIFT(chan, 2U)))	//!< Macro for address offset computation ON_L for PWM channels


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Computes values for PCA9685 registers (4bytes) following duty and delay
** \note Useful to manually fill an array with registers values to send multiple channels at once in one I2C transaction
** \note If no delay is required, PCA9685_CalcVal_NoDelay may be used instead
** \param[in,out] val - Pointer to 4 bytes array output
** \param[in] duty - Duty cycle coded on 12b (0-4095)
** \param[in] delay - Delay coded on 12b (0-<4095)
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9685_CalcVal(uint8_t val[4], const uint16_t duty, const uint16_t delay);

/*!\brief Computes values for PCA9685 registers (4bytes) following duty (without delay)
** \note Useful to manually fill an array with registers values to send multiple channels at once in one I2C transaction
** \param[in,out] val - Pointer to 4 bytes array output
** \param[in] duty - Duty cycle coded on 12b (0-4095)
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9685_CalcVal_NoDelay(uint8_t val[4], const uint16_t duty);

/*!\brief Convert Frequency to Prescaler byte register
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in,out] byte - Pointer to output value
** \param[in] freq - Outputs frequency in Hz
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9685_Freq_To_Byte(const PCA9685_t * const pCpnt, uint8_t * const byte, const uint16_t freq);

/*!\brief Convert Prescaler byte register to Frequency (in Hz)
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in] reg - Prescaler register value
** \return Frequency of PCA9685 outputs
**/
__INLINE float NONNULL__ PCA9685_Byte_To_Freq(PCA9685_t * const pCpnt, const uint8_t reg) {
	return ((float) pCpnt->cfg.Clock / (4096U * (reg + 1U))); }


/*!\brief Set latch type for PCA9685 peripheral
** \param[in,out] pCpnt - Pointer to PCA9685 component
** \param[in] latch - Latch type
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9685_Set_Latch(PCA9685_t * const pCpnt, const PCA96xx_latch latch);


/*!\brief Set frequency for PCA9685 peripheral
** \note Prescaler is calculated according to the use of PCA9685 internal oscillator
** \note Freq(Hz) = Clock / ((PSCx(8b) + 1) * 4096) -> Internal clock is 25000000
** \warning beware for higher frequencies, using internal clock: 1526Hz max (3 in reg) drops to 1220Hz (4 in reg), 1017Hz (5 in reg)....
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in] freq - Frequency to apply (in Hz)
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9685_Set_Frequency(PCA9685_t * const pCpnt, const uint16_t freq);


/*!\brief Read I2C lighting values from a LED and Computes the corresponding duty cycle value (12b)
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in] chan - Channel number (1 to 16)
** \param[in,out] duty - Pointer to the DutyCycle data for receive coded on 12b
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9685_ReadVal(PCA9685_t * const pCpnt, const PCA9xxx_chan chan, uint16_t * duty);


/*!\brief Read I2C lighting values from a LED and Computes the corresponding duty cycle value (Byte)
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in] chan - chan number (1 to 16)
** \param[in,out] duty - Pointer to the DutyCycle data for receive coded on a Byte
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9685_ReadValByte(PCA9685_t * const pCpnt, const PCA9xxx_chan chan, uint8_t * duty);


/*!\brief Compute and send PWM lighting values to apply on a PCA9685 channel or all channels (12b)
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in] chan - Channel number (1 to 16 / PCA9685_ALLCALL can be used to address all channels at the same time)
** \param[in] duty - Duty cycle coded on 12b (0-4095)
** \param[in] delay - Delay coded on 12b (0-<4095)
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9685_PutVal(PCA9685_t * const pCpnt, const PCA9xxx_chan chan, const uint16_t duty, const uint16_t delay);

/*!\brief Compute and send PWM lighting values to apply on a PCA9685 channel or all channels (Byte)
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in] chan - Channel number (1 to 16 / PCA9685_ALLCALL can be used to address all channels at the same time)
** \param[in] duty - Duty cycle coded on uint8_t (0-255)
** \param[in] delay - Delay coded on uint8_t (0-<255)
** \return FctERR - ErrorCode
**/
__INLINE FctERR NONNULL_INLINE__ PCA9685_PutValByte(PCA9685_t * const pCpnt, const PCA9xxx_chan chan, const uint8_t duty, const uint8_t delay) {
	return PCA9685_PutVal(pCpnt, chan, conv8upto16Bits(duty, 4U), conv8upto16Bits(delay, 4U)); }

/*!\brief Compute and send PWM lighting values to apply on a PCA9685 channel or all channels (Percent)
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in] chan - Channel number (1 to 16 / PCA9685_ALLCALL can be used to address all channels at the same time)
** \param[in] duty - Duty cycle coded on float (0.0-100.0%)
** \param[in] delay - Delay coded on float (0.0-<100.0%)
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9685_PutValPerc(PCA9685_t * const pCpnt, const PCA9xxx_chan chan, const float duty, const float delay);

/*!\brief Compute and send PWM Full ON value to apply on a PCA9685 channel or all channels (no delay)
** \note No configured delay
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in] chan - Channel number (1 to 16 / PCA9685_ALLCALL can be used to address all channels at the same time)
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9685_SetVal(PCA9685_t * const pCpnt, const PCA9xxx_chan chan);


/*!\brief Compute and send PWM Full OFF value to apply on a PCA9685 channel or all channels (no delay)
** \note No configured delay
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in] chan - Channel number (1 to 16 / PCA9685_ALLCALL can be used to address all channels at the same time)
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9685_ClrVal(PCA9685_t * const pCpnt, const PCA9xxx_chan chan);


/*!\brief Reset for PCA9685 peripheral
** \param[in] pCpnt - Pointer to PCA9685 component
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9685_Reset(PCA9685_t * const pCpnt);


/*!\brief General call reset function for PCA9685
** \param[in] hi2c - pointer to general call I2C instance
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9685_Reset_All(I2C_HandleTypeDef * const hi2c);


/*!\brief Reads register from PCA9685
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in] reg - Register address to read from
** \param[in,out] val - Pointer to the data for receive
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9685_ReadRegister(PCA9685_t * const pCpnt, const PCA9685_reg reg, uint8_t * const val);


/*******************/
/*** GPIO access ***/
/*******************/

/*!\brief Output Enable GPIO pin init for PCA9685
** \weak PCA9685 Output Enable GPIO pin init may be user implemented if needed
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in] GPIOx - OE port
** \param[in] GPIO_Pin - OE pin
** \param[in] GPIO_Active: OE pin active state
**/
void NONNULL__ PCA9685_OE_GPIO_Init(PCA9685_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active);

/*!\brief Output Enable GPIO pin setter for PCA9685
** \weak PCA9685 Output Enable GPIO pin setter may be user implemented if needed
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in] state - state to write on OE pin (0: inactive, 1: active)
**/
void NONNULL__ PCA9685_OE_GPIO_Set(PCA9685_t * const pCpnt, const bool state);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif	/* PCA9685_EX_H__ */
/****************************************************************/
