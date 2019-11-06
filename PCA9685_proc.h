/*!\file PCA9685_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
** \brief PCA9685 Driver procedures
** \details PCA9685: 16-channel, 12-bit PWM Fm+ I2C-bus LED controller
**/
/****************************************************************/
#ifndef __PCA9685_PROC_H__
	#define __PCA9685_PROC_H__

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
#define PCA9685_DEF_FREQ		500			//!< Default frequency to affect to PCA9685 register (range from 24Hz to 1526Hz)

#define	PCA9685_FREQ_HZ_MIN		24			//!< Lower admissible frequency (when internal clock used)
#define	PCA9685_FREQ_HZ_MAX		1526		//!< Upper admissible frequency (when internal clock used)

#define PCA9685_CLOCK_FREQ		25000000	//!< PCA9685 Internal clock frequency
#define PCA9685_MAX_CLOCK_FREQ	50000000	//!< PCA9685 Maximum external clock frequency

#define	PCA9685_NB_CHANNELS		16			//!< PCA9685 Number of channels


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct PCA9685_t
** \brief PCA9685 user interface struct
**/
typedef struct PCA9685_t {
	struct {
	I2C_slave_t *	slave_inst;		//!< Slave structure
	uint16_t		Frequency;		//!< Outputs frequency
	uint32_t		Clock;			//!< Clock frequency
	} cfg;
} PCA9685_t;

extern PCA9685_t	PCA9685[I2C_PCA9685_NB];	//!< PCA9685 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for PCA9685 peripheral
** \weak PCA9685 Init sequence may be user implemented if custom initialization sequence needed
**
** \param[in] pCpnt - Pointer to PCA9685 component
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9685_Init_Sequence(PCA9685_t * pCpnt);


/*!\brief Sets external clock frequency
**
** \param[in,out] pCpnt - Pointer to PCA9685 component
** \param[in] freq - External clock frequency in Hz
** \return FctERR - ErrorCode
**/
__INLINE FctERR NONNULL_INLINE__ PCA9685_Set_Clock_Frequency(PCA9685_t * pCpnt, const uint32_t freq) {
	if (freq > PCA9685_MAX_CLOCK_FREQ)	{ return ERROR_VALUE; }
	pCpnt->cfg.Clock = freq;
	return ERROR_OK; }


/*!\brief Sets register value for PCA9685 according to desired output frequency
** \warning if wrong frequency given, value for 500Hz will be returned
**
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in] freq - Outputs frequency in Hz
** \return prescaler register value for PCA9685
**/
uint8_t PCA9685_Get_PWM_Prescaler(PCA9685_t * pCpnt, uint16_t freq);


/*!\brief Get frequency from prescaler register value of PCA9685
**
** \param[in] pCpnt - Pointer to PCA9685 component
** \param[in] reg - Prescaler register value
** \return Frequency of PCA9685 outputs
**/
__INLINE uint16_t NONNULL__ PCA9685_Get_PWM_Frequency(PCA9685_t * pCpnt, uint8_t reg) {
	return (uint16_t) (pCpnt->cfg.Clock / (4096 * (reg + 1))); }


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif	/* __PCA9685_PROC_H__ */
/****************************************************************/
