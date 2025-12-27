/*!\file TSL2591_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief TSL2591 Driver procedures
** \details TSL2591: Very-high sensitivity light-to-digital converter
**/
/****************************************************************/
#include "TSL2591.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TSL2591)
/****************************************************************/


TSL2591_t TSL2591[I2C_TSL2591_NB] = { 0 };

const uint16_t TSL2591_gain_tab[4] = { 1U, 25U, 400U, 9200U };	// Medium gain is 24.5, thus 25 used
const uint16_t TSL2591_integ_tab[6] = { 100U, 200U, 300U, 400U, 500U, 600U };


/****************************************************************/


__WEAK FctERR NONNULL__ TSL2591_Init_Sequence(TSL2591_t * const pCpnt)
{
	uTSL2591_REG__ENABLE	EN = { 0 };
	uTSL2591_REG__CONFIG	CFG = { 0 };
	FctERR					err = ERROR_OK;

	pCpnt->cfg.Gain = TSL2591__MEDIUM_GAIN;
	pCpnt->cfg.Integ = TSL2591__INTEG_100MS;
	pCpnt->cfg.LowThreshold = 0x8FFU;
	pCpnt->cfg.HighThreshold = 0x8FFU;
	pCpnt->cfg.AIEN = true;

	// get ID & check against values for TSL2591
	err = TSL2591_Get_ChipID(pCpnt, &pCpnt->cfg.ID);
	if (err != ERROR_OK)					{ return err; }
	if (pCpnt->cfg.ID != TSL2591_CHIP_ID)	{ return ERROR_COMMON; }	// Unknown device

	EN.Bits.PON = true;		// Turn ON Osc
	err = TSL2591_Write_En(pCpnt, EN.Byte);
	if (err != ERROR_OK)	{ return err; }

	CFG.Bits.AGAIN = pCpnt->cfg.Gain;
	CFG.Bits.ATIME = pCpnt->cfg.Integ;
	err = TSL2591_Write_Cfg(pCpnt, CFG.Byte);
	if (err != ERROR_OK)	{ return err; }

	TSL2591_Set_CPL(pCpnt);

	err = TSL2591_Set_AIT(pCpnt, pCpnt->cfg.LowThreshold, pCpnt->cfg.HighThreshold);
	if (err != ERROR_OK)	{ return err; }

	EN.Bits.AEN = pCpnt->cfg.AIEN;	// Turn ON ALS following cfg
	EN.Bits.AIEN = pCpnt->cfg.AIEN;	// Turn ON ALS interrupts following cfg
	return TSL2591_Write_En(pCpnt, EN.Byte);
}


/****************************************************************/


void NONNULL__ TSL2591_Set_CPL(TSL2591_t * const pCpnt)
{
	// GA = 1 / Transmitivity -> e.g., for 5% transmissive glass, GA = 1 / 0.05
	const float	GA = 100.0f / TSL2591_GLASS_TRANSMISSIVITY;

	// CPL = (ATIME_ms * AGAINx) / (GA * DF)	(Counts per Lux)
	float CPL = (TSL2591_integ_tab[pCpnt->cfg.Integ] * TSL2591_gain_tab[pCpnt->cfg.Gain]) / (GA * TSL2591_DEVICE_FACTOR);

	pCpnt->cfg.DER = 2.0f / CPL;
	pCpnt->cfg.CPkL = (uint32_t) (CPL * 1000.0f);	// convert Counts Per Lux into kCounts
}


/*!\brief Computes an approximation of Illuminance (in lux) from Full and IR values
** \param[in] full - Red value
** \param[in] ir - Green value
** \return FctERR - error code
**/
static NONNULL__ FctERR calculateLux(TSL2591_t * const pCpnt, const uint16_t full, const uint16_t ir)
{
	const float	B = 1.64f, C = 0.59f, D = 0.86f, DF = 408.0f;
	// SATURATION = 1024 * (256 - ATIME_ms) if ATIME_ms > 172ms
	uint16_t	sat = (TSL2591_integ_tab[pCpnt->cfg.Integ] > 172U) ? 65535U : 36863U;

	// Check for saturation
	if ((full >= sat) || (ir >= sat))
	{
		pCpnt->Saturation = true;
		return ERROR_OVERFLOW;	// Signal an overflow
	}
	else { pCpnt->Saturation = false; }

	// Check for ripple saturation
	sat *= 0.75f;
	if ((full >= sat) || (ir >= sat))	{ pCpnt->SaturationRipple = true; }
	else								{ pCpnt->SaturationRipple = false; }

	float CPL = (TSL2591_integ_tab[pCpnt->cfg.Integ] * TSL2591_gain_tab[pCpnt->cfg.Gain]) / DF;

	float IAC1 = full - (B * ir);			// IAC1 = (C0DATA - (B * C1DATA))
	float IAC2 = (C * full) - (D * ir);		// IAC2 = (C * C0DATA) - (D * C1DATA)
	float IAC = max(0, max(IAC1, IAC2));	// IAC = MAX(IAC1, IAC2, 0)

	pCpnt->Lux = IAC / CPL;					// Lux = IAC / CPL
	pCpnt->IRF = IAC / full;				// IRF = IAC / C0DATA (Infra Red Factor)

	return ERROR_OK;
}


__WEAK FctERR NONNULL__ TSL2591_handler(TSL2591_t * const pCpnt)
{
	uint8_t					DATA[5];
	uTSL2591_REG__STATUS *	ST = (uTSL2591_REG__STATUS *) DATA;

	FctERR err = TSL2591_Read(pCpnt->cfg.slave_inst, DATA, TSL2591__STATUS, sizeof(DATA));
	if (err != ERROR_OK)	{ goto ret; }

	if ((ST->Bits.AINT) && (ST->Bits.AVALID))
	{
		pCpnt->Full = MAKEWORD(DATA[1], DATA[2]);
		pCpnt->IR = MAKEWORD(DATA[3], DATA[4]);
		err = calculateLux(pCpnt, pCpnt->Full, pCpnt->IR);

		#if defined(VERBOSE)
			const uint8_t idx = pCpnt - TSL2591;
			if (err == ERROR_OVERFLOW)	{ printf("TSL2591 id%d: Sensor saturation reached!\r\n", idx); }
			else						{ printf("TSL2591 id%d: Full %d IR %d Lux: %lul\r\n", idx, pCpnt->Full, pCpnt->IR, pCpnt->Lux); }
		#endif
	}

	if (ST->Bits.AINT)	{ err = TSL2591_SF_Clear_IT(pCpnt); }

	ret:
	return err;
}


__WEAK FctERR NONNULL__ TSL2591_handler_it(TSL2591_t * const pCpnt)
{
	FctERR err = ERROR_OK;

	if (TSL2591_INT_GPIO_Get(pCpnt))	{ err = TSL2591_handler(pCpnt); }

	return err;
}


FctERR TSL2591_handler_all(void)
{
	FctERR err = ERROR_OK;

	for (TSL2591_t * pCpnt = TSL2591 ; pCpnt < &TSL2591[I2C_TSL2591_NB] ; pCpnt++)
	{
		err |= TSL2591_handler(pCpnt);
	}

	return err;
}

FctERR TSL2591_handler_it_all(void)
{
	FctERR err = ERROR_OK;

	for (TSL2591_t * pCpnt = TSL2591 ; pCpnt < &TSL2591[I2C_TSL2591_NB] ; pCpnt++)
	{
		err |= TSL2591_handler_it(pCpnt);
	}

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
