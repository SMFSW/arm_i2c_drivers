/*!\file MB85RC256V.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief MB85RC256V Driver declarations
** \details MB85RC256V: 256-Kbit (32K * 8) I2C Memory FRAM
**/
/****************************************************************/
#ifndef __MB85RC256V_H__
	#define __MB85RC256V_H__

#include "I2C_component.h"
#include "FctERR.h"

#include "sarmfsw.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define MB85RC256V_BASE_ADDR	0x50			//!< MB85RC256V Base address

#define MB85RC256V_SIZE			0x8000			//!< MB85RC256V size (in bytes)


// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern I2C_slave MB85RC256V_hal;	//!< MB85RC256V Slave instance


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for MB85RC256V peripheral
** \return FctERR - error code
**/
FctERR MB85RC256V_Init(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for MB85RC256V
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR MB85RC256V_Write(uint8_t * data, uint16_t addr, uint16_t nb);


/*!\brief I2C Read function for MB85RC256V
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR MB85RC256V_Read(uint8_t * data, uint16_t addr, uint16_t nb);


/*!\brief I2C Read ID function for MB85RC256V (reads 3 bytes)
** \param[in,out] data - pointer to read to
** \return FctERR - error code
**/
FctERR MB85RC256V_Read_ID(uint8_t * data);


/****************************************************************/
#include "MB85RC256V_ex.h"		// Include extensions
#include "MB85RC256V_proc.h"	// Include procedures
#endif
#endif /* __MB85RC256V_H__ */
/****************************************************************/
