/*!\file MCP9808.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief MCP9808 Driver
** \details MCP9808: +/-0.5C Maximum Accuracy Digital Temperature Sensor
**/
/****************************************************************/
#include "MCP9808.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MCP9808)
/****************************************************************/


static const I2C_slave_t MCP9808_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };

static I2C_slave_t MCP9808_hal[I2C_MCP9808_NB];		//!< MCP9808 Slave structure


/****************************************************************/


FctERR NONNULL__ MCP9808_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	FctERR err;

	assert_param(IS_I2C_PERIPHERAL(MCP9808, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(MCP9808, idx);

	I2C_slave_init(&MCP9808_hal[idx], hi2c, devAddress, MCP9808_hal[idx].cfg.timeout);

	err = MCP9808_Init_Sequence(&MCP9808[idx]);

	if (err != ERROR_OK)	{ I2C_set_enable(&MCP9808_hal[idx], false); }

	return err;
}

FctERR MCP9808_Init_Single(void) {
	return MCP9808_Init(0, I2C_MCP9808, MCP9808_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ MCP9808_Write(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR	err = ERROR_OK;
	uint8_t DATA[2];

	if (!I2C_is_enabled(pSlave))				{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > MCP9808__RESOLUTION + 1U)	{ err = ERROR_OVERFLOW; }	// More bytes than registers
	if (err != ERROR_OK)						{ goto ret; }

	I2C_set_busy(pSlave, true);

	if (addr == MCP9808__RESOLUTION)
	{
		DATA[0] = (uint8_t) *data;

		pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, MCP9808__RESOLUTION, pSlave->cfg.mem_size, DATA, 1U, pSlave->cfg.timeout);
	}
	else
	{
		for (uintCPU_t i = 0 ; i < nb ; i++)
		{
			DATA[0] = HIBYTE(data[i]);
			DATA[1] = LOBYTE(data[i]);

			pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, DATA, 2U, pSlave->cfg.timeout);
			if (pSlave->status != HAL_OK)	{ break; }
		}
	}

	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


FctERR NONNULL__ MCP9808_Read(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR	err = ERROR_OK;
	uint8_t DATA[2];

	if (!I2C_is_enabled(pSlave))				{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > MCP9808__RESOLUTION + 1U)	{ err = ERROR_OVERFLOW; }	// More bytes than registers
	if (err != ERROR_OK)						{ goto ret; }

	I2C_set_busy(pSlave, true);

	if (addr == MCP9808__RESOLUTION)
	{
		pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, MCP9808__RESOLUTION, pSlave->cfg.mem_size, DATA, 1U, pSlave->cfg.timeout);

		if (pSlave->status == HAL_OK)	{ *data = DATA[0]; }
	}
	else
	{
		for (uintCPU_t i = 0 ; i < nb ; i++)
		{
			pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, DATA, 2U, pSlave->cfg.timeout);
			if (pSlave->status != HAL_OK)	{ break; }

			data[i] = MAKEWORD(DATA[1], DATA[0]);
		}
	}

	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
