/*!\file PCA9956_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief PCA9956 Driver procedures
** \details PCA9956: 24-channel Fm+ I2C-bus 57 mA/20 V constant current LED driver
**/
/****************************************************************/
#ifndef PCA9956_PROC_H__
	#define PCA9956_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "PCA9956.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define	PCA9956_NB_CHANNELS		24							//!< PCA9956 Number of channels

#define	PCA9956_DEFAULT_SUBX_CALL_ADDR		(0xEE >> 1)		//!< PCA9956 SUBx CALL Address


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct PCA9956_t
** \brief PCA9956 user interface struct
**/
typedef struct PCA9956_t {
	uPCA9956_REG__LEDOUT	LDR;				//!< LED output drive registers
	struct {
	I2C_slave_t *			slave_inst;			//!< Slave structure
	PeripheralGPIO_t		RST_GPIO;			//!< Reset GPIO struct
	PeripheralGPIO_t		OE_GPIO;			//!< Output Enable GPIO struct
	PCA995x_reg_inc			auto_inc;			//!< Auto increment configuration
	} cfg;
} PCA9956_t;

extern PCA9956_t	PCA9956[I2C_PCA9956_NB];	//!< PCA9956 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for PCA9956 peripheral
** \weak PCA9956 Init sequence may be user implemented if custom initialization sequence needed
** \param[in] pCpnt - Pointer to PCA9956 component
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ PCA9956_Init_Sequence(PCA9956_t * const pCpnt);


/*!\brief Compute IREF for PCA9956 peripheral
** \param[in,out] pIREF - Pointer to IREF result
** \param[in] Rext - Rext value (in KOhm)
** \param[in] current - Target current (in mA)
** \return FctERR - ErrorCode (in case of IREF value overflow)
**/
FctERR PCA9956_calc_IREF(uint8_t * const pIREF, const float Rext, const float current);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif	/* PCA9956_PROC_H__ */
/****************************************************************/
