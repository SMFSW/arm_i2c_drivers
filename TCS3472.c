/*!\file TCS3472.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief TCS3472 Driver
** \details TCS3472: Color light-to-digital converter with IR filter
**/
/****************************************************************/
#include "TCS3472.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TCS3472)
/****************************************************************/
#if defined(I2C_TCS3400) && !defined(NO_WARN_I2C_DRIVERS)
#warning "TCS3472 -> Multiple TCS34xx types: use with caution, might have same I2C addresses if on same I2C bus!!!"
#endif
/****************************************************************/


static const I2C_slave_t TCS3472_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };

static I2C_slave_t TCS3472_hal[I2C_TCS3472_NB];		//!< TCS3472 Slave structure


/****************************************************************/


FctERR NONNULL__ TCS3472_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	FctERR err;

	assert_param(IS_I2C_PERIPHERAL(TCS3472, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(TCS3472, idx);

	I2C_slave_init(&TCS3472_hal[idx], hi2c, devAddress, TCS3472_hal[idx].cfg.timeout);

	err = TCS3472_Init_Sequence(&TCS3472[idx]);

	if (err != ERROR_OK)	{ I2C_set_enable(&TCS3472_hal[idx], false); }

	return err;
}

FctERR TCS3472_Init_Single(void) {
	return TCS3472_Init(0, I2C_TCS3472, TCS3472_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ TCS3472_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR			err = ERROR_OK;
	uTCS3472_CMD	CMD;

	if (!I2C_is_enabled(pSlave))				{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > TCS3472__CONTROL + 1U)	{ err = ERROR_OVERFLOW; }	// More bytes than registers
	if (err != ERROR_OK)						{ goto ret; }

	CMD.Bits.CMD = 1U;
	CMD.Bits.TRANSACTION = TCS3472__TRANS_NORMAL_OP;
	CMD.Bits.ADDR = addr;

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, CMD.Byte, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


FctERR NONNULL__ TCS3472_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR			err = ERROR_OK;
	uTCS3472_CMD	CMD;

	if (!I2C_is_enabled(pSlave))			{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > TCS3472__BDATAH + 1U)	{ err = ERROR_OVERFLOW; }	// More bytes than registers
	if (err != ERROR_OK)					{ goto ret; }

	CMD.Bits.CMD = 1U;
	CMD.Bits.TRANSACTION = TCS3472__TRANS_NORMAL_OP;
	CMD.Bits.ADDR = addr;

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, CMD.Byte, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


FctERR NONNULL__ TCS3472_Write_Word(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr)
{
	FctERR err = ERROR_FRAMING;

	if (isEven(addr))	// Check unaligned word access
	{
		const uint8_t WREG[2] = { LOBYTE(*data), HIBYTE(*data) };

		err = TCS3472_Write(pSlave, WREG, addr, 2U);
	}

	return err;
}


FctERR NONNULL__ TCS3472_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr)
{
	FctERR err = ERROR_FRAMING;

	if (isEven(addr))	// Check unaligned word access
	{
		uint8_t	RREG[2] = { 0 };

		err = TCS3472_Read(pSlave, RREG, addr, 2U);
		if (err != ERROR_OK)	{ goto ret; }

		*data = MAKEWORD(RREG[0], RREG[1]);
	}

	ret:
	return err;
}


FctERR NONNULL__ TCS3472_Write_Special(I2C_slave_t * const pSlave, const TCS3472_spec_func func)
{
	FctERR			err;
	uTCS3472_CMD	CMD;

	if (func != TCS3472__SF_CLR_IT)	{ err = ERROR_VALUE; goto ret; }	// Unknown special function

	CMD.Bits.CMD = 1U;
	CMD.Bits.TRANSACTION = TCS3472__TRANS_SPECIAL_FUNC;
	CMD.Bits.ADDR = func;

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Master_Transmit(pSlave->cfg.bus_inst, pSlave->cfg.addr, &CMD.Byte, 1U, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/

