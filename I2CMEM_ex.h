/*!\file I2CMEM_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief FRAM / EEPROM Driver extensions
** \note Fully compatible between EEPROM / FRAM type components
** \note When EEPROM compatibility is not needed, buf_size at init can be set to \ref I2CMEM_WBUF_NONE for more efficiency
**/
/****************************************************************/
#ifndef I2CMEM_EX_H__
	#define I2CMEM_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "I2CMEM.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/

#ifndef I2CMEM_CLR_VAL
//! \note I2CMEM_CLR_VAL can be set to any value if needed, defaults to 0xFF
#define I2CMEM_CLR_VAL		0xFFU	//!< Mass erase value for I2CMEM
#endif


#define I2CMEM_Read_Type(cpnt, name, type, addr)	__INLINE FctERR NONNULL_INLINE__ I2CMEM_Get_##name(type * rd) {	\
														return I2CMEM_Read(cpnt, &rd, addr, sizeof(type)); }	//!< Macro to create function to read value(s) from I2CMEM

#define I2CMEM_Write_Type(cpnt, name, type, addr)	__INLINE FctERR INLINE__ I2CMEM_Set_##name(type wr) {	\
														return I2CMEM_Write(cpnt, &wr, addr, sizeof(type)); }	//!< Macro to create function to write value(s) to I2CMEM

#define I2CMEM_1K			0x0080U	//!< 1Kb size
#define I2CMEM_2K			0x0100U	//!< 2Kb size
#define I2CMEM_4K			0x0200U	//!< 4Kb size
#define I2CMEM_8K			0x0400U	//!< 8Kb size
#define I2CMEM_16K			0x0800U	//!< 16Kb size
#define I2CMEM_32K			0x1000U	//!< 32Kb size
#define I2CMEM_64K			0x2000U	//!< 64Kb size
#define I2CMEM_128K			0x4000U	//!< 128Kb size
#define I2CMEM_256K			0x8000U	//!< 256Kb size

#define I2CMEM_WBUF_NONE	0		//!< No buffer
#define I2CMEM_WBUF_8		8U		//!< 8B buffer
#define I2CMEM_WBUF_16		16U		//!< 16B buffer
#define I2CMEM_WBUF_32		32U		//!< 32B buffer
#define I2CMEM_WBUF_64		64U		//!< 64B buffer


/*** Known device aliases ***/
#define _24xx32_Init(idx, hi2c, devAddress)		I2CMEM_Init(idx, hi2c, devAddress, I2CMEM_32K, I2CMEM_WBUF_32)		//!< EEPROM 24xx32 Init alias
#define _24xx32_Init_Single()					I2CMEM_Init_Single(I2CMEM_32K, I2CMEM_WBUF_32)						//!< EEPROM 24xx32 Single Init alias

#define _24xx64_Init(idx, hi2c, devAddress)		I2CMEM_Init(idx, hi2c, devAddress, I2CMEM_64K, I2CMEM_WBUF_32)		//!< EEPROM 24xx64 Init alias
#define _24xx64_Init_Single()					I2CMEM_Init_Single(I2CMEM_64K, I2CMEM_WBUF_32)						//!< EEPROM 24xx64 Single Init alias

#define _24xx128_Init(idx, hi2c, devAddress)	I2CMEM_Init(idx, hi2c, devAddress, I2CMEM_128K, I2CMEM_WBUF_64)		//!< EEPROM 24xx128 Init alias
#define _24xx128_Init_Single()					I2CMEM_Init_Single(I2CMEM_128K, I2CMEM_WBUF_64)						//!< EEPROM 24xx128 Single Init alias

#define _FM24C04_Init(idx, hi2c, devAddress)	I2CMEM_Init(idx, hi2c, devAddress, I2CMEM_4K, I2CMEM_WBUF_NONE)		//!< FRAM FM24C04 Init alias
#define _FM24C04_Init_Single()					I2CMEM_Init_Single(I2CMEM_4K, I2CMEM_WBUF_NONE)						//!< FRAM FM24C04 Single Init alias

#define _FM24C16_Init(idx, hi2c, devAddress)	I2CMEM_Init(idx, hi2c, devAddress, I2CMEM_16K, I2CMEM_WBUF_NONE)	//!< FRAM FM24C16 Init alias
#define _FM24C16_Init_Single()					I2CMEM_Init_Single(I2CMEM_16K, I2CMEM_WBUF_NONE)					//!< FRAM FM24C16 Single Init alias

#define _FM24C64_Init(idx, hi2c, devAddress)	I2CMEM_Init(idx, hi2c, devAddress, I2CMEM_64K, I2CMEM_WBUF_NONE)	//!< FRAM FM24C64 Init alias
#define _FM24C64_Init_Single()					I2CMEM_Init_Single(I2CMEM_64K, I2CMEM_WBUF_NONE)					//!< FRAM FM24C64 Single Init alias

#define _M24C16_Init(idx, hi2c, devAddress)		I2CMEM_Init(idx, hi2c, devAddress, I2CMEM_16K, I2CMEM_WBUF_16)		//!< EEPROM M24C16 Init alias
#define _M24C16_Init_Single()					I2CMEM_Init_Single(I2CMEM_16K, I2CMEM_WBUF_16)						//!< EEPROM M24C16 Single Init alias

#define _MB85RC256_Init(idx, hi2c, devAddress)	I2CMEM_Init(idx, hi2c, devAddress, I2CMEM_256K, I2CMEM_WBUF_NONE)	//!< FRAM MB85RC256 Init alias
#define _MB85C256_Init_Single()					I2CMEM_Init_Single(I2CMEM_256K, I2CMEM_WBUF_NONE)					//!< FRAM MB85RC256 Single Init alias


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

/*!\brief Mass erase of I2CMEM
** \param[in] pCpnt - Pointer to I2CMEM component
** \return FctERR - error code
**/
FctERR NONNULL__ I2CMEM_Mass_Erase(I2CMEM_t * const pCpnt);


/*******************/
/*** GPIO access ***/
/*******************/

/*!\brief Write Protect GPIO pin init for I2CMEM
** \weak I2CMEM Write Protect GPIO pin init may be user implemented if needed
** \param[in] pCpnt - Pointer to I2CMEM component
** \param[in] GPIOx - WP port
** \param[in] GPIO_Pin - WP pin
** \param[in] GPIO_Active: WP pin active state
**/
void NONNULL__ I2CMEM_WP_GPIO_Init(I2CMEM_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active);

/*!\brief Write Protect GPIO pin setter for I2CMEM
** \weak I2CMEM Write Protect GPIO pin setter may be user implemented if needed
** \param[in] pCpnt - Pointer to I2CMEM component
** \param[in] state - state to write on WP pin (0: inactive, 1: active)
**/
void NONNULL__ I2CMEM_WP_GPIO_Set(I2CMEM_t * const pCpnt, const bool state);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* I2CMEM_EX_H__ */
/****************************************************************/
