/*!\file S11059.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief S11059 Driver
** \details S11059: I2C interface-compatible color sensor
**/
/****************************************************************/
#include "S11059.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_S11059)
/****************************************************************/


I2C_slave S11059_hal = { { pNull, I2C_ADDR(S11059_BASE_ADDR), I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };


/****************************************************************/


__WEAK FctERR S11059_Init(void)
{
	FctERR err;

	err = I2C_slave_init(&S11059_hal, I2C_S11059, S11059_BASE_ADDR, I2C_slave_timeout);
	if (!err)	{ err = S11059_Init_Sequence(); }

	if (err)	{ I2C_set_enable(&S11059_hal, false); }

	return err;
}


/****************************************************************/


FctERR S11059_Write(const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(&S11059_hal))					{ return ERROR_DISABLED; }	// Peripheral disabled
	if (!data)											{ return ERROR_MEMORY; }	// Null pointer
	if (addr > S11059__MANUAL_TIMING_LSB)				{ return ERROR_RANGE; }		// Unknown register (or read only register)
	if ((addr + nb) > S11059__MANUAL_TIMING_LSB + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers (or write-able registers)

	I2C_set_busy(&S11059_hal, true);

	S11059_hal.status = HAL_I2C_Mem_Write(S11059_hal.cfg.bus_inst, S11059_hal.cfg.addr, addr, S11059_hal.cfg.mem_size, (uint8_t *) data, nb, S11059_hal.cfg.timeout);

	I2C_set_busy(&S11059_hal, false);
	return HALERRtoFCTERR(S11059_hal.status);
}


FctERR S11059_Read(uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(&S11059_hal))			{ return ERROR_DISABLED; }	// Peripheral disabled
	if (!data)									{ return ERROR_MEMORY; }	// Null pointer
	if (addr > S11059__IR_DATA_LSB)				{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > S11059__IR_DATA_LSB + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&S11059_hal, true);

	S11059_hal.status = HAL_I2C_Mem_Read(S11059_hal.cfg.bus_inst, S11059_hal.cfg.addr, addr, S11059_hal.cfg.mem_size, data, nb, S11059_hal.cfg.timeout);

	I2C_set_busy(&S11059_hal, false);
	return HALERRtoFCTERR(S11059_hal.status);
}


FctERR S11059_Write_Word(const uint16_t * data, const uint16_t addr)
{
	uint8_t	WREG[2];

	if (addr > S11059__MANUAL_TIMING_LSB)	{ return ERROR_RANGE; }		// Unknown register (or read only register)

	WREG[0] = HIBYTE(*data);
	WREG[1] = LOBYTE(*data);
	return S11059_Write(WREG, addr, 2);
}


FctERR S11059_Read_Word(uint16_t * data, const uint16_t addr)
{
	uint8_t	WREG[2];
	FctERR	err;

	if (addr > S11059__IR_DATA_MSB)			{ return ERROR_RANGE; }		// Unknown register

	err = S11059_Read(WREG, addr, 2);
	if (err)	{ return err; }

	*data = MAKEWORD(WREG[1], WREG[0]);
	return ERROR_OK;
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_S11059 in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/
