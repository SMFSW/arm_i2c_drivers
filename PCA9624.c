/*!\file PCA9624.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief PCA9624 Driver code
** \details PCA9624: 8-bit Fm+ I2C-bus 100mA 40V LED driver
**/
/****************************************************************/
#include "PCA9624.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9624)
/****************************************************************/
#if defined(I2C_PCA9685)
#warning "PCA9624 -> Multiple PCA96xx types: use with caution if using CALL addresses if on same I2C bus!!!"
#endif
/****************************************************************/


I2C_slave PCA9624_hal = { { pNull, I2C_ADDR(PCA9624_BASE_ADDR), I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FMP }, 0, HAL_OK, true, false };


/****************************************************************/


__WEAK FctERR PCA9624_Init(void)
{
	I2C_slave_init(&PCA9624_hal, I2C_PCA9624, PCA9624_BASE_ADDR, I2C_slave_timeout);

	FctERR err = PCA9624_Init_Sequence();
	if (err)	{ I2C_set_enable(&PCA9624_hal, false); }

	return err;
}


/****************************************************************/


FctERR PCA9624_Write(const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(&PCA9624_hal))			{ return ERROR_DISABLED; }	// Peripheral disabled
	if (!data)									{ return ERROR_MEMORY; }	// Null pointer
	if (addr > PCA9624__ALLCALLADR)				{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > PCA9624__ALLCALLADR + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&PCA9624_hal, true);

	PCA9624_hal.status = HAL_I2C_Mem_Write(PCA9624_hal.cfg.inst, PCA9624_hal.cfg.addr, addr, PCA9624_hal.cfg.mem_size, (uint8_t *) data, nb, PCA9624_hal.cfg.timeout);

	I2C_set_busy(&PCA9624_hal, false);
	return HALERRtoFCTERR(PCA9624_hal.status);
}


FctERR PCA9624_Read(uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(&PCA9624_hal))			{ return ERROR_DISABLED; }	// Peripheral disabled
	if (!data)									{ return ERROR_MEMORY; }	// Null pointer
	if (addr > PCA9624__ALLCALLADR)				{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > PCA9624__ALLCALLADR + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&PCA9624_hal, true);

	PCA9624_hal.status = HAL_I2C_Mem_Read(PCA9624_hal.cfg.inst, PCA9624_hal.cfg.addr, addr, PCA9624_hal.cfg.mem_size, data, nb, PCA9624_hal.cfg.timeout);

	I2C_set_busy(&PCA9624_hal, false);
	return HALERRtoFCTERR(PCA9624_hal.status);
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_PCA9624 in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/
