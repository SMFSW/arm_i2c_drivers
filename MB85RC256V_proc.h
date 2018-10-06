/*!\file MB85RC256V_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief MB85RC256V Driver procedures
** \details MB85RC256V: 256-Kbit (32K * 8) I2C Memory FRAM
**/
/****************************************************************/
#ifndef __MB85RC256V_PROC_H__
	#define __MB85RC256V_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "MB85RC256V.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define MB85RC256V_MANUFACTURE_ID	0x0A		//!< MB85RC256V Manufacture ID (Fujitsu) to check against
#define MB85RC256V_PRODUCT_ID		0x10		//!< MB85RC256V Product ID to check against



// *****************************************************************************
// Section: Types
// *****************************************************************************
typedef struct MB85RC256V_t {
	struct {
	I2C_slave_t *	slave_inst;			//!< Slave structure
	uint16_t		Manufacture_ID;
	uint8_t			Density;
	uint8_t			Product_ID;
	} cfg;
} MB85RC256V_t;

extern MB85RC256V_t	MB85RC256V;			//!< MB85RC256V User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for MB85RC256V peripheral
** \weak MB85RC256V Init sequence may be user implemented if custom initialization sequence needed
** \return FctERR - error code
**/
__WEAK FctERR MB85RC256V_Init_Sequence(void);


/*!\brief Get ID from MB85RC256V peripheral
** \return FctERR - error code
**/
FctERR MB85RC256V_Get_ID(void);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __MB85RC256V_PROC_H__ */
/****************************************************************/
