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
	uint32_t		Clock;			//!< Clock frequency
	float			Frequency;		//!< Outputs frequency
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
** \param[in] pCpnt - Pointer to PCA9685 component
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9685_Init_Sequence(PCA9685_t * pCpnt);


/*!\brief Sets external clock frequency
** \param[in,out] pCpnt - Pointer to PCA9685 component
** \param[in] freq - External clock frequency in Hz
** \return FctERR - ErrorCode
**/
__INLINE FctERR NONNULL_INLINE__ PCA9685_Set_Clock_Freq(PCA9685_t * pCpnt, const uint32_t freq) {
	if (freq > PCA9685_MAX_CLOCK_FREQ)	{ return ERROR_VALUE; }
	pCpnt->cfg.Clock = freq;
	return ERROR_OK; }


/*!\brief Gets external clock frequency
** \param[in] pCpnt - Pointer to PCA9685 component
** \return Internal clock frequency
**/
__INLINE uint32_t NONNULL_INLINE__ PCA9685_Get_Clock_Freq(PCA9685_t * pCpnt) {
	return pCpnt->cfg.Clock; }


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif	/* __PCA9685_PROC_H__ */
/****************************************************************/
