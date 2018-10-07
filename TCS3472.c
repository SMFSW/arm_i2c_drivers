/*!\file TCS3472.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief TCS3472 Driver
** \details TCS3472: Color light-to-digital converter with IR filter
**/
/****************************************************************/
#include "TCS3472.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TCS3472)
/****************************************************************/
#if defined(I2C_TCS3400)
#warning "TCS3472 -> Multiple TCS34xx types: use with caution, might have same I2C addresses if on same I2C bus!!!"
#endif
/****************************************************************/


I2C_slave_t TCS3472_hal = { { pNull, I2C_ADDR(TCS3472_BASE_ADDR), I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };


/****************************************************************/


__WEAK FctERR TCS3472_Init(void)
{
	FctERR err;

	err = I2C_slave_init(&TCS3472_hal, I2C_TCS3472, TCS3472_BASE_ADDR, I2C_slave_timeout);
	if (!err)	{ err = TCS3472_Init_Sequence(); }

	if (err)	{ I2C_set_enable(&TCS3472_hal, false); }

	return err;
}


/****************************************************************/


FctERR NONNULL__ TCS3472_Write(const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	uTCS3472_CMD CMD;

	if (!I2C_is_enabled(&TCS3472_hal))		{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > TCS3472__CONTROL)			{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > TCS3472__CONTROL + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&TCS3472_hal, true);

	CMD.Bits.CMD = 1;
	CMD.Bits.TRANSACTION = TCS3472__TRANS_NORMAL_OP;
	CMD.Bits.ADDR = addr;

	TCS3472_hal.status = HAL_I2C_Mem_Write(TCS3472_hal.cfg.bus_inst, TCS3472_hal.cfg.addr, CMD.Byte, TCS3472_hal.cfg.mem_size, (uint8_t *) data, nb, TCS3472_hal.cfg.timeout);

	I2C_set_busy(&TCS3472_hal, false);
	return HALERRtoFCTERR(TCS3472_hal.status);
}


FctERR NONNULL__ TCS3472_Read(uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	uTCS3472_CMD CMD;

	if (!I2C_is_enabled(&TCS3472_hal))		{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > TCS3472__BDATAH)				{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > TCS3472__BDATAH + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&TCS3472_hal, true);

	CMD.Bits.CMD = 1;
	CMD.Bits.TRANSACTION = TCS3472__TRANS_NORMAL_OP;
	CMD.Bits.ADDR = addr;

	TCS3472_hal.status = HAL_I2C_Mem_Read(TCS3472_hal.cfg.bus_inst, TCS3472_hal.cfg.addr, CMD.Byte, TCS3472_hal.cfg.mem_size, data, nb, TCS3472_hal.cfg.timeout);

	I2C_set_busy(&TCS3472_hal, false);
	return HALERRtoFCTERR(TCS3472_hal.status);
}


FctERR NONNULL__ TCS3472_Write_Word(const uint16_t * data, const uint16_t addr)
{
	uint8_t	WREG[2];

	if (addr > TCS3472__CONTROL)		{ return ERROR_RANGE; }		// Unknown register

	WREG[0] = LOBYTE(*data);
	WREG[1] = HIBYTE(*data);
	return TCS3472_Write(WREG, addr, 2);
}


FctERR NONNULL__ TCS3472_Read_Word(uint16_t * data, const uint16_t addr)
{
	uint8_t	WREG[2];
	FctERR	err;

	if (addr > TCS3472__BDATAH)		{ return ERROR_RANGE; }		// Unknown register

	err = TCS3472_Read(WREG, addr, 2);
	if (err)	{ return err; }

	*data = MAKEWORD(WREG[0], WREG[1]);
	return ERROR_OK;
}


FctERR TCS3472_Write_Special(const TCS3472_spec_func func)
{
	uTCS3472_CMD CMD;

	if (func != TCS3472__SF_CLR_IT)		{ return ERROR_VALUE; }		// Unknown special function

	I2C_set_busy(&TCS3472_hal, true);

	CMD.Bits.CMD = 1;
	CMD.Bits.TRANSACTION = TCS3472__TRANS_SPECIAL_FUNC;
	CMD.Bits.ADDR = func;

	TCS3472_hal.status = HAL_I2C_Master_Transmit(TCS3472_hal.cfg.bus_inst, TCS3472_hal.cfg.addr, &CMD.Byte, 1, TCS3472_hal.cfg.timeout);

	I2C_set_busy(&TCS3472_hal, false);
	return HALERRtoFCTERR(TCS3472_hal.status);
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_TCS3472 in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/

