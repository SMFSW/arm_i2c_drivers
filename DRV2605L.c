/*!\file DRV2605L.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief DRV2605L Driver
** \details DRV2605L: 2 to 5.2V Haptic Driver for LRA and ERM With Effect Library and Smart-Loop Architecture
**/
/****************************************************************/
#include "DRV2605L.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_DRV2605L)
/****************************************************************/


static const I2C_slave_t DRV2605L_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };

static I2C_slave_t DRV2605L_hal;	//!< DRV2605L Slave structure


/****************************************************************/


__WEAK FctERR DRV2605L_Init(void)
{
	FctERR err;

	I2C_PERIPHERAL_SET_DEFAULTS_SINGLETON(DRV2605L);

	I2C_slave_init(&DRV2605L_hal, I2C_DRV2605L, DRV2605L_BASE_ADDR, DRV2605L_hal.cfg.timeout);

	err = DRV2605L_Init_Sequence();

	if (err != ERROR_OK)	{ I2C_set_enable(&DRV2605L_hal, false); }

	return err;
}


/****************************************************************/


FctERR NONNULL__ DRV2605L_Write(const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(&DRV2605L_hal))					{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > DRV__LRA_RESONANCE_PERIOD + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&DRV2605L_hal, true);
	DRV2605L_hal.status = HAL_I2C_Mem_Write(DRV2605L_hal.cfg.bus_inst, DRV2605L_hal.cfg.addr, addr, DRV2605L_hal.cfg.mem_size, (uint8_t *) data, nb, DRV2605L_hal.cfg.timeout);
	I2C_set_busy(&DRV2605L_hal, false);
	return HALERRtoFCTERR(DRV2605L_hal.status);
}


FctERR NONNULL__ DRV2605L_Read(uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(&DRV2605L_hal))					{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > DRV__LRA_RESONANCE_PERIOD + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&DRV2605L_hal, true);
	DRV2605L_hal.status = HAL_I2C_Mem_Read(DRV2605L_hal.cfg.bus_inst, DRV2605L_hal.cfg.addr, addr, DRV2605L_hal.cfg.mem_size, data, nb, DRV2605L_hal.cfg.timeout);
	I2C_set_busy(&DRV2605L_hal, false);
	return HALERRtoFCTERR(DRV2605L_hal.status);
}


/****************************************************************/
#endif
#endif
/****************************************************************/
