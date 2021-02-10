/*!\file PCA9956_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief PCA9956 Driver procedures
** \details PCA9956: 24-channel Fm+ I2C-bus 57 mA/20 V constant current LED driver
**/
/****************************************************************/
#ifndef __PCA9956_PROC_H__
	#define __PCA9956_PROC_H__

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
#define	PCA9956_NB_CHANNELS		24			//!< PCA9956 Number of channels


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct PCA9956_t
** \brief PCA9956 user interface struct
**/
typedef struct PCA9956_t {
	uPCA9956_REG__LEDOUT	LDR;			//!< LED output drive registers
	struct {
	I2C_slave_t *			slave_inst;		//!< Slave structure
	PCA9956_reg_inc			auto_inc;		//!< Auto increment configuration
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
FctERR NONNULL__ PCA9956_Init_Sequence(PCA9956_t * pCpnt);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif	/* __PCA9956_PROC_H__ */
/****************************************************************/
