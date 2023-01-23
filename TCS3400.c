/*!\file TCS3400.c
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief TCS3400 Driver
** \details TCS3400: Color Light-to-Digital Converter
**/
/****************************************************************/
#include "TCS3400.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TCS3400)
/****************************************************************/
#if defined(I2C_TCS3472)
#warning "TCS3400 -> Multiple TCS34xx types: use with caution, might have same I2C addresses if on same I2C bus!!!"
#endif
/****************************************************************/


static const I2C_slave_t TCS3400_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };

I2C_slave_t TCS3400_hal[I2C_TCS3400_NB];


/****************************************************************/


FctERR NONNULL__ TCS3400_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	FctERR err;

	assert_param(IS_I2C_PERIPHERAL(TCS3400, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(TCS3400, idx, devAddress);

	err = I2C_slave_init(&TCS3400_hal[idx], hi2c, devAddress, TCS3400_hal[idx].cfg.timeout);
	if (!err)	{ err = TCS3400_Init_Sequence(&TCS3400[idx]); }

	if (err)	{ I2C_set_enable(&TCS3400_hal[idx], false); }

	return err;
}

FctERR TCS3400_Init_Single(void) {
	return TCS3400_Init(0, I2C_TCS3400, TCS3400_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ TCS3400_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(pSlave))			{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > TCS3400__AICLEAR)			{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > TCS3400__AICLEAR + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ TCS3400_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(pSlave))			{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > TCS3400__IR)					{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > TCS3400__IR + 1)		{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ TCS3400_Write_Word(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr)
{
	uint8_t	WREG[2];

	if (addr > TCS3400__CICLEAR)		{ return ERROR_RANGE; }		// Unknown register

	WREG[0] = LOBYTE(*data);
	WREG[1] = HIBYTE(*data);
	return TCS3400_Write(pSlave, WREG, addr, 2);
}


FctERR NONNULL__ TCS3400_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr)
{
	uint8_t	WREG[2];
	FctERR	err;

	if (addr > TCS3400__BDATAH)		{ return ERROR_RANGE; }		// Unknown register

	err = TCS3400_Read(pSlave, WREG, addr, 2);
	if (err)	{ return err; }

	*data = MAKEWORD(WREG[0], WREG[1]);
	return ERROR_OK;
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_TCS3400 for component to be enabled!"
#endif
#endif
/****************************************************************/

