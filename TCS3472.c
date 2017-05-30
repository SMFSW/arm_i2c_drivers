/*!\file TCS3472.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief TCS3472 Driver code
** \details TCS3472: Color light-to-digital converter with IR filter
**/
/****************************************************************/
#include "TCS3472.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TCS3472)
/****************************************************************/
#if defined(I2C_TCS3400) && !defined(NO_WARN_I2C_DRIVERS)
#warning "TCS3472 -> Multiple TCS34xx types: use with caution, might have same I2C addresses if on same I2C bus!!!"
#endif
/****************************************************************/


I2C_slave TCS3472_hal = { { pNull, I2C_ADDR(TCS3472_BASE_ADDR), I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, false };


/****************************************************************/


FctERR TCS3472_Init(void)
{
	I2C_slave_init(&TCS3472_hal, I2C_TCS3472, TCS3472_BASE_ADDR, I2C_slave_timeout);
	return TCS3472_Init_Sequence();
}


/****************************************************************/


FctERR TCS3472_Write(uint8_t * data, uint16_t addr, uint16_t nb)
{
	uTCS3472_CMD CMD;

	if (!data)								{ return ERR_MEMORY; }		// Null pointer
	if (addr > TCS3472__CONTROL)			{ return ERR_RANGE; }		// Unknown register
	if ((addr + nb) > TCS3472__CONTROL + 1)	{ return ERR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&TCS3472_hal, true);

	CMD.Bits.CMD = 1;
	CMD.Bits.TRANSACTION = TCS3472__TRANS_NORMAL_OP;
	CMD.Bits.ADDR = addr;

	TCS3472_hal.status = HAL_I2C_Mem_Write(TCS3472_hal.cfg.inst, TCS3472_hal.cfg.addr, CMD.Byte, TCS3472_hal.cfg.mem_size, data, nb, TCS3472_hal.cfg.timeout);

	I2C_set_busy(&TCS3472_hal, false);
	return HALERRtoFCTERR(TCS3472_hal.status);
}


FctERR TCS3472_Read(uint8_t * data, uint16_t addr, uint16_t nb)
{
	uTCS3472_CMD CMD;

	if (!data)								{ return ERR_MEMORY; }		// Null pointer
	if (addr > TCS3472__BDATAH)				{ return ERR_RANGE; }		// Unknown register
	if ((addr + nb) > TCS3472__BDATAH + 1)	{ return ERR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&TCS3472_hal, true);

	CMD.Bits.CMD = 1;
	CMD.Bits.TRANSACTION = TCS3472__TRANS_NORMAL_OP;
	CMD.Bits.ADDR = addr;

	TCS3472_hal.status = HAL_I2C_Mem_Read(TCS3472_hal.cfg.inst, TCS3472_hal.cfg.addr, CMD.Byte, TCS3472_hal.cfg.mem_size, data, nb, TCS3472_hal.cfg.timeout);

	I2C_set_busy(&TCS3472_hal, false);
	return HALERRtoFCTERR(TCS3472_hal.status);
}


FctERR TCS3472_Write_Word(uint16_t * data, uint16_t addr)
{
	uint8_t	WREG[2];

	if (addr > TCS3472__CONTROL)		{ return ERR_RANGE; }		// Unknown register

	WREG[0] = (uint8_t) (*data / 0x100);
	WREG[1] = (uint8_t) *data;
	return TCS3472_Write(WREG, addr, 2);
}


FctERR TCS3472_Read_Word(uint16_t * data, uint16_t addr)
{
	uint8_t	WREG[2];
	FctERR	err;

	if (addr > TCS3472__BDATAH)		{ return ERR_RANGE; }		// Unknown register

	err = TCS3472_Read(WREG, addr, 2);
	if (err)	{ return err; }

	*data = (WREG[0] * 0x100) + WREG[1];
	return ERR_OK;
}


FctERR TCS3472_Write_Special(TCS3472_spec_func func)
{
	uTCS3472_CMD CMD;

	if (func != TCS3472__SF_CLR_IT)		{ return ERR_VALUE; }		// Unknown special function

	I2C_set_busy(&TCS3472_hal, true);

	CMD.Bits.CMD = 1;
	CMD.Bits.TRANSACTION = TCS3472__TRANS_SPECIAL_FUNC;
	CMD.Bits.ADDR = func;

	TCS3472_hal.status = HAL_I2C_Master_Transmit(TCS3472_hal.cfg.inst, TCS3472_hal.cfg.addr, &CMD.Byte, 1, TCS3472_hal.cfg.timeout);

	I2C_set_busy(&TCS3472_hal, false);
	return HALERRtoFCTERR(TCS3472_hal.status);
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_TCS3472 in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/

