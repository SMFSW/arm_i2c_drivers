/*!\file PCA9685_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief PCA9685 Driver procedures
** \details PCA9685: 16-channel, 12-bit PWM Fm+ I2C-bus LED controller
**/
/****************************************************************/
#ifndef PCA9685_PROC_H__
	#define PCA9685_PROC_H__

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
#define PCA9685_DEF_FREQ		500U						//!< Default frequency to affect to PCA9685 register (range from 24Hz to 1526Hz)

#define PCA9685_MAX_CLOCK_FREQ	50000000UL					//!< PCA9685 Maximum external clock frequency

#define	PCA9685_NB_CHANNELS		16U							//!< PCA9685 Number of channels


#define	PCA9685_DEFAULT_SUB1_CALL_ADDR		(0xE2U >> 1U)	//!< PCA9685 SUB1 CALL Address
#define	PCA9685_DEFAULT_SUB2_CALL_ADDR		(0xE4U >> 1U)	//!< PCA9685 SUB2 CALL Address
#define	PCA9685_DEFAULT_SUB3_CALL_ADDR		(0xE8U >> 1U)	//!< PCA9685 SUB3 CALL Address


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct PCA9685_t
** \brief PCA9685 user interface struct
**/
typedef struct PCA9685_t {
	struct {
	I2C_slave_t *		slave_inst;		//!< Slave structure
	PeripheralGPIO_t	OE_GPIO;		//!< Output Enable GPIO struct
	uint32_t			Clock;			//!< Clock frequency
	float				Frequency;		//!< Outputs frequency
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
FctERR NONNULL__ PCA9685_Init_Sequence(PCA9685_t * const pCpnt);


/*!\brief Sets external clock frequency
** \param[in,out] pCpnt - Pointer to PCA9685 component
** \param[in] freq - External clock frequency in Hz
** \return FctERR - ErrorCode
**/
__INLINE FctERR NONNULL_INLINE__ PCA9685_Set_Clock_Freq(PCA9685_t * const pCpnt, const uint32_t freq) {
	if (freq > PCA9685_MAX_CLOCK_FREQ)	{ return ERROR_VALUE; }
	pCpnt->cfg.Clock = freq;
	return ERROR_OK; }


/*!\brief Gets external clock frequency
** \param[in] pCpnt - Pointer to PCA9685 component
** \return Internal clock frequency
**/
__INLINE uint32_t NONNULL_INLINE__ PCA9685_Get_Clock_Freq(PCA9685_t * const pCpnt) {
	return pCpnt->cfg.Clock; }


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif	/* PCA9685_PROC_H__ */
/****************************************************************/
