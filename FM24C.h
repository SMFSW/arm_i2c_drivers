/*!\file FM24C.h
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief FM24C FRAM Driver (bank switching at I2C address level protocol)
** \details FM24C16B: 16-Kbit (2K * 8) Serial I2C F-RAM
**			FM24C04B: 4-Kbit (512 * 8) Serial I2C F-RAM
** \note Legacy component: consider switching to I2CMEM component for new designs
** \note Compatibility (tested):
**				- FM24C16B
**				- FM24C04B
**				- BR24T04FVM
**/
/****************************************************************/
#ifndef FM24C_H__
	#define FM24C_H__

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
#define I2C_FM24C_NB	1U	//!< Number of FM24C peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
#ifndef FM24C_BASE_ADDR
//! \note Define FM24C_BASE_ADDR to change default device base address
#define FM24C_BASE_ADDR			0x50U					//!< FM24C Base address
#endif

#define	FM24C_BANK_SIZE			0x100U					//!< FM24C bank size (in bytes)

#define	FM24C04B_SIZE			0x200U					//!< FM24C04 size (in bytes)
#define	FM24C16B_SIZE			0x800U					//!< FM24C16 size (in bytes)

#ifndef FM24C_SIZE
//! \note FM24C_SIZE defaults to FM24C16B size, but can be defined to FM24C04B_SIZE (for example)
#define FM24C_SIZE				FM24C16B_SIZE			//!< FM24C size
#endif


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct FM24C_t
** \brief FM24C user interface struct
**/
typedef struct {
	struct {
	I2C_slave_t *		slave_inst;		//!< Slave structure
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
FctERR NONNULL__ FM24C_Write(FM24C_t * const pCpnt, const uint8_t * const data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for FM24C
**
** \param[in] pCpnt - Pointer to FM24C component
** \param[in,out] data - pointer to read/write to/from
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ FM24C_Read(FM24C_t * const pCpnt, uint8_t * const data, const uint16_t addr, const uint16_t nb);


/****************************************************************/
// cppcheck-suppress-begin misra-c2012-20.1 ; include directives after other declarations
#include "FM24C_ex.h"		// Include extensions
// cppcheck-suppress-end misra-c2012-20.1

#endif

#ifdef __cplusplus
	}
#endif

#endif /* FM24C_H__ */
/****************************************************************/
