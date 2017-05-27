/*!\file MB85RC256V_ex.h
** \author SMFSW
** \version v0.2
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief MB85RC256V Driver extensions declarations
** \details MB85RC256V: 256-Kbit (32K * 8) I2C Memory FRAM
**/
/****************************************************************/
#ifndef __MB85RC256V_EX_H__
	#define __MB85RC256V_EX_H__

#include "MB85RC256V.h"

#include "sarmfsw.h"
#include CMSIS_INC
#include CMSIS_CFG

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

/*!\brief Mass erase of MB85RC256V
** \return FctERR - error code
**/
FctERR MB85RC256V_Mass_Erase(void);


/****************************************************************/
#endif
#endif /* __MB85RC256V_EX_H__ */
/****************************************************************/
