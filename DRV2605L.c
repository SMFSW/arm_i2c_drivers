/*!\file DRV2605L.c
** \author SMFSW
** \version v0.3
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


I2C_slave DRV2605_hal = { { pNull, I2C_ADDR(DRV2605L_BASE_ADDR), I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, false };


/****************************************************************/


FctERR DRV2605L_Init(void)
{
	I2C_slave_init(&DRV2605_hal, I2C_DRV2605L, DRV2605L_BASE_ADDR, I2C_slave_timeout);
	return DRV2605L_Init_Sequence();
}


/****************************************************************/


FctERR DRV2605L_Write(uint8_t * data, uint16_t addr, uint16_t nb)
{
	if (!data)											{ return ERR_MEMORY; }		// Null pointer
	if (addr > DRV__LRA_RESONANCE_PERIOD)				{ return ERR_RANGE; }		// Unknown register
	if ((addr + nb - 1) > DRV__LRA_RESONANCE_PERIOD)	{ return ERR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&DRV2605_hal, true);

	DRV2605_hal.status = HAL_I2C_Mem_Write(DRV2605_hal.cfg.inst, DRV2605_hal.cfg.addr, addr, DRV2605_hal.cfg.mem_size, data, nb, DRV2605_hal.cfg.timeout);

	I2C_set_busy(&DRV2605_hal, false);
	return HALERRtoFCTERR(DRV2605_hal.status);
}


FctERR DRV2605L_Read(uint8_t * data, uint16_t addr, uint16_t nb)
{
	if (!data)											{ return ERR_MEMORY; }		// Null pointer
	if (addr > DRV__LRA_RESONANCE_PERIOD)				{ return ERR_RANGE; }		// Unknown register
	if ((addr + nb - 1) > DRV__LRA_RESONANCE_PERIOD)	{ return ERR_OVERFLOW; }	// More bytes than registers

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
