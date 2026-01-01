/*!\file PCA9532.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief PCA9532 Driver
** \details PCA9532: 16-bit I2C-bus LED dimmer
**/
/****************************************************************/
#include "PCA9532.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9532)
/****************************************************************/


static const I2C_slave_t PCA9532_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FMP }, 0, HAL_OK, true, false };

static I2C_slave_t PCA9532_hal[I2C_PCA9532_NB];	//!< PCA9532 Slave structure


/****************************************************************/


FctERR NONNULL__ PCA9532_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	FctERR err;

	assert_param(IS_I2C_PERIPHERAL(PCA9532, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(PCA9532, idx);

	I2C_slave_init(&PCA9532_hal[idx], hi2c, devAddress, PCA9532_hal[idx].cfg.timeout);

	err = PCA9532_Set_Auto_Increment(&PCA9532[idx], PCA95xx__AUTO_INC_ALL);
	if (err == ERROR_OK)	{ err = PCA9532_Init_Sequence(&PCA9532[idx]); }

	if (err != ERROR_OK)	{ I2C_set_enable(&PCA9532_hal[idx], false); }

	return err;
}

FctERR PCA9532_Init_Single(void) {
	return PCA9532_Init(0, I2C_PCA9532, PCA9532_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ PCA9532_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR					err = ERROR_OK;
	const PCA95xx_reg_inc	inc_mode = PCA9532[pSlave - PCA9532_hal].cfg.auto_inc;

	if (!I2C_is_enabled(pSlave))							{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > PCA9532__LS3 + 1U)					{ err = ERROR_OVERFLOW; }	// More bytes than registers
	if ((nb > 1U) && (inc_mode == PCA95xx__AUTO_INC_NONE))	{ err = ERROR_NOTAVAIL; }	// Writing more than 1 byte not available in no auto-increment mode
	if (err != ERROR_OK)									{ goto ret; }

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr | inc_mode, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


FctERR NONNULL__ PCA9532_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR					err = ERROR_OK;
	const PCA95xx_reg_inc	inc_mode = PCA9532[pSlave - PCA9532_hal].cfg.auto_inc;

	if (!I2C_is_enabled(pSlave))							{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > PCA9532__LS3 + 1U)					{ err = ERROR_OVERFLOW; }	// More bytes than registers
	if ((nb > 1U) && (inc_mode == PCA95xx__AUTO_INC_NONE))	{ err = ERROR_NOTAVAIL; }	// Writing more than 1 byte not available in no auto-increment mode
	if (err != ERROR_OK)									{ goto ret; }

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr | inc_mode, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
