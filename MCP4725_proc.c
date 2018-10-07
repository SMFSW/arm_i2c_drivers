/*!\file MCP4725_t.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief MCP4725 Driver procedures
** \details MCP4725: 12-Bit Digital-to-Analog Converter with EEPROM Memory
**/
/****************************************************************/
#include "MCP4725.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MCP4725)
/****************************************************************/


MCP4725_t MCP4725 = { 0, 0.0f, { &MCP4725_hal, MCP4725__FAST_MODE, MCP4725__PWR_NORMAL, 0.0f } };


/****************************************************************/


__WEAK FctERR MCP4725_Init_Sequence(void)
{
	FctERR err = ERROR_OK;

	err = MCP4725_Set_VRef(3.3f);
	if (err)	{ return err; }
	err = MCP4725_Set_Mode(MCP4725__FAST_MODE);
	if (err)	{ return err; }
	return MCP4725_Set_PowerDown(MCP4725__PWR_NORMAL);
}


/****************************************************************/


FctERR MCP4725_Set_Mode(const MCP4725_cmd mode)
{
	if (mode > MCP4725__WRITE_DAC_EEP)		{ return ERROR_VALUE; }

	MCP4725.cfg.Mode = mode;
	return ERROR_OK;
}


FctERR MCP4725_Set_PowerDown(const MCP4725_pd pd)
{
	if (pd > MCP4725__PWR_DOWN_500K)		{ return ERROR_VALUE; }

	MCP4725.cfg.PowerDown = pd;
	return MCP4725_Write_Command(MCP4725.DAC_4096);
}


FctERR MCP4725_Set_VRef(float vref)
{
	if ((vref < 2.7f) || (vref > 5.5f))		{ return ERROR_VALUE; }

	MCP4725.cfg.VRef = vref;
	return ERROR_OK;
}


FctERR MCP4725_Set_Val(const uint16_t val)
{
	FctERR err;

	if (MCP4725.cfg.VRef == 0.0f)	{ return ERROR_COMMON; }	// VRef not initialized
	if (val > 4095)					{ return ERROR_VALUE; }	// Value over the limit

	err = MCP4725_Write_Command(val);
	if (err)	{ return err; }

	MCP4725.DAC_4096 = val;
	MCP4725.DAC_Volts = (MCP4725.cfg.VRef * val) / 4096.0f;

	return ERROR_OK;
}

FctERR MCP4725_Set_Volts(const float volts)
{
	uint16_t	VAL;
	FctERR		err;

	if (MCP4725.cfg.VRef == 0.0f)	{ return ERROR_COMMON; }	// VRef not initialized
	if (volts > MCP4725.cfg.VRef)	{ return ERROR_RANGE; }	// Voltage is outside range

	VAL = (volts * 4096.0f) / MCP4725.cfg.VRef;

	err = MCP4725_Write_Command(VAL);
	if (err)	{ return err; }

	MCP4725.DAC_4096 = VAL;
	MCP4725.DAC_Volts = volts;

	return err;
}



/****************************************************************/
#endif
#endif
/****************************************************************/
