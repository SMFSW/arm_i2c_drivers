/*!\file S11059_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
** \brief S11059 Driver extensions
** \details S11059: I2C interface-compatible color sensor
**/
/****************************************************************/
#ifndef __S11059_EX_H__
	#define __S11059_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "S11059.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum S11059_chan
** \brief Light type channels of S11059
**/
typedef enum PACK__ S11059_chan {
	S11059__CHAN_RED = 0,	//!< Red light channel
	S11059__CHAN_GREEN,		//!< Green light channel
	S11059__CHAN_BLUE,		//!< Blue light channel
	S11059__CHAN_IR			//!< Infra-red light channel
} S11059_chan;


/*!\union uS11059_REG_MAP
** \brief Union of S11059 registry map
**/
typedef union uS11059_REG_MAP {
	uint8_t Bytes[11];
	struct {
		uS11059_REG__CONTROL	Control;			//!< Control register
		uint8_t					Manual_Timing_MSB;	//!< Integration time manual setting register MSB
		uint8_t					Manual_Timing_LSB;	//!< Integration time manual setting register LSB
		uint8_t					Red_MSB;			//!< Output data red MSB
		uint8_t					Red_LSB;			//!< Output data red LSB
		uint8_t					Green_MSB;			//!< Output data green MSB
		uint8_t					Green_LSB;			//!< Output data green LSB
		uint8_t					Blue_MSB;			//!< Output data blue MSB
		uint8_t					Blue_LSB;			//!< Output data blue LSB
		uint8_t					IR_MSB;				//!< Output data infra-red MSB
		uint8_t					IR_LSB;				//!< Output data infra-red LSB
	} Reg;
} uS11059_REG_MAP;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

/*** Configuration ***/
/*!\brief Write S11059 Control register
** \param[in] pSlave - Pointer to I2C slave instance
** \param[in] ctl - Control value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ S11059_Write_Ctl(I2C_slave_t * pSlave, const uint8_t ctl) {
	return S11059_Write(pSlave, (uint8_t *) &ctl, S11059__CONTROL, 1); }


/*!\brief ADC Operation / Reset
** \param[in] pCpnt - Pointer to S11059 component
** \param[in] adc - ADC mode
** \return FctERR - error code
**/
FctERR NONNULL__ S11059_Set_ADC(S11059_t * pCpnt, const S11059_adc_reset adc);


/*!\brief Set Standby / Operating mode
** \param[in] pCpnt - Pointer to S11059 component
** \param[in] standby - standby mode
** \return FctERR - error code
**/
FctERR NONNULL__ S11059_Set_Standby(S11059_t * pCpnt, const S11059_standby_fct standby);


/*!\brief Set ADC gain
** \param[in] pCpnt - Pointer to S11059 component
** \param[in] gain - Gain selection
** \return FctERR - error code
**/
FctERR NONNULL__ S11059_Set_Gain(S11059_t * pCpnt, const S11059_gain gain);


/*!\brief Set integration mode & prescaler
** \param[in] pCpnt - Pointer to S11059 component
** \param[in] mode - Fixed period / Manual integration
** \param[in] prescaler - Integration time base (prescaler)
** \return FctERR - error code
**/
FctERR NONNULL__ S11059_Set_Integration(S11059_t * pCpnt, const S11059_integ mode, const S11059_prescaler prescaler);


/*!\brief Set S11059 integration multiplier (used when integration mode set to Manual)
** \param[in] pCpnt - Pointer to S11059 component
** \param[in] mult - Integration time multiplier value
** \return FctERR - error code
**/
FctERR NONNULL__ S11059_Set_Integration_Multiplier(S11059_t * pCpnt, const uint16_t mult);


/*** Operation ***/
/*!\brief Get Standby function monitor
** \param[in] pCpnt - Pointer to S11059 component
** \param[in,out] standby - pointer to standby function result
** \return FctERR - error code
**/
FctERR NONNULL__ S11059_Get_Standby(S11059_t * pCpnt, S11059_standby_fct * standby);

/*!\brief Get Specific channel conversion
** \param[in] pCpnt - Pointer to S11059 component
** \param[in,out] buf - pointer to conversions result
** \param[in] chan - Channel to get
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ S11059_Get_Channel(S11059_t * pCpnt, uint16_t * buf, const S11059_chan chan) {
	if (chan > S11059__CHAN_IR)	{ return ERROR_VALUE; }	// Unknown channel
	return S11059_Read_Word(pCpnt->cfg.slave_inst, buf, S11059__RED_DATA_MSB + (2 * chan)); }

/*!\brief Get Red channel conversion
** \param[in] pCpnt - Pointer to S11059 component
** \param[in,out] r - pointer to Red conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ S11059_Get_Red(S11059_t * pCpnt, uint16_t * r) {
	return S11059_Read_Word(pCpnt->cfg.slave_inst, r, S11059__RED_DATA_MSB); }

/*!\brief Get Green channel conversion
** \param[in] pCpnt - Pointer to S11059 component
** \param[in,out] g - pointer to Green conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ S11059_Get_Green(S11059_t * pCpnt, uint16_t * g) {
	return S11059_Read_Word(pCpnt->cfg.slave_inst, g, S11059__GREEN_DATA_MSB); }

/*!\brief Get Blue channel conversion
** \param[in] pCpnt - Pointer to S11059 component
** \param[in,out] b - pointer to Blue conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ S11059_Get_Blue(S11059_t * pCpnt, uint16_t * b) {
	return S11059_Read_Word(pCpnt->cfg.slave_inst, b, S11059__BLUE_DATA_MSB); }

/*!\brief Get Infra-red channel conversion
** \param[in] pCpnt - Pointer to S11059 component
** \param[in,out] ir - pointer to Infra-red conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ S11059_Get_IR(S11059_t * pCpnt, uint16_t * ir) {
	return S11059_Read_Word(pCpnt->cfg.slave_inst, ir, S11059__IR_DATA_MSB); }


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __S11059_EX_H__ */
/****************************************************************/
