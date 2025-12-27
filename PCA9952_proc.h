/*!\file PCA9952_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief PCA9952 Driver procedures
** \details PCA9952: 16-channel Fm+ I2C-bus 57 mA constant current LED driver
**/
/****************************************************************/
#ifndef PCA9952_PROC_H__
	#define PCA9952_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "PCA9952.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define	PCA9952_NB_CHANNELS					16U				//!< PCA9952 Number of channels

#define	PCA9952_DEFAULT_SUBX_CALL_ADDR		(0xECU >> 1U)	//!< PCA9952 SUBx CALL Address


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct PCA9952_t
** \brief PCA9952 user interface struct
**/
typedef struct PCA9952_t {
	uPCA9952_REG__LEDOUT	LDR;				//!< LED output drive registers
	struct {
	I2C_slave_t *			slave_inst;			//!< Slave structure
	PeripheralGPIO_t		RST_GPIO;			//!< Reset GPIO struct
	PeripheralGPIO_t		OE_GPIO;			//!< Output Enable GPIO struct
	PCA995x_reg_inc			auto_inc;			//!< Auto increment configuration
	} cfg;
} PCA9952_t;

extern PCA9952_t	PCA9952[I2C_PCA9952_NB];	//!< PCA9952 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for PCA9952 peripheral
** \weak PCA9952 Init sequence may be user implemented if custom initialization sequence needed
** \param[in] pCpnt - Pointer to PCA9952 component
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9952_Init_Sequence(PCA9952_t * const pCpnt);


/*!\brief Compute IREF for PCA9952 peripheral
** \param[in,out] pIREF - Pointer to IREF result
** \param[in] Rext - Rext value (in KOhm)
** \param[in] current - Target current (in mA)
** \return FctERR - ErrorCode (in case of IREF value overflow)
**/
FctERR PCA9952_calc_IREF(uint8_t * const pIREF, const float Rext, const float current);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif	/* PCA9952_PROC_H__ */
/****************************************************************/
