/*!\file MCP9808_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
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


MCP9808_t MCP9808 = { 0.0f, false, false, false, true, 0, { &MCP9808_hal, MCP9808__RES_0_125, 0.0f, 0.0f, 0.0f, 0, 0 } };

static uint8_t MCP9808_conv_time[4] = { 30, 65, 130, 250 };						//!< Conversion times for MCP9808
static float MCP9808_resolution_steps[4] = { 0.5f, 0.25f, 0.125f, 0.0625f };	//!< Resolution steps for MCP9808


/****************************************************************/


__WEAK FctERR MCP9808_Init_Sequence(void)
{
	FctERR err;

	// get ID & check against values for MCP9808
	err = MCP9808_Get_ManufacturerID(&MCP9808.cfg.Manufacturer_Id);
	if (err)	{ return err; }
	err = MCP9808_Get_ChipID(&MCP9808.cfg.Device_Id);
	if (err)	{ return err; }

	if (MCP9808.cfg.Device_Id != MCP9808_CHIP_ID)				{ return ERROR_COMMON; }	// Unknown device
	if (MCP9808.cfg.Manufacturer_Id != MCP9808_MANUFACTURER_ID)	{ return ERROR_COMMON; }	// Unknown device

/*	err = MCP9808_Set_AlertTemp(30.0f, MCP9808__ALERT_HIGH);
	if (err)	{ return err; }
	err = MCP9808_Set_AlertTemp(32.0f, MCP9808__ALERT_LOW);
	if (err)	{ return err; }
	err = MCP9808_Set_AlertTemp(32.5f, MCP9808__ALERT_CRIT);
	if (err)	{ return err; }

	// Checking if Set Alert works properly
	err = MCP9808_Get_AlertTemp(0, MCP9808__ALERT_HIGH);
	if (err)	{ return err; }
	err = MCP9808_Get_AlertTemp(0, MCP9808__ALERT_LOW);
	if (err)	{ return err; }
	err = MCP9808_Get_AlertTemp(0, MCP9808__ALERT_CRIT);
	if (err)	{ return err; }
*/
	return MCP9808_Set_Resolution(MCP9808.cfg.Resolution);
}


/****************************************************************/


FctERR MCP9808_Set_AlertTemp(const float temp, const MCP9808_alert alt)
{
	uMCP9808_REG__TEMP_LIM	ALT;
	float *					alert = &MCP9808.cfg.HighAlert + alt;
	FctERR					err;

	if (alt > MCP9808__ALERT_CRIT)	{ return ERROR_VALUE; }	// Unknown alert
	if (fabs(temp) >= 256.0f)		{ return ERROR_RANGE; }	// Temperature too low/high

	ALT.Word = 0;
	if (temp < 0.0f)	{ ALT.Bits.Sign = true; }
	ALT.Bits.Integer = (uint8_t) temp;
	ALT.Bits.Decimal = (uint8_t) ((temp - (uint8_t) temp) / 0.25f);

	// Temperature alerts shall be set only when in shutdown mode
	err = MCP9808_Shutdown(true);
	if (err)	{ return err; }

	err = MCP9808_Write(&ALT.Word, MCP9808__ALERT_UPPER + alt, 1);
	if (err)	{ return err; }

	*alert = temp;

	return ERROR_OK;
}


FctERR MCP9808_Get_AlertTemp(float * temp, MCP9808_alert alt)
{
	uMCP9808_REG__TEMP_LIM	ALT;
	float					tmp;
	float *					alert = &MCP9808.cfg.HighAlert + alt;
	FctERR					err;

	if (alt > MCP9808__ALERT_CRIT)	{ return ERROR_VALUE; }	// Unknown alert

	err = MCP9808_Read(&ALT.Word, MCP9808__ALERT_UPPER + alt, 1);
	if (err)	{ return err; }

	tmp = (float) ALT.Bits.Integer;
	tmp += (ALT.Bits.Decimal * 0.25f);
	if (ALT.Bits.Sign)	{ tmp = -tmp; }

	*alert = tmp;

	if (temp)	{ *temp = tmp; }

	return ERROR_OK;
}


FctERR MCP9808_Get_Temperature(float * temp)
{
	uMCP9808_REG__TEMP_AMB	TEMP;
	float					tmp;
	FctERR					err;

	err = MCP9808_Get_Temperature_Raw(&TEMP.Word);
	if (err)	{ return err; }

	tmp = (float) TEMP.Bits.Integer;
	tmp += ((TEMP.Bits.Decimal >> (3 - MCP9808.cfg.Resolution)) * MCP9808_resolution_steps[MCP9808.cfg.Resolution]);
	if (TEMP.Bits.Sign)	{ tmp = -tmp; }
	MCP9808.Temperature = tmp;

	MCP9808.TUpper = TEMP.Bits.VsTHigh;
	MCP9808.TLower = TEMP.Bits.VsTLow;
	MCP9808.TCrit = TEMP.Bits.VsTCrit;

	if (temp)	{ *temp = MCP9808.Temperature; }

	return ERROR_OK;
}


__WEAK FctERR MCP9808_handler(void)
{
	FctERR	err = ERROR_NOTAVAIL;	// In case no new data available

	if (TPSSUP_MS(MCP9808.hLast, MCP9808_conv_time[MCP9808.cfg.Resolution]))	{ MCP9808.NewData = true; }

	if (MCP9808.NewData)
	{

		err = MCP9808_Get_Temperature(0);
		if (err)	{ return err; }

		MCP9808.NewData = false;
		MCP9808.hLast = HAL_GetTick();
	}

	#if defined(VERBOSE)
		printf("MCP9808: Temperature %d.%03d°C\r\n", (int16_t) MCP9808.Temperature, get_fp_dec(MCP9808.Temperature, 3));
	#endif

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
