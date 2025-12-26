/*!\file ADS1115_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief ADS1115 Driver procedures
** \details ADS1115: Ultra-Small, Low-Power, 16-Bit Analog-to-Digital Converter with Internal Reference
** \note	Compatibility with:
**				- ADS1113 (restrictions given in datasheet)
**				- ADS1114 (restrictions given in datasheet)
**				- ADS1115
** \note	Compatibility should be assured with:
**				- ADS1x1x (check datasheets for compatibility informations)
**/
/****************************************************************/
#ifndef ADS1115_PROC_H__
	#define ADS1115_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "ADS1115.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum ADS1115_func
** \brief Function enum for ADS1115
**/
typedef enum ADS1115_func {
	ADS1115__FUNC_SINGLE_DIFF = 0,	//!< Continuous conversion mode
	ADS1115__FUNC_MULTIPLE_DIFF,	//!< Power-down single-shot mode
	ADS1115__FUNC_SINGLE_ENDED,		//!< Power-down single-shot mode
} ADS1115_func;


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct ADS1115_t
** \brief ADS1115 user interface struct
**/
typedef struct ADS1115_t {
	int16_t				AIN[4];				//!< Conversion results
	uint8_t				AIN_idx;			//!< Current AIN index
	struct {
	I2C_slave_t *		slave_inst;			//!< Slave structure
	PeripheralGPIO_t	RDY_GPIO;			//!< Ready GPIO struct
	ADS1115_func		function;			//!< Functional mode
	ADS1115_mode		mode;				//!< Mode (single shot / Continuous conversion)
	uint8_t				nb;					//!< Number of channels (irrelevant for ADS1115__FUNC_SINGLE_DIFF func)
	uADS1115_REG__CFG	Config;				//!< Config register
	int16_t				Lo_Thresh;			//!< Lo-thresh register
	int16_t				Hi_Thresh;			//!< Hi-thresh register
	} cfg;
} ADS1115_t;

extern ADS1115_t ADS1115[I2C_ADS1115_NB];		//!< ADS1115 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for ADS1115 peripheral
** \weak ADS1115 Init sequence may be user implemented if custom initialization sequence needed
** \param[in] pCpnt - Pointer to ADS1115 component
** \return FctERR - error code
**/
FctERR NONNULL__ ADS1115_Init_Sequence(ADS1115_t * const pCpnt);


/*!\brief Handler for ADS1115 peripheral
** \weak ADS1115 handler may be user implemented to suit custom needs
** \note May be called periodically to handle ADS1115 tasks
** \note Alternately may be called when event occurs on ADS1115 pin (or by calling \ref ADS1115_handler_it instead)
** \param[in] pCpnt - Pointer to ADS1115 component
** \return FctERR - error code
**/
FctERR NONNULL__ ADS1115_handler(ADS1115_t * const pCpnt);

/*!\brief Handler for ADS1115 peripheral GPIO interrupt
** \note \ref ADS1115_RDY_GPIO_Init has to be called at init before using interrupt handler function
** \weak ADS1115 GPIO interrupt handler may be user implemented to suit custom needs
** \note May be called periodically to handle ADS1115 tasks through interrupts
** \param[in] pCpnt - Pointer to ADS1115 component
** \return FctERR - error code
**/
FctERR NONNULL__ ADS1115_handler_it(ADS1115_t * const pCpnt);

/*!\brief Handler for all ADS1115 peripherals
** \note May be called periodically to handle all ADS1115 tasks
** \return FctERR - error code
**/
FctERR ADS1115_handler_all(void);

/*!\brief Handler for all ADS1115 peripherals GPIO interrupt
** \note \ref ADS1115_INT_GPIO_Init has to be called at init before using interrupt handler function
** \note May be called periodically to handle all ADS1115 tasks
** \return FctERR - error code
**/
FctERR ADS1115_handler_it_all(void);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* ADS1115_PROC_H__ */
/****************************************************************/
