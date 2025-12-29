/*!\file MCP4725_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief MCP4725 Driver procedures
** \details MCP4725: 12-Bit Digital-to-Analog Converter with EEPROM Memory
**/
/****************************************************************/
#include "MCP4725.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MCP4725)
/****************************************************************/


MCP4725_t MCP4725[I2C_MCP4725_NB] = { 0 };


/****************************************************************/


__WEAK FctERR NONNULL__ MCP4725_Init_Sequence(MCP4725_t * const pCpnt)
{
	FctERR err;

	err = MCP4725_Set_VRef(pCpnt, 3.3f);
	if (err != ERROR_OK)	{ goto ret; }

	err = MCP4725_Set_Mode(pCpnt, MCP4725__FAST_MODE);
	if (err != ERROR_OK)	{ goto ret; }

	err = MCP4725_Set_PowerDown(pCpnt, MCP4725__PWR_NORMAL);

	ret:
	return err;
}


/****************************************************************/


FctERR NONNULL__ MCP4725_Set_Mode(MCP4725_t * const pCpnt, const MCP4725_cmd mode)
{
	FctERR err = ERROR_OK;

	if (mode > MCP4725__WRITE_DAC_EEP)		{ err = ERROR_VALUE; }
	else									{ pCpnt->cfg.Mode = mode; }

	return err;
}


FctERR NONNULL__ MCP4725_Set_PowerDown(MCP4725_t * const pCpnt, const MCP4725_pd pd)
{
	FctERR err = ERROR_OK;

	if (pd > MCP4725__PWR_DOWN_500K)		{ err = ERROR_VALUE; }
	else
	{
		pCpnt->cfg.PowerDown = pd;
		err = MCP4725_Write_Command(pCpnt, pCpnt->DAC_4096);
	}

	return err;
}


FctERR NONNULL__ MCP4725_Set_VRef(MCP4725_t * const pCpnt, float vref)
{
	FctERR err = ERROR_OK;

	if ((vref < 2.7f) || (vref > 5.5f))		{ err = ERROR_VALUE; }
	else									{ pCpnt->cfg.VRef = vref; }

	return err;
}


FctERR NONNULL__ MCP4725_Set_Val(MCP4725_t * const pCpnt, const uint16_t val)
{
	FctERR err = ERROR_OK;

	if (pCpnt->cfg.VRef == 0.0f)	{ err = ERROR_COMMON; }	// VRef not initialized
	if (val > 4095U)				{ err = ERROR_VALUE; }	// Value over the limit
	if (err != ERROR_OK)			{ goto ret; }

	err = MCP4725_Write_Command(pCpnt, val);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->DAC_4096 = val;
	pCpnt->DAC_Volts = (pCpnt->cfg.VRef * val) / 4096.0f;

	ret:
	return err;
}

FctERR NONNULL__ MCP4725_Set_Volts(MCP4725_t * const pCpnt, const float volts)
{
	FctERR err = ERROR_OK;

	if (pCpnt->cfg.VRef == 0.0f)	{ err = ERROR_COMMON; }	// VRef not initialized
	if (volts > pCpnt->cfg.VRef)	{ err = ERROR_RANGE; }	// Voltage is outside range
	if (err != ERROR_OK)			{ goto ret; }

	const uint16_t VAL = (volts * 4096.0f) / pCpnt->cfg.VRef;

	err = MCP4725_Write_Command(pCpnt, VAL);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->DAC_4096 = VAL;
	pCpnt->DAC_Volts = volts;

	ret:
	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
