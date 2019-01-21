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
#define PCA9685_FREQ			500			//!< Frequency to affect to PCA9685 register (range from 24Hz to 1526Hz)
//!\warning Outputs frequency may be in range 24Hz-1526Hz

#define	PCA9685_FREQ_HZ_MIN		24			//!< Lower admissible frequency (when internal clock used)
#define	PCA9685_FREQ_HZ_MAX		1526		//!< Upper admissible frequency (when internal clock used)

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


/*!\brief Sets register value for PCA9685 according to desired output frequency
** \note PCA9685 internal oscillator used (at 25MHz)
** \warning if wrong frequency given, value for 500Hz will be returned
**
** \param [in] freq - Outputs frequency in Hz (from 26 to 1700Hz)
** \return prescaler register value for PCA9685
**/
uint8_t PCA9685_Get_PWM_Prescaler(const uint16_t freq);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif	/* __PCA9685_PROC_H__ */
/****************************************************************/
