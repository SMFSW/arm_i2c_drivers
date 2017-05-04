/*!\file I2C_component.h
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Base I2C component declarations
**/
/****************************************************************/
#ifndef __I2C_COMPONENT_H__
	#define __I2C_COMPONENT_H__

#include "FctERR.h"

#include "sarmfsw.h"
#include CMSIS_INC
//#include CMSIS_CFG

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/
// TODO: doxygen


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define I2C_slave_timeout		20		//!< Default timeout for transaction on I2C


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*! \struct StructI2CSlave
**  \brief ci2c slave config and control parameters
**  \attribute packed struct
**/
typedef struct PACK__ StructI2Cslave {
	/*! \struct cfg
	 *  \brief ci2c slave parameters
	 */
	struct {
		I2C_HandleTypeDef *	inst;		//!< Slave I2C bus instance
		uint16_t			addr;		//!< Slave address
		uint16_t			mem_size;	//!< Slave internal registers size
		uint32_t			timeout;	//!< Slave transaction timeout
	} cfg;
	uint16_t				addr;		//!< Current internal register address
	HAL_StatusTypeDef		status;		//!< Status of the last transaction
	bool					busy;		//!< TODO: implement to be checked if transaction pending (it?)
} I2C_slave;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
FctERR I2C_slave_init(I2C_slave * slave, I2C_HandleTypeDef * hi2c, uint16_t devAddress, uint16_t size, uint32_t timeout);

// TODO: inlines or macros for the following ??
FctERR I2C_set_slave_instance(I2C_slave * slave, I2C_HandleTypeDef * hi2c);
FctERR I2C_set_slave_address(I2C_slave * slave, uint16_t devAddress);
FctERR I2C_set_slave_mem_size(I2C_slave * slave, uint16_t size);
FctERR I2C_set_slave_timeout(I2C_slave * slave, uint32_t timeout);

/*** STATUS ***/
__INLINE uint32_t INLINE__ I2C_get_current_mem_address(I2C_slave * slave) {
	return slave->addr; }

__INLINE uint32_t INLINE__ I2C_is_busy(I2C_slave * slave) {
	return slave->busy; }


/*** CONFIG ***/
__INLINE I2C_HandleTypeDef * INLINE__ I2C_get_slave_instance(I2C_slave * slave) {
	return slave->cfg.inst; }

__INLINE uint16_t INLINE__ I2C_get_slave_address(I2C_slave * slave) {
	return slave->cfg.addr; }

__INLINE uint16_t INLINE__ I2C_get_slave_mem_size(I2C_slave * slave) {
	return slave->cfg.mem_size; }

__INLINE uint32_t INLINE__ I2C_get_slave_timeout(I2C_slave * slave) {
	return slave->cfg.timeout; }


/****************************************************************/
#endif
#endif	/* __I2C_COMPONENT_H__ */
/****************************************************************/
