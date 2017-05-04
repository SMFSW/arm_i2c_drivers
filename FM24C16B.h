/*!\file FM24C16B.h
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief FM24C16B Driver declarations
*/
/****************************************************************/
#ifndef __FM24C16B_H__
	#define __FM24C16B_H__

#include "I2C_component.h"
#include "FctERR.h"

#include "sarmfsw.h"
#include CMSIS_INC
#include CMSIS_CFG

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/
// TODO: doxygen


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define FM24C16B_BASE_ADDR	(0x50 << 1)	//!< FM24C16B Base address
#define	FM24C16B_SIZE		0x800		//!< FM24C16B size (in bytes)
#define	FM24C16B_BANK_SIZE	0x100		//!< FM24C16B bank size (in bytes)


#define ONE_BYTE	1					//!< Address width of 1 byte
#define TWO_BYTE	2					//!< Address width of 2 bytes

#define FRAM_ADDRWIDTH		((FM24C16B_BANK_SIZE-1 > 0xFF) ? TWO_BYTE : ONE_BYTE)	//!< FM24C16B Address width


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// Slave init
FctERR FM24C16B_Init(void);

// Low level access
FctERR FM24C16B_Write_Banked(uint8_t * Buffer, uint16_t Addr, uint8_t Bank, uint16_t nb);
FctERR FM24C16B_Read_Banked(uint8_t * Buffer, uint16_t Addr, uint8_t Bank, uint16_t nb);

FctERR FM24C16B_ReadWrite(uint8_t * Buffer, uint16_t Addr, uint16_t nb, bool wr);

__INLINE FctERR INLINE__ FM24C16B_Write(uint8_t * Buffer, uint16_t Addr, uint16_t nb) {
	return FM24C16B_ReadWrite(Buffer, Addr, nb, true);
}

__INLINE FctERR INLINE__ FM24C16B_Read(uint8_t * Buffer, uint16_t Addr, uint16_t nb) {
	return FM24C16B_ReadWrite(Buffer, Addr, nb, false);
}


/****************************************************************/
#endif
#endif /* __FM24C16B_H__ */
/****************************************************************/
