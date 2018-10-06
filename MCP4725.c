/*!\file MCP4725.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief MCP4725 Driver
** \details MCP4725: 12-Bit Digital-to-Analog Converter with EEPROM Memory
**/
/****************************************************************/
#include "MCP4725.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MCP4725)
/****************************************************************/


I2C_slave_t MCP4725_hal = { { pNull, I2C_ADDR(MCP4725_BASE_ADDR), I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_HS }, 0, HAL_OK, true, false };


/****************************************************************/


__WEAK FctERR MCP4725_Init(void)
{
	FctERR err;

	err = I2C_slave_init(&MCP4725_hal, I2C_MCP4725, MCP4725_BASE_ADDR, I2C_slave_timeout);
	if (!err)	{ err = MCP4725_Init_Sequence(); }

	if (err)	{ I2C_set_enable(&MCP4725_hal, false); }

	return err;
}


/****************************************************************/


FctERR NONNULL__ MCP4725_Write(const uint8_t * data, const uint16_t nb)
{
	if (!I2C_is_enabled(&MCP4725_hal))	{ return ERROR_DISABLED; }	// Peripheral disabled
	if (nb > 3)							{ return ERROR_RANGE; }		// More bytes than registers

	I2C_set_busy(&MCP4725_hal, true);

	MCP4725_hal.status = HAL_I2C_Master_Transmit(MCP4725_hal.cfg.bus_inst, MCP4725_hal.cfg.addr, (uint8_t *) data, nb, MCP4725_hal.cfg.timeout);

	I2C_set_busy(&MCP4725_hal, false);
	return HALERRtoFCTERR(MCP4725_hal.status);
}


FctERR NONNULL__ MCP4725_Read(uint8_t * data, const uint16_t nb)
{
	if (!I2C_is_enabled(&MCP4725_hal))	{ return ERROR_DISABLED; }	// Peripheral disabled
	if (nb > 3)							{ return ERROR_RANGE; }		// More bytes than registers

	I2C_set_busy(&MCP4725_hal, true);

	MCP4725_hal.status = HAL_I2C_Master_Receive(MCP4725_hal.cfg.bus_inst, MCP4725_hal.cfg.addr, data, nb, MCP4725_hal.cfg.timeout);

	I2C_set_busy(&MCP4725_hal, false);
	return HALERRtoFCTERR(MCP4725_hal.status);
}


FctERR MCP4725_General_Call(const uint8_t cmd)
{
	if ((cmd != MCP4725__RESET) && (cmd != MCP4725__WAKEUP))	{ return ERROR_CMD; }	// Unknown command

	I2C_set_busy(&MCP4725_hal, true);

	MCP4725_hal.status = HAL_I2C_Master_Receive(MCP4725_hal.cfg.bus_inst, I2C_ADDR_General_Call, (uint8_t *) &cmd, 1, MCP4725_hal.cfg.timeout);

	I2C_set_busy(&MCP4725_hal, false);
	return HALERRtoFCTERR(MCP4725_hal.status);
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_MCP4725 in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/
