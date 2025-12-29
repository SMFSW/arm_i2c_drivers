/*!\file ADS1115_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief ADS1115 Driver extensions
** \details ADS1115: Ultra-Small, Low-Power, 16-Bit Analog-to-Digital Converter with Internal Reference
** \note	Compatibility with:
**				- ADS1113 (restrictions given in datasheet)
**				- ADS1114 (restrictions given in datasheet)
**				- ADS1115
** \note	Compatibility should be assured with:
**				- ADS1x1x (check datasheets for compatibility informations)
**/
/****************************************************************/
#include "ADS1115.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_ADS1115)
/****************************************************************/


//! \note Assuming max data rate drift following temperature does not exceed 3% (* 1.3)
#define ADS1115_SPStoUS(SPS)	((uint32_t) (((1.0f / (float) (SPS)) * 1.3f) * 1000000UL))
#define ADS1115_SPStoMS(SPS)	((uint32_t) (((1.0f / (float) (SPS)) * 1.3f) * 1000UL))

/*!\brief ADS1115 conversion time in µs
** \note Depending the configured rate \ref ADS1115_rate
** \warning A few margin shall be added to cover temperature drift
**/
static const uint32_t ADS1115_conv_us[8] = {
	ADS1115_SPStoUS(8U),
	ADS1115_SPStoUS(16U),
	ADS1115_SPStoUS(32U),
	ADS1115_SPStoUS(64U),
	ADS1115_SPStoUS(128U),
	ADS1115_SPStoUS(250U),
	ADS1115_SPStoUS(475U),
	ADS1115_SPStoUS(860U)
};

/*!\brief ADS1115 conversion time in ms
** \note Depending the configured rate \ref ADS1115_rate
** \warning A few margin shall be added to cover temperature drift
**/
static const uint32_t ADS1115_conv_ms[8] = {
	ADS1115_SPStoMS(8U),
	ADS1115_SPStoMS(16U),
	ADS1115_SPStoMS(32U),
	ADS1115_SPStoMS(64U),
	ADS1115_SPStoMS(128U),
	ADS1115_SPStoMS(250U),
	ADS1115_SPStoMS(475U),
	ADS1115_SPStoMS(860U)
};


/*!\brief ADS1115 step value in µV (following configured gain)
** \note Depending the configured gain \ref ADS1115_gain
**/
static const float ADS1115_step[8] = {
	187.5f,		// ±6.144V gain
	125.0f,		// ±4.096V gain
	62.5f,		// ±2.048V gain
	31.25f,		// ±1.024V gain
	15.625f,	// ±0.512V gain
	7.8125f,	// ±0.256V gain
	7.8125f,	// ±0.256V gain
	7.8125f		// ±0.256V gain
};


/*!\brief Write configuration register of ADS1115 peripheral
** \param[in] pCpnt - Pointer to ADS1115 component
** \return FctERR - error code
**/
__STATIC_INLINE FctERR NONNULL_INLINE__ ADS1115_write_cfg(ADS1115_t * const pCpnt) {
	return ADS1115_Write(pCpnt->cfg.slave_inst, &pCpnt->cfg.Config.Word, ADS1115__CONFIG); }


uint32_t ADS1115_Get_conv_us(ADS1115_t * const pCpnt) {
	return ADS1115_conv_us[pCpnt->cfg.Config.Bits.DR]; }

uint32_t ADS1115_Get_conv_ms(ADS1115_t * const pCpnt) {
	return ADS1115_conv_ms[pCpnt->cfg.Config.Bits.DR]; }


FctERR NONNULL__ ADS1115_Get_Conversion(ADS1115_t * const pCpnt) {
	return ADS1115_Read(pCpnt->cfg.slave_inst, (uint16_t *) &pCpnt->AIN[pCpnt->AIN_idx], ADS1115__CONVERSION); }


FctERR NONNULL__ ADS1115_Set_Function(ADS1115_t * const pCpnt, const ADS1115_func func, const ADS1115_mode mode, const uint8_t nb)
{
	FctERR err;

	if (func > ADS1115__FUNC_SINGLE_ENDED)	{ err = ERROR_VALUE; }	// Unknown conversion
	if (mode > ADS1115__MODE_SINGLE_SHOT)	{ err = ERROR_VALUE; }	// Unknown mode
	if ((!nb) || (nb > 4U))					{ err = ERROR_VALUE; }	// Too much or 0 channels
	if (err != ERROR_OK)					{ goto ret; }

	pCpnt->cfg.nb = nb;
	pCpnt->cfg.Config.Bits.MODE = mode;

	switch (func)
	{
		default:
			pCpnt->cfg.Config.Bits.MODE = ADS1115__MODE_SINGLE_SHOT;	// Forcing single (bad) shot
			pCpnt->cfg.nb = 0;
			break;

		case ADS1115__FUNC_SINGLE_DIFF:
			pCpnt->cfg.Config.Bits.MUX = ADS1115__MUX_pAIN0_nAIN1;
			pCpnt->cfg.nb = 1U;
			break;

		case ADS1115__FUNC_MULTIPLE_DIFF:
			pCpnt->cfg.Config.Bits.MUX = ADS1115__MUX_pAIN0_nAIN3;
			break;

		case ADS1115__FUNC_SINGLE_ENDED:
			pCpnt->cfg.Config.Bits.MUX = ADS1115__MUX_pAIN0_nGND;
			break;
	}

	pCpnt->cfg.function = func;
	pCpnt->cfg.mode = pCpnt->cfg.Config.Bits.MODE;
	pCpnt->AIN_idx = 0;

	err = ADS1115_write_cfg(pCpnt);

	ret:
	return err;
}


FctERR NONNULL__ ADS1115_Set_Mode(ADS1115_t * const pCpnt, const ADS1115_mode mode)
{
	FctERR err = ERROR_VALUE;

	if (mode <= ADS1115__MODE_SINGLE_SHOT)
	{
		pCpnt->cfg.mode = mode;
		pCpnt->cfg.Config.Bits.MODE = mode;
		err =ADS1115_write_cfg(pCpnt);
	}

	return err;
}


FctERR NONNULL__ ADS1115_Set_Gain(ADS1115_t * const pCpnt, const ADS1115_gain gain)
{
	FctERR err = ERROR_VALUE;

	if (gain <= ADS1115__FS_256mV_3)
	{
		pCpnt->cfg.Config.Bits.PGA = gain;
		err = ADS1115_write_cfg(pCpnt);
	}

	return err;
}


FctERR NONNULL__ ADS1115_Set_Rate(ADS1115_t * const pCpnt, const ADS1115_rate rate)
{
	FctERR err = ERROR_VALUE;

	if (rate <= ADS1115__SPS_860)
	{
		pCpnt->cfg.Config.Bits.DR = rate;
		err = ADS1115_write_cfg(pCpnt);
	}

	return err;
}


FctERR NONNULL__ ADS1115_Set_CompMode(ADS1115_t * const pCpnt, const ADS1115_comp comp)
{
	FctERR err = ERROR_VALUE;

	if (comp <= ADS1115__COMP_WINDOW)
	{
		pCpnt->cfg.Config.Bits.COMP_MODE = comp;
		err = ADS1115_write_cfg(pCpnt);
	}

	return err;
}


FctERR NONNULL__ ADS1115_Set_CompPolarity(ADS1115_t * const pCpnt, const ADS1115_polarity pol)
{
	FctERR err = ERROR_VALUE;

	if (pol <= ADS1115__POL_HIGH)
	{
		pCpnt->cfg.Config.Bits.COMP_POL = pol;
		err = ADS1115_write_cfg(pCpnt);
	}

	return err;
}


FctERR NONNULL__ ADS1115_Set_CompLatch(ADS1115_t * const pCpnt, const ADS1115_latch latch)
{
	FctERR err = ERROR_VALUE;

	if (latch <= ADS1115__LATCH_ENABLE)
	{
		pCpnt->cfg.Config.Bits.COMP_LAT = latch;
		err = ADS1115_write_cfg(pCpnt);
	}

	return err;
}


FctERR NONNULL__ ADS1115_Set_CompQueue(ADS1115_t * const pCpnt, const ADS1115_queue queue)
{
	FctERR err = ERROR_VALUE;

	if (queue <= ADS1115__QUEUE_DISABLE)
	{
		pCpnt->cfg.Config.Bits.COMP_QUE = queue;
		err = ADS1115_write_cfg(pCpnt);
	}

	return err;
}


FctERR NONNULL__ ADS1115_Start_Conversion(ADS1115_t * const pCpnt, const ADS1115_mux chan)
{
	FctERR err = ERROR_VALUE;

	if (chan <= ADS1115__MUX_pAIN3_nGND)
	{
		pCpnt->cfg.Config.Bits.MUX = chan;
		pCpnt->cfg.Config.Bits.OS = (pCpnt->cfg.mode == ADS1115__MODE_SINGLE_SHOT) ? 1U : 0U;
		err = ADS1115_write_cfg(pCpnt);
	}

	return err;
}


FctERR NONNULL__ ADS1115_Start_NextConversion(ADS1115_t * const pCpnt)
{
	FctERR err = ERROR_VALUE;

	if (pCpnt->cfg.function <= ADS1115__FUNC_SINGLE_ENDED)
	{
		if (	(pCpnt->cfg.function == ADS1115__FUNC_SINGLE_DIFF)
			||	(pCpnt->cfg.nb <= 1U))
		{
			if (pCpnt->cfg.mode == ADS1115__MODE_SINGLE_SHOT)
			{
				err = ADS1115_Start_Conversion(pCpnt, pCpnt->cfg.Config.Bits.MUX);
			}
			else
			{
				err = ERROR_NOTAVAIL;
			}
		}
		else
		{
			if (++pCpnt->AIN_idx >= pCpnt->cfg.nb)	{ pCpnt->AIN_idx = 0; }

			pCpnt->cfg.Config.Bits.MUX = pCpnt->AIN_idx + ((pCpnt->cfg.function == ADS1115__FUNC_SINGLE_ENDED) ? ADS1115__MUX_pAIN0_nGND : ADS1115__MUX_pAIN0_nAIN3);

			err = ADS1115_Start_Conversion(pCpnt, pCpnt->cfg.Config.Bits.MUX);
		}
	}

	return err;
}


/*!\brief Convert ADS1115 value to µV
** \param[in] val - ADS1115 value
** \param[in] gain - ADS1115 configured gain
** \return Converted value
**/
static float ADS1115_convert_to_uV(const int16_t val, const ADS1115_gain gain)
{
	return (gain > ADS1115__FS_256mV_3) ? 0.0f : ((float) val * ADS1115_step[gain]);
}

/*!\brief Convert ADS1115 value to mV
** \param[in] val - ADS1115 value
** \param[in] gain - ADS1115 configured gain
** \return Converted value
**/
__STATIC_INLINE float ADS1115_convert_to_mV(const int16_t val, const ADS1115_gain gain)
{
	return (ADS1115_convert_to_uV(val, gain) / 1000U);
}

/*!\brief Convert ADS1115 value to V
** \param[in] val - ADS1115 value
** \param[in] gain - ADS1115 configured gain
** \return Converted value
**/
__STATIC_INLINE float ADS1115_convert_to_V(const int16_t val, const ADS1115_gain gain)
{
	return (ADS1115_convert_to_uV(val, gain) / 1000000UL);
}


float NONNULL__ ADS1115_Get_converted_value_uV(const ADS1115_t * const pCpnt, const uint8_t chan)
{
	return ADS1115_convert_to_uV(ADS1115_Get_raw_value(pCpnt, chan), pCpnt->cfg.Config.Bits.PGA);
}

float NONNULL__ ADS1115_Get_converted_value_mV(const ADS1115_t * const pCpnt, const uint8_t chan)
{
	return ADS1115_convert_to_mV(ADS1115_Get_raw_value(pCpnt, chan), pCpnt->cfg.Config.Bits.PGA);
}

float NONNULL__ ADS1115_Get_converted_value_V(const ADS1115_t * const pCpnt, const uint8_t chan)
{
	return ADS1115_convert_to_V(ADS1115_Get_raw_value(pCpnt, chan), pCpnt->cfg.Config.Bits.PGA);
}


/****************************************************************/


__WEAK void NONNULL__ ADS1115_RDY_GPIO_Init(ADS1115_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&pCpnt->cfg.RDY_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK bool NONNULL__ ADS1115_RDY_GPIO_Get(const ADS1115_t * const pCpnt) {
	return I2C_peripheral_GPIO_get(&pCpnt->cfg.RDY_GPIO); }


/****************************************************************/
#endif
#endif
/****************************************************************/
