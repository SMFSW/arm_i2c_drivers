/*!\file MB85RC256V.c
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief MB85RC256V Driver
** \details MB85RC256V: 256-Kbit (32K * 8) I2C Memory FRAM
** \note	Compatibility (tested):
**				- MB85RC256V
**				- FM24CL64B
** \note	Compatibility:
**				- other components using same i2c protocol may be compatible
**/
/****************************************************************/
#include "MB85RC256V.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MB85RC256V)
/****************************************************************/
#if defined(I2C_FM24C)
#warning "MB85RC256V -> Defined along with FM24CxxB: use with caution, might have same I2C addresses if on same I2C bus!!!"
#endif
/****************************************************************/


static const I2C_slave_t MB85RC256V_defaults = { { pNull, 0, I2C_eep_slave_timeout, I2C_MEMADD_SIZE_16BIT, I2C_FMP }, 0, HAL_OK, true, false };

static I2C_slave_t MB85RC256V_hal[I2C_MB85RC256V_NB];	//!< MB85RC256V Slave structure

MB85RC256V_t MB85RC256V[I2C_MB85RC256V_NB] = { 0 };


/****************************************************************/


FctERR NONNULL__ MB85RC256V_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	FctERR err;

	assert_param(IS_I2C_PERIPHERAL(MB85RC256V, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(MB85RC256V, idx);

	err = I2C_slave_init(&MB85RC256V_hal[idx], hi2c, devAddress, MB85RC256V_hal[idx].cfg.timeout);
	if (!err)	{ err = MB85RC256V_Init_Sequence(&MB85RC256V[idx]); }

	if (err)	{ I2C_set_enable(&MB85RC256V_hal[idx], false); }

	return err;
}

FctERR MB85RC256V_Init_Single(void) {
	return MB85RC256V_Init(0, I2C_MB85RC256V, MB85RC256V_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ MB85RC256V_Write(MB85RC256V_t * const pCpnt, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	I2C_slave_t * const pSlave = pCpnt->cfg.slave_inst;

	if (!I2C_is_enabled(pSlave))		{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr >= MB85RC256V_SIZE)		{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > MB85RC256V_SIZE)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(pSlave, true);

	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);

	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ MB85RC256V_Read(MB85RC256V_t * const pCpnt, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	I2C_slave_t * const pSlave = pCpnt->cfg.slave_inst;

	if (!I2C_is_enabled(pSlave))		{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr >= MB85RC256V_SIZE)		{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > MB85RC256V_SIZE)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ MB85RC256V_Read_ID(MB85RC256V_t * const pCpnt, uint8_t * data)
{
	I2C_slave_t * const pSlave = pCpnt->cfg.slave_inst;

	if (!I2C_is_enabled(pSlave))		{ return ERROR_DISABLED; }	// Peripheral disabled

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, 0xF8, pSlave->cfg.addr, I2C_MEMADD_SIZE_8BIT, data, 3, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}

/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_MB85RC256V for component to be enabled!"
#endif
#endif
/****************************************************************/
