/*!\file MCP4725_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief MCP4725 Driver extensions
** \details MCP4725: 12-Bit Digital-to-Analog Converter with EEPROM Memory
**/
/****************************************************************/
#include "MCP4725.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MCP4725)
/****************************************************************/


FctERR NONNULL__ MCP4725_Write_Command(MCP4725_t * const pCpnt, const uint16_t val)
{
	uint8_t	CMD[3];

	if (pCpnt->cfg.Mode > MCP4725__WRITE_DAC_EEP)	{ return ERROR_CMD; }	// Unknown command

	if (pCpnt->cfg.Mode == MCP4725__FAST_MODE)
	{
		CMD[0] = LSHIFT(pCpnt->cfg.PowerDown, 4U) + (RSHIFT(val, 8U) & 0x0FU);
		CMD[1] = (uint8_t) val;
		return MCP4725_Write(pCpnt->cfg.slave_inst, CMD, 2);
	}
	else
	{
		CMD[0] = LSHIFT(pCpnt->cfg.Mode, 5U) + LSHIFT(pCpnt->cfg.PowerDown, 1U);
		CMD[1] = RSHIFT(val, 4U);
		CMD[2] = LSHIFT(val & 0x0FU, 4U);
		return MCP4725_Write(pCpnt->cfg.slave_inst, CMD, 3U);
	}
}


FctERR NONNULL__ MCP4725_Read_DAC(MCP4725_t * const pCpnt, uint16_t * val)
{
	uint8_t	REG[3];
	FctERR	err;

	err = MCP4725_Read(pCpnt->cfg.slave_inst, REG, 3U);
	if (err != ERROR_OK)	{ return err; }

	*val = RSHIFT(REG[2], 4U) | LSHIFT(REG[1], 4U);
	return ERROR_OK;
}


FctERR NONNULL__ MCP4725_Read_State(MCP4725_t * const pCpnt, bool * state)
{
	uint8_t	REG;
	FctERR	err;

	err = MCP4725_Read(pCpnt->cfg.slave_inst, &REG, 1U);
	if (err != ERROR_OK)	{ return err; }

	*state = (REG & 0x80U) ? true : false;
	return ERROR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
