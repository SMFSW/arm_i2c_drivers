/*!\file TSL2591.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief TSL2591 Driver code
** \details TSL2591: Very-high sensitivity light-to-digital converter
**/
/****************************************************************/
#include "TSL2591.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TSL2591)
/****************************************************************/


I2C_slave TSL2591_hal = { { pNull, I2C_ADDR(TSL2591_BASE_ADDR), I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };


/****************************************************************/


__WEAK FctERR TSL2591_Init(void)
{
	I2C_slave_init(&TSL2591_hal, I2C_TSL2591, TSL2591_BASE_ADDR, I2C_slave_timeout);

	FctERR err = TSL2591_Init_Sequence();
	if (err)	{ I2C_set_enable(&TSL2591_hal, false); }

	return err;
}


/****************************************************************/


FctERR TSL2591_Write(const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	uTSL2591_CMD CMD;

	if (!I2C_is_enabled(&TSL2591_hal))		{ return ERROR_DISABLED; }	// Peripheral disabled
	if (!data)								{ return ERROR_MEMORY; }	// Null pointer
	if (addr > TSL2591__PERSIST)			{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > TSL2591__PERSIST + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&TSL2591_hal, true);

	CMD.Bits.CMD = 1;
	CMD.Bits.TRANSACTION = TSL2591__TRANS_NORMAL_OP;
	CMD.Bits.ADDR = addr;

	TSL2591_hal.status = HAL_I2C_Mem_Write(TSL2591_hal.cfg.inst, TSL2591_hal.cfg.addr, CMD.Byte, TSL2591_hal.cfg.mem_size, (uint8_t *) data, nb, TSL2591_hal.cfg.timeout);

	I2C_set_busy(&TSL2591_hal, false);
	return HALERRtoFCTERR(TSL2591_hal.status);
}


FctERR TSL2591_Read(uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	uTSL2591_CMD CMD;

	if (!I2C_is_enabled(&TSL2591_hal))		{ return ERROR_DISABLED; }	// Peripheral disabled
	if (!data)								{ return ERROR_MEMORY; }	// Null pointer
	if (addr > TSL2591__C1DATAH)			{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > TSL2591__C1DATAH + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&TSL2591_hal, true);

	CMD.Bits.CMD = 1;
	CMD.Bits.TRANSACTION = TSL2591__TRANS_NORMAL_OP;
	CMD.Bits.ADDR = addr;

	TSL2591_hal.status = HAL_I2C_Mem_Read(TSL2591_hal.cfg.inst, TSL2591_hal.cfg.addr, CMD.Byte, TSL2591_hal.cfg.mem_size, data, nb, TSL2591_hal.cfg.timeout);

	I2C_set_busy(&TSL2591_hal, false);
	return HALERRtoFCTERR(TSL2591_hal.status);
}


FctERR TSL2591_Write_Word(const uint16_t * data, const uint16_t addr)
{
	uint8_t	WREG[2];

	if (addr > TSL2591__PERSIST)		{ return ERROR_RANGE; }		// Unknown register

	WREG[0] = LOBYTE(*data);
	WREG[1] = HIBYTE(*data);
	return TSL2591_Write(WREG, addr, 2);
}


FctERR TSL2591_Read_Word(uint16_t * data, const uint16_t addr)
{
	uint8_t	WREG[2];
	FctERR	err;

	if (addr > TSL2591__C1DATAH)		{ return ERROR_RANGE; }		// Unknown register

	err = TSL2591_Read(WREG, addr, 2);
	if (err)	{ return err; }

	*data = MAKEWORD(WREG[0], WREG[1]);
	return ERROR_OK;
}


FctERR TSL2591_Write_Special(const TSL2591_spec_func func)
{
	uTSL2591_CMD CMD;

	if (	(func != TSL2591__SF_FORCE_IT)
		&&	(func != TSL2591__SF_CLR_ALS_IT)
		&&	(func != TSL2591__SF_CLR_ALS_AND_NO_PERS)
		&&	(func != TSL2591__SF_CLR_NO_PERS))			{ return ERROR_VALUE; }		// Unknown special function

	I2C_set_busy(&TSL2591_hal, true);

	CMD.Bits.CMD = 1;
	CMD.Bits.TRANSACTION = TSL2591__TRANS_SPECIAL_FUNC;
	CMD.Bits.ADDR = func;

	TSL2591_hal.status = HAL_I2C_Master_Transmit(TSL2591_hal.cfg.inst, TSL2591_hal.cfg.addr, &CMD.Byte, 1, TSL2591_hal.cfg.timeout);

	I2C_set_busy(&TSL2591_hal, false);
	return HALERRtoFCTERR(TSL2591_hal.status);
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_TSL2591 in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/

