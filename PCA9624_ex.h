/*!\file PCA9624_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
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
	struct PACK__ {
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
/*!\brief Set auto increment option for PCA9624 registers
** \param[in,out] pCpnt - Pointer to PCA9624 component
** \param[in] inc - auto increment option
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ PCA9624_Set_Auto_Increment(PCA9624_t * const pCpnt, const PCA962x_reg_inc inc) {
	if (RSHIFT(inc, 5) > RSHIFT(PCA9xxx__AUTO_INC_BRIGHT_GLOBAL, 5))	{ return ERROR_VALUE; }
	pCpnt->cfg.auto_inc = inc & PCA9xxx__AUTO_INC_BRIGHT_GLOBAL;		// Mask inc just in case
	return ERROR_OK; }

/*!\brief Set MODE1 register
** \param[in,out] pCpnt - Pointer to PCA9624 component
** \param[in] MODE1 - MODE1 register value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ PCA9624_Set_MODE1(PCA9624_t * const pCpnt, const uPCA9624_REG__MODE1 MODE1) {
	return PCA9624_Write(pCpnt->cfg.slave_inst, (uint8_t *) &MODE1, PCA9624__MODE1, sizeof(MODE1)); }

/*!\brief Set MODE2 register
** \param[in,out] pCpnt - Pointer to PCA9624 component
** \param[in] MODE2 - MODE2 register value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ PCA9624_Set_MODE2(PCA9624_t * const pCpnt, uPCA9624_REG__MODE2 MODE2) {
	MODE2.Byte = (MODE2.Byte | 0x05) & ~0x12;	// Setting reserved bits to appropriate values
	return PCA9624_Write(pCpnt->cfg.slave_inst, (uint8_t *) &MODE2, PCA9624__MODE2, sizeof(MODE2)); }


/*!\brief Set latch type for PCA9624 peripheral
** \param[in,out] pCpnt - Pointer to PCA9624 component
** \param[in] latch - Latch type
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9624_Set_Latch(PCA9624_t * const pCpnt, const PCA96xx_latch latch);


/*!\brief Set PCA9624 peripheral LED mode
** \param[in,out] pCpnt - Pointer to PCA9624 component
** \param[in] chan - PCA9624 channel
** \param[in] mode - channel mode (on/off/dimming/blinking)
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9624_Set_Mode_LED(PCA9624_t * const pCpnt, const PCA9xxx_chan chan, const PCA962x_ledout mode);


/*!\brief Set PCA9624 peripheral multiple LEDs mode
** \param[in,out] pCpnt - Pointer to PCA9624 component
** \param[in] chans - PCA9624 channels mask (masked channels will be affected to new mode, other ones remaining unchanged)
** \param[in] mode - channel mode (on/off/dimming/blinking)
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9624_Set_Mode_LEDs(PCA9624_t * const pCpnt, const uint8_t chans, const PCA962x_ledout mode);


/*!\brief Read PWM duty cycle array of values of a PCA9624
** \param[in] pCpnt - Pointer to PCA9624 component
** \param[in,out] pDuty - Pointer to duty cycle values
** \param[in] indexed - if set to true, pDuty start address will be indexed by the value of start parameter
**						(thus pDuty always needs first channels cells even if not sent)
** \param[in] start - starting channel number
** \param[in] end - ending channel number
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9624_ReadVals(PCA9624_t * const pCpnt, uint8_t pDuty[], const bool indexed, const PCA9xxx_chan start, const PCA9xxx_chan end);


/*!\brief Read PWM duty cycle array of values of a PCA9624
** \param[in] pCpnt - Pointer to PCA9624 component
** \param[in,out] pDuty - Pointer to duty cycle values
** \param[in] start - starting channel number
** \param[in] end - ending channel number
** \return FctERR - ErrorCode
**/
__INLINE FctERR NONNULL_INLINE__ PCA9624_ReadVals_Array(PCA9624_t * const pCpnt, uint8_t pDuty[], const PCA9xxx_chan start, const PCA9xxx_chan end) {
	return PCA9624_ReadVals(pCpnt, pDuty, false, start, end); }


/*!\brief Read PWM duty cycle array of values of a PCA9624
** \param[in] pCpnt - Pointer to PCA9624 component
** \param[in,out] pDuty - Pointer to duty cycle values (pDuty start address will be indexed by the value of start parameter, thus needs first channels cells)
** \param[in] start - starting channel number
** \param[in] end - ending channel number
** \return FctERR - ErrorCode
**/
__INLINE FctERR NONNULL_INLINE__ PCA9624_ReadVals_IndexedArray(PCA9624_t * const pCpnt, uint8_t pDuty[], const PCA9xxx_chan start, const PCA9xxx_chan end) {
	return PCA9624_ReadVals(pCpnt, pDuty, true, start, end); }


/*!\brief Read all PWM duty cycle array of values of a PCA9624 (for the 8 channels)
** \param[in] pCpnt - Pointer to PCA9624 component
** \param[in,out] pDuty - Pointer to duty cycle values (8 bytes)
** \return FctERR - ErrorCode
**/
__INLINE FctERR NONNULL_INLINE__ PCA9624_ReadVals_All(PCA9624_t * const pCpnt, uint8_t pDuty[8]) {
	return PCA9624_ReadVals(pCpnt, pDuty, false, PCA9xxx__PWM1, PCA9xxx__PWM8); }


/*!\brief Read PWM duty cycle value of a PCA9624 channel
** \param[in] pCpnt - Pointer to PCA9624 component
** \param[in] chan - channel number
** \param[in,out] pDuty - Pointer to duty cycle data
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9624_ReadVal(PCA9624_t * const pCpnt, const PCA9xxx_chan chan, uint8_t * const pDuty);


/*!\brief Send PWM duty cycle array of values to apply on a PCA9624
** \param[in] pCpnt - Pointer to PCA9624 component
** \param[in] pDuty - Pointer to duty cycle values
** \param[in] indexed - if set to true, pDuty start address will be indexed by the value of start parameter
**						(thus pDuty always needs first channels cells even if not sent)
** \param[in] start - starting channel number
** \param[in] end - ending channel number
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9624_PutVals(PCA9624_t * const pCpnt, const uint8_t pDuty[], const bool indexed, const PCA9xxx_chan start, const PCA9xxx_chan end);


/*!\brief Send PWM duty cycle array of values to apply on a PCA9624
** \param[in] pCpnt - Pointer to PCA9624 component
** \param[in] pDuty - Pointer to duty cycle values
** \param[in] start - starting channel number
** \param[in] end - ending channel number
** \return FctERR - ErrorCode
**/
__INLINE FctERR NONNULL_INLINE__ PCA9624_PutVals_Array(PCA9624_t * const pCpnt, const uint8_t pDuty[], const PCA9xxx_chan start, const PCA9xxx_chan end) {
	return PCA9624_PutVals(pCpnt, pDuty, false, start, end); }


/*!\brief Send PWM duty cycle array of values to apply on a PCA9624
** \param[in] pCpnt - Pointer to PCA9624 component
** \param[in] pDuty - Pointer to duty cycle values (pDuty start address will be indexed by the value of start parameter, thus needs first channels cells)
** \param[in] start - starting channel number
** \param[in] end - ending channel number
** \return FctERR - ErrorCode
**/
__INLINE FctERR NONNULL_INLINE__ PCA9624_PutVals_IndexedArray(PCA9624_t * const pCpnt, const uint8_t pDuty[], const PCA9xxx_chan start, const PCA9xxx_chan end) {
	return PCA9624_PutVals(pCpnt, pDuty, true, start, end); }


/*!\brief Send all PWM duty cycle array of values to apply on a PCA9624 (for the 8 channels)
** \param[in] pCpnt - Pointer to PCA9624 component
** \param[in] pDuty - Pointer to duty cycle values (8 bytes)
** \return FctERR - ErrorCode
**/
__INLINE FctERR NONNULL_INLINE__ PCA9624_PutVals_All(PCA9624_t * const pCpnt, const uint8_t pDuty[8]) {
	return PCA9624_PutVals(pCpnt, pDuty, false, PCA9xxx__PWM1, PCA9xxx__PWM8); }


/*!\brief Send PWM duty cycle value to apply on a PCA9624 channel
** \param[in] pCpnt - Pointer to PCA9624 component
** \param[in] chan - channel number
** \param[in] duty - Duty cycle
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9624_PutVal(PCA9624_t * const pCpnt, const PCA9xxx_chan chan, const uint8_t duty);


/*!\brief Send PWM Full ON value to apply on a PCA9624 channel
** \param[in] pCpnt - Pointer to PCA9624 component
** \param[in] chan - channel number
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9624_SetVal(PCA9624_t * const pCpnt, const PCA9xxx_chan chan);


/*!\brief Send PWM Full OFF value to apply on a PCA9624 channel
** \param[in] pCpnt - Pointer to PCA9624 component
** \param[in] chan - channel number
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9624_ClrVal(PCA9624_t * const pCpnt, const PCA9xxx_chan chan);


/*!\brief Reset for PCA9624 peripheral
** \param[in] pCpnt - Pointer to PCA9624 component
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9624_Reset(PCA9624_t * const pCpnt);

/*!\brief General call reset function for PCA9624
** \param[in] hi2c - pointer to general call I2C instance
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9624_Reset_All(const I2C_HandleTypeDef * const hi2c);

/*!\brief Reads register from PCA9624
** \param[in] pCpnt - Pointer to PCA9624 component
** \param[in] reg - Register address to read from
** \param[in,out] val - Pointer to the data for receive
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9624_ReadRegister(PCA9624_t * const pCpnt, const PCA9624_reg reg, uint8_t * const val);


/*******************/
/*** GPIO access ***/
/*******************/

/*!\brief Output Enable GPIO pin init for PCA9624
** \weak PCA9624 Output Enable GPIO pin init may be user implemented if needed
** \param[in] pCpnt - Pointer to PCA9624 component
** \param[in] GPIOx - OE port
** \param[in] GPIO_Pin - OE pin
** \param[in] GPIO_Active: OE pin active state
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9624_OE_GPIO_Init(PCA9624_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active);

/*!\brief Output Enable GPIO pin setter for PCA9624
** \weak PCA9624 Output Enable GPIO pin setter may be user implemented if needed
** \param[in] pCpnt - Pointer to PCA9624 component
** \param[in] state - state to write on OE pin (0: inactive, 1: active)
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9624_OE_GPIO_Set(PCA9624_t * const pCpnt, const bool state);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif	/* __PCA9624_EX_H__ */
/****************************************************************/
