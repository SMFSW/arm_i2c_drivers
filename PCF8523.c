/*!\file PCF8523.c
** \author SMFSW
** \version v0.3
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief PCF8523 Driver code
** \details PCF8523: Real-Time Clock (RTC) and calendar
**/
/****************************************************************/
#include "PCF8523.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCF8523)
/****************************************************************/


I2C_slave PCF8523_hal = { { pNull, I2C_ADDR(PCF8523_BASE_ADDR), I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FMP }, 0, HAL_OK, false };


/****************************************************************/


FctERR PCF8523_Init(void)
{
	I2C_slave_init(&PCF8523_hal, I2C_PCF8523, PCF8523_BASE_ADDR, I2C_slave_timeout);
	return PCF8523_Init_Sequence();
}


/****************************************************************/


FctERR PCF8523_Write(uint8_t * data, uint16_t addr, uint16_t nb)
{
	if (!data)									{ return ERR_MEMORY; }		// Null pointer
	if (addr > PCF8523__TMR_B_REG)				{ return ERR_RANGE; }		// Unknown register
	if ((addr + nb -1) > PCF8523__TMR_B_REG)	{ return ERR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&PCF8523_hal, true);

	PCF8523_hal.status = HAL_I2C_Mem_Write(PCF8523_hal.cfg.inst, PCF8523_hal.cfg.addr, addr, PCF8523_hal.cfg.mem_size, data, nb, PCF8523_hal.cfg.timeout);

	I2C_set_busy(&PCF8523_hal, false);
	return HALERRtoFCTERR(PCF8523_hal.status);
}


FctERR PCF8523_Read(uint8_t * data, uint16_t addr, uint16_t nb)
{
	if (!data)									{ return ERR_MEMORY; }		// Null pointer
	if (addr > PCF8523__TMR_B_REG)				{ return ERR_RANGE; }		// Unknown register
	if ((addr + nb - 1) > PCF8523__TMR_B_REG)	{ return ERR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&PCF8523_hal, true);

	PCF8523_hal.status = HAL_I2C_Mem_Read(PCF8523_hal.cfg.inst, PCF8523_hal.cfg.addr, addr, PCF8523_hal.cfg.mem_size, data, nb, PCF8523_hal.cfg.timeout);

	I2C_set_busy(&PCF8523_hal, false);
	return HALERRtoFCTERR(PCF8523_hal.status);
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_PCF8523 in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/
