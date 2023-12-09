/*!\file PCA9956.c
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief PCA9956 Driver
** \details PCA9956: 24-channel Fm+ I2C-bus 57 mA/20 V constant current LED driver
**/
/****************************************************************/
#include "PCA9956.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9956)
/****************************************************************/
#if defined(I2C_PCA9624) || defined(PCA9685)
#warning "PCA9956 -> Multiple PCA9xxx types: use with caution if using CALL addresses if on same I2C bus!!!"
#endif
/****************************************************************/


static const I2C_slave_t PCA9956_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FMP }, 0, HAL_OK, true, false };

static I2C_slave_t PCA9956_hal[I2C_PCA9956_NB];		//!< PCA9956 Slave structure


/****************************************************************/


FctERR NONNULL__ PCA9956_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	FctERR err;

	assert_param(IS_I2C_PERIPHERAL(PCA9956, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(PCA9956, idx);

	err = I2C_slave_init(&PCA9956_hal[idx], hi2c, devAddress, PCA9956_hal[idx].cfg.timeout);
	if (!err)	{ err = PCA9956_Init_Sequence(&PCA9956[idx]); }

	if (err)	{ I2C_set_enable(&PCA9956_hal[idx], false); }

	return err;
}

FctERR PCA9956_Init_Single(void) {
	return PCA9956_Init(0, I2C_PCA9956, PCA9956_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ PCA9956_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	const PCA962x_reg_inc inc_mode = PCA9956[pSlave - PCA9956_hal].cfg.auto_inc & 0x80;

	if (!I2C_is_enabled(pSlave))				{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > PCA9956__IREFALL + 1)		{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr | inc_mode, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ PCA9956_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	const PCA962x_reg_inc inc_mode = PCA9956[pSlave - PCA9956_hal].cfg.auto_inc & 0x80;

	if (!I2C_is_enabled(pSlave))				{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > PCA9956__EFLAG5 + 1)		{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr | inc_mode, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


/****************************************************************/
#endif
#endif
/****************************************************************/
