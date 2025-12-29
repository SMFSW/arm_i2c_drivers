/*!\file AMG88.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief AMG88 Driver
** \details AMG88: Infrared Array Sensor (Grid-EYE)
**/
/****************************************************************/
#include "AMG88.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_AMG88)
/****************************************************************/


static const I2C_slave_t AMG88_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };

static I2C_slave_t AMG88_hal[I2C_AMG88_NB];		//!< AMG88 Slave structure


/****************************************************************/


FctERR NONNULL__ AMG88_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	FctERR	err;

	assert_param(IS_I2C_PERIPHERAL(AMG88, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(AMG88, idx);

	I2C_slave_init(&AMG88_hal[idx], hi2c, devAddress, AMG88_hal[idx].cfg.timeout);

	err = AMG88_Init_Sequence(&AMG88[idx]);

	if (err != ERROR_OK)	{ I2C_set_enable(&AMG88_hal[idx], false); }

	return err;
}

FctERR AMG88_Init_Single(void) {
	return AMG88_Init(0, I2C_AMG88, AMG88_BASE_ADDR); }

/****************************************************************/


FctERR NONNULL__ AMG88_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR err = ERROR_OK;

	if (!I2C_is_enabled(pSlave))			{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > (AMG88__IHYSH + 1U))	{ err = ERROR_OVERFLOW; }	// More bytes than registers
	if (err != ERROR_OK)					{ goto ret; }

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


FctERR NONNULL__ AMG88_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR err = ERROR_OK;

	if (!I2C_is_enabled(pSlave))			{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > (AMG88__T64H + 1U))	{ err = ERROR_OVERFLOW; }	// More bytes than registers
	if (err != ERROR_OK)					{ goto ret; }

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


FctERR NONNULL__ AMG88_Write_Word(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr)
{
	FctERR err = ERROR_FRAMING;

	if (isEven(addr))	// Check unaligned word access
	{
		const uint8_t WREG[2] = { LOBYTE(*data), HIBYTE(*data) };

		err = AMG88_Write(pSlave, WREG, addr, 2U);
	}

	return err;
}


FctERR NONNULL__ AMG88_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr)
{
	FctERR err = ERROR_FRAMING;

	if (isEven(addr))	// Check unaligned word access
	{
		uint8_t	RREG[2] = { 0 };

		err = AMG88_Read(pSlave, RREG, addr, 2U);
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
