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

	if (err != ERROR_OK)	{ I2C_set_enable(&S11059_hal[idx], false); }

	return err;
}

FctERR S11059_Init_Single(void) {
	return S11059_Init(0, I2C_S11059, S11059_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ S11059_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR err = ERROR_OK;

	if (!I2C_is_enabled(pSlave))						{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > S11059__MANUAL_TIMING_LSB + 1U)	{ err = ERROR_OVERFLOW; }	// More bytes than registers (or write-able registers)
	if (err != ERROR_OK)								{ goto ret; }

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


FctERR NONNULL__ S11059_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR err = ERROR_OK;

	if (!I2C_is_enabled(pSlave))				{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > S11059__IR_DATA_LSB + 1U)	{ err = ERROR_OVERFLOW; }	// More bytes than registers
	if (err != ERROR_OK)						{ goto ret; }

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


FctERR NONNULL__ S11059_Write_Word(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr)
{
	FctERR err = ERROR_FRAMING;

	if (isEven(addr))	// Check unaligned word access
	{
		const uint8_t WREG[2] = { LOBYTE(*data), HIBYTE(*data) };

		err = S11059_Write(pSlave, WREG, addr, 2U);
	}

	return err;
}


FctERR NONNULL__ S11059_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr)
{
	FctERR err = ERROR_FRAMING;

	if (isEven(addr))	// Check unaligned word access
	{
		uint8_t	RREG[2] = { 0 };

		err = S11059_Read(pSlave, RREG, addr, 2U);
		if (err != ERROR_OK)	{ goto ret; }

		*data = MAKEWORD(RREG[0], RREG[1]);
	}

	ret:
	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
