/*!\file FM24C_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief FRAM / EEPROM Driver (bank switching at I2C address level protocol) extensions
** \note The driver handles writing specificities for EEPROM type components
** \note The driver is fully compatible with FRAM type components
** \note When EEPROM / FRAM compatibility is not needed, FM24C_WRITE_SIZE can be set to FM24C_BANK_SIZE for more efficiency
** \details FM24C16B: 16-Kbit (2K * 8) Serial I2C F-RAM
**			FM24C04B: 4-Kbit (512 * 8) Serial I2C F-RAM
** \note	Compatibility (tested):
**				- FM24C16B
**				- FM24C04B
**				- BR24T04FVM
**				...
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
FctERR NONNULL__ FM24C_Mass_Erase(FM24C_t * const pCpnt);


/*******************/
/*** GPIO access ***/
/*******************/

/*!\brief Write Protect GPIO pin init for FM24C
** \weak FM24C Write Protect GPIO pin init may be user implemented if needed
** \param[in] pCpnt - Pointer to FM24C component
** \param[in] GPIOx - WP port
** \param[in] GPIO_Pin - WP pin
** \param[in] GPIO_Active: WP pin active state
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ FM24C_WP_GPIO_Init(FM24C_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active);

/*!\brief Write Protect GPIO pin getter for FM24C
** \weak FM24C Write Protect GPIO pin getter may be user implemented if needed
** \param[in] pCpnt - Pointer to FM24C component
** \param[in,out] pState - Pointer to WP pin state variable (0: inactive, 1: active)
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ FM24C_WP_GPIO_Get(FM24C_t * const pCpnt, bool * const pState);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* __FM24C_EX_H__ */
/****************************************************************/
