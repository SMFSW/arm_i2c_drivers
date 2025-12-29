/*!\file MB85RC256V.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief MB85RC256V FRAM Driver
** \details MB85RC256V: 256-Kbit (32K * 8) I2C Memory FRAM
** \note Legacy component: consider switching to I2CMEM component for new designs
** \note Compatibility (tested):
**				- MB85RC256V
**				- FM24CL64B
**/
/****************************************************************/
#include "MB85RC256V.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MB85RC256V)
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

	I2C_slave_init(&MB85RC256V_hal[idx], hi2c, devAddress, MB85RC256V_hal[idx].cfg.timeout);

	err = MB85RC256V_Init_Sequence(&MB85RC256V[idx]);

	if (err != ERROR_OK)	{ I2C_set_enable(&MB85RC256V_hal[idx], false); }

	return err;
}

FctERR MB85RC256V_Init_Single(void) {
	return MB85RC256V_Init(0, I2C_MB85RC256V, MB85RC256V_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ MB85RC256V_Write(MB85RC256V_t * const pCpnt, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR				err = ERROR_OK;
	I2C_slave_t * const pSlave = pCpnt->cfg.slave_inst;

	if (!I2C_is_enabled(pSlave))		{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > MB85RC256V_SIZE)	{ err = ERROR_OVERFLOW; }	// More bytes than registers
	if (err != ERROR_OK)				{ goto ret; }

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


FctERR NONNULL__ MB85RC256V_Read(MB85RC256V_t * const pCpnt, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR				err = ERROR_OK;
	I2C_slave_t * const pSlave = pCpnt->cfg.slave_inst;

	if (!I2C_is_enabled(pSlave))		{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > MB85RC256V_SIZE)	{ err = ERROR_OVERFLOW; }	// More bytes than registers
	if (err != ERROR_OK)				{ goto ret; }

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


FctERR NONNULL__ MB85RC256V_Read_ID(MB85RC256V_t * const pCpnt, uint8_t * data)
{
	FctERR				err = ERROR_OK;
	I2C_slave_t * const pSlave = pCpnt->cfg.slave_inst;

	if (!I2C_is_enabled(pSlave))	{ err = ERROR_DISABLED; }	// Peripheral disabled
	if (err != ERROR_OK)			{ goto ret; }

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, 0xF8, pSlave->cfg.addr, I2C_MEMADD_SIZE_8BIT, data, 3U, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}

/****************************************************************/
#endif
#endif
/****************************************************************/
