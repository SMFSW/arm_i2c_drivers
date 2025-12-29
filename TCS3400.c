/*!\file TCS3400.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief TCS3400 Driver
** \details TCS3400: Color Light-to-Digital Converter
**/
/****************************************************************/
#include "TCS3400.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TCS3400)
/****************************************************************/
#if defined(I2C_TCS3472) && !defined(NO_WARN_I2C_DRIVERS)
#warning "TCS3400 -> Multiple TCS34xx types: use with caution, might have same I2C addresses if on same I2C bus!!!"
#endif
/****************************************************************/


static const I2C_slave_t TCS3400_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };

static I2C_slave_t TCS3400_hal[I2C_TCS3400_NB];		//!< TCS3400 Slave structure


/****************************************************************/


FctERR NONNULL__ TCS3400_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	FctERR err;

	assert_param(IS_I2C_PERIPHERAL(TCS3400, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(TCS3400, idx);

	I2C_slave_init(&TCS3400_hal[idx], hi2c, devAddress, TCS3400_hal[idx].cfg.timeout);

	err = TCS3400_Init_Sequence(&TCS3400[idx]);

	if (err != ERROR_OK)	{ I2C_set_enable(&TCS3400_hal[idx], false); }

	return err;
}

FctERR TCS3400_Init_Single(void) {
	return TCS3400_Init(0, I2C_TCS3400, TCS3400_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ TCS3400_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR err = ERROR_OK;

	if (!I2C_is_enabled(pSlave))				{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > TCS3400__AICLEAR + 1U)	{ err = ERROR_OVERFLOW; }	// More bytes than registers
	if (err != ERROR_OK)						{ goto ret; }

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


FctERR NONNULL__ TCS3400_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR err = ERROR_OK;

	if (!I2C_is_enabled(pSlave))			{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > TCS3400__IR + 1U)		{ err = ERROR_OVERFLOW; }	// More bytes than registers
	if (err != ERROR_OK)					{ goto ret; }

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


FctERR NONNULL__ TCS3400_Write_Word(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr)
{
	FctERR err = ERROR_FRAMING;

	if (isEven(addr))	// Check unaligned word access
	{
		const uint8_t WREG[2] = { LOBYTE(*data), HIBYTE(*data) };

		err = TCS3400_Write(pSlave, WREG, addr, 2U);
	}

	return err;
}


FctERR NONNULL__ TCS3400_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr)
{
	FctERR err = ERROR_FRAMING;

	if (isEven(addr))	// Check unaligned word access
	{
		uint8_t	RREG[2] = { 0 };

		err = TCS3400_Read(pSlave, RREG, addr, 2U);
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

