/*!\file APDS9930.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief APDS9930 Driver
** \details APDS9930: Digital Proximity and Ambient Light Sensor
**/
/****************************************************************/
#include "APDS9930.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_APDS9930)
/****************************************************************/
#if defined(I2C_APDS9960) && !defined(NO_WARN_I2C_DRIVERS)
#warning "APDS9930 -> Multiple APDS99xx types: use with caution, might have same I2C addresses if on same I2C bus!!!"
#endif
/****************************************************************/


static const I2C_slave_t APDS9930_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };

static I2C_slave_t APDS9930_hal[I2C_APDS9930_NB];	//!< APDS9930 Slave structure


/****************************************************************/


FctERR NONNULL__ APDS9930_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	FctERR err;

	assert_param(IS_I2C_PERIPHERAL(APDS9930, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(APDS9930, idx);

	I2C_slave_init(&APDS9930_hal[idx], hi2c, devAddress, APDS9930_hal[idx].cfg.timeout);

	err = APDS9930_Init_Sequence(&APDS9930[idx]);

	if (err != ERROR_OK)	{ I2C_set_enable(&APDS9930_hal[idx], false); }

	return err;
}

FctERR APDS9930_Init_Single(void) {
	return APDS9930_Init(0, I2C_APDS9930, APDS9930_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ APDS9930_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR			err = ERROR_OK;
	uAPDS9930_CMD	CMD;

	if (!I2C_is_enabled(pSlave))				{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > APDS9930__POFFSET + 1U)	{ err = ERROR_OVERFLOW; }	// More bytes than registers
	if (err != ERROR_OK)						{ goto ret; }

	CMD.Bits.CMD = 1U;
	CMD.Bits.TRANSACTION = APDS9930__TRANS_AUTO_INCREMENT;
	CMD.Bits.ADDR = addr;

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, CMD.Byte, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


FctERR NONNULL__ APDS9930_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR			err = ERROR_OK;
	uAPDS9930_CMD	CMD;

	if (!I2C_is_enabled(pSlave))				{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > APDS9930__POFFSET + 1U)	{ err = ERROR_OVERFLOW; }	// More bytes than registers
	if (err != ERROR_OK)						{ goto ret; }

	CMD.Bits.CMD = 1U;
	CMD.Bits.TRANSACTION = APDS9930__TRANS_AUTO_INCREMENT;
	CMD.Bits.ADDR = addr;

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, CMD.Byte, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


FctERR NONNULL__ APDS9930_Write_Word(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr)
{
	FctERR err = ERROR_FRAMING;

	if (isEven(addr))	// Check unaligned word access
	{
		const uint8_t WREG[2] = { LOBYTE(*data), HIBYTE(*data) };

		err = APDS9930_Write(pSlave, WREG, addr, 2U);
	}

	return err;
}


FctERR NONNULL__ APDS9930_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr)
{
	FctERR err = ERROR_FRAMING;

	if (isEven(addr))	// Check unaligned word access
	{
		uint8_t	RREG[2] = { 0 };

		err = APDS9930_Read(pSlave, RREG, addr, 2U);
		if (err != ERROR_OK)	{ goto ret; }

		*data = MAKEWORD(RREG[0], RREG[1]);
	}

	ret:
	return err;
}


FctERR NONNULL__ APDS9930_Write_Special(I2C_slave_t * const pSlave, const APDS9930_spec_func func)
{
	FctERR			err;
	uAPDS9930_CMD	CMD;

	if (	(func != APDS9930__SF_CLR_PROX_IT)
		&&	(func != APDS9930__SF_CLR_ALS_IT)
		&&	(func != APDS9930__SF_CLR_PROX_ALS_IT))	{ err = ERROR_VALUE; goto ret; }	// Unknown special function

	CMD.Bits.CMD = 1U;
	CMD.Bits.TRANSACTION = APDS9930__TRANS_SPECIAL_FUNC;
	CMD.Bits.ADDR = func;

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Master_Transmit(pSlave->cfg.bus_inst, pSlave->cfg.addr, &CMD.Byte, 1U, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/

