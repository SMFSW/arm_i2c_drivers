/*!\file MCP4725_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief MCP4725 Driver procedures
** \details MCP4725: 12-Bit Digital-to-Analog Converter with EEPROM Memory
**/
/****************************************************************/
#include "MCP4725.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MCP4725)
/****************************************************************/


MCP4725_t MCP4725[I2C_MCP4725_NB];


/****************************************************************/


__WEAK FctERR NONNULL__ MCP4725_Init_Sequence(MCP4725_t * pCpnt)
{
	FctERR err = ERROR_OK;

	err = MCP4725_Set_VRef(pCpnt, 3.3f);
	if (err)	{ return err; }
	err = MCP4725_Set_Mode(pCpnt, MCP4725__FAST_MODE);
	if (err)	{ return err; }
	return MCP4725_Set_PowerDown(pCpnt, MCP4725__PWR_NORMAL);
}


/****************************************************************/


FctERR NONNULL__ MCP4725_Set_Mode(MCP4725_t * pCpnt, const MCP4725_cmd mode)
{
	if (mode > MCP4725__WRITE_DAC_EEP)		{ return ERROR_VALUE; }

	pCpnt->cfg.Mode = mode;
	return ERROR_OK;
}


FctERR NONNULL__ MCP4725_Set_PowerDown(MCP4725_t * pCpnt, const MCP4725_pd pd)
{
	if (pd > MCP4725__PWR_DOWN_500K)		{ return ERROR_VALUE; }

	pCpnt->cfg.PowerDown = pd;
	return MCP4725_Write_Command(pCpnt, pCpnt->DAC_4096);
}


FctERR NONNULL__ MCP4725_Set_VRef(MCP4725_t * pCpnt, float vref)
{
	if ((vref < 2.7f) || (vref > 5.5f))		{ return ERROR_VALUE; }

	pCpnt->cfg.VRef = vref;
	return ERROR_OK;
}


FctERR NONNULL__ MCP4725_Set_Val(MCP4725_t * pCpnt, const uint16_t val)
{
	FctERR err;

	if (pCpnt->cfg.VRef == 0.0f)	{ return ERROR_COMMON; }	// VRef not initialized
	if (val > 4095)					{ return ERROR_VALUE; }		// Value over the limit

	err = MCP4725_Write_Command(pCpnt, val);
	if (err)	{ return err; }

	pCpnt->DAC_4096 = val;
	pCpnt->DAC_Volts = (pCpnt->cfg.VRef * val) / 4096.0f;

	return ERROR_OK;
}

FctERR NONNULL__ MCP4725_Set_Volts(MCP4725_t * pCpnt, const float volts)
{
	uint16_t	VAL;
	FctERR		err;

	if (pCpnt->cfg.VRef == 0.0f)	{ return ERROR_COMMON; }	// VRef not initialized
	if (volts > pCpnt->cfg.VRef)	{ return ERROR_RANGE; }		// Voltage is outside range

	VAL = (volts * 4096.0f) / pCpnt->cfg.VRef;

	err = MCP4725_Write_Command(pCpnt, VAL);
	if (err)	{ return err; }

	pCpnt->DAC_4096 = VAL;
	pCpnt->DAC_Volts = volts;

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
