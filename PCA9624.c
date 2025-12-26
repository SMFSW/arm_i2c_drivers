/*!\file PCA9624.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief PCA9624 Driver
** \details PCA9624: 8-bit Fm+ I2C-bus 100mA 40V LED driver
**/
/****************************************************************/
#include "PCA9624.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9624)
/****************************************************************/
#if (defined(I2C_PCA9685) || defined(I2C_PCA9952) || defined(I2C_PCA9956)) && !defined(NO_WARN_I2C_DRIVERS)
#warning "PCA9624 -> Multiple PCA9xxx types: use with caution if using CALL addresses if on same I2C bus!!!"
#endif
/****************************************************************/


static const I2C_slave_t PCA9624_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FMP }, 0, HAL_OK, true, false };

static I2C_slave_t PCA9624_hal[I2C_PCA9624_NB];		//!< PCA9624 Slave structure


/****************************************************************/


FctERR NONNULL__ PCA9624_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	FctERR err;

	assert_param(IS_I2C_PERIPHERAL(PCA9624, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(PCA9624, idx);

	I2C_slave_init(&PCA9624_hal[idx], hi2c, devAddress, PCA9624_hal[idx].cfg.timeout);

	err = PCA9624_Set_Auto_Increment(&PCA9624[idx], PCA9xxx__AUTO_INC_ALL);
	if (!err)	{ err = PCA9624_Init_Sequence(&PCA9624[idx]); }

	if (err != ERROR_OK)	{ I2C_set_enable(&PCA9624_hal[idx], false); }

	return err;
}

FctERR PCA9624_Init_Single(void) {
	return PCA9624_Init(0, I2C_PCA9624, PCA9624_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ PCA9624_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	const PCA962x_reg_inc inc_mode = PCA9624[pSlave - PCA9624_hal].cfg.auto_inc;

	if (!I2C_is_enabled(pSlave))							{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > PCA9624__ALLCALLADR + 1)				{ return ERROR_OVERFLOW; }	// More bytes than registers
	if ((nb > 1) && (inc_mode == PCA9xxx__AUTO_INC_NONE))	{ return ERROR_NOTAVAIL; }	// Writing more than 1 byte not available in no auto-increment mode

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr | inc_mode, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ PCA9624_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	const PCA962x_reg_inc inc_mode = PCA9624[pSlave - PCA9624_hal].cfg.auto_inc;

	if (!I2C_is_enabled(pSlave))							{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > PCA9624__ALLCALLADR + 1)				{ return ERROR_OVERFLOW; }	// More bytes than registers
	if ((nb > 1) && (inc_mode == PCA9xxx__AUTO_INC_NONE))	{ return ERROR_NOTAVAIL; }	// Writing more than 1 byte not available in no auto-increment mode

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr | inc_mode, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


/****************************************************************/
#endif
#endif
/****************************************************************/
