/*!\file FM24C.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief FM24C Driver
** \details FM24C16B: 16-Kbit (2K * 8) Serial I2C F-RAM
**			FM24C04B: 4-Kbit (512 * 8) Serial I2C F-RAM
**/
/****************************************************************/
#ifndef __FM24C_H__
	#define __FM24C_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "globals.h"

#include "I2C_component.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define FM24C_BASE_ADDR		0x50				//!< FM24C Base address


#define	FM24C_BANK_SIZE		0x100				//!< FM24C bank size (in bytes)

#define	FM24C04B_SIZE		0x100				//!< FM24C04B size (in bytes)
#define	FM24C16B_SIZE		0x800				//!< FM24C size (in bytes)

#ifndef FM24C_SIZE
//!< FM24C_SIZE defaults to FM24C16B size, but can be defined in globals.h or at project level for FM24C04B for example
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
	I2C_slave_t *	slave_inst;		//!< Slave structure
	} cfg;
} FM24C_t;


// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern I2C_slave_t	FM24C_hal;	//!< FM24C Slave structure
extern FM24C_t	FM24C;		//!< FM24C User structure

// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for FM24C peripheral
** \weak FM24C Base address may be changed if user implemented
** \return FctERR - error code
**/
FctERR FM24C_Init(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for FM24C
**
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] bank - Bank to write
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR NONNULL__ FM24C_Write_Banked(const uint8_t * data, const uint16_t addr, const uint8_t bank, const uint16_t nb);

/*!\brief I2C Read function for FM24C
**
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] bank - Bank to read
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ FM24C_Read_Banked(uint8_t * data, const uint16_t addr, const uint8_t bank, const uint16_t nb);

/*!\brief I2C Read/Write function for FM24C
**
** \param[in,out] data - pointer to read/write to/from
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \param[in] wr - 0: Read / 1: Write
** \return FctERR - error code
**/
FctERR NONNULL__ FM24C_ReadWrite(uint8_t * data, const uint16_t addr, const uint16_t nb, const bool wr);


/*!\brief I2C Write function for FM24C
**
** \param[in,out] data - pointer to read/write to/from
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ FM24C_Write(const uint8_t * data, const uint16_t addr, const uint16_t nb) {
	return FM24C_ReadWrite((uint8_t *) data, addr, nb, true);
}

/*!\brief I2C Read function for FM24C
**
** \param[in,out] data - pointer to read/write to/from
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ FM24C_Read(uint8_t * data, const uint16_t addr, const uint16_t nb) {
	return FM24C_ReadWrite(data, addr, nb, false);
}


/****************************************************************/
#include "FM24C_ex.h"		// Include extensions

#ifdef __cplusplus
	}
#endif

#endif
#endif /* __FM24C_H__ */
/****************************************************************/
