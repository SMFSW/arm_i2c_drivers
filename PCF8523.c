/*!\file PCF8523.c
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief PCF8523 Driver
** \details PCF8523: Real-Time Clock (RTC) and calendar
**/
/****************************************************************/
#include "PCF8523.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCF8523)
/****************************************************************/


I2C_slave_t PCF8523_hal = { { pNull, I2C_ADDR(PCF8523_BASE_ADDR), I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FMP }, 0, HAL_OK, true, false };


/****************************************************************/


__WEAK FctERR PCF8523_Init(void)
{
	FctERR err;

	err = I2C_slave_init(&PCF8523_hal, I2C_PCF8523, PCF8523_BASE_ADDR, PCF8523_hal.cfg.timeout);
	if (!err)	{ err = PCF8523_Init_Sequence(); }

	if (err)	{ I2C_set_enable(&PCF8523_hal, false); }

	return err;
}


/****************************************************************/


FctERR NONNULL__ PCF8523_Write(const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(&PCF8523_hal))			{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > PCF8523__TMR_B_REG)				{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > PCF8523__TMR_B_REG + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&PCF8523_hal, true);
	PCF8523_hal.status = HAL_I2C_Mem_Write(PCF8523_hal.cfg.bus_inst, PCF8523_hal.cfg.addr, addr, PCF8523_hal.cfg.mem_size, (uint8_t *) data, nb, PCF8523_hal.cfg.timeout);
	I2C_set_busy(&PCF8523_hal, false);
	return HALERRtoFCTERR(PCF8523_hal.status);
}


FctERR NONNULL__ PCF8523_Read(uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(&PCF8523_hal))			{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > PCF8523__TMR_B_REG)				{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > PCF8523__TMR_B_REG + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&PCF8523_hal, true);
	PCF8523_hal.status = HAL_I2C_Mem_Read(PCF8523_hal.cfg.bus_inst, PCF8523_hal.cfg.addr, addr, PCF8523_hal.cfg.mem_size, data, nb, PCF8523_hal.cfg.timeout);
	I2C_set_busy(&PCF8523_hal, false);
	return HALERRtoFCTERR(PCF8523_hal.status);
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_PCF8523 for component to be enabled!"
#endif
#endif
/****************************************************************/
