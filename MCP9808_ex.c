/*!\file MCP9808_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief MCP9808 Driver extensions
** \details MCP9808: +/-0.5C Maximum Accuracy Digital Temperature Sensor
**/
/****************************************************************/
#include "MCP9808.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MCP9808)
/****************************************************************/


FctERR MCP9808_Write_Config(const uint16_t cfg)
{
	uMCP9808_REG__CFG	CFG;
	FctERR				err;

	CFG.Word = cfg;
	CFG.Bits.SHDN = true;	// Writing config when in shutdown mode (yet trying to write cfg when shutting down)

	err = MCP9808_Write(&CFG.Word, MCP9808__CONFIGURATION, 1);
	if (err)	{ return err; }

	CFG.Word = cfg;
	CFG.Bits.SHDN = false;	// Re-enable continuous conversion when sending config

	err = MCP9808_Write(&CFG.Word, MCP9808__CONFIGURATION, 1);
	if (err)	{ return err; }

	return ERROR_OK;
}


FctERR MCP9808_Shutdown(const bool shutdown)
{
	uMCP9808_REG__CFG	CFG;
	FctERR				err;

	err = MCP9808_Read_Config(&CFG.Word);
	if (err)	{ return err; }

	CFG.Bits.SHDN = shutdown;
	return MCP9808_Write(&CFG.Word, MCP9808__CONFIGURATION, 1);
}


FctERR MCP9808_Set_AlertHysteresis(const MCP9808_hyst hys)
{
	uMCP9808_REG__CFG	CFG;
	FctERR				err;

	err = MCP9808_Read_Config(&CFG.Word);
	if (err)	{ return err; }

	CFG.Bits.Hysteresis = hys;
	return MCP9808_Write_Config(CFG.Word);
}


FctERR MCP9808_Set_AlertType(const bool comparator)
{
	uMCP9808_REG__CFG	CFG;
	FctERR				err;

	err = MCP9808_Read_Config(&CFG.Word);
	if (err)	{ return err; }

	CFG.Bits.AlertMod = !comparator;
	err = MCP9808_Write_Config(CFG.Word);
	if (err)	{ return err; }


	return ERROR_OK;
}


FctERR MCP9808_Set_AlertOutput(const bool en, const bool alt)
{
	uMCP9808_REG__CFG	CFG;
	FctERR				err;

	err = MCP9808_Read_Config(&CFG.Word);
	if (err)	{ return err; }

	CFG.Bits.AlertCnt = en;
	CFG.Bits.AlertStat = en;
	CFG.Bits.AlertSel = alt;
	return MCP9808_Write_Config(CFG.Word);
}


FctERR MCP9808_Set_AlertLock(const MCP9808_alert alt, const bool lock)
{
	uMCP9808_REG__CFG	CFG;
	FctERR				err;

	if (alt > MCP9808__ALERT_CRIT)	{ return ERROR_VALUE; }	// Unknown alert

	err = MCP9808_Read_Config(&CFG.Word);
	if (err)	{ return err; }

	switch (alt) {
		case MCP9808__ALERT_CRIT:	CFG.Bits.CrtLoc = lock; break;
		default:					CFG.Bits.WinLoc = lock; break;
	}
	return MCP9808_Write_Config(CFG.Word);
}


FctERR MCP9808_Set_Resolution(const MCP9808_res res)
{
	uint16_t	RES = res;
	FctERR		err;

	if (res > MCP9808__RES_0_0625)	{ return ERROR_VALUE; }	// Unknown resolution

	err = MCP9808_Write(&RES, MCP9808__RESOLUTION, 1);
	if (err)	{ return err; }

	MCP9808.NewData = false;
	MCP9808.hLast = HAL_GetTick();

	return ERROR_OK;
}


FctERR MCP9808_Get_Resolution(MCP9808_res * res)
{
	uint16_t	RES;
	FctERR		err;

	err = MCP9808_Read(&RES, MCP9808__RESOLUTION, 1);
	if (err)	{ return err; }

	*res = RES;
	return ERROR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
