/*!\file MCP9808_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief MCP9808 Driver extensions
** \details MCP9808: +/-0.5C Maximum Accuracy Digital Temperature Sensor
**/
/****************************************************************/
#include "MCP9808.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MCP9808)
/****************************************************************/


FctERR NONNULL__ MCP9808_Write_Config(MCP9808_t * const pCpnt, const uint16_t cfg)
{
	uMCP9808_REG__CFG	CFG;
	FctERR				err;

	CFG.Word = cfg;
	CFG.Bits.SHDN = true;	// Writing config when in shutdown mode (yet trying to write cfg when shutting down)

	err = MCP9808_Write(pCpnt->cfg.slave_inst, &CFG.Word, MCP9808__CONFIGURATION, 1);
	if (err != ERROR_OK)	{ return err; }

	CFG.Word = cfg;
	CFG.Bits.SHDN = false;	// Re-enable continuous conversion when sending config

	err = MCP9808_Write(pCpnt->cfg.slave_inst, &CFG.Word, MCP9808__CONFIGURATION, 1);
	if (err != ERROR_OK)	{ return err; }

	return ERROR_OK;
}


FctERR NONNULL__ MCP9808_Shutdown(MCP9808_t * const pCpnt, const bool shutdown)
{
	uMCP9808_REG__CFG	CFG;
	FctERR				err;

	err = MCP9808_Read_Config(pCpnt, &CFG.Word);
	if (err != ERROR_OK)	{ return err; }

	CFG.Bits.SHDN = shutdown;
	return MCP9808_Write(pCpnt->cfg.slave_inst, &CFG.Word, MCP9808__CONFIGURATION, 1);
}


FctERR NONNULL__ MCP9808_Set_AlertHysteresis(MCP9808_t * const pCpnt, const MCP9808_hyst hys)
{
	uMCP9808_REG__CFG	CFG;
	FctERR				err;

	err = MCP9808_Read_Config(pCpnt, &CFG.Word);
	if (err != ERROR_OK)	{ return err; }

	CFG.Bits.Hysteresis = hys;
	return MCP9808_Write_Config(pCpnt, CFG.Word);
}


FctERR NONNULL__ MCP9808_Set_AlertType(MCP9808_t * const pCpnt, const bool comparator)
{
	uMCP9808_REG__CFG	CFG;
	FctERR				err;

	err = MCP9808_Read_Config(pCpnt, &CFG.Word);
	if (err != ERROR_OK)	{ return err; }

	CFG.Bits.AlertMod = !comparator;
	err = MCP9808_Write_Config(pCpnt, CFG.Word);
	if (err != ERROR_OK)	{ return err; }


	return ERROR_OK;
}


FctERR NONNULL__ MCP9808_Set_AlertOutput(MCP9808_t * const pCpnt, const bool en, const bool alt)
{
	uMCP9808_REG__CFG	CFG;
	FctERR				err;

	err = MCP9808_Read_Config(pCpnt, &CFG.Word);
	if (err != ERROR_OK)	{ return err; }

	CFG.Bits.AlertCnt = en;
	CFG.Bits.AlertStat = en;
	CFG.Bits.AlertSel = alt;
	return MCP9808_Write_Config(pCpnt, CFG.Word);
}


FctERR NONNULL__ MCP9808_Set_AlertLock(MCP9808_t * const pCpnt, const MCP9808_alert alt, const bool lock)
{
	uMCP9808_REG__CFG	CFG;
	FctERR				err;

	if (alt > MCP9808__ALERT_CRIT)	{ return ERROR_VALUE; }	// Unknown alert

	err = MCP9808_Read_Config(pCpnt, &CFG.Word);
	if (err != ERROR_OK)	{ return err; }

	switch (alt) {
		case MCP9808__ALERT_CRIT:	CFG.Bits.CrtLoc = lock; break;
		default:					CFG.Bits.WinLoc = lock; break;
	}
	return MCP9808_Write_Config(pCpnt, CFG.Word);
}


FctERR NONNULL__ MCP9808_Set_Resolution(MCP9808_t * const pCpnt, const MCP9808_res res)
{
	uint16_t	RES = res;
	FctERR		err;

	if (res > MCP9808__RES_0_0625)	{ return ERROR_VALUE; }	// Unknown resolution

	err = MCP9808_Write(pCpnt->cfg.slave_inst, &RES, MCP9808__RESOLUTION, 1);
	if (err != ERROR_OK)	{ return err; }

	pCpnt->NewData = false;
	pCpnt->hLast = HAL_GetTick();

	return ERROR_OK;
}


FctERR NONNULL__ MCP9808_Get_Resolution(MCP9808_t * const pCpnt, MCP9808_res * res)
{
	uint16_t	RES;
	FctERR		err;

	err = MCP9808_Read(pCpnt->cfg.slave_inst, &RES, MCP9808__RESOLUTION, 1);
	if (err != ERROR_OK)	{ return err; }

	*res = RES;
	return ERROR_OK;
}


/****************************************************************/


__WEAK void NONNULL__ MCP9808_Alert_GPIO_Init(MCP9808_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&pCpnt->cfg.Alert_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK bool NONNULL__ MCP9808_Alert_GPIO_Get(MCP9808_t * const pCpnt) {
	return I2C_peripheral_GPIO_get(&pCpnt->cfg.Alert_GPIO); }


/****************************************************************/
#endif
#endif
/****************************************************************/
