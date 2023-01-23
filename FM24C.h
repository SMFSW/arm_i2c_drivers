/*!\file FM24C.h
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief FM24C Driver
** \details FM24C16B: 16-Kbit (2K * 8) Serial I2C F-RAM
**			FM24C04B: 4-Kbit (512 * 8) Serial I2C F-RAM
** \note	Compatibility (tested):
**				- FM24C16B
**				- FM24C04B
**				- BR24T04FVM
** \note	Compatibility:
**				- other components using same i2c protocol may be compatible
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

#ifndef FM24C_SIZE
//! \note FM24C_SIZE defaults to FM24C16B size, but can be defined level to FM24C04B_SIZE (for example)
#define FM24C_SIZE			FM24C16B_SIZE		//!< FM24C size
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
	PeripheralGPIO_t	WP_GPIO;		//!< Write Protect GPIO struct
	} cfg;
} FM24C_t;


// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern I2C_slave_t	FM24C_hal[I2C_FM24C_NB];	//!< FM24C Slave structure
extern FM24C_t		FM24C[I2C_FM24C_NB];		//!< FM24C User structure

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
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] bank - Bank to write
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR NONNULL__ FM24C_Write_Banked(FM24C_t * const pCpnt, const uint8_t * data, const uint16_t addr, const uint8_t bank, const uint16_t nb);


/*!\brief I2C Read function for FM24C
**
** \param[in] pCpnt - Pointer to FM24C component
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] bank - Bank to read
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ FM24C_Read_Banked(FM24C_t * const pCpnt, uint8_t * data, const uint16_t addr, const uint8_t bank, const uint16_t nb);


/*!\brief I2C Read/Write function for FM24C
**
** \param[in] pCpnt - Pointer to FM24C component
** \param[in,out] data - pointer to read/write to/from
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \param[in] wr - 0: Read / 1: Write
** \return FctERR - error code
**/
FctERR NONNULL__ FM24C_ReadWrite(FM24C_t * const pCpnt, uint8_t * data, const uint16_t addr, const uint16_t nb, const bool wr);


/*!\brief I2C Write function for FM24C
**
** \param[in] pCpnt - Pointer to FM24C component
** \param[in,out] data - pointer to read/write to/from
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ FM24C_Write(FM24C_t * const pCpnt, const uint8_t * data, const uint16_t addr, const uint16_t nb) {
	return FM24C_ReadWrite(pCpnt, (uint8_t *) data, addr, nb, true);
}


/*!\brief I2C Read function for FM24C
**
** \param[in] pCpnt - Pointer to FM24C component
** \param[in,out] data - pointer to read/write to/from
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ FM24C_Read(FM24C_t * const pCpnt, uint8_t * data, const uint16_t addr, const uint16_t nb) {
	return FM24C_ReadWrite(pCpnt, data, addr, nb, false);
}


/****************************************************************/
#include "FM24C_ex.h"		// Include extensions

#endif

#ifdef __cplusplus
	}
#endif

#endif /* __FM24C_H__ */
/****************************************************************/
