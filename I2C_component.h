/*!\file I2C_component.h
** \author SMFSW
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


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define	I2C_ADDR_General_Call	0			//!< I2C general call address

#define I2C_slave_timeout		20			//!< Default timeout for transaction on I2C


#define I2C_ADDR(a)				((a) << 1)	//!< Macro to shift \b a 1 bit left to define 7b I2C address


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*! \enum I2C_speed
**  \brief I2C bus speed
**/
typedef enum PACK__ I2C_speed {
	I2C_STD = 100,		//!< I2C Standard speed (100KHz)
	I2C_FM = 400,		//!< I2C Fast Mode (400KHz)
	I2C_FMP = 1000,		//!< I2C Fast Mode + (1MHz)
	I2C_HS = 3400		//!< I2C High Speed (3.4MHz)
} I2C_speed;


/*! \enum I2C_reg_size
**  \brief I2C slave internal address registers size
**/
typedef enum PACK__ I2C_reg_size {
	I2C_NO_REG = 0x00,						//!< Internal address registers not applicable for slave
	I2C_8B_REG = I2C_MEMADD_SIZE_8BIT,		//!< Slave internal address registers space is 8bits wide
	I2C_16B_REG = I2C_MEMADD_SIZE_16BIT		//!< Slave internal address registers space is 16bits wide
} I2C_reg_size;


/*! \struct StructI2CSlave
**  \brief ci2c slave config and control parameters
**/
typedef struct PACK__ StructI2Cslave {	// TODO: check if the struct really needs to be packed
	/*! \struct cfg
	**  \brief I2C slave parameters
	**/
	struct {
		I2C_HandleTypeDef *	inst;		//!< Slave I2C bus instance
		uint16_t			addr;		//!< Slave address
		uint32_t			timeout;	//!< Slave transaction timeout
		const I2C_reg_size	mem_size;	//!< Slave internal registers size
		const I2C_speed		max_speed;	//!< Maximal speed allowed by the component
	} cfg;
	uint16_t				addr;		//!< Current internal register address (not used yet)
	HAL_StatusTypeDef		status;		//!< Status of the last transaction
	bool					busy;		//!< TODO: implement to be checked if transaction pending (it?)
} I2C_slave;


#define IS_I2C_7B_ADDR(ADDR)	((ADDR) <= 0x7F)	//!< Macro for use with assert_param to check I2C \b ADDR for slave


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief I2C Slave device initialization
** \param[in,out] slave - pointer to I2C slave instance to initialize
** \param[in] hi2c - pointer to HAL I2C instance
** \param[in] devAddress - Device Address
** \param[in] timeout - Transaction timeout
** \return Error code
**/
FctERR I2C_slave_init(I2C_slave * slave, I2C_HandleTypeDef * hi2c, uint16_t devAddress, uint32_t timeout);


/***************/
/*** SETTERS ***/
/***************/
/*!\brief I2C Slave device HAL instance change
** \param[in,out] slave - pointer to I2C slave instance
** \param[in] hi2c - pointer to HAL I2C instance
** \return Error code
**/
FctERR I2C_set_slave_instance(I2C_slave * slave, I2C_HandleTypeDef * hi2c);

/*!\brief I2C Slave device address change
** \param[in,out] slave - pointer to I2C slave instance
** \param[in] devAddress - Device Address
** \return Error code
**/
FctERR I2C_set_slave_address(I2C_slave * slave, uint16_t devAddress);

/*!\brief I2C Slave device transaction timeout change
** \param[in,out] slave - pointer to I2C slave instance
** \param[in] timeout - Transaction timeout
** \return Error code
**/
FctERR I2C_set_slave_timeout(I2C_slave * slave, uint32_t timeout);


/*!\brief Set I2C Slave device current internal memory address
** \param[in,out] slave - pointer to I2C slave instance
** \param[in] addr - address to store in slave instance
** \return Nothing
**/
__INLINE FctERR INLINE__ I2C_set_current_mem_address(I2C_slave * slave, uint16_t addr) {
	slave->addr = addr;
	return ERR_OK; }

/*!\brief Set I2C Slave bus/device business
** \param[in,out] slave - pointer to I2C slave instance
** \param[in] busy - I2C bus/device state
** \return Nothing
**/
__INLINE FctERR INLINE__ I2C_set_busy(I2C_slave * slave, bool busy) {
	slave->busy = busy;
	return ERR_OK; }


/***************/
/*** GETTERS ***/
/***************/
/*!\brief Get I2C Slave device current internal memory address
** \param[in,out] slave - pointer to I2C slave instance
** \return Current internal memory address
**/
__INLINE uint32_t INLINE__ I2C_get_current_mem_address(I2C_slave * slave) {
	return slave->addr; }

/*!\brief Get I2C Slave device busy state
** \param[in,out] slave - pointer to I2C slave instance
** \return true if I2C bus/slave is busy
**/
__INLINE bool INLINE__ I2C_is_busy(I2C_slave * slave) {
	return slave->busy; }

/*!\brief Get I2C Slave device HAL I2C instance
** \param[in,out] slave - pointer to I2C slave instance
** \return I2C Slave device HAL I2C instance
**/
__INLINE I2C_HandleTypeDef * INLINE__ I2C_get_slave_instance(I2C_slave * slave) {
	return slave->cfg.inst; }

/*!\brief Get I2C Slave device address
** \param[in,out] slave - pointer to I2C slave instance
** \return I2C Slave device address
**/
__INLINE uint16_t INLINE__ I2C_get_slave_address(I2C_slave * slave) {
	return slave->cfg.addr; }

/*!\brief Get I2C Slave device transaction timeout
** \param[in,out] slave - pointer to I2C slave instance
** \return I2C Slave device transaction timeout
**/
__INLINE uint32_t INLINE__ I2C_get_slave_timeout(I2C_slave * slave) {
	return slave->cfg.timeout; }

/*!\brief Get I2C Slave device internal memory size
** \param[in,out] slave - pointer to I2C slave instance
** \return I2C Slave device internal memory size
**/
__INLINE uint16_t INLINE__ I2C_get_slave_mem_size(I2C_slave * slave) {
	return slave->cfg.mem_size; }

/*!\brief Get I2C Slave device max speed
** \param[in,out] slave - pointer to I2C slave instance
** \return I2C Slave device maximum speed
**/
__INLINE uint16_t INLINE__ I2C_get_slave_max_speed(I2C_slave * slave) {
	return slave->cfg.max_speed; }


/****************************************************************/
#endif
#endif	/* __I2C_COMPONENT_H__ */
/****************************************************************/
