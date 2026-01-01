/*!\file APDS9960.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief APDS9960 Driver
** \details APDS9960: Digital Proximity, Ambient Light, RGB and Gesture Sensor
**/
/****************************************************************/
#include "APDS9960.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_APDS9960)
/****************************************************************/
#if defined(I2C_APDS9930) && !defined(NO_WARN_I2C_DRIVERS)
#warning "APDS9960 -> Multiple APDS99xx types: use with caution, might have same I2C addresses if on same I2C bus!!!"
#endif
/****************************************************************/


static const I2C_slave_t APDS9960_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };

static I2C_slave_t APDS9960_hal[I2C_APDS9960_NB];	//!< APDS9960 Slave structure


/****************************************************************/


FctERR NONNULL__ APDS9960_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	FctERR	err;

	assert_param(IS_I2C_PERIPHERAL(APDS9960, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(APDS9960, idx);

	I2C_slave_init(&APDS9960_hal[idx], hi2c, devAddress, APDS9960_hal[idx].cfg.timeout);

	err = APDS9960_Init_Sequence(&APDS9960[idx]);

	if (err != ERROR_OK)	{ I2C_set_enable(&APDS9960_hal[idx], false); }

	return ERROR_OK;
}

FctERR APDS9960_Init_Single(void) {
	return APDS9960_Init(0, I2C_APDS9960, APDS9960_BASE_ADDR); }

/****************************************************************/


FctERR NONNULL__ APDS9960_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR err = ERROR_OK;

	if (!I2C_is_enabled(pSlave))				{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > APDS9960__AICLEAR + 1U)	{ err = ERROR_OVERFLOW; }	// More bytes than registers
	if (err != ERROR_OK)						{ goto ret; }

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


FctERR NONNULL__ APDS9960_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR err = ERROR_OK;

	if (!I2C_is_enabled(pSlave))				{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > APDS9960__GFIFO_R + 1U)	{ err = ERROR_OVERFLOW; }	// More bytes than registers
	if (err != ERROR_OK)						{ goto ret; }

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


FctERR NONNULL__ APDS9960_Write_Word(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr)
{
	FctERR err = ERROR_FRAMING;

	if (isEven(addr))	// Check unaligned word access
	{
		const uint8_t WREG[2] = { LOBYTE(*data), HIBYTE(*data) };

		err = APDS9960_Write(pSlave, WREG, addr, 2U);
	}

	return err;
}


FctERR NONNULL__ APDS9960_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr)
{
	FctERR err = ERROR_FRAMING;

	if (isEven(addr))	// Check unaligned word access
	{
		uint8_t	RREG[2] = { 0 };

		err = APDS9960_Read(pSlave, RREG, addr, 2U);
		if (err != ERROR_OK)	{ goto ret; }

		*data = MAKEWORD(RREG[0], RREG[1]);
	}

	ret:
	return err;
}


FctERR NONNULL__ APDS9960_Write_Special(I2C_slave_t * const pSlave, const APDS9960_spec_func func)
{
	FctERR err;

	if (func > APDS9960__SF_AICLEAR)	{ err = ERROR_VALUE; goto ret; }	// Unknown special function

	const uint8_t DATA = APDS9960__IFORCE + func;

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Master_Transmit(pSlave->cfg.bus_inst, pSlave->cfg.addr, (uint8_t *) &DATA, 1U, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}

/****************************************************************/
#endif
#endif
/****************************************************************/
