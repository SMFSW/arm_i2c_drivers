/*!\file BMP180.c
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief BMP180 Driver
** \details BMP180: Digital pressure sensor
**/
/****************************************************************/
#include "BMP180.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_BMP180)
/****************************************************************/


static const I2C_slave_t BMP180_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_HS }, 0, HAL_OK, true, false };

I2C_slave_t BMP180_hal[I2C_BMP180_NB];


/****************************************************************/


FctERR NONNULL__ BMP180_Init(const uint8_t idx, const I2C_HandleTypeDef * hi2c, const uint16_t devAddress)
{
	FctERR err;

	assert_param(IS_I2C_PERIPHERAL(BMP180, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(BMP180, idx, devAddress);

	err = I2C_slave_init(&BMP180_hal[idx], hi2c, devAddress, BMP180_hal[idx].cfg.timeout);
	if (!err)	{ err = BMP180_Init_Sequence(&BMP180[idx]); }

	if (err)	{ I2C_set_enable(&BMP180_hal[idx], false); }

	return err;
}

FctERR BMP180_Init_Single(void) {
	return BMP180_Init(0, I2C_BMP180, BMP180_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ BMP180_Write(I2C_slave_t * pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(pSlave))			{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > BMP180__OUT_XLSB)			{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > BMP180__OUT_XLSB + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ BMP180_Read(I2C_slave_t * pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(pSlave))			{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > BMP180__OUT_XLSB)			{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > BMP180__OUT_XLSB + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ BMP180_Read_Word(I2C_slave_t * pSlave, uint16_t * data, const uint16_t addr)
{
	uint8_t	RREG[2];
	FctERR	err;

	if (addr > BMP180__OUT_XLSB)		{ return ERROR_RANGE; }		// Unknown register

	err = BMP180_Read(pSlave, RREG, addr, 2);
	if (err)	{ return err; }

	*data = MAKEWORD(RREG[1], RREG[0]);
	return HALERRtoFCTERR(pSlave->status);
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_BMP180 for component to be enabled!"
#endif
#endif
/****************************************************************/
