/*!\file MB85RC256V_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief MB85RC256V Driver extensions
** \details MB85RC256V: 256-Kbit (32K * 8) I2C Memory FRAM
** \note	Compatibility (tested):
**				- MB85RC256V
**				- MB85RC256VL64B
** \note	Compatibility:
**				- other components using same i2c protocol may be compatible
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

#define MB85RC256V_Read_Type(cpnt, name, type, addr)	__INLINE FctERR NONNULL_INLINE__ MB85RC256V_Get_##name(type * rd) {	\
															return MB85RC256V_Read(cpnt, &rd, addr, sizeof(type)); }	//!< Macro to create function to read value(s) from MB85RC256V

#define MB85RC256V_Write_Type(cpnt, name, type, addr)	__INLINE FctERR INLINE__ MB85RC256V_Set_##name(type wr) {	\
															return MB85RC256V_Write(cpnt, &wr, addr, sizeof(type)); }	//!< Macro to create function to write value(s) to MB85RC256V

// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

/*!\brief Mass erase of MB85RC256V
** \param[in] pCpnt - Pointer to MB85RC256V component
** \return FctERR - error code
**/
FctERR NONNULL__ MB85RC256V_Mass_Erase(MB85RC256V_t * const pCpnt);


/*******************/
/*** GPIO access ***/
/*******************/

/*!\brief Write Protect GPIO pin init for MB85RC256V
** \weak MB85RC256V Write Protect GPIO pin init may be user implemented if needed
** \param[in] pCpnt - Pointer to MB85RC256V component
** \param[in] GPIOx - WP port
** \param[in] GPIO_Pin - WP pin
** \param[in] GPIO_Active: WP pin active state
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ MB85RC256V_WP_GPIO_Init(MB85RC256V_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active);

/*!\brief Write Protect GPIO pin getter for MB85RC256V
** \weak MB85RC256V Write Protect GPIO pin getter may be user implemented if needed
** \param[in] pCpnt - Pointer to MB85RC256V component
** \param[in,out] pState - Pointer to WP pin state variable (0: inactive, 1: active)
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ MB85RC256V_WP_GPIO_Get(MB85RC256V_t * const pCpnt, bool * const pState);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* __MB85RC256V_EX_H__ */
/****************************************************************/
