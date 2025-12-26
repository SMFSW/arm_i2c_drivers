/*!\file DS_GPMS.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief DS-GPM.S Driver
** \details DS-GPM.S: 99 Channel Positioning System (GPS + GLONASS) Shield
**/
/****************************************************************/
#include "DS_GPMS.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_GPMS)
/****************************************************************/


static const I2C_slave_t GPMS_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };

static I2C_slave_t GPMS_hal;	//!< GPMS Slave structure


/****************************************************************/


__WEAK FctERR GPMS_Init(void)
{
	FctERR err;

	I2C_PERIPHERAL_SET_DEFAULTS_SINGLETON(GPMS);

	I2C_slave_init(&GPMS_hal, I2C_GPMS, GPMS_BASE_ADDR, GPMS_hal.cfg.timeout);

	err = GPMS_Init_Sequence();

	if (err != ERROR_OK)	{ I2C_set_enable(&GPMS_hal, false); }

	return err;
}


/****************************************************************/


FctERR NONNULL__ GPMS_Write(const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(&GPMS_hal))				{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > GPMS__IO_PORT_OUTPUT + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&GPMS_hal, true);
	GPMS_hal.status = HAL_I2C_Mem_Write(GPMS_hal.cfg.bus_inst, GPMS_hal.cfg.addr, addr, GPMS_hal.cfg.mem_size, (uint8_t *) data, nb, GPMS_hal.cfg.timeout);
	I2C_set_busy(&GPMS_hal, false);
	return HALERRtoFCTERR(GPMS_hal.status);
}


FctERR NONNULL__ GPMS_Read(uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(&GPMS_hal))		{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > GPMS__STATUS + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&GPMS_hal, true);
	GPMS_hal.status = HAL_I2C_Mem_Read(GPMS_hal.cfg.bus_inst, GPMS_hal.cfg.addr, addr, GPMS_hal.cfg.mem_size, data, nb, GPMS_hal.cfg.timeout);
	I2C_set_busy(&GPMS_hal, false);
	return HALERRtoFCTERR(GPMS_hal.status);
}


/****************************************************************/
#endif
#endif
/****************************************************************/
