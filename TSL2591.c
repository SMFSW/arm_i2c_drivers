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


I2C_slave TSL2591_hal = { { pNull, I2C_ADDR(TSL2591_BASE_ADDR), I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, false };


/****************************************************************/


FctERR TSL2591_Init(void)
{
	I2C_slave_init(&TSL2591_hal, I2C_TSL2591, TSL2591_BASE_ADDR, I2C_slave_timeout);
	return TSL2591_Init_Sequence();
}


/****************************************************************/


FctERR TSL2591_Write(uint8_t * data, uint16_t addr, uint16_t nb)
{
	uTSL2591_CMD CMD;

	if (!data)								{ return ERR_MEMORY; }		// Null pointer
	if (addr > TSL2591__PERSIST)			{ return ERR_RANGE; }		// Unknown register
	if ((addr + nb) > TSL2591__PERSIST + 1)	{ return ERR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&TSL2591_hal, true);

	CMD.Bits.CMD = 1;
	CMD.Bits.TRANSACTION = TSL2591__TRANS_NORMAL_OP;
	CMD.Bits.ADDR = addr;

	TSL2591_hal.status = HAL_I2C_Mem_Write(TSL2591_hal.cfg.inst, TSL2591_hal.cfg.addr, CMD.Byte, TSL2591_hal.cfg.mem_size, data, nb, TSL2591_hal.cfg.timeout);

	I2C_set_busy(&TSL2591_hal, false);
	return HALERRtoFCTERR(TSL2591_hal.status);
}


FctERR TSL2591_Read(uint8_t * data, uint16_t addr, uint16_t nb)
{
	uTSL2591_CMD CMD;

	if (!data)								{ return ERR_MEMORY; }		// Null pointer
	if (addr > TSL2591__C1DATAH)			{ return ERR_RANGE; }		// Unknown register
	if ((addr + nb) > TSL2591__C1DATAH + 1)	{ return ERR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&TSL2591_hal, true);

	CMD.Bits.CMD = 1;
	CMD.Bits.TRANSACTION = TSL2591__TRANS_NORMAL_OP;
	CMD.Bits.ADDR = addr;

	TSL2591_hal.status = HAL_I2C_Mem_Read(TSL2591_hal.cfg.inst, TSL2591_hal.cfg.addr, CMD.Byte, TSL2591_hal.cfg.mem_size, data, nb, TSL2591_hal.cfg.timeout);

	I2C_set_busy(&TSL2591_hal, false);
	return HALERRtoFCTERR(TSL2591_hal.status);
}


FctERR TSL2591_Write_Word(uint16_t * data, uint16_t addr)
{
	uint8_t	WREG[2];

	if (addr > TSL2591__PERSIST)		{ return ERR_RANGE; }		// Unknown register

	WREG[0] = (uint8_t) (*data / 0x100);
	WREG[1] = (uint8_t) *data;
	return TSL2591_Write(WREG, addr, 2);
}


FctERR TSL2591_Read_Word(uint16_t * data, uint16_t addr)
{
	uint8_t	WREG[2];
	FctERR	err;

	if (addr > TSL2591__C1DATAH)		{ return ERR_RANGE; }		// Unknown register

	err = TSL2591_Read(WREG, addr, 2);
	if (err)	{ return err; }

	*data = (WREG[0] * 0x100) + WREG[1];
	return ERR_OK;
}


FctERR TSL2591_Write_Special(TSL2591_spec_func func)
{
	uTSL2591_CMD CMD;

	if (	(func != TSL2591__SF_FORCE_IT)
		&&	(func != TSL2591__SF_CLR_ALS_IT)
		&&	(func != TSL2591__SF_CLR_ALS_AND_NO_PERS)
		&&	(func != TSL2591__SF_CLR_NO_PERS))			{ return ERR_VALUE; }		// Unknown special function

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

