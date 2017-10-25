/*!\file DS_GPMS.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief DS-GPM.S Driver code
** \details DS-GPM.S: 99 Channel Positioning System (GPS + GLONASS) Shield
**/
/****************************************************************/
#include "DS_GPMS.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_GPMS)
/****************************************************************/


I2C_slave GPMS_hal = { { pNull, I2C_ADDR(GPMS_BASE_ADDR), I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };


/****************************************************************/


__weak FctERR GPMS_Init(void)
{
	I2C_slave_init(&GPMS_hal, I2C_GPMS, GPMS_BASE_ADDR, I2C_slave_timeout);

	FctERR err = GPMS_Init_Sequence();
	if (err)	{ I2C_set_enable(&GPMS_hal, false); }

	return err;
}


/****************************************************************/


FctERR GPMS_Write(uint8_t * data, uint16_t addr, uint16_t nb)
{
	if (!I2C_is_enabled(&GPMS_hal))				{ return ERR_DISABLED; }	// Peripheral disabled
	if (!data)									{ return ERR_MEMORY; }		// Null pointer
	if (addr > GPMS__IO_PORT_OUTPUT)			{ return ERR_RANGE; }		// Unknown register
	if ((addr + nb) > GPMS__IO_PORT_OUTPUT + 1)	{ return ERR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&GPMS_hal, true);

	GPMS_hal.status = HAL_I2C_Mem_Write(GPMS_hal.cfg.inst, GPMS_hal.cfg.addr, addr, GPMS_hal.cfg.mem_size, data, nb, GPMS_hal.cfg.timeout);

	I2C_set_busy(&GPMS_hal, false);
	return HALERRtoFCTERR(GPMS_hal.status);
}


FctERR GPMS_Read(uint8_t * data, uint16_t addr, uint16_t nb)
{
	if (!I2C_is_enabled(&GPMS_hal))		{ return ERR_DISABLED; }	// Peripheral disabled
	if (!data)							{ return ERR_MEMORY; }		// Null pointer
	if (addr > GPMS__STATUS)			{ return ERR_RANGE; }		// Unknown register
	if ((addr + nb) > GPMS__STATUS + 1)	{ return ERR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&GPMS_hal, true);

	GPMS_hal.status = HAL_I2C_Mem_Read(GPMS_hal.cfg.inst, GPMS_hal.cfg.addr, addr, GPMS_hal.cfg.mem_size, data, nb, GPMS_hal.cfg.timeout);

	I2C_set_busy(&GPMS_hal, false);
	return HALERRtoFCTERR(GPMS_hal.status);
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_GPMS in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/
