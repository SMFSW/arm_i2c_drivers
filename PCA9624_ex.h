/*!\file PCA9624_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
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
	if (RSHIFT(inc, 5) > RSHIFT(PCA962x__AUTO_INC_BRIGHT_GLOBAL, 5))	{ return ERROR_VALUE; }
	pCpnt->cfg.auto_inc = inc & PCA962x__AUTO_INC_BRIGHT_GLOBAL;		// Mask inc just in case
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

/*!\brief Reads I2C register from PCA9624
** \param[in] pCpnt - Pointer to PCA9624 component
** \param[in] reg - Register address to read from
** \param[in,out] val - Pointer to the data for receive
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9624_ReadRegister(PCA9624_t * const pCpnt, const PCA9624_reg reg, uint8_t * const val);


/*!\brief Reads I2C lighting values from a LED (4 bytes) and Computes the corresponding duty cycle value (uint8_t)
** \param[in] pCpnt - Pointer to PCA9624 component
** \param[in] chan - channel number
** \param[in,out] duty - Pointer to the DutyCycle data for receive coded on a uint8_t
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9624_ReadVal(PCA9624_t * const pCpnt, const PCA9xxx_chan chan, uint8_t * const duty);


/*!\brief Computes and send I2C lighting values to apply to a particular channel for PCA9624
** \param[in] pCpnt - Pointer to PCA9624 component
** \param[in] chan - channel number
** \param[in] duty - Duty cycle coded on a uint8_t
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9624_PutVal(PCA9624_t * const pCpnt, const PCA9xxx_chan chan, const uint8_t duty);


/*!\brief Sends I2C PWM ON values to apply to a particular channel for PCA9624
** \param[in] pCpnt - Pointer to PCA9624 component
** \param[in] chan - channel number
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9624_SetVal(PCA9624_t * const pCpnt, const PCA9xxx_chan chan);


/*!\brief Sends I2C PWM OFF values to apply to a particular channel for PCA9624
** \param[in] pCpnt - Pointer to PCA9624 component
** \param[in] chan - channel number
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9624_ClrVal(PCA9624_t * const pCpnt, const PCA9xxx_chan chan);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif	/* __PCA9624_EX_H__ */
/****************************************************************/
