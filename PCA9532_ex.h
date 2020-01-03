/*!\file PCA9532_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief PCA9532 Driver extensions
** \details PCA9532: 16-bit I2C-bus LED dimmer
**/
/****************************************************************/
#ifndef __PCA9532_EX_H__
	#define __PCA9532_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "PCA9532.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Types
// *****************************************************************************
typedef union uPCA9532_REG_MAP {
	uint8_t Bytes[10];
	struct {
		uPCA9532_REG__INPUT0	INPUT0;
		uPCA9532_REG__INPUT1	INPUT1;
		uint8_t					PSC0;
		uint8_t					PWM0;
		uint8_t					PSC1;
		uint8_t					PWM1;
		uPCA9532_REG__LS0		LS0;
		uPCA9532_REG__LS1		LS1;
		uPCA9532_REG__LS2		LS2;
		uPCA9532_REG__LS3		LS3;
	} Reg;
} uPCA9532_REG_MAP;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Set auto increment option for PCA9532 registers
** \param[in,out] pCpnt - Pointer to PCA9532 component
** \param[in] inc - auto increment option
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ PCA9532_Set_Auto_Increment(PCA9532_t * const pCpnt, const PCA95xx_reg_inc inc) {
	if (RSHIFT(inc, 4) > 1)		{ return ERROR_VALUE; }
	pCpnt->cfg.auto_inc = inc & PCA95xx__AUTO_INC_ALL;	// Mask inc just in case
	return ERROR_OK; }


/*!\brief Convert Frequency to PSCx byte register
** \warning beware for higher frequencies, 152Hz max (0 in reg) drops to 76Hz (1 in reg), 50Hz (3 in reg)....
** \note BLINKx(Hz) = (PSCx(8b) + 1) / 152
** \param[in,out] byte - Pointer to output value
** \param[in] freq - Frequency in Hz (0.592Hz to 152Hz)
** \return FctERR - error code
**/
__INLINE FctERR PCA9532_Freq_To_Byte(uint8_t * const byte, const float freq) {
	if ((freq < 0.592f)	|| (freq > 152.0f))	{ return ERROR_RANGE; }	// Told 0.591Hz min in datasheet, but would be an issue for cast after calculation
	*byte = (uint8_t) (152.0f / freq) - 1;
	return ERROR_OK; }


/*!\brief Convert Frequency to PWMx byte register
** \warning Duty cycle can be set from 0% to 99.6%
** \note BLINKx(%) = (PWMx(8b) / 256) * 100
** \param[in,out] byte - Pointer to output value
** \param[in] duty - Duty Cycle in %
** \return FctERR - error code
**/
__INLINE FctERR PCA9532_DutyCycle_To_Byte(uint8_t * const byte, const float duty) {
	if ((duty < 0.0f) || (duty > 100.0f))	{ return ERROR_RANGE; }
	*byte = (uint8_t) min(255, (duty * 2.56f));
	return ERROR_OK; }


/*!\brief Convert PSCx byte register to Frequency (in Hz)
** \param[in] freq - 8b PSCx register value
** \return Frequency
**/
__INLINE float PCA9532_Byte_To_Freq(const uint8_t freq) {
	return (1.0f / (((float) freq + 1.0f) / 152.0f)); }


/*!\brief Convert PWMx byte register to Duty Cycle (in %)
** \param[in] duty - 8b PWMx register value
** \return Frequency
**/
__INLINE float PCA9532_Byte_To_Duty(const uint8_t duty) {
	return ((float) duty / 2.56f); }


/*!\brief Reads I2C register from PCA9532
** \param[in] pCpnt - Pointer to PCA9532 component
** \param[in] reg - Register address to read from
** \param[in,out] val - Pointer to the data for receive
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9532_ReadRegister(PCA9532_t * const pCpnt, const PCA9532_reg reg, uint8_t * const val);


/*!\brief Reads LED0 to LED7 input register
** \warning TODO: test if working in auto-increment mode
** \param[in] pCpnt - Pointer to PCA9532 component
** \param[in] pINPUT0 - Pointer to INPUT0 output variable
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9532_Read_INPUT0(PCA9532_t * const pCpnt, uPCA9532_REG__INPUT0 * const pINPUT0);

/*!\brief Reads LED8 to LED15 input register
** \param[in] pCpnt - Pointer to PCA9532 component
** \param[in] pINPUT1 - Pointer to INPUT1 output variable
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9532_Read_INPUT1(PCA9532_t * const pCpnt, uPCA9532_REG__INPUT1 * const pINPUT1);


/*!\brief Set PCA9532 peripheral LED mode
** \param[in,out] pCpnt - Pointer to PCA9532 component
** \param[in] chan - PCA9532 channel
** \param[in] mode - channel mode (on/off/PWM0/PWM1)
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9532_Set_Mode_LED(PCA9532_t * const pCpnt, const PCA9xxx_chan chan, const PCA95xx_ledsel mode);


/*!\brief Set PCA9532 peripheral multiple LEDs mode
** \param[in,out] pCpnt - Pointer to PCA9532 component
** \param[in] chans - PCA9532 channels mask (masked channels will be affected to new mode, other ones remaining unchanged)
** \param[in] mode - channel mode (on/off/PWM0/PWM1)
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9532_Set_Mode_LEDs(PCA9532_t * const pCpnt, const uint16_t chans, const PCA95xx_ledsel mode);


/*!\brief Sets PSCx frequency of PCA9532
** \param[in,out] pCpnt - Pointer to PCA9532 component
** \param[in] freq - Frequency (in Hz)
** \param[in] index - false: PSC0 / true: PSC1
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9532_SetFrequency(PCA9532_t * const pCpnt, const float freq, const bool index);


/*!\brief Sets PWMx duty cycle of PCA9532
** \param[in,out] pCpnt - Pointer to PCA9532 component
** \param[in] duty - Duty Cycle (in %)
** \param[in] index - false: PWM0 / true: PWM1
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9532_SetDuty(PCA9532_t * const pCpnt, const float duty, const bool index);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif	/* __PCA9532_EX_H__ */
/****************************************************************/
