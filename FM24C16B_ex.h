/*!\file FM24C16B_ex.h
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief FM24C16B Driver extensions declarations
*/
/****************************************************************/
#ifndef __FM24C16B_EX_H__
	#define __FM24C16B_EX_H__

#include "FM24C16B.h"

#include "sarmfsw.h"
#include CMSIS_INC
#include CMSIS_CFG

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// High level functions
FctERR FM24C16B_Mass_Erase(void);


/****************************************************************/
#endif
#endif /* __FM24C16B_EX_H__ */
/****************************************************************/
