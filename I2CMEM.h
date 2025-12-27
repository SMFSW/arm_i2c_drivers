/*!\file I2CMEM.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief FRAM / EEPROM Driver
** \note Fully compatible between EEPROM / FRAM type components
** \note When EEPROM compatibility is not needed, buf_size at init can be set to \ref I2CMEM_WBUF_NONE for more efficiency
**/
/****************************************************************/
#ifndef I2CMEM_H__
	#define I2CMEM_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/

#ifndef I2C_I2CMEM_NB
//! \note Define I2C_I2CMEM_NB to enable multiple peripherals of this type
#define I2C_I2CMEM_NB	1U	//!< Number of I2CMEM peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
#ifndef I2CMEM_BASE_ADDR
//! \note Define I2CMEM_BASE_ADDR to change default device base address
#define I2CMEM_BASE_ADDR		0x50U		//!< I2CMEM Base address
#endif

#define	I2CMEM_BANK_SIZE		0x100U		//!< I2CMEM bank size (in bytes) for <=16Kb devices


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct _I2CMEM_t
** \brief I2CMEM user interface struct
**/
typedef struct _I2CMEM_t {
	struct {
	I2C_slave_t *		slave_inst;		//!< Slave structure
	size_t				chip_size;		//!< Memory range (number of bytes)
	size_t				buf_size;		//!< Useful for EEPROM writes (FRAM not restricted, can be changed to I2CMEM_BANK_SIZE)
	PeripheralGPIO_t	WP_GPIO;		//!< Write Protect GPIO struct
	} cfg;
} I2CMEM_t;


// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern I2CMEM_t I2CMEM[I2C_I2CMEM_NB];		//!< I2CMEM User structure

// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for I2CMEM peripheral
** \param[in] idx - I2CMEM index
** \param[in] hi2c - pointer to I2CMEM I2C instance
** \param[in] devAddress - I2CMEM device address
** \param[in] size - I2CMEM device size
** \param[in] buf_size - I2CMEM device write buffer size (typically \ref I2CMEM_WBUF_NONE for FRAM component)
** \return FctERR - error code
**/
FctERR NONNULL__ I2CMEM_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress, const size_t size, const size_t buf_size);

/*!\brief Initialization for I2CMEM peripheral
** \warning In case multiple devices (defined by I2C_I2CMEM_NB > 1), you shall use I2CMEM_Init instead
** \param[in] size - I2CMEM device size
** \param[in] buf_size - I2CMEM device write buffer size (typically \ref I2CMEM_WBUF_NONE for FRAM component)
** \return FctERR - error code
**/
FctERR I2CMEM_Init_Single(const size_t size, const size_t buf_size);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for I2CMEM
**
** \param[in] pCpnt - Pointer to I2CMEM component
** \param[in,out] data - pointer to read/write to/from
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ I2CMEM_Write(I2CMEM_t * const pCpnt, const uint8_t * const data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for I2CMEM
**
** \param[in] pCpnt - Pointer to I2CMEM component
** \param[in,out] data - pointer to read/write to/from
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ I2CMEM_Read(I2CMEM_t * const pCpnt, uint8_t * const data, const uint16_t addr, const uint16_t nb);


/****************************************************************/
// cppcheck-suppress-begin misra-c2012-20.1 ; include directives after other declarations
#include "I2CMEM_ex.h"		// Include extensions
// cppcheck-suppress-end misra-c2012-20.1

#endif

#ifdef __cplusplus
	}
#endif

#endif /* I2CMEM_H__ */
/****************************************************************/
