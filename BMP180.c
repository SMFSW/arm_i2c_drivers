/*!\file BMP180.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief BMP180 Driver code
** \details BMP180: Digital pressure sensor
**/
/****************************************************************/
#include "BMP180.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_BMP180)
/****************************************************************/


I2C_slave BMP180_hal = { { pNull, I2C_ADDR(BMP180_BASE_ADDR), I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_HS }, 0, HAL_OK, true, false };


/****************************************************************/


__WEAK FctERR BMP180_Init(void)
{
	I2C_slave_init(&BMP180_hal, I2C_BMP180, BMP180_BASE_ADDR, I2C_slave_timeout);

	FctERR err = BMP180_Init_Sequence();
	if (err)	{ I2C_set_enable(&BMP180_hal, false); }

	return err;
}


/****************************************************************/


FctERR BMP180_Write(uint8_t * data, uint16_t addr, uint16_t nb)
{
	if (!I2C_is_enabled(&BMP180_hal))		{ return ERR_DISABLED; }	// Peripheral disabled
	if (!data)								{ return ERR_MEMORY; }		// Null pointer
	if (addr > BMP180__OUT_XLSB)			{ return ERR_RANGE; }		// Unknown register
	if ((addr + nb) > BMP180__OUT_XLSB + 1)	{ return ERR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&BMP180_hal, true);

	BMP180_hal.status = HAL_I2C_Mem_Write(BMP180_hal.cfg.inst, BMP180_hal.cfg.addr, addr, BMP180_hal.cfg.mem_size, data, nb, BMP180_hal.cfg.timeout);

	I2C_set_busy(&BMP180_hal, false);
	return HALERRtoFCTERR(BMP180_hal.status);
}


FctERR BMP180_Read(uint8_t * data, uint16_t addr, uint16_t nb)
{
	if (!I2C_is_enabled(&BMP180_hal))		{ return ERR_DISABLED; }	// Peripheral disabled
	if (!data)								{ return ERR_MEMORY; }		// Null pointer
	if (addr > BMP180__OUT_XLSB)			{ return ERR_RANGE; }		// Unknown register
	if ((addr + nb) > BMP180__OUT_XLSB + 1)	{ return ERR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&BMP180_hal, true);

	BMP180_hal.status = HAL_I2C_Mem_Read(BMP180_hal.cfg.inst, BMP180_hal.cfg.addr, addr, BMP180_hal.cfg.mem_size, data, nb, BMP180_hal.cfg.timeout);

	I2C_set_busy(&BMP180_hal, false);
	return HALERRtoFCTERR(BMP180_hal.status);
}


FctERR BMP180_Read_Word(uint16_t * data, uint16_t addr)
{
	uint8_t	WREG[2];
	FctERR	err;

	if (addr > BMP180__OUT_XLSB)		{ return ERR_RANGE; }		// Unknown register

	err = BMP180_Read(WREG, addr, 2);
	if (err)	{ return err; }

	*data = MAKEWORD(WREG[1], WREG[0]);
	return HALERRtoFCTERR(BMP180_hal.status);
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_BMP180 in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/
