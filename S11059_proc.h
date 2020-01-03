/*!\file S11059_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief S11059 Driver procedures
** \details S11059: I2C interface-compatible color sensor
**/
/****************************************************************/
#ifndef __S11059_PROC_H__
	#define __S11059_PROC_H__

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
/*!\struct S11059_t
** \brief S11059 user interface struct
**/
typedef struct S11059_t {
	uint16_t			Red;					//!< Current Red value
	uint16_t			Green;					//!< Current Green value
	uint16_t			Blue;					//!< Current Blue value
	uint16_t			IR;						//!< Current IR value
	uint32_t			hStartConversion;		//!< Last conversion start tick
	struct {
	I2C_slave_t *		slave_inst;				//!< Slave structure
	uint32_t			FullIntegrationTime;	//!< Current integration time in us
	S11059_prescaler	IntegrationPrescaler;	//!< Current base integration time (prescaler)
	uint16_t			IntegrationTimeMult;	//!< Current integration time multiplier
	S11059_integ		IntegrationMode;		//!< Current integration mode
	S11059_gain			GainSelection;			//!< Current gain
	S11059_standby_fct	Standby;				//!< Current standby state
	S11059_adc_reset	ADCMode;				//!< Current ADC state
	} cfg;
} S11059_t;

extern S11059_t S11059[I2C_S11059_NB];			//!< S11059 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for S11059 peripheral
** \weak S11059 Init sequence may be user implemented if custom initialization sequence needed
** \param[in] pCpnt - Pointer to S11059 component
** \return FctERR - error code
**/
FctERR NONNULL__ S11059_Init_Sequence(S11059_t * pCpnt);


/*!\brief Get S11059 integration time for all channels
** \param[in] mode - Fixed period / Manual integration
** \param[in] prescaler - Integration time base (prescaler)
** \param[in] mult - Integration time multiplier value
** \return Full integration time in us
**/
uint32_t S11059_Get_Full_Integration_Time(const S11059_integ mode, const S11059_prescaler prescaler, const uint16_t mult);


/*!\brief Handler for S11059 peripheral
** \weak S11059 handler may be user implemented to suit custom needs
** \note May be called periodically to handle S11059 tasks
** \note Alternately may be called when event occurs on S11059 pin
** \param[in] pCpnt - Pointer to S11059 component
** \return FctERR - error code
**/
FctERR NONNULL__ S11059_handler(S11059_t * pCpnt);


/*!\brief Handler for all S11059 peripherals
** \note May be called periodically to handle all S11059 tasks
**/
__INLINE void INLINE__ S11059_handler_all(void) {
	for (S11059_t * pCpnt = S11059 ; pCpnt < &S11059[I2C_S11059_NB] ; pCpnt++) {
		S11059_handler(pCpnt); }
}


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __S11059_PROC_H__ */
/****************************************************************/
