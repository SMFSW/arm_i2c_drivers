/*!\file PCA9952.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief PCA9952 Driver
** \details PCA9952: 16-channel Fm+ I2C-bus 57 mA constant current LED driver
**/
/****************************************************************/
#include "PCA9952.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9952)
/****************************************************************/
#if (defined(I2C_PCA9624) || defined(PCA9685) || defined(I2C_PCA9956)) && !defined(NO_WARN_I2C_DRIVERS)
#warning "PCA9952 -> Multiple PCA9xxx types: use with caution if using CALL addresses if on same I2C bus!!!"
#endif
/****************************************************************/


static const I2C_slave_t PCA9952_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FMP }, 0, HAL_OK, true, false };

static I2C_slave_t PCA9952_hal[I2C_PCA9952_NB];		//!< PCA9952 Slave structure


/****************************************************************/


FctERR NONNULL__ PCA9952_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	FctERR err;

	assert_param(IS_I2C_PERIPHERAL(PCA9952, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(PCA9952, idx);

	I2C_slave_init(&PCA9952_hal[idx], hi2c, devAddress, PCA9952_hal[idx].cfg.timeout);

	err = PCA9952_Init_Sequence(&PCA9952[idx]);

	if (err != ERROR_OK)	{ I2C_set_enable(&PCA9952_hal[idx], false); }

	return err;
}

FctERR PCA9952_Init_Single(void) {
	return PCA9952_Init(0, I2C_PCA9952, PCA9952_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ PCA9952_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	const PCA962x_reg_inc inc_mode = PCA9952[pSlave - PCA9952_hal].cfg.auto_inc & 0x80U;

	if (!I2C_is_enabled(pSlave))				{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > PCA9952__IREFALL + 1U)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr | inc_mode, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ PCA9952_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	const PCA962x_reg_inc inc_mode = PCA9952[pSlave - PCA9952_hal].cfg.auto_inc & 0x80U;

	if (!I2C_is_enabled(pSlave))				{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > PCA9952__EFLAG1 + 1U)		{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr | inc_mode, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


/****************************************************************/
#endif
#endif
/****************************************************************/
