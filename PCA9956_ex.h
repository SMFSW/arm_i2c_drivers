/*!\file PCA9956_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief PCA9956 Driver extensions
** \details PCA9956: 24-channel Fm+ I2C-bus 57 mA/20 V constant current LED driver
**/
/****************************************************************/
#ifndef __PCA9956_EX_H__
	#define __PCA9956_EX_H__

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
FctERR NONNULL__ PCA9956_Set_Latch(PCA9956_t * pCpnt, const PCA96xx_latch latch);


/*!\brief Set PCA9956 peripheral LED mode
** \param[in,out] pCpnt - Pointer to PCA9956 component
** \param[in] chan - PCA9956 channel
** \param[in] mode - channel mode (on/off/dimming/blinking)
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9956_Set_Mode_LED(PCA9956_t * const pCpnt, const PCA9xxx_chan chan, const PCA9956_ledout mode);


/*!\brief Set PCA9956 peripheral multiple LEDs mode
** \param[in,out] pCpnt - Pointer to PCA9956 component
** \param[in] chans - PCA9956 channels mask (masked channels will be affected to new mode, other ones remaining unchanged)
** \param[in] mode - channel mode (on/off/dimming/blinking)
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9956_Set_Mode_LEDs(PCA9956_t * const pCpnt, const uint32_t chans, const PCA9956_ledout mode);


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


/*!\brief Reads I2C lighting values from a LED (4 bytes) and Computes the corresponding duty cycle value (uint8_t)
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in] chan - channel number
** \param[in,out] duty - Pointer to the DutyCycle data for receive coded on a uint8_t
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9956_ReadVal(PCA9956_t * const pCpnt, const PCA9xxx_chan chan, uint8_t * const duty);


/*!\brief Computes and send I2C lighting values to apply to a particular channel for PCA9956
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in] chan - channel number
** \param[in] duty - Duty cycle coded on a uint8_t
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9956_PutVal(PCA9956_t * const pCpnt, const PCA9xxx_chan chan, const uint8_t duty);


/*!\brief Sends I2C PWM ON values to apply to a particular channel for PCA9956
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in] chan - channel number
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9956_SetVal(PCA9956_t * const pCpnt, const PCA9xxx_chan chan);


/*!\brief Sends I2C PWM OFF values to apply to a particular channel for PCA9956
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in] chan - channel number
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9956_ClrVal(PCA9956_t * const pCpnt, const PCA9xxx_chan chan);


/*!\brief Reads I2C LEDs error flags
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in,out] eflags - Pointer to the error flags data for receive
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9956_ReadEFLAGs(PCA9956_t * const pCpnt, uPCA9956_REG__EFLAG * const eflags);


/*!\brief Reads I2C register from PCA9956
** \param[in] pCpnt - Pointer to PCA9956 component
** \param[in] reg - Register address to read from
** \param[in,out] val - Pointer to the data for receive
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9956_ReadRegister(PCA9956_t * pCpnt, const PCA9956_reg reg, uint8_t * val);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif	/* __PCA9956_EX_H__ */
/****************************************************************/
