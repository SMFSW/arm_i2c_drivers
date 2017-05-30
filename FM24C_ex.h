/*!\file FM24C_ex.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief FM24C Driver extensions declarations
** \details FM24C16B: 16-Kbit (2K * 8) Serial I2C F-RAM
**			FM24C04B: 4-Kbit (512 * 8) Serial I2C F-RAM
**/
/****************************************************************/
#ifndef __FM24C_EX_H__
	#define __FM24C_EX_H__

#include "FM24C.h"

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

/*!\brief Mass erase of FM24C
** \return FctERR - error code
**/
FctERR FM24C_Mass_Erase(void);


/****************************************************************/
#endif
#endif /* __FM24C_EX_H__ */
/****************************************************************/
