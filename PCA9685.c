/*!\file PCA9685.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief PCA9685 Driver code
** \details PCA9685: 16-channel, 12-bit PWM Fm+ I2C-bus LED controller
**/
/****************************************************************/
#include "PCA9685.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9685)
/****************************************************************/
#if defined(I2C_PCA9624)
#warning "PCA9685 -> Multiple PCA96xx types: use with caution if using CALL addresses if on same I2C bus!!!"
#endif
/****************************************************************/


I2C_slave PCA9685_hal = { { pNull, I2C_ADDR(PCA9685_BASE_ADDR), I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FMP }, 0, HAL_OK, true, false };


/****************************************************************/


__WEAK FctERR PCA9685_Init(void)
{
	I2C_slave_init(&PCA9685_hal, I2C_PCA9685, PCA9685_BASE_ADDR, I2C_slave_timeout);

	FctERR err = PCA9685_Init_Sequence();
	if (err)	{ I2C_set_enable(&PCA9685_hal, false); }

	return err;
}


/****************************************************************/


FctERR PCA9685_Write(uint8_t * data, uint16_t addr, uint16_t nb)
{
	if (!I2C_is_enabled(&PCA9685_hal))			{ return ERROR_DISABLED; }	// Peripheral disabled
	if (!data)									{ return ERROR_MEMORY; }		// Null pointer
	if (addr > PCA9685__TestMode)				{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > PCA9685__TestMode + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&PCA9685_hal, true);

	PCA9685_hal.status = HAL_I2C_Mem_Write(PCA9685_hal.cfg.inst, PCA9685_hal.cfg.addr, addr, PCA9685_hal.cfg.mem_size, data, nb, PCA9685_hal.cfg.timeout);

	I2C_set_busy(&PCA9685_hal, false);
	return HALERRtoFCTERR(PCA9685_hal.status);
}


FctERR PCA9685_Read(uint8_t * data, uint16_t addr, uint16_t nb)
{
	if (!I2C_is_enabled(&PCA9685_hal))			{ return ERROR_DISABLED; }	// Peripheral disabled
	if (!data)									{ return ERROR_MEMORY; }		// Null pointer
	if (addr > PCA9685__TestMode)				{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > PCA9685__TestMode + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&PCA9685_hal, true);
	PCA9685_hal.status = HAL_I2C_Mem_Read(PCA9685_hal.cfg.inst, PCA9685_hal.cfg.addr, addr, PCA9685_hal.cfg.mem_size, data, nb, PCA9685_hal.cfg.timeout);

	I2C_set_busy(&PCA9685_hal, false);
	return HALERRtoFCTERR(PCA9685_hal.status);
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_PCA9685 in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/
