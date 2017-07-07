/*!\file TCS3400.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief TCS3400 Driver code
** \details TCS3400: Color Light-to-Digital Converter
**/
/****************************************************************/
#include "TCS3400.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TCS3400)
/****************************************************************/
#if defined(I2C_TCS3472) && !defined(NO_WARN_I2C_DRIVERS)
#warning "TCS3400 -> Multiple TCS34xx types: use with caution, might have same I2C addresses if on same I2C bus!!!"
#endif
/****************************************************************/


I2C_slave TCS3400_hal = { { pNull, I2C_ADDR(TCS3400_BASE_ADDR), I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };


/****************************************************************/


FctERR TCS3400_Init(void)
{
	I2C_slave_init(&TCS3400_hal, I2C_TCS3400, TCS3400_BASE_ADDR, I2C_slave_timeout);

	FctERR err = TCS3400_Init_Sequence();
	if (err)	{ I2C_set_enable(&TCS3400_hal, false); }

	return err;
}


/****************************************************************/


FctERR TCS3400_Write(uint8_t * data, uint16_t addr, uint16_t nb)
{
	if (!I2C_is_enabled(&TCS3400_hal))		{ return ERR_DISABLED; }	// Peripheral disabled
	if (!data)								{ return ERR_MEMORY; }		// Null pointer
	if (addr > TCS3400__AICLEAR)			{ return ERR_RANGE; }		// Unknown register
	if ((addr + nb) > TCS3400__AICLEAR + 1)	{ return ERR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&TCS3400_hal, true);

	TCS3400_hal.status = HAL_I2C_Mem_Write(TCS3400_hal.cfg.inst, TCS3400_hal.cfg.addr, addr, TCS3400_hal.cfg.mem_size, data, nb, TCS3400_hal.cfg.timeout);

	I2C_set_busy(&TCS3400_hal, false);
	return HALERRtoFCTERR(TCS3400_hal.status);
}


FctERR TCS3400_Read(uint8_t * data, uint16_t addr, uint16_t nb)
{
	if (!I2C_is_enabled(&TCS3400_hal))		{ return ERR_DISABLED; }	// Peripheral disabled
	if (!data)							{ return ERR_MEMORY; }		// Null pointer
	if (addr > TCS3400__IR)				{ return ERR_RANGE; }		// Unknown register
	if ((addr + nb) > TCS3400__IR + 1)	{ return ERR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&TCS3400_hal, true);

	TCS3400_hal.status = HAL_I2C_Mem_Read(TCS3400_hal.cfg.inst, TCS3400_hal.cfg.addr, addr, TCS3400_hal.cfg.mem_size, data, nb, TCS3400_hal.cfg.timeout);

	I2C_set_busy(&TCS3400_hal, false);
	return HALERRtoFCTERR(TCS3400_hal.status);
}


FctERR TCS3400_Write_Word(uint16_t * data, uint16_t addr)
{
	uint8_t	WREG[2];

	if (addr > TCS3400__CICLEAR)		{ return ERR_RANGE; }		// Unknown register

	WREG[0] = (uint8_t) (*data / 0x100);
	WREG[1] = (uint8_t) *data;
	return TCS3400_Write(WREG, addr, 2);
}


FctERR TCS3400_Read_Word(uint16_t * data, uint16_t addr)
{
	uint8_t	WREG[2];
	FctERR	err;

	if (addr > TCS3400__BDATAH)		{ return ERR_RANGE; }		// Unknown register

	err = TCS3400_Read(WREG, addr, 2);
	if (err)	{ return err; }

	*data = (WREG[0] * 0x100) + WREG[1];
	return ERR_OK;
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_TCS3400 in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/

