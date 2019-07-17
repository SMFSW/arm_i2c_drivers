/*!\file FM24C_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
** \brief FM24C Driver extensions
** \details FM24C16B: 16-Kbit (2K * 8) Serial I2C F-RAM
**			FM24C04B: 4-Kbit (512 * 8) Serial I2C F-RAM
**/
/****************************************************************/
#ifndef __FM24C_EX_H__
	#define __FM24C_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "FM24C.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


#define FM24C_Read_Type(cpnt, name, type, addr)		__INLINE FctERR NONNULL_INLINE__ FM24C_Get_##name(type * rd) {	\
														return FM24C_Read(cpnt, &rd, addr, sizeof(type)); }		//!< Macro to create function to read value(s) from FM24C

#define FM24C_Write_Type(cpnt, name, type, addr)	__INLINE FctERR INLINE__ FM24C_Set_##name(type wr) {	\
														return FM24C_Write(cpnt, &wr, addr, sizeof(type)); }	//!< Macro to create function to write value(s) to FM24C


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

/*!\brief Mass erase of FM24C
** \param[in] pCpnt - Pointer to FM24C component
** \return FctERR - error code
**/
FctERR NONNULL__ FM24C_Mass_Erase(FM24C_t * pCpnt);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __FM24C_EX_H__ */
/****************************************************************/
