/*!\file S11059.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief S11059 Driver
** \details S11059: I2C interface-compatible color sensor
**/
/****************************************************************/
#include "S11059.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_S11059)
/****************************************************************/


static const I2C_slave_t S11059_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };

static I2C_slave_t S11059_hal[I2C_S11059_NB];	//!< S11059 Slave structure


/****************************************************************/


FctERR NONNULL__ S11059_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	FctERR	err;

	assert_param(IS_I2C_PERIPHERAL(S11059, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(S11059, idx);

	I2C_slave_init(&S11059_hal[idx], hi2c, devAddress, S11059_hal[idx].cfg.timeout);

	err = S11059_Init_Sequence(&S11059[idx]);

	if (err)	{ I2C_set_enable(&S11059_hal[idx], false); }

	return err;
}

FctERR S11059_Init_Single(void) {
	return S11059_Init(0, I2C_S11059, S11059_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ S11059_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(pSlave))						{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > S11059__MANUAL_TIMING_LSB + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers (or write-able registers)

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ S11059_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(pSlave))				{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > S11059__IR_DATA_LSB + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ S11059_Write_Word(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr)
{
	uint8_t	WREG[2];

	if ((addr % sizeof(uint16_t)) == 0)		{ return ERROR_FRAMING; }		// Unaligned word access

	WREG[0] = HIBYTE(*data);
	WREG[1] = LOBYTE(*data);
	return S11059_Write(pSlave, WREG, addr, 2);
}


FctERR NONNULL__ S11059_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr)
{
	uint8_t	WREG[2];
	FctERR	err;

	if ((addr % sizeof(uint16_t)) == 0)		{ return ERROR_FRAMING; }		// Unaligned word access

	err = S11059_Read(pSlave, WREG, addr, 2);
	if (err)	{ return err; }

	*data = MAKEWORD(WREG[1], WREG[0]);
	return ERROR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
