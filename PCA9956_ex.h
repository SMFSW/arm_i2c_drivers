/*!\file PCA9956_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief PCA9956 Driver extensions
** \details PCA9956: 24-channel Fm+ I2C-bus 57 mA/20 V constant current LED driver
**/
/****************************************************************/
#ifndef PCA9956_EX_H__
	#define PCA9956_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "PCA9956.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Set MODE1 register
** \param[in,out] pCpnt - Pointer to PCA9956 component
** \param[in] MODE1 - MODE1 register value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ PCA9956_Set_MODE1(PCA9956_t * const pCpnt, const uPCA9956_REG__MODE1 MODE1) {
	return PCA9956_Write(pCpnt->cfg.slave_inst, (uint8_t *) &MODE1, PCA9956__MODE1, sizeof(MODE1)); }

/*!\brief Set MODE2 register
** \param[in,out] pCpnt - Pointer to PCA9956 component
** \param[in] MODE2 - MODE2 register value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ PCA9956_Set_MODE2(PCA9956_t * const pCpnt, uPCA9956_REG__MODE2 MODE2) {
	MODE2.Byte = (MODE2.Byte | 0x05) & ~0xC2;	// Setting reserved bits to appropriate values
	return PCA9956_Write(pCpnt->cfg.slave_inst, (uint8_t *) &MODE2, PCA9956__MODE2, sizeof(MODE2)); }


/*!\brief Set auto increment option for PCA9956 registers
** \param[in,out] pCpnt - Pointer to PCA9956 component
** \param[in] inc - auto increment option
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9956_Set_Auto_Increment(PCA9956_t * const pCpnt, const PCA962x_reg_inc inc);


/*!\brief Set latch type for PCA9956 peripheral
** \param[in,out] pCpnt - Pointer to PCA9956 component
** \param[in] latch - Latch type
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9956_Set_Latch(PCA9956_t * const pCpnt, const PCA96xx_latch latch);


/*!\brief Set PCA9956 peripheral LED mode
** \param[in,out] pCpnt - Pointer to PCA9956 component
** \param[in] chan - PCA9956 channel
** \param[in] mode - channel mode (on/off/dimming/blinking)
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9956_Set_Mode_LED(PCA9956_t * const pCpnt, const PCA9xxx_chan chan, const PCA995x_ledout mode);


/*!\brief Set PCA9956 peripheral multiple LEDs mode
** \param[in,out] pCpnt - Pointer to PCA9956 component
** \param[in] chans - PCA9956 channels mask (masked channels will be affected to new mode, other ones remaining unchanged)
** \param[in] mode - channel mode (on/off/dimming/blinking)
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9956_Set_Mode_LEDs(PCA9956_t * const pCpnt, const uint32_t chans, const PCA995x_ledout mode);


/*!\brief Set PCA9956 output gain control value
** \param[in,out] pCpnt - Pointer to PCA9956 component
** \param[in] chan - PCA9956 channel
** \param[in] iref - output gain control value
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9956_Set_IREF(PCA9956_t * const pCpnt, const PCA9xxx_chan chan, const uint8_t iref);


/*!\brief Set PCA9956 output gain control value
** \param[in,out] pCpnt - Pointer to PCA9956 component
** \param[in] chans - PCA9956 channels mask (masked channels will be affected to new mode, other ones remaining unchanged)
** \param[in] iref - output gain control value
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9956_Set_IREFs(PCA9956_t * const pCpnt, const uint32_t chans, const uint8_t iref);


/*!\brief Set PCA9956 peripheral delay between channels
** \param[in,out] pCpnt - Pointer to PCA9956 component
** \param[in] offset - delay between channels
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9956_Set_Offset(PCA9956_t * const pCpnt, const PCA9956_offset offset);


/*!\brief Read PWM duty cycle array of values of a PCA9956
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in,out] pDuty - Pointer to duty cycle values
** \param[in] indexed - if set to true, pDuty start address will be indexed by the value of start parameter
**						(thus pDuty always needs first channels cells even if not sent)
** \param[in] start - starting channel number
** \param[in] end - ending channel number
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9956_ReadVals(PCA9956_t * const pCpnt, uint8_t pDuty[], const bool indexed, const PCA9xxx_chan start, const PCA9xxx_chan end);


/*!\brief Read PWM duty cycle array of values of a PCA9956
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in,out] pDuty - Pointer to duty cycle values
** \param[in] start - starting channel number
** \param[in] end - ending channel number
** \return FctERR - ErrorCode
**/
__INLINE FctERR NONNULL_INLINE__ PCA9956_ReadVals_Array(PCA9956_t * const pCpnt, uint8_t pDuty[], const PCA9xxx_chan start, const PCA9xxx_chan end) {
	return PCA9956_ReadVals(pCpnt, pDuty, false, start, end); }


/*!\brief Read PWM duty cycle array of values of a PCA9956
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in,out] pDuty - Pointer to duty cycle values (pDuty start address will be indexed by the value of start parameter, thus needs first channels cells)
** \param[in] start - starting channel number
** \param[in] end - ending channel number
** \return FctERR - ErrorCode
**/
__INLINE FctERR NONNULL_INLINE__ PCA9956_ReadVals_IndexedArray(PCA9956_t * const pCpnt, uint8_t pDuty[], const PCA9xxx_chan start, const PCA9xxx_chan end) {
	return PCA9956_ReadVals(pCpnt, pDuty, true, start, end); }


/*!\brief Read all PWM duty cycle array of values of a PCA9956 (for the 24 channels)
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in,out] pDuty - Pointer to duty cycle values (24 bytes)
** \return FctERR - ErrorCode
**/
__INLINE FctERR NONNULL_INLINE__ PCA9956_ReadVals_All(PCA9956_t * const pCpnt, uint8_t pDuty[24]) {
	return PCA9956_ReadVals(pCpnt, pDuty, false, PCA9xxx__PWM1, PCA9xxx__PWM24); }


/*!\brief Read PWM duty cycle value of a PCA9956 channel
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in] chan - channel number
** \param[in,out] pDuty - Pointer to duty cycle data
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9956_ReadVal(PCA9956_t * const pCpnt, const PCA9xxx_chan chan, uint8_t * const pDuty);


/*!\brief Send PWM duty cycle array of values to apply on a PCA9956
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in] pDuty - Pointer to duty cycle values
** \param[in] indexed - if set to true, pDuty start address will be indexed by the value of start parameter
**						(thus pDuty always needs first channels cells even if not sent)
** \param[in] start - starting channel number
** \param[in] end - ending channel number
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9956_PutVals(PCA9956_t * const pCpnt, const uint8_t pDuty[], const bool indexed, const PCA9xxx_chan start, const PCA9xxx_chan end);


/*!\brief Send PWM duty cycle array of values to apply on a PCA9956
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in] pDuty - Pointer to duty cycle values
** \param[in] start - starting channel number
** \param[in] end - ending channel number
** \return FctERR - ErrorCode
**/
__INLINE FctERR NONNULL_INLINE__ PCA9956_PutVals_Array(PCA9956_t * const pCpnt, const uint8_t pDuty[], const PCA9xxx_chan start, const PCA9xxx_chan end) {
	return PCA9956_PutVals(pCpnt, pDuty, false, start, end); }


/*!\brief Send PWM duty cycle array of values to apply on a PCA9956
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in] pDuty - Pointer to duty cycle values (pDuty start address will be indexed by the value of start parameter, thus needs first channels cells)
** \param[in] start - starting channel number
** \param[in] end - ending channel number
** \return FctERR - ErrorCode
**/
__INLINE FctERR NONNULL_INLINE__ PCA9956_PutVals_IndexedArray(PCA9956_t * const pCpnt, const uint8_t pDuty[], const PCA9xxx_chan start, const PCA9xxx_chan end) {
	return PCA9956_PutVals(pCpnt, pDuty, true, start, end); }


/*!\brief Send all PWM duty cycle array of values to apply on a PCA9956 (for the 24 channels)
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in] pDuty - Pointer to duty cycle values (24 bytes)
** \return FctERR - ErrorCode
**/
__INLINE FctERR NONNULL_INLINE__ PCA9956_PutVals_All(PCA9956_t * const pCpnt, const uint8_t pDuty[24]) {
	return PCA9956_PutVals(pCpnt, pDuty, false, PCA9xxx__PWM1, PCA9xxx__PWM24); }


/*!\brief Send PWM duty cycle value to apply on a PCA9956 channel
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in] chan - channel number
** \param[in] duty - Duty cycle coded on a uint8_t
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9956_PutVal(PCA9956_t * const pCpnt, const PCA9xxx_chan chan, const uint8_t duty);


/*!\brief Send PWM Full ON value to apply on a PCA9956 channel
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in] chan - channel number
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9956_SetVal(PCA9956_t * const pCpnt, const PCA9xxx_chan chan);


/*!\brief Send PWM Full OFF value to apply on a PCA9956 channel
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in] chan - channel number
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9956_ClrVal(PCA9956_t * const pCpnt, const PCA9xxx_chan chan);


/*!\brief Read LEDs error flags
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in,out] eflags - Pointer to the error flags data for receive
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9956_ReadEFLAGs(PCA9956_t * const pCpnt, uPCA9956_REG__EFLAG * const eflags);


/*!\brief Read register from PCA9956
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in] reg - Register address to read from
** \param[in,out] val - Pointer to the data for receive
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9956_ReadRegister(PCA9956_t * const pCpnt, const PCA9956_reg reg, uint8_t * val);


/*******************/
/*** GPIO access ***/
/*******************/

/*!\brief Reset GPIO pin init for PCA9956
** \weak PCA9956 Reset GPIO pin init may be user implemented if needed
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in] GPIOx - RST port
** \param[in] GPIO_Pin - RST pin
** \param[in] GPIO_Active: RST pin active state
**/
void NONNULL__ PCA9956_RST_GPIO_Init(PCA9956_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active);

/*!\brief Reset GPIO pin setter for PCA9956
** \weak PCA9956 Reset GPIO pin setter may be user implemented if needed
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in] state - state to write on RST pin (0: inactive, 1: active)
**/
void NONNULL__ PCA9956_RST_GPIO_Set(PCA9956_t * const pCpnt, const bool state);


/*!\brief Output Enable GPIO pin init for PCA9956
** \weak PCA9956 Output Enable GPIO pin init may be user implemented if needed
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in] GPIOx - OE port
** \param[in] GPIO_Pin - OE pin
** \param[in] GPIO_Active: OE pin active state
**/
void NONNULL__ PCA9956_OE_GPIO_Init(PCA9956_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active);

/*!\brief Output Enable GPIO pin setter for PCA9956
** \weak PCA9956 Output Enable GPIO pin setter may be user implemented if needed
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in] state - state to write on OE pin (0: inactive, 1: active)
**/
void NONNULL__ PCA9956_OE_GPIO_Set(PCA9956_t * const pCpnt, const bool state);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif	/* PCA9956_EX_H__ */
/****************************************************************/
