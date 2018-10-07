/*!\file MTCH6102.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief MTCH6102 Driver
** \details MTCH6102: Low-Power Projected Capacitive Touch Controller
**/
/****************************************************************/
#include "MTCH6102.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MTCH6102)
/****************************************************************/


I2C_slave_t MTCH6102_hal = { { pNull, I2C_ADDR(MTCH6102_BASE_ADDR), I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };


/****************************************************************/


__WEAK FctERR MTCH6102_Init(void)
{
	FctERR err;

	err = I2C_slave_init(&MTCH6102_hal, I2C_MTCH6102, MTCH6102_BASE_ADDR, I2C_slave_timeout);
	if (!err)	{ err = MTCH6102_Init_Sequence(); }

	if (err)	{ I2C_set_enable(&MTCH6102_hal, false); }

	return err;
}


/****************************************************************/


FctERR NONNULL__ MTCH6102_Write(const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(&MTCH6102_hal))		{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > MTCH__RAW_ADC_31)			{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > MTCH__RAW_ADC_31 + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&MTCH6102_hal, true);

	MTCH6102_hal.status = HAL_I2C_Mem_Write(MTCH6102_hal.cfg.bus_inst, MTCH6102_hal.cfg.addr, addr, MTCH6102_hal.cfg.mem_size, (uint8_t *) data, nb, MTCH6102_hal.cfg.timeout);

	I2C_set_busy(&MTCH6102_hal, false);
	return HALERRtoFCTERR(MTCH6102_hal.status);
}


FctERR NONNULL__ MTCH6102_Read(uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(&MTCH6102_hal))		{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > MTCH__RAW_ADC_31)			{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > MTCH__RAW_ADC_31 + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&MTCH6102_hal, true);

	MTCH6102_hal.status = HAL_I2C_Mem_Read(MTCH6102_hal.cfg.bus_inst, MTCH6102_hal.cfg.addr, addr, MTCH6102_hal.cfg.mem_size, data, nb, MTCH6102_hal.cfg.timeout);

	I2C_set_busy(&MTCH6102_hal, false);
	return HALERRtoFCTERR(MTCH6102_hal.status);
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_MTCH6102 in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/

