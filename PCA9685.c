/*!\file PCA9685.c
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
** \brief PCA9685 Driver
** \details PCA9685: 16-channel, 12-bit PWM Fm+ I2C-bus LED controller
**/
/****************************************************************/
#include "PCA9685.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9685)
/****************************************************************/
#if defined(I2C_PCA9624)
#warning "PCA9685 -> Multiple PCA96xx types: use with caution if using CALL addresses if on same I2C bus!!!"
#endif
/****************************************************************/


static const I2C_slave_t PCA9685_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FMP }, 0, HAL_OK, true, false };

 I2C_slave_t PCA9685_hal[I2C_PCA9685_NB];


/****************************************************************/


FctERR NONNULL__ PCA9685_Init(const uint8_t idx, const I2C_HandleTypeDef * hi2c, const uint16_t devAddress)
{
	FctERR err;

	assert_param(IS_I2C_PERIPHERAL(PCA9685, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(PCA9685, idx, devAddress);

	err = I2C_slave_init(&PCA9685_hal[idx], hi2c, devAddress, I2C_slave_timeout);
	if (!err)	{ err = PCA9685_Init_Sequence(&PCA9685[idx]); }

	if (err)	{ I2C_set_enable(&PCA9685_hal[idx], false); }

	return err;
}

FctERR PCA9685_Init_Single(void) {
	return PCA9685_Init(0, I2C_PCA9685, PCA9685_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ PCA9685_Write(I2C_slave_t * pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(pSlave))				{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > PCA9685__TestMode)				{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > PCA9685__TestMode + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ PCA9685_Read(I2C_slave_t * pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(pSlave))				{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > PCA9685__TestMode)				{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > PCA9685__TestMode + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_PCA9685 in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/
