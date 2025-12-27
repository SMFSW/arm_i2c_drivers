/*!\file APDS9960.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
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
	if (!I2C_is_enabled(pSlave))				{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > APDS9960__AICLEAR + 1U)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ APDS9960_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(pSlave))				{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > APDS9960__GFIFO_R + 1U)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ APDS9960_Write_Word(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr)
{
	uint8_t	WREG[2];

	if (addr % sizeof(uint16_t))	{ return ERROR_FRAMING; }		// Unaligned word access

	WREG[0] = LOBYTE(*data);
	WREG[1] = HIBYTE(*data);
	return APDS9960_Write(pSlave, WREG, addr, 2U);
}


FctERR NONNULL__ APDS9960_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr)
{
	uint8_t	WREG[2];
	FctERR	err;

	if (addr % sizeof(uint16_t))	{ return ERROR_FRAMING; }		// Unaligned word access

	err = APDS9960_Read(pSlave, WREG, addr, 2U);
	if (err != ERROR_OK)	{ return err; }

	*data = MAKEWORD(WREG[0], WREG[1]);
	return ERROR_OK;
}


FctERR NONNULL__ APDS9960_Write_Special(I2C_slave_t * const pSlave, const APDS9960_spec_func func)
{
	if (func > APDS9960__SF_AICLEAR)	{ return ERROR_VALUE; }		// Unknown special function

	const uint8_t DATA = APDS9960__IFORCE + func;

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Master_Transmit(pSlave->cfg.bus_inst, pSlave->cfg.addr, (uint8_t *) &DATA, 1U, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}

/****************************************************************/
#endif
#endif
/****************************************************************/
