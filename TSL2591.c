/*!\file TSL2591.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief TSL2591 Driver
** \details TSL2591: Very-high sensitivity light-to-digital converter
**/
/****************************************************************/
#include "TSL2591.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TSL2591)
/****************************************************************/


static const I2C_slave_t TSL2591_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };

static I2C_slave_t TSL2591_hal[I2C_TSL2591_NB];		//!< TSL2591 Slave structure


/****************************************************************/


FctERR NONNULL__ TSL2591_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	FctERR err;

	assert_param(IS_I2C_PERIPHERAL(TSL2591, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(TSL2591, idx);

	I2C_slave_init(&TSL2591_hal[idx], hi2c, devAddress, TSL2591_hal[idx].cfg.timeout);

	err = TSL2591_Init_Sequence(&TSL2591[idx]);

	if (err)	{ I2C_set_enable(&TSL2591_hal[idx], false); }

	return err;
}

FctERR TSL2591_Init_Single(void) {
	return TSL2591_Init(0, I2C_TSL2591, TSL2591_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ TSL2591_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	uTSL2591_CMD CMD;

	if (!I2C_is_enabled(pSlave))			{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > TSL2591__PERSIST + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	CMD.Bits.CMD = 1;
	CMD.Bits.TRANSACTION = TSL2591__TRANS_NORMAL_OP;
	CMD.Bits.ADDR = addr;

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, CMD.Byte, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ TSL2591_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	uTSL2591_CMD CMD;

	if (!I2C_is_enabled(pSlave))			{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > TSL2591__C1DATAH + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	CMD.Bits.CMD = 1;
	CMD.Bits.TRANSACTION = TSL2591__TRANS_NORMAL_OP;
	CMD.Bits.ADDR = addr;

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, CMD.Byte, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ TSL2591_Write_Word(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr)
{
	uint8_t	WREG[2];

	if (addr % sizeof(uint16_t))	{ return ERROR_FRAMING; }		// Unaligned word access

	WREG[0] = LOBYTE(*data);
	WREG[1] = HIBYTE(*data);
	return TSL2591_Write(pSlave, WREG, addr, 2);
}


FctERR NONNULL__ TSL2591_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr)
{
	uint8_t	WREG[2];
	FctERR	err;

	if (addr % sizeof(uint16_t))	{ return ERROR_FRAMING; }		// Unaligned word access

	err = TSL2591_Read(pSlave, WREG, addr, 2);
	if (err)	{ return err; }

	*data = MAKEWORD(WREG[0], WREG[1]);
	return ERROR_OK;
}


FctERR NONNULL__ TSL2591_Write_Special(I2C_slave_t * const pSlave, const TSL2591_spec_func func)
{
	uTSL2591_CMD CMD;

	if (	(func != TSL2591__SF_FORCE_IT)
		&&	(func != TSL2591__SF_CLR_ALS_IT)
		&&	(func != TSL2591__SF_CLR_ALS_AND_NO_PERS)
		&&	(func != TSL2591__SF_CLR_NO_PERS))			{ return ERROR_VALUE; }		// Unknown special function

	CMD.Bits.CMD = 1;
	CMD.Bits.TRANSACTION = TSL2591__TRANS_SPECIAL_FUNC;
	CMD.Bits.ADDR = func;

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Master_Transmit(pSlave->cfg.bus_inst, pSlave->cfg.addr, &CMD.Byte, 1, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


/****************************************************************/
#endif
#endif
/****************************************************************/

