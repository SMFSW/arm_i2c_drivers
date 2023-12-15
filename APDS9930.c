/*!\file APDS9930.c
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
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

	err = I2C_slave_init(&APDS9930_hal[idx], hi2c, devAddress, APDS9930_hal[idx].cfg.timeout);
	if (!err)	{ err = APDS9930_Init_Sequence(&APDS9930[idx]); }

	if (err)	{ I2C_set_enable(&APDS9930_hal[idx], false); }

	return err;
}

FctERR APDS9930_Init_Single(void) {
	return APDS9930_Init(0, I2C_APDS9930, APDS9930_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ APDS9930_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	uAPDS9930_CMD CMD;

	if (!I2C_is_enabled(pSlave))				{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > APDS9930__POFFSET + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	CMD.Bits.CMD = 1;
	CMD.Bits.TRANSACTION = APDS9930__TRANS_AUTO_INCREMENT;
	CMD.Bits.ADDR = addr;

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, CMD.Byte, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ APDS9930_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	uAPDS9930_CMD CMD;

	if (!I2C_is_enabled(pSlave))				{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > APDS9930__POFFSET + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	CMD.Bits.CMD = 1;
	CMD.Bits.TRANSACTION = APDS9930__TRANS_AUTO_INCREMENT;
	CMD.Bits.ADDR = addr;

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, CMD.Byte, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ APDS9930_Write_Word(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr)
{
	uint8_t	WREG[2];

	if (addr % sizeof(uint16_t))	{ return ERROR_FRAMING; }		// Unaligned word access

	WREG[0] = LOBYTE(*data);
	WREG[1] = HIBYTE(*data);
	return APDS9930_Write(pSlave, WREG, addr, 2);
}


FctERR NONNULL__ APDS9930_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr)
{
	uint8_t	WREG[2];
	FctERR	err;

	if (addr % sizeof(uint16_t))	{ return ERROR_FRAMING; }		// Unaligned word access

	err = APDS9930_Read(pSlave, WREG, addr, 2);
	if (err)	{ return err; }

	*data = MAKEWORD(WREG[0], WREG[1]);
	return ERROR_OK;
}


FctERR NONNULL__ APDS9930_Write_Special(I2C_slave_t * const pSlave, const APDS9930_spec_func func)
{
	uAPDS9930_CMD CMD;

	if (	(func != APDS9930__SF_CLR_PROX_IT)
		&&	(func != APDS9930__SF_CLR_ALS_IT)
		&&	(func != APDS9930__SF_CLR_PROX_ALS_IT))		{ return ERROR_VALUE; }		// Unknown special function

	CMD.Bits.CMD = 1;
	CMD.Bits.TRANSACTION = APDS9930__TRANS_SPECIAL_FUNC;
	CMD.Bits.ADDR = func;

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Master_Transmit(pSlave->cfg.bus_inst, pSlave->cfg.addr, &CMD.Byte, 1, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


/****************************************************************/
#endif
#endif
/****************************************************************/

