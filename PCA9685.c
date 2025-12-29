/*!\file PCA9685.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief PCA9685 Driver
** \details PCA9685: 16-channel, 12-bit PWM Fm+ I2C-bus LED controller
**/
/****************************************************************/
#include "PCA9685.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9685)
/****************************************************************/
#if (defined(I2C_PCA9624) || defined(I2C_PCA9952) || defined(I2C_PCA9956)) && !defined(NO_WARN_I2C_DRIVERS)
#warning "PCA9685 -> Multiple PCA9xxx types: use with caution if using CALL addresses if on same I2C bus!!!"
#endif
/****************************************************************/


static const I2C_slave_t PCA9685_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FMP }, 0, HAL_OK, true, false };

static I2C_slave_t PCA9685_hal[I2C_PCA9685_NB];		//!< PCA9685 Slave structure


/****************************************************************/


FctERR NONNULL__ PCA9685_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	FctERR err;

	assert_param(IS_I2C_PERIPHERAL(PCA9685, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(PCA9685, idx);

	I2C_slave_init(&PCA9685_hal[idx], hi2c, devAddress, PCA9685_hal[idx].cfg.timeout);

	err = PCA9685_Init_Sequence(&PCA9685[idx]);

	if (err != ERROR_OK)	{ I2C_set_enable(&PCA9685_hal[idx], false); }

	return err;
}

FctERR PCA9685_Init_Single(void) {
	return PCA9685_Init(0, I2C_PCA9685, PCA9685_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ PCA9685_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR err = ERROR_OK;

	if (!I2C_is_enabled(pSlave))				{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > PCA9685__TestMode + 1U)	{ err = ERROR_OVERFLOW; }	// More bytes than registers
	if (err != ERROR_OK)						{ goto ret; }

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


FctERR NONNULL__ PCA9685_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR err = ERROR_OK;

	if (!I2C_is_enabled(pSlave))				{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > PCA9685__TestMode + 1U)	{ err = ERROR_OVERFLOW; }	// More bytes than registers
	if (err != ERROR_OK)						{ goto ret; }

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
