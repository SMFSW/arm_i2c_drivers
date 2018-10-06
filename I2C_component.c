/*!\file I2C_component.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief Base I2C component
** \warning Components with a xxx_proc file can access I2C_slave_t instance through CPNT->cfg.slave_inst.
**			Please keep in mind some components are somewhat custom and needs to be accesses through CPNT->cfg.slave_inst_xxx instead if needed.
**/
/****************************************************************/
#include "I2C_component.h"
#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


FctERR NONNULL__ I2C_slave_init(I2C_slave_t * slave, const I2C_HandleTypeDef * hi2c, const uint16_t devAddress, const uint32_t timeout)
{
	// TODO: assert speed if possible (as there's max_speed for device now)
	/* Check the parameters */
	assert_param(IS_I2C_ALL_INSTANCE(hi2c->Instance));
	assert_param(IS_I2C_7B_ADDR(devAddress));

	//memcpy(slave, &slave_hal, sizeof(I2C_slave_t));		//! \note max_speed is const and should be set at init

	slave->cfg.bus_inst = (I2C_HandleTypeDef *) hi2c;
	slave->cfg.addr = I2C_ADDR(devAddress);
	slave->cfg.timeout = timeout;
	return ERROR_OK;
}


FctERR NONNULL__ I2C_set_slave_instance(I2C_slave_t * slave, const I2C_HandleTypeDef * hi2c)
{
	/* Check the parameters */
	assert_param(IS_I2C_ALL_INSTANCE(hi2c->Instance));

	slave->cfg.bus_inst = (I2C_HandleTypeDef *) hi2c;
	return ERROR_OK;
}


FctERR NONNULL__ I2C_set_slave_address(I2C_slave_t * slave, const uint16_t devAddress)
{
	/* Check the parameters */
	assert_param(IS_I2C_7B_ADDR(devAddress));

	slave->cfg.addr = I2C_ADDR(devAddress);
	return ERROR_OK;
}


/****************************************************************/
#endif
/****************************************************************/
