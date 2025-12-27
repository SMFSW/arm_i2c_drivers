/*!\file MB85RC256V.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief MB85RC256V FRAM Driver
** \details MB85RC256V: 256-Kbit (32K * 8) I2C Memory FRAM
** \note Legacy component: consider switching to I2CMEM component for new designs
** \note Compatibility (tested):
**				- MB85RC256V
**				- FM24CL64B
**/
/****************************************************************/
#ifndef MB85RC256V_H__
	#define MB85RC256V_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/

#ifndef I2C_MB85RC256V_NB
//! \note Define I2C_MB85RC256V_NB to enable multiple peripherals of this type
#define I2C_MB85RC256V_NB	1U	//!< Number of MB85RC256V peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
#ifndef MB85RC256V_BASE_ADDR
//! \note Define MB85RC256V_BASE_ADDR to change default device base address
#define MB85RC256V_BASE_ADDR	0x50U			//!< MB85RC256V Base address
#endif


#define MB85RC256V_SIZE			0x8000U			//!< MB85RC256V size (in bytes)


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct _MB85RC256V_t
** \brief MB85RC256V user interface struct
**/
typedef struct _MB85RC256V_t {
	struct {
	I2C_slave_t *		slave_inst;		//!< Slave structure
	PeripheralGPIO_t	WP_GPIO;		//!< Write Protect GPIO struct
	} cfg;
	uint16_t			Manufacture_ID;	//!< Manufacturer ID
	uint8_t				Density;		//!< Chip density
	uint8_t				Product_ID;		//!< Product ID
} MB85RC256V_t;


// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern MB85RC256V_t	MB85RC256V[I2C_MB85RC256V_NB];		//!< MB85RC256V User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for MB85RC256V peripheral
** \param[in] idx - MB85RC256V index
** \param[in] hi2c - pointer to MB85RC256V I2C instance
** \param[in] devAddress - MB85RC256V device address
** \return FctERR - error code
**/
FctERR NONNULL__ MB85RC256V_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress);

/*!\brief Initialization for MB85RC256V peripheral
** \warning In case multiple devices (defined by I2C_MB85RC256V_NB > 1), you shall use MB85RC256V_Init instead
** \return FctERR - error code
**/
FctERR MB85RC256V_Init_Single(void);

/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for MB85RC256V
** \param[in] pCpnt - Pointer to MB85RC256V component
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR NONNULL__ MB85RC256V_Write(MB85RC256V_t * const pCpnt, const uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for MB85RC256V
** \param[in] pCpnt - Pointer to MB85RC256V component
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ MB85RC256V_Read(MB85RC256V_t * const pCpnt, uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read ID function for MB85RC256V (reads 3 bytes)
** \param[in] pCpnt - Pointer to MB85RC256V component
** \param[in,out] data - pointer to read to
** \return FctERR - error code
**/
FctERR NONNULL__ MB85RC256V_Read_ID(MB85RC256V_t * const pCpnt, uint8_t * data);


/****************************************************************/
// cppcheck-suppress-begin misra-c2012-20.1 ; include directives after other declarations
#include "MB85RC256V_proc.h"	// Include procedures
#include "MB85RC256V_ex.h"		// Include extensions
// cppcheck-suppress-end misra-c2012-20.1

#endif

#ifdef __cplusplus
	}
#endif

#endif /* MB85RC256V_H__ */
/****************************************************************/
