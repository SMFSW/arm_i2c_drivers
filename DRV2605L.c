/*!\file DRV2605L.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief DRV2605L Driver code
** \details DRV2605L: 2 to 5.2V Haptic Driver for LRA and ERM With Effect Library and Smart-Loop Architecture
**/
/****************************************************************/
#include "DRV2605L.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_DRV2605L)
/****************************************************************/


I2C_slave DRV2605_hal = { { pNull, I2C_ADDR(DRV2605L_BASE_ADDR), I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };


/****************************************************************/


__WEAK FctERR DRV2605L_Init(void)
{
	FctERR err;

	err = I2C_slave_init(&DRV2605_hal, I2C_DRV2605L, DRV2605L_BASE_ADDR, I2C_slave_timeout);
	if (!err)	{ err = DRV2605L_Init_Sequence(); }

	if (err)	{ I2C_set_enable(&DRV2605_hal, false); }

	return err;
}


/****************************************************************/


FctERR DRV2605L_Write(const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(&DRV2605_hal))					{ return ERROR_DISABLED; }	// Peripheral disabled
	if (!data)											{ return ERROR_MEMORY; }	// Null pointer
	if (addr > DRV__LRA_RESONANCE_PERIOD)				{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > DRV__LRA_RESONANCE_PERIOD + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&DRV2605_hal, true);

	DRV2605_hal.status = HAL_I2C_Mem_Write(DRV2605_hal.cfg.inst, DRV2605_hal.cfg.addr, addr, DRV2605_hal.cfg.mem_size, (uint8_t *) data, nb, DRV2605_hal.cfg.timeout);

	I2C_set_busy(&DRV2605_hal, false);
	return HALERRtoFCTERR(DRV2605_hal.status);
}


FctERR DRV2605L_Read(uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(&DRV2605_hal))					{ return ERROR_DISABLED; }	// Peripheral disabled
	if (!data)											{ return ERROR_MEMORY; }	// Null pointer
	if (addr > DRV__LRA_RESONANCE_PERIOD)				{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > DRV__LRA_RESONANCE_PERIOD + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&DRV2605_hal, true);

	DRV2605_hal.status = HAL_I2C_Mem_Read(DRV2605_hal.cfg.inst, DRV2605_hal.cfg.addr, addr, DRV2605_hal.cfg.mem_size, data, nb, DRV2605_hal.cfg.timeout);

	I2C_set_busy(&DRV2605_hal, false);
	return HALERRtoFCTERR(DRV2605_hal.status);
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_DRV2605L in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/
