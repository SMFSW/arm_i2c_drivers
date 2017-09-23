/*!\file FM24C.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief FM24C Driver declarations
** \details FM24C16B: 16-Kbit (2K * 8) Serial I2C F-RAM
**			FM24C04B: 4-Kbit (512 * 8) Serial I2C F-RAM
**/
/****************************************************************/
#ifndef __FM24C_H__
	#define __FM24C_H__

#include "I2C_component.h"
#include "FctERR.h"

#include "sarmfsw.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define FM24C_BASE_ADDR		0x50				//!< FM24C Base address


#define	FM24C_BANK_SIZE		0x100				//!< FM24C bank size (in bytes)

#define	FM24C04B_SIZE		0x100				//!< FM24C04B size (in bytes)
#define	FM24C16B_SIZE		0x800				//!< FM24C size (in bytes)

#define FM24C_SIZE			FM24C16B_SIZE		//!< FM24C size


// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern I2C_slave FM24C_hal;	//!< FM24C Slave instance


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for FM24C peripheral
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
FctERR FM24C_Write_Banked(uint8_t * data, uint16_t addr, uint8_t bank, uint16_t nb);

/*!\brief I2C Read function for FM24C
**
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] bank - Bank to read
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR FM24C_Read_Banked(uint8_t * data, uint16_t addr, uint8_t bank, uint16_t nb);

/*!\brief I2C Read/Write function for FM24C
**
** \param[in,out] data - pointer to read/write to/from
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \param[in] wr - 0: Read / 1: Write
** \return FctERR - error code
**/
FctERR FM24C_ReadWrite(uint8_t * data, uint16_t addr, uint16_t nb, bool wr);


/*!\brief I2C Write function for FM24C
**
** \param[in,out] data - pointer to read/write to/from
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ FM24C_Write(uint8_t * data, uint16_t addr, uint16_t nb) {
	return FM24C_ReadWrite(data, addr, nb, true);
}

/*!\brief I2C Read function for FM24C
**
** \param[in,out] data - pointer to read/write to/from
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ FM24C_Read(uint8_t * data, uint16_t addr, uint16_t nb) {
	return FM24C_ReadWrite(data, addr, nb, false);
}


/****************************************************************/
#endif
#endif /* __FM24C_H__ */
/****************************************************************/
