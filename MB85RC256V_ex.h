/*!\file MB85RC256V_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief MB85RC256V Driver extensions
** \details MB85RC256V: 256-Kbit (32K * 8) I2C Memory FRAM
**/
/****************************************************************/
#ifndef __MB85RC256V_EX_H__
	#define __MB85RC256V_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "MB85RC256V.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/

#define MB85RC256V_Read_Type(name, type, addr)	__INLINE FctERR NONNULL_INLINE__ MB85RC256V_Get_##name(type * rd) {	\
													return MB85RC256V_Read(&rd, addr, sizeof(type)); }	//!< Macro to create function to read value(s) from MB85RC256V

#define MB85RC256V_Write_Type(name, type, addr)	__INLINE FctERR INLINE__ MB85RC256V_Set_##name(type wr) {	\
													return MB85RC256V_Write(&wr, addr, sizeof(type)); }	//!< Macro to create function to write value(s) to MB85RC256V

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
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __MB85RC256V_EX_H__ */
/****************************************************************/
