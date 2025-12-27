/*!\file TMP1075.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief TMP1075 Driver
** \details TMP1075: Temperature Sensor With I2C and SMBus Interface in Industry Standard LM75 Form Factor and Pinout
**/
/****************************************************************/
#include "TMP1075.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TMP1075)
/****************************************************************/


static const I2C_slave_t TMP1075_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FMP }, 0, HAL_OK, true, false };

static I2C_slave_t TMP1075_hal[I2C_TMP1075_NB];	//!< TMP1075 Slave structure


/****************************************************************/


FctERR NONNULL__ TMP1075_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	FctERR	err;

	assert_param(IS_I2C_PERIPHERAL(TMP1075, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(TMP1075, idx);

	I2C_slave_init(&TMP1075_hal[idx], hi2c, devAddress, TMP1075_hal[idx].cfg.timeout);

	err = TMP1075_Init_Sequence(&TMP1075[idx]);

	if (err != ERROR_OK)	{ I2C_set_enable(&TMP1075_hal[idx], false); }

	return ERROR_OK;
}

FctERR TMP1075_Init_Single(void) {
	return TMP1075_Init(0, I2C_TMP1075, TMP1075_BASE_ADDR); }

/****************************************************************/


FctERR NONNULL__ TMP1075_Write_Byte(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr)
{
	if (!I2C_is_enabled(pSlave))		{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > TMP1075__HLIM)			{ return ERROR_VALUE; }		// Address mismatch

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, (uint8_t *) data, 1U, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ TMP1075_Read_Byte(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr)
{
	if (!I2C_is_enabled(pSlave))							{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr > TMP1075__HLIM) && (addr != TMP1075__DIEID))	{ return ERROR_VALUE; }		// Address mismatch

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, data, 1U, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ TMP1075_Write_Word(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr)
{
	if (!I2C_is_enabled(pSlave))		{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > TMP1075__HLIM)			{ return ERROR_VALUE; }		// Address mismatch

	uint8_t	WREG[2] = { HIBYTE(*data), LOBYTE(*data) };

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, WREG, sizeof(WREG), pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ TMP1075_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr)
{
	if (!I2C_is_enabled(pSlave))							{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr > TMP1075__HLIM) && (addr != TMP1075__DIEID))	{ return ERROR_VALUE; }		// Address mismatch

	uint8_t	WREG[2];

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, WREG, sizeof(WREG), pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);

	*data = MAKEWORD(WREG[1], WREG[0]);
	return HALERRtoFCTERR(pSlave->status);
}


/****************************************************************/
#endif
#endif
/****************************************************************/
