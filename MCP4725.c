/*!\file MCP4725.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief MCP4725 Driver
** \details MCP4725: 12-Bit Digital-to-Analog Converter with EEPROM Memory
**/
/****************************************************************/
#include "MCP4725.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MCP4725)
/****************************************************************/


static const I2C_slave_t MCP4725_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_HS }, 0, HAL_OK, true, false };

static I2C_slave_t MCP4725_hal[I2C_MCP4725_NB];	//!< MCP4725 Slave structure


/****************************************************************/


FctERR NONNULL__ MCP4725_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	FctERR err;

	assert_param(IS_I2C_PERIPHERAL(MCP4725, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(MCP4725, idx);

	I2C_slave_init(&MCP4725_hal[idx], hi2c, devAddress, MCP4725_hal[idx].cfg.timeout);

	err = MCP4725_Init_Sequence(&MCP4725[idx]);

	if (err != ERROR_OK)	{ I2C_set_enable(&MCP4725_hal[idx], false); }

	return err;
}

FctERR MCP4725_Init_Single(void) {
	return MCP4725_Init(0, I2C_MCP4725, MCP4725_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ MCP4725_General_Call(I2C_HandleTypeDef * const hi2c, const uint8_t cmd)
{
	FctERR err = ERROR_CMD;

	if ((cmd == MCP4725__RESET) || (cmd == MCP4725__WAKEUP))	// Check unknown command
	{
		err = HALERRtoFCTERR(HAL_I2C_Master_Transmit(hi2c, I2C_ADDR_General_Call, (uint8_t *) &cmd, 1U, I2C_slave_timeout));
	}

	return err;
}


FctERR NONNULL__ MCP4725_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t nb)
{
	FctERR err = ERROR_OK;

	if (!I2C_is_enabled(pSlave))	{ err = ERROR_DISABLED; }	// Peripheral disabled
	if (nb > 3U)					{ err = ERROR_RANGE; }		// More bytes than registers
	if (err != ERROR_OK)			{ goto ret; }

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Master_Transmit(pSlave->cfg.bus_inst, pSlave->cfg.addr, (uint8_t *) data, nb, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


FctERR NONNULL__ MCP4725_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t nb)
{
	FctERR err = ERROR_OK;

	if (!I2C_is_enabled(pSlave))	{ err = ERROR_DISABLED; }	// Peripheral disabled
	if (nb > 3U)					{ err = ERROR_RANGE; }		// More bytes than registers
	if (err != ERROR_OK)			{ goto ret; }

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Master_Receive(pSlave->cfg.bus_inst, pSlave->cfg.addr, data, nb, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
