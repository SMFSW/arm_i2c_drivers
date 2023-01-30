/*!\file FM24C.h
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief FRAM / EEPROM Driver (bank switching at I2C address level protocol)
** \note The driver handles writing specificities for EEPROM type components
** \note The driver is fully compatible with FRAM type components
** \note When EEPROM / FRAM compatibility is not needed, FM24C_WRITE_SIZE can be set to FM24C_BANK_SIZE for more efficiency
** \details FM24C16B: 16-Kbit (2K * 8) Serial I2C F-RAM
**			FM24C04B: 4-Kbit (512 * 8) Serial I2C F-RAM
**			...
** \note	Compatibility (tested):
**				- FM24C16B
**				- FM24C04B
**				- BR24T04FVM
**/
/****************************************************************/
#ifndef __FM24C_H__
	#define __FM24C_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/

#ifndef I2C_FM24C_NB
//! \note Define I2C_FM24C_NB to enable multiple peripherals of this type
#define I2C_FM24C_NB	1	//!< Number of FM24C peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
#ifndef FM24C_BASE_ADDR
//! \note Define FM24C_BASE_ADDR to change default device base address
#define FM24C_BASE_ADDR		0x50				//!< FM24C Base address
#endif

#define	FM24C_BANK_SIZE		0x100				//!< FM24C bank size (in bytes)

#define	FM24C04B_SIZE		0x100				//!< FM24C04B size (in bytes)
#define	FM24C16B_SIZE		0x800				//!< FM24C size (in bytes)

#define EEP_WRITE_SIZE		0x10				//!< Write buffer size (used for write procedures)

#ifndef FM24C_SIZE
//! \note FM24C_SIZE defaults to FM24C16B size, but can be defined to FM24C04B_SIZE (for example)
#define FM24C_SIZE			FM24C16B_SIZE		//!< FM24C size
#endif

#ifndef FM24C_WRITE_SIZE
//! \note FM24C_WRITE_SIZE defaults to EEP_WRITE_SIZE, but can be defined to FM24C_BANK_SIZE (in case of FRAM, no use of EEPROM write buffer)
#define FM24C_WRITE_SIZE	EEP_WRITE_SIZE		//!< EEPROM type page size used for compatibility with both EEPROM/FRAM ships
#endif


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct FM24C_t
** \brief FM24C user interface struct
**/
typedef struct FM24C_t {
	struct {
	I2C_slave_t *		slave_inst;		//!< Slave structure
	size_t				write_size;		//!< Useful for EEPROM writes (FRAM not restricted, can be changed to FM24C_BANK_SIZE)
	PeripheralGPIO_t	WP_GPIO;		//!< Write Protect GPIO struct
	} cfg;
} FM24C_t;


// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern FM24C_t FM24C[I2C_FM24C_NB];		//!< FM24C User structure

// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for FM24C peripheral
** \param[in] idx - FM24C index
** \param[in] hi2c - pointer to FM24C I2C instance
** \param[in] devAddress - FM24C device address
** \return FctERR - error code
**/
FctERR NONNULL__ FM24C_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress);

/*!\brief Initialization for FM24C peripheral
** \warning In case multiple devices (defined by I2C_FM24C_NB > 1), you shall use FM24C_Init instead
** \return FctERR - error code
**/
FctERR FM24C_Init_Single(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for FM24C
**
** \param[in] pCpnt - Pointer to FM24C component
** \param[in,out] data - pointer to read/write to/from
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ FM24C_Write(FM24C_t * const pCpnt, const uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for FM24C
**
** \param[in] pCpnt - Pointer to FM24C component
** \param[in,out] data - pointer to read/write to/from
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ FM24C_Read(FM24C_t * const pCpnt, uint8_t * data, const uint16_t addr, const uint16_t nb);


/****************************************************************/
#include "FM24C_ex.h"		// Include extensions

#endif

#ifdef __cplusplus
	}
#endif

#endif /* __FM24C_H__ */
/****************************************************************/
