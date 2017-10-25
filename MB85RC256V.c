/*!\file MB85RC256V.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief MB85RC256V Driver code
** \details MB85RC256V: 256-Kbit (32K * 8) I2C Memory FRAM
**/
/****************************************************************/
#include "MB85RC256V.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MB85RC256V)
/****************************************************************/
#if defined(I2C_FM24C)
#warning "MB85RC256V -> Defined along with FM24CxxB: use with caution, might have same I2C addresses if on same I2C bus!!!"
#endif
/****************************************************************/


I2C_slave MB85RC256V_hal = { { pNull, I2C_ADDR(MB85RC256V_BASE_ADDR), I2C_slave_timeout, I2C_MEMADD_SIZE_16BIT, I2C_FMP }, 0, HAL_OK, true, false };


/****************************************************************/


__weak FctERR MB85RC256V_Init(void)
{
	I2C_slave_init(&MB85RC256V_hal, I2C_MB85RC256V, MB85RC256V_BASE_ADDR, I2C_slave_timeout);

	FctERR err = MB85RC256V_Init_Sequence();
	//if (err)	{ I2C_set_enable(&MB85RC256V_hal, false); }

	return err;
}


/****************************************************************/


FctERR MB85RC256V_Write(uint8_t * data, uint16_t addr, uint16_t nb)
{
	if (!I2C_is_enabled(&MB85RC256V_hal))	{ return ERR_DISABLED; }	// Peripheral disabled
	if (!data)								{ return ERR_MEMORY; }		// Null pointer
	if (addr >= MB85RC256V_SIZE)			{ return ERR_RANGE; }		// Unknown register
	if ((addr + nb) > MB85RC256V_SIZE)		{ return ERR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&MB85RC256V_hal, true);

	MB85RC256V_hal.status = HAL_I2C_Mem_Write(MB85RC256V_hal.cfg.inst, MB85RC256V_hal.cfg.addr, addr, MB85RC256V_hal.cfg.mem_size, data, nb, MB85RC256V_hal.cfg.timeout);

	I2C_set_busy(&MB85RC256V_hal, false);
	return HALERRtoFCTERR(MB85RC256V_hal.status);
}


FctERR MB85RC256V_Read(uint8_t * data, uint16_t addr, uint16_t nb)
{
	if (!I2C_is_enabled(&MB85RC256V_hal))	{ return ERR_DISABLED; }	// Peripheral disabled
	if (!data)								{ return ERR_MEMORY; }		// Null pointer
	if (addr >= MB85RC256V_SIZE)			{ return ERR_RANGE; }		// Unknown register
	if ((addr + nb) > MB85RC256V_SIZE)		{ return ERR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&MB85RC256V_hal, true);

	MB85RC256V_hal.status = HAL_I2C_Mem_Read(MB85RC256V_hal.cfg.inst, MB85RC256V_hal.cfg.addr, addr, MB85RC256V_hal.cfg.mem_size, data, nb, MB85RC256V_hal.cfg.timeout);

	I2C_set_busy(&MB85RC256V_hal, false);
	return HALERRtoFCTERR(MB85RC256V_hal.status);
}


FctERR MB85RC256V_Read_ID(uint8_t * data)
{
	if (!data)							{ return ERR_MEMORY; }		// Null pointer

	I2C_set_busy(&MB85RC256V_hal, true);

	MB85RC256V_hal.status = HAL_I2C_Mem_Read(MB85RC256V_hal.cfg.inst, 0xF8, MB85RC256V_hal.cfg.addr, I2C_MEMADD_SIZE_8BIT, data, 3, MB85RC256V_hal.cfg.timeout);

	I2C_set_busy(&MB85RC256V_hal, false);
	return HALERRtoFCTERR(MB85RC256V_hal.status);
}

/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_MB85RC256V in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/
