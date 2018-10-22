/*!\file TSL2591_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief TSL2591 Driver procedures
** \details TSL2591: Very-high sensitivity light-to-digital converter
**/
/****************************************************************/
#include "TSL2591.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TSL2591)
/****************************************************************/


TSL2591_t TSL2591[I2C_TSL2591_NB];

const uint16_t TSL2591_gain_tab[4] = { 1, 25, 400, 9200 };	// Medium gain is 24.5, thus 25 used
const uint16_t TSL2591_integ_tab[6] = { 100, 200, 300, 400, 500, 600 };


/****************************************************************/


__WEAK FctERR NONNULL__ TSL2591_Init_Sequence(TSL2591_t * pCpnt)
{
	uTSL2591_REG__ENABLE	EN = { 0 };
	uTSL2591_REG__CONFIG	CFG = { 0 };
	FctERR					err = ERROR_OK;

	pCpnt->cfg.Gain = TSL2591__MEDIUM_GAIN;
	pCpnt->cfg.Integ = TSL2591__INTEG_100MS;
	pCpnt->cfg.LowThreshold = 0x8FF;
	pCpnt->cfg.HighThreshold = 0x8FF;
	pCpnt->cfg.AIEN = true;

	// get ID & check against values for TSL2591
	err = TSL2591_Get_ChipID(pCpnt, &pCpnt->cfg.ID);
	if (err)								{ return err; }
	if (pCpnt->cfg.ID != TSL2591_CHIP_ID)	{ return ERROR_COMMON; }	// Unknown device

	EN.Bits.PON = true;		// Turn ON Osc
	err = TSL2591_Write_En(pCpnt, EN.Byte);
	if (err)			{ return err; }

	CFG.Bits.AGAIN = pCpnt->cfg.Gain;
	CFG.Bits.ATIME = pCpnt->cfg.Integ;
	err = TSL2591_Write_Cfg(pCpnt, CFG.Byte);
	if (err)			{ return err; }

	TSL2591_Set_CPL(pCpnt);

	err = TSL2591_Set_AIT(pCpnt, pCpnt->cfg.LowThreshold, pCpnt->cfg.HighThreshold);
	if (err)			{ return err; }

	EN.Bits.AEN = true;				// Turn ON ALS
	EN.Bits.AIEN = pCpnt->cfg.AIEN;	// Turn ON ALS interrupts
	return TSL2591_Write_En(pCpnt, EN.Byte);
}


/****************************************************************/


void NONNULL__ TSL2591_Set_CPL(TSL2591_t * pCpnt)
{
	// GA = 1 / Transmitivity -> e.g., for 5% transmissive glass, GA = 1 / 0.05
	const float	GA = 100.0f / TSL2591_GLASS_TRANSMITIVITY;

	// CPL = (ATIME_ms * AGAINx) / (GA * 53)	(Counts per Lux)
	// 6.82f is coef to convert from uW/cm2 to Lux -> https://forums.adafruit.com/viewtopic.php?f=19&t=85343&start=15
	float CPL = (TSL2591_integ_tab[pCpnt->cfg.Integ] * TSL2591_gain_tab[pCpnt->cfg.Gain] /* * 6.82f*/) / /*264.1f*/(GA * 53);

	pCpnt->cfg.DER = 2.0f / CPL;
	pCpnt->cfg.CPkL = (uint32_t) (CPL * 1000);	// convert Counts Per Lux into kCounts
}


#define ADAFRUIT_LUX_TSL2591

/*!\brief Computes an approximation of Illuminance (in lux) from Full and IR values
** \param[in] full - Red value
** \param[in] ir - Green value
** \return FctERR - error code
**/
static NONNULL__ FctERR calculateLux(TSL2591_t * pCpnt, uint16_t full, uint16_t ir)
{
	uint32_t	LUX1, LUX2;
	// SATURATION = 1024 * (256 - ATIME_ms) if ATIME_ms > 172ms
	uint16_t	sat = (TSL2591_integ_tab[pCpnt->cfg.Integ] > 172) ? 65535 : 36863;

	// Check for saturation
	if ((full >= sat) || (ir >= sat))
	{
		pCpnt->Saturation = true;
		return ERROR_OVERFLOW;	// Signal an overflow
	}
	else { pCpnt->Saturation = false; }

	// Check for ripple saturation
	sat = (uint16_t) (sat * 0.75f);
	if ((full >= sat) || (ir >= sat))	{ pCpnt->SaturationRipple = true; }
	else								{ pCpnt->SaturationRipple = false; }

	// LUX = Raw / (gain * (integ / 100)) ????

	// 1.67f factor is obtained as the ratio of Irradiance Responsivity (257.5 / 154.1) or invert ADC count ratio val (1 / 0.598)
	// Lux1 = (C0DATA - 2 * C1DATA) / CPL
	LUX1 = (float) ((1000 * full) - (1670 * ir)) / pCpnt->cfg.CPkL;

	// TODO: find about 2nd segment equation for TSL2591
	// Lux2 = (0.6 * C0DATA - C1DATA) / CPL
	LUX2 = (float) ((600 * full) - (1000 * ir)) / pCpnt->cfg.CPkL;

	// Lux = MAX(Lux1, Lux2, 0)
	pCpnt->Lux = LUX1;// max(LUX1, LUX2);

	// IRF = Lux * CPL / C0DATA;	(Infra Red Factor)
	pCpnt->IRF = (float) pCpnt->Lux * ((float) pCpnt->cfg.CPkL / 1000.0f) / full;

	// MaxLux = IRF * SATURATION / CPL;
	pCpnt->MaxLux = pCpnt->IRF * sat / ((float) pCpnt->cfg.CPkL / 1000.0f);

	#if defined(ADAFRUIT_LUX_TSL2591)
		float		cpl, lux1, lux2;
		uint32_t	lux;

		// Equation from Adafruit libraries
		cpl = (TSL2591_integ_tab[pCpnt->cfg.Integ] * TSL2591_gain_tab[pCpnt->cfg.Gain]) / 408.0f;

		lux1 = ((float) full - (1.6f * (float) ir)) / cpl;
		lux2 = ((0.59f * (float) full) - (0.86f * (float) ir)) / cpl;
		lux = max((uint32_t) lux1, (uint32_t) lux2);

		printf("TSL2591: adafruit lux: %lul\r\n", lux);
	#endif

	return ERROR_OK;
}


__WEAK FctERR NONNULL__ TSL2591_handler(TSL2591_t * pCpnt)
{
	uint8_t	DATA[4];
	FctERR	err;

	err = TSL2591_Read(pCpnt->cfg.slave_inst, DATA, TSL2591__C0DATAL, 4);
	if (err)	{ return err; }

	pCpnt->Full = MAKEWORD(DATA[0], DATA[1]);
	pCpnt->IR = MAKEWORD(DATA[2], DATA[3]);
	err = calculateLux(pCpnt, pCpnt->Full, pCpnt->IR);

	#if defined(VERBOSE)
		if (err == ERROR_OVERFLOW)	{ printf("TSL2591; Sensor saturation reached!\r\n"); }
		else						{ printf("TSL2591: Full %d IR %d Lux: %lul\r\n", pCpnt->Full, pCpnt->IR, pCpnt->Lux); }
	#endif

	if (pCpnt->cfg.AIEN)
	{
		err = TSL2591_SF_Clear_IT(pCpnt);
		if (err)	{ return err; }
	}

	return ERROR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
