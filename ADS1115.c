/*!\file ADS1115.c
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief ADS1115 Driver
** \details ADS1115: Ultra-Small, Low-Power, 16-Bit Analog-to-Digital Converter with Internal Reference
** \note	Compatibility with:
**				- ADS1113 (restrictions given in datasheet)
**				- ADS1114 (restrictions given in datasheet)
**				- ADS1115
** \note	Compatibility should be assured with:
**				- ADS1x1x (check datasheets for compatibility informations)
**/
/****************************************************************/
#include "ADS1115.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_ADS1115)
/****************************************************************/


static const I2C_slave_t ADS1115_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FMP }, 0, HAL_OK, true, false };

I2C_slave_t ADS1115_hal[I2C_ADS1115_NB];


/****************************************************************/


FctERR NONNULL__ ADS1115_Init(const uint8_t idx, const I2C_HandleTypeDef * hi2c, const uint16_t devAddress)
{
	FctERR	err;

	assert_param(IS_I2C_PERIPHERAL(ADS1115, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(ADS1115, idx, devAddress);

	err = I2C_slave_init(&ADS1115_hal[idx], hi2c, devAddress, ADS1115_hal[idx].cfg.timeout);
	if (!err)	{ err = ADS1115_Init_Sequence(&ADS1115[idx]); }

	if (err)	{ I2C_set_enable(&ADS1115_hal[idx], false); }

	return ERROR_OK;
}

FctERR ADS1115_Init_Single(void) {
	return ADS1115_Init(0, I2C_ADS1115, ADS1115_BASE_ADDR); }

/****************************************************************/


FctERR NONNULL__ ADS1115_Write(I2C_slave_t * pSlave, const uint16_t * data, const uint16_t addr)
{
	if (!I2C_is_enabled(pSlave))		{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > ADS1115__HI_THRESH)		{ return ERROR_RANGE; }		// Unknown register

	uint8_t WREG[2] = { HIBYTE(*data), LOBYTE(*data) };

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, WREG, 2, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ ADS1115_Read(I2C_slave_t * pSlave, uint16_t * data, const uint16_t addr)
{
	uint8_t	RREG[2];

	if (!I2C_is_enabled(pSlave))		{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > ADS1115__HI_THRESH)		{ return ERROR_RANGE; }		// Unknown register

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, RREG, 2, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);

	if (pSlave->status == HAL_OK)	{ *data = MAKEWORD(RREG[1], RREG[0]); }

	return HALERRtoFCTERR(pSlave->status);
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_ADS1115 in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/
