/*!\file MCP9808.c
** \author SMFSW
** \version v0.2
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief MCP9808 Driver code
** \details MCP9808: +/-0.5C Maximum Accuracy Digital Temperature Sensor
**/
/****************************************************************/
#include "MCP9808.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MCP9808)
/****************************************************************/


I2C_slave MCP9808_hal = { { pNull, I2C_ADDR(MCP9808_BASE_ADDR), I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, false };


/****************************************************************/


FctERR MCP9808_Init(void)
{
	I2C_slave_init(&MCP9808_hal, I2C_MCP9808, MCP9808_BASE_ADDR, I2C_slave_timeout);
	return MCP9808_Init_Sequence();
}


/****************************************************************/


FctERR MCP9808_Write(uint16_t * data, uint16_t addr, uint16_t nb)
{
	uint8_t DATA[2];

	if (!data)									{ return ERR_MEMORY; }		// Null pointer
	if (addr > MCP9808__RESOLUTION)				{ return ERR_RANGE; }		// Unknown register
	if ((addr + nb - 1) > MCP9808__RESOLUTION)	{ return ERR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&MCP9808_hal, true);

	if (addr == MCP9808__RESOLUTION)
	{
		DATA[0] = (uint8_t) *data;

		MCP9808_hal.status = HAL_I2C_Mem_Write(MCP9808_hal.cfg.inst, MCP9808_hal.cfg.addr, MCP9808__RESOLUTION, MCP9808_hal.cfg.mem_size, DATA, 1, MCP9808_hal.cfg.timeout);
	}
	else
	{
		for (int i = 0; i < nb ; i++)
		{
			DATA[0] = data[i] / 0x100;
			DATA[1] = (uint8_t) data[i];

			MCP9808_hal.status = HAL_I2C_Mem_Write(MCP9808_hal.cfg.inst, MCP9808_hal.cfg.addr, addr, MCP9808_hal.cfg.mem_size, DATA, 2, MCP9808_hal.cfg.timeout);
			if (MCP9808_hal.status != HAL_OK)	{ break; }
		}
	}

	I2C_set_busy(&MCP9808_hal, false);
	return HALERRtoFCTERR(MCP9808_hal.status);
}


FctERR MCP9808_Read(uint16_t * data, uint16_t addr, uint16_t nb)
{
	uint8_t DATA[2];

	if (!data)									{ return ERR_MEMORY; }		// Null pointer
	if (addr > MCP9808__RESOLUTION)				{ return ERR_RANGE; }		// Unknown register
	if ((addr + nb - 1) > MCP9808__RESOLUTION)	{ return ERR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&MCP9808_hal, true);

	if (addr == MCP9808__RESOLUTION)
	{
		MCP9808_hal.status = HAL_I2C_Mem_Read(MCP9808_hal.cfg.inst, MCP9808_hal.cfg.addr, MCP9808__RESOLUTION, MCP9808_hal.cfg.mem_size, DATA, 1, MCP9808_hal.cfg.timeout);

		if (MCP9808_hal.status == HAL_OK)	{ *data = DATA[0]; }
	}
	else
	{
		for (int i = 0; i < nb ; i++)
		{
			MCP9808_hal.status = HAL_I2C_Mem_Read(MCP9808_hal.cfg.inst, MCP9808_hal.cfg.addr, addr, MCP9808_hal.cfg.mem_size, DATA, 2, MCP9808_hal.cfg.timeout);
			if (MCP9808_hal.status != HAL_OK)	{ break; }

			data[i] = (DATA[0] * 0x100) + DATA[1];
		}
	}

	I2C_set_busy(&MCP9808_hal, false);
	return HALERRtoFCTERR(MCP9808_hal.status);
}


/****************************************************************/
#else
#warning "You have to define I2C_MCP9808 in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/
