/*!\file TCS3472.c
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
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


static const I2C_slave_t TCS3472_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };

I2C_slave_t TCS3472_hal[I2C_TCS3472_NB];


/****************************************************************/


FctERR NONNULL__ TCS3472_Init(const uint8_t idx, const I2C_HandleTypeDef * hi2c, const uint16_t devAddress)
{
	FctERR err;

	assert_param(IS_I2C_PERIPHERAL(TCS3472, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(TCS3472, idx, devAddress);

	err = I2C_slave_init(&TCS3472_hal[idx], hi2c, devAddress, TCS3472_hal[idx].cfg.timeout);
	if (!err)	{ err = TCS3472_Init_Sequence(&TCS3472[idx]); }

	if (err)	{ I2C_set_enable(&TCS3472_hal[idx], false); }

	return err;
}

FctERR TCS3472_Init_Single(void) {
	return TCS3472_Init(0, I2C_TCS3472, TCS3472_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ TCS3472_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	uTCS3472_CMD CMD;

	if (!I2C_is_enabled(pSlave))			{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > TCS3472__CONTROL)			{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > TCS3472__CONTROL + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	CMD.Bits.CMD = 1;
	CMD.Bits.TRANSACTION = TCS3472__TRANS_NORMAL_OP;
	CMD.Bits.ADDR = addr;

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, CMD.Byte, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ TCS3472_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	uTCS3472_CMD CMD;

	if (!I2C_is_enabled(pSlave))			{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > TCS3472__BDATAH)				{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > TCS3472__BDATAH + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	CMD.Bits.CMD = 1;
	CMD.Bits.TRANSACTION = TCS3472__TRANS_NORMAL_OP;
	CMD.Bits.ADDR = addr;

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, CMD.Byte, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ TCS3472_Write_Word(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr)
{
	uint8_t	WREG[2];

	if (addr > TCS3472__CONTROL)	{ return ERROR_RANGE; }		// Unknown register

	WREG[0] = LOBYTE(*data);
	WREG[1] = HIBYTE(*data);
	return TCS3472_Write(pSlave, WREG, addr, 2);
}


FctERR NONNULL__ TCS3472_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr)
{
	uint8_t	WREG[2];
	FctERR	err;

	if (addr > TCS3472__BDATAH)		{ return ERROR_RANGE; }		// Unknown register

	err = TCS3472_Read(pSlave, WREG, addr, 2);
	if (err)	{ return err; }

	*data = MAKEWORD(WREG[0], WREG[1]);
	return ERROR_OK;
}


FctERR NONNULL__ TCS3472_Write_Special(I2C_slave_t * const pSlave, const TCS3472_spec_func func)
{
	uTCS3472_CMD CMD;

	if (func != TCS3472__SF_CLR_IT)	{ return ERROR_VALUE; }		// Unknown special function

	CMD.Bits.CMD = 1;
	CMD.Bits.TRANSACTION = TCS3472__TRANS_SPECIAL_FUNC;
	CMD.Bits.ADDR = func;

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Master_Transmit(pSlave->cfg.bus_inst, pSlave->cfg.addr, &CMD.Byte, 1, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_TCS3472 for component to be enabled!"
#endif
#endif
/****************************************************************/

