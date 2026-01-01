/*!\file TMP1075_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief TMP1075 Driver procedures
** \details TMP1075: Temperature Sensor With I2C and SMBus Interface in Industry Standard LM75 Form Factor and Pinout
**/
/****************************************************************/
#include "TMP1075.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TMP1075)
/****************************************************************/


TMP1075_t TMP1075[I2C_TMP1075_NB] = { 0 };

static const uint8_t TMP1075_Single_conv_time[2] = { 7U, 15U };			//!< Single shot conversion times for TMP1075, TMP1075N
static const uint8_t TMP1075_conv_time[4] = { 28U, 55U, 110U, 220U };	//!< Conversion times for TMP1075
static const uint8_t TMP1075N_conv_time[4] = { 28U, 55U, 110U, 250U };	//!< Conversion times for TMP1075N
static const float TMP1075_resolution = 0.0625f;						//!< Resolution steps for TMP1075


/****************************************************************/


__WEAK FctERR NONNULL__ TMP1075_Init_Sequence(TMP1075_t * const pCpnt)
{
	uTMP1075_REG__CFGR_WORD CFG;
	FctERR err;

	// get CFGR word to check if TMP1075 or TMP1075N
	err = TMP1075_Read_Word(pCpnt->cfg.slave_inst, &CFG.Word, TMP1075__CFGR);
	if (err != ERROR_OK)	{ goto ret; }

	if (CFG.Bits.FFh != TMP1075_CFGR_7_0)	{ pCpnt->cfg.TMP1075N = true; }	// Set to TMP1075N
	pCpnt->cfg.Rate = CFG.Bits.R;

	if (pCpnt->cfg.TMP1075N)
	{
		err = TMP1075N_Get_ChipID(pCpnt, &pCpnt->cfg.ChipID);
		if (err != ERROR_OK)						{ goto ret; }
		if (pCpnt->cfg.ChipID != TMP1075N_CHIP_ID)	{ err = ERROR_COMMON; goto ret; }	// Unknown device
	}

	err = TMP1075_Set_AlertMode(pCpnt, TMP1075__MODE_INT);
	if (err != ERROR_OK)	{ goto ret; }

	err = TMP1075_Set_ConversionRate(pCpnt, TMP1075__CR_220MS);

	ret:
	return err;
}


/****************************************************************/


static float TMP1075_convert_raw_temp(const int16_t temp)
{
	return ((float) (temp / 16) * TMP1075_resolution);		// Divide by 16 as value is 12 bit resolution of MSBs (with sign as MSB)
}

static int16_t TMP1075_convert_to_raw_temp(const float temp)
{
	return (int16_t) ((temp * 16.0f) / TMP1075_resolution);	// Multiply by 16 as value is 12 bit resolution of MSBs (with sign as MSB)
}


FctERR NONNULL__ TMP1075_Set_LimitTemp(TMP1075_t * const pCpnt, const float temp, const TMP1075_limit lim)
{
	FctERR err = ERROR_OK;

	if (lim > TMP1075__LIMIT_HIGH)				{ err = ERROR_VALUE; }	// Unknown limit
	if ((temp < -128.0f) || (temp > 127.9375f))	{ err = ERROR_RANGE; }	// Temperature too low/high
	if (err != ERROR_OK)						{ goto ret; }

	float *	limit = &pCpnt->cfg.LowLimit + lim;
	const int16_t TEMP = TMP1075_convert_to_raw_temp(temp);

	err = TMP1075_Write_Word(pCpnt->cfg.slave_inst, (const uint16_t *) &TEMP, TMP1075__LLIM + lim);
	if (err != ERROR_OK)	{ goto ret; }

	*limit = temp;

	ret:
	return err;
}


FctERR NONNULLX__(1) TMP1075_Get_LimitTemp(TMP1075_t * const pCpnt, float * temp, const TMP1075_limit lim)
{
	FctERR err = ERROR_OK;

	if (lim > TMP1075__LIMIT_HIGH)	{ err = ERROR_VALUE; }	// Unknown limit
	if (err != ERROR_OK)			{ goto ret; }

	float * limit = &pCpnt->cfg.LowLimit + lim;
	int16_t	TEMP;

	err = TMP1075_Read_Word(pCpnt->cfg.slave_inst, (uint16_t *) &TEMP, TMP1075__LLIM + lim);
	if (err != ERROR_OK)	{ goto ret; }

	*limit = TMP1075_convert_raw_temp(TEMP);

	if (temp != NULL)	{ *temp = *limit; }

	ret:
	return err;
}


FctERR NONNULLX__(1) TMP1075_Get_Temperature(TMP1075_t * const pCpnt, float * temp)
{
	int16_t	TEMP;
	FctERR	err;

	err = TMP1075_Get_Temperature_Raw(pCpnt, &TEMP);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->Temperature = TMP1075_convert_raw_temp(TEMP);

	if (temp != NULL)	{ *temp = pCpnt->Temperature; }

	ret:
	return err;
}


__WEAK FctERR NONNULL__ TMP1075_handler(TMP1075_t * const pCpnt)
{
	FctERR err = ERROR_OK;

	if (!pCpnt->NewData)
	{
		if (pCpnt->cfg.Continuous)
		{
			const uint8_t * aConvTime = pCpnt->cfg.TMP1075N ? TMP1075N_conv_time : TMP1075_conv_time;
			if (TPSSUP_MS(pCpnt->hLast, aConvTime[pCpnt->cfg.Rate]))										{ pCpnt->NewData = true; }
		}
		else
		{
			if (pCpnt->Ongoing && TPSSUP_MS(pCpnt->hLast, TMP1075_Single_conv_time[pCpnt->cfg.TMP1075N]))	{ pCpnt->NewData = true; }
		}
	}

	if (pCpnt->NewData)
	{
		err = TMP1075_Get_Temperature(pCpnt, 0);
		if (err != ERROR_OK)	{ goto ret; }

		pCpnt->NewData = false;
		pCpnt->hLast = HAL_GetTick();

		#if defined(VERBOSE)
			const uint8_t idx = pCpnt - TMP1075;
			UNUSED_RET printf("TMP1075 id%d: Temperature %d.%03ld°C\r\n", idx, (int16_t) pCpnt->Temperature,
								get_fp_dec(pCpnt->Temperature, 3U));
		#endif
	}

	ret:
	return err;
}


__WEAK FctERR NONNULL__ TMP1075_handler_it(TMP1075_t * const pCpnt)
{
	FctERR err = ERROR_OK;

	if (TMP1075_INT_GPIO_Get(pCpnt))
	{
		pCpnt->NewData = true;
		err = TMP1075_handler(pCpnt);
	}

	return err;
}


FctERR TMP1075_handler_all(void)
{
	FctERR err = ERROR_OK;

	for (TMP1075_t * pCpnt = TMP1075 ; pCpnt < &TMP1075[I2C_TMP1075_NB] ; pCpnt++)
	{
		err |= TMP1075_handler(pCpnt);
	}

	return err;
}

FctERR TMP1075_handler_it_all(void)
{
	FctERR err = ERROR_OK;

	for (TMP1075_t * pCpnt = TMP1075 ; pCpnt < &TMP1075[I2C_TMP1075_NB] ; pCpnt++)
	{
		err |= TMP1075_handler_it(pCpnt);
	}

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
