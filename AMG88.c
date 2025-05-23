/*!\file AMG88.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief AMG88 Driver
** \details AMG88: Infrared Array Sensor (Grid-EYE)
**/
/****************************************************************/
#include "AMG88.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_AMG88)
/****************************************************************/


static const I2C_slave_t AMG88_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };

static I2C_slave_t AMG88_hal[I2C_AMG88_NB];		//!< AMG88 Slave structure


/****************************************************************/


FctERR NONNULL__ AMG88_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	FctERR	err;

	assert_param(IS_I2C_PERIPHERAL(AMG88, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(AMG88, idx);

	I2C_slave_init(&AMG88_hal[idx], hi2c, devAddress, AMG88_hal[idx].cfg.timeout);

	err = AMG88_Init_Sequence(&AMG88[idx]);

	if (err)	{ I2C_set_enable(&AMG88_hal[idx], false); }

	return ERROR_OK;
}

FctERR AMG88_Init_Single(void) {
	return AMG88_Init(0, I2C_AMG88, AMG88_BASE_ADDR); }

/****************************************************************/


FctERR NONNULL__ AMG88_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(pSlave))		{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > AMG88__IHYSH + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ AMG88_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(pSlave))		{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > AMG88__T64H + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ AMG88_Write_Word(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr)
{
	uint8_t	WREG[2];

	if (addr % sizeof(uint16_t))	{ return ERROR_FRAMING; }		// Unaligned word access

	WREG[0] = LOBYTE(*data);
	WREG[1] = HIBYTE(*data);
	return AMG88_Write(pSlave, WREG, addr, 2);
}


FctERR NONNULL__ AMG88_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr)
{
	uint8_t	WREG[2];
	FctERR	err;

	if (addr % sizeof(uint16_t))	{ return ERROR_FRAMING; }		// Unaligned word access

	err = AMG88_Read(pSlave, WREG, addr, 2);
	if (err)	{ return err; }

	*data = MAKEWORD(WREG[0], WREG[1]);
	return ERROR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
