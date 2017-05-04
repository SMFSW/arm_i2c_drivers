/*!\file I2C_component.c
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Base I2C component code
**/
/****************************************************************/
#include "I2C_component.h"
#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/
// TODO: doxygen


FctERR I2C_slave_init(I2C_slave * slave, I2C_HandleTypeDef * hi2c, uint16_t devAddress, uint16_t size, uint32_t timeout)
{
	// TODO: assert params slave & hi2c
	/* Check the parameters */
	assert_param(IS_I2C_MEMADD_SIZE(size));

	if (devAddress & 0x1)		{ return ERR_VALUE; }

	slave->cfg.inst = hi2c;
	slave->cfg.addr = devAddress;
	slave->cfg.mem_size = size;
	slave->cfg.timeout = timeout;

	return ERR_OK;
}

FctERR I2C_set_slave_instance(I2C_slave * slave, I2C_HandleTypeDef * hi2c)
{
	// TODO: assert param hi2c (may be done for init too)

	slave->cfg.inst = hi2c;
	return ERR_OK;
}

FctERR I2C_set_slave_address(I2C_slave * slave, uint16_t devAddress)
{
	// TODO: assert param devAddress (may be done for init too)

	if (devAddress & 0x1)		{ return ERR_VALUE; }

	slave->cfg.addr = devAddress;
	return ERR_OK;
}

FctERR I2C_set_slave_mem_size(I2C_slave * slave, uint16_t size)
{
	/* Check the parameters */
	assert_param(IS_I2C_MEMADD_SIZE(size));

	slave->cfg.mem_size = size;
	return ERR_OK;
}

FctERR I2C_set_slave_timeout(I2C_slave * slave, uint32_t timeout)
{
	slave->cfg.timeout = timeout;
	return ERR_OK;
}


/****************************************************************/
#endif
/****************************************************************/
