/*!\file I2C_component.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Base I2C component code
**/
/****************************************************************/
#include "I2C_component.h"
#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/
#include <string.h>
/****************************************************************/


//static const I2C_slave slave_hal = { { pNull, 0, I2C_slave_timeout, 0, I2C_STD }, 0, HAL_OK, true, false };	//!< default slave hal values


FctERR I2C_slave_init(I2C_slave * slave, I2C_HandleTypeDef * hi2c, uint16_t devAddress, uint32_t timeout)
{
	// TODO: assert speed if possible (as there's max_speed for device now)
	/* Check the parameters */
	assert_param(IS_I2C_ALL_INSTANCE(hi2c->Instance));
	assert_param(IS_I2C_7B_ADDR(devAddress));

	if ((!slave) || (!hi2c))	{ return ERR_INSTANCE; }	// Unknown instance (null pointer)

	//memcpy(slave, &slave_hal, sizeof(I2C_slave));		//! \note max_speed is const and should be set at init

	slave->cfg.inst = hi2c;
	slave->cfg.addr = I2C_ADDR(devAddress);
	slave->cfg.timeout = timeout;
	return ERR_OK;
}


FctERR I2C_set_slave_instance(I2C_slave * slave, I2C_HandleTypeDef * hi2c)
{
	/* Check the parameters */
	assert_param(IS_I2C_ALL_INSTANCE(hi2c->Instance));

	if ((!slave) || (!hi2c))	{ return ERR_INSTANCE; }	// Unknown instance (null pointer)

	slave->cfg.inst = hi2c;
	return ERR_OK;
}


FctERR I2C_set_slave_address(I2C_slave * slave, uint16_t devAddress)
{
	/* Check the parameters */
	assert_param(IS_I2C_7B_ADDR(devAddress));

	if (!slave)					{ return ERR_INSTANCE; }	// Unknown instance (null pointer)

	slave->cfg.addr = I2C_ADDR(devAddress);
	return ERR_OK;
}


FctERR I2C_set_slave_timeout(I2C_slave * slave, uint32_t timeout)
{
	if (!slave)					{ return ERR_INSTANCE; }	// Unknown instance (null pointer)

	slave->cfg.timeout = timeout;
	return ERR_OK;
}


/****************************************************************/
#endif
/****************************************************************/
