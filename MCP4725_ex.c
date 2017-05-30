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


extern MCP4725_proc MCP4725;


FctERR MCP4725_Write_Command(uint16_t val)
{
	uint8_t	CMD[3];

	if (MCP4725.cfg.Mode > MCP4725__WRITE_DAC_EEP)	{ return ERR_CMD; }	// Unknown command

	if (MCP4725.cfg.Mode == MCP4725__FAST_MODE)
	{
		CMD[0] = (MCP4725.cfg.PowerDown * 0x10) + (uint8_t) ((val / 0x100) & 0x0F);
		CMD[1] = (uint8_t) val;
		return MCP4725_Write(CMD, 2);
	}
	else
	{
		CMD[0] = (MCP4725.cfg.Mode * 0x20) + (MCP4725.cfg.PowerDown * 0x02);
		CMD[1] = (uint8_t) (val / 0x10);
		CMD[2] = ((uint8_t) val & 0x0F) * 0x10;
		return MCP4725_Write(CMD, 3);
	}
}


FctERR MCP4725_Read_DAC(uint16_t * val)
{
	uint8_t	REG[3];
	FctERR	err;

	err = MCP4725_Read(REG, 3);
	if (err)	{ return err; }

	*val = (REG[2] / 0x10) + (REG[1] * 0x100);
	return ERR_OK;
}


FctERR MCP4725_Read_State(bool * state)
{
	uint8_t	REG;
	FctERR	err;

	err = MCP4725_Read(&REG, 1);
	if (err)	{ return err; }

	*state = (REG & 0x80) ? true : false;
	return ERR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
