/*!\file MCP9808_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief MCP9808 Driver procedures
** \details MCP9808: +/-0.5C Maximum Accuracy Digital Temperature Sensor
**/
/****************************************************************/
#include "MCP9808.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MCP9808)
/****************************************************************/
// std libs
#include <math.h>
/****************************************************************/


MCP9808_t MCP9808[I2C_MCP9808_NB] = { 0 };

static const uint8_t MCP9808_conv_time[4] = { 30U, 65U, 130U, 250U };				//!< Conversion times for MCP9808
static const float MCP9808_resolution_steps[4] = { 0.5f, 0.25f, 0.125f, 0.0625f };	//!< Resolution steps for MCP9808


/****************************************************************/


__WEAK FctERR NONNULL__ MCP9808_Init_Sequence(MCP9808_t * const pCpnt)
{
	FctERR err;

	pCpnt->cfg.Resolution = MCP9808__RES_0_125;

	// get ID & check against values for MCP9808
	err = MCP9808_Get_ManufacturerID(pCpnt, &pCpnt->cfg.Manufacturer_Id);
	if (err != ERROR_OK)	{ return err; }
	err = MCP9808_Get_ChipID(pCpnt, &pCpnt->cfg.Device_Id);
	if (err != ERROR_OK)	{ return err; }

	if (pCpnt->cfg.Device_Id != MCP9808_CHIP_ID)				{ return ERROR_COMMON; }	// Unknown device
	if (pCpnt->cfg.Manufacturer_Id != MCP9808_MANUFACTURER_ID)	{ return ERROR_COMMON; }	// Unknown device

/*	err = MCP9808_Set_AlertTemp(pCpnt, 30.0f, MCP9808__ALERT_HIGH);
	if (err != ERROR_OK)	{ return err; }
	err = MCP9808_Set_AlertTemp(pCpnt, 32.0f, MCP9808__ALERT_LOW);
	if (err != ERROR_OK)	{ return err; }
	err = MCP9808_Set_AlertTemp(pCpnt, 32.5f, MCP9808__ALERT_CRIT);
	if (err != ERROR_OK)	{ return err; }

	// Checking if Set Alert works properly
	err = MCP9808_Get_AlertTemp(pCpnt, 0, MCP9808__ALERT_HIGH);
	if (err != ERROR_OK)	{ return err; }
	err = MCP9808_Get_AlertTemp(pCpnt, 0, MCP9808__ALERT_LOW);
	if (err != ERROR_OK)	{ return err; }
	err = MCP9808_Get_AlertTemp(pCpnt, 0, MCP9808__ALERT_CRIT);
	if (err != ERROR_OK)	{ return err; }
*/
	return MCP9808_Set_Resolution(pCpnt, pCpnt->cfg.Resolution);
}


/****************************************************************/


FctERR NONNULL__ MCP9808_Set_AlertTemp(MCP9808_t * const pCpnt, const float temp, const MCP9808_alert alt)
{
	uMCP9808_REG__TEMP_LIM	ALT = { 0 };
	float *					alert = &pCpnt->cfg.HighAlert + alt;
	FctERR					err;

	if (alt > MCP9808__ALERT_CRIT)	{ return ERROR_VALUE; }	// Unknown alert
	if (fabs(temp) >= 256.0f)		{ return ERROR_RANGE; }	// Temperature too low/high

	if (temp < 0.0f)	{ ALT.Bits.Sign = true; }
	ALT.Bits.Integer = (uint8_t) temp;
	ALT.Bits.Decimal = (uint8_t) ((temp - (uint8_t) temp) / 0.25f);

	// Temperature alerts shall be set only when in shutdown mode
	err = MCP9808_Shutdown(pCpnt, true);
	if (err != ERROR_OK)	{ return err; }

	err = MCP9808_Write(pCpnt->cfg.slave_inst, &ALT.Word, MCP9808__ALERT_UPPER + alt, 1U);
	if (err != ERROR_OK)	{ return err; }

	*alert = temp;

	return ERROR_OK;
}


FctERR NONNULLX__(1) MCP9808_Get_AlertTemp(MCP9808_t * const pCpnt, float * temp, MCP9808_alert alt)
{
	uMCP9808_REG__TEMP_LIM	ALT;
	float					tmp;
	float *					alert = &pCpnt->cfg.HighAlert + alt;
	FctERR					err;

	if (alt > MCP9808__ALERT_CRIT)	{ return ERROR_VALUE; }	// Unknown alert

	err = MCP9808_Read(pCpnt->cfg.slave_inst, &ALT.Word, MCP9808__ALERT_UPPER + alt, 1U);
	if (err != ERROR_OK)	{ return err; }

	tmp = (float) ALT.Bits.Integer;
	tmp += (ALT.Bits.Decimal * 0.25f);
	if (ALT.Bits.Sign)	{ tmp = -tmp; }

	*alert = tmp;

	if (temp)	{ *temp = tmp; }

	return ERROR_OK;
}


FctERR NONNULLX__(1) MCP9808_Get_Temperature(MCP9808_t * const pCpnt, float * temp)
{
	uMCP9808_REG__TEMP_AMB	TEMP;
	float					tmp;
	FctERR					err;

	err = MCP9808_Get_Temperature_Raw(pCpnt, &TEMP.Word);
	if (err != ERROR_OK)	{ return err; }

	tmp = (float) TEMP.Bits.Integer;
	tmp += ((TEMP.Bits.Decimal >> (3U - pCpnt->cfg.Resolution)) * MCP9808_resolution_steps[pCpnt->cfg.Resolution]);
	if (TEMP.Bits.Sign)	{ tmp = -tmp; }
	pCpnt->Temperature = tmp;

	pCpnt->TUpper = TEMP.Bits.VsTHigh;
	pCpnt->TLower = TEMP.Bits.VsTLow;
	pCpnt->TCrit = TEMP.Bits.VsTCrit;

	if (temp)	{ *temp = pCpnt->Temperature; }

	return ERROR_OK;
}


__WEAK FctERR NONNULL__ MCP9808_handler(MCP9808_t * const pCpnt)
{
	FctERR err = ERROR_OK;

	if ((!pCpnt->NewData) && TPSSUP_MS(pCpnt->hLast, MCP9808_conv_time[pCpnt->cfg.Resolution]))	{ pCpnt->NewData = true; }

	if (pCpnt->NewData)
	{
		err = MCP9808_Get_Temperature(pCpnt, 0);
		if (err != ERROR_OK)	{ goto ret; }

		pCpnt->NewData = false;
		pCpnt->hLast = HAL_GetTick();

		#if defined(VERBOSE)
			const uint8_t idx = pCpnt - MCP9808;
			printf("MCP9808 id%d: Temperature %d.%03ld°C\r\n", idx, (int16_t) pCpnt->Temperature, get_fp_dec(pCpnt->Temperature, 3U));
		#endif
	}

	ret:
	return err;
}


__WEAK FctERR NONNULL__ MCP9808_handler_it(MCP9808_t * const pCpnt)
{
	FctERR err = ERROR_OK;

	if (MCP9808_Alert_GPIO_Get(pCpnt))
	{
		pCpnt->NewData = true;
		err = MCP9808_handler(pCpnt);
	}

	return err;
}


FctERR MCP9808_handler_all(void)
{
	FctERR err = ERROR_OK;

	for (MCP9808_t * pCpnt = MCP9808 ; pCpnt < &MCP9808[I2C_MCP9808_NB] ; pCpnt++)
	{
		err |= MCP9808_handler(pCpnt);
	}

	return err;
}

FctERR MCP9808_handler_it_all(void)
{
	FctERR err = ERROR_OK;

	for (MCP9808_t * pCpnt = MCP9808 ; pCpnt < &MCP9808[I2C_MCP9808_NB] ; pCpnt++)
	{
		err |= MCP9808_handler_it(pCpnt);
	}

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
