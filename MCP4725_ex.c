/*!\file MCP4725_ex.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief MCP4725 Driver extensions code
** \details MCP4725: 12-Bit Digital-to-Analog Converter with EEPROM Memory
**/
/****************************************************************/
#include "MCP4725.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MCP4725)
/****************************************************************/


FctERR MCP4725_Write_Command(uint16_t val)
{
	uint8_t	CMD[3];

	if (MCP4725.cfg.Mode > MCP4725__WRITE_DAC_EEP)	{ return ERROR_CMD; }	// Unknown command

	if (MCP4725.cfg.Mode == MCP4725__FAST_MODE)
	{
		CMD[0] = LSHIFT(MCP4725.cfg.PowerDown, 4) + (RSHIFT(val, 8) & 0x0F);
		CMD[1] = (uint8_t) val;
		return MCP4725_Write(CMD, 2);
	}
	else
	{
		CMD[0] = LSHIFT(MCP4725.cfg.Mode, 5) + (MCP4725.cfg.PowerDown << 1);
		CMD[1] = RSHIFT(val, 4);
		CMD[2] = LSHIFT(val & 0x0F, 4);
		return MCP4725_Write(CMD, 3);
	}
}


FctERR MCP4725_Read_DAC(uint16_t * val)
{
	uint8_t	REG[3];
	FctERR	err;

	err = MCP4725_Read(REG, 3);
	if (err)	{ return err; }

	*val = RSHIFT(REG[2], 4) | LSHIFT(REG[1], 4);
	return ERROR_OK;
}


FctERR MCP4725_Read_State(bool * state)
{
	uint8_t	REG;
	FctERR	err;

	err = MCP4725_Read(&REG, 1);
	if (err)	{ return err; }

	*state = (REG & 0x80) ? true : false;
	return ERROR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
