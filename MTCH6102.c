/*!\file MTCH6102.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief MTCH6102 Driver
** \details MTCH6102: Low-Power Projected Capacitive Touch Controller
**/
/****************************************************************/
#include "MTCH6102.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MTCH6102)
/****************************************************************/


static const I2C_slave_t MTCH6102_defaults  = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };

I2C_slave_t MTCH6102_hal[I2C_MTCH6102_NB];


/****************************************************************/


FctERR NONNULL__ MTCH6102_Init(const uint8_t idx, const I2C_HandleTypeDef * hi2c, const uint16_t devAddress)
{
	FctERR err;

	assert_param(IS_I2C_PERIPHERAL(MTCH6102, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(MTCH6102, idx, devAddress);

	err = I2C_slave_init(&MTCH6102_hal[idx], hi2c, devAddress, I2C_slave_timeout);
	if (!err)	{ err = MTCH6102_Init_Sequence(&MTCH6102[idx]); }

	if (err)	{ I2C_set_enable(&MTCH6102_hal[idx], false); }

	return err;
}

FctERR MTCH6102_Init_Single(void) {
	return MTCH6102_Init(0, I2C_MTCH6102, MTCH6102_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ MTCH6102_Write(I2C_slave_t * pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(pSlave))			{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > MTCH__RAW_ADC_31)			{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > MTCH__RAW_ADC_31 + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ MTCH6102_Read(I2C_slave_t * pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(pSlave))			{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > MTCH__RAW_ADC_31)			{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > MTCH__RAW_ADC_31 + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_MTCH6102 in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/

