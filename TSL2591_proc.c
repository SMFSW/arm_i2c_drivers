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


TSL2591_t TSL2591 = { 0, 0, 0, 0, 0.0f, false, false, { &TSL2591_hal, 0, 0.0f,TSL2591__MEDIUM_GAIN, TSL2591__INTEG_100MS, true, 0 } };

const uint16_t TSL2591_gain_tab[4] = { 1, 25, 400, 9200 };	// Medium gain is 24.5, thus 25 used
const uint16_t TSL2591_integ_tab[6] = { 100, 200, 300, 400, 500, 600 };


/****************************************************************/


__WEAK FctERR TSL2591_Init_Sequence(void)
{
	uTSL2591_REG__ENABLE	EN;
	uTSL2591_REG__CONFIG	CFG;
	FctERR					err = ERROR_OK;

	// get ID & check against values for TSL2591
	err = TSL2591_Get_ChipID(&TSL2591.cfg.Id);
	if (err)								{ return err; }
	if (TSL2591.cfg.Id != TSL2591_CHIP_ID)	{ return ERROR_COMMON; }	// Unknown device

	EN.Byte = 0;
	EN.Bits.PON = true;		// Turn ON Osc
	err = TSL2591_Write_En(EN.Byte);
	if (err)			{ return err; }

	CFG.Byte = 0;
	CFG.Bits.AGAIN = TSL2591.cfg.Gain;
	CFG.Bits.ATIME = TSL2591.cfg.Integ;
	err = TSL2591_Write_Cfg(CFG.Byte);
	if (err)			{ return err; }

	TSL2591_Set_CPL();

	err = TSL2591_Set_AILT(0x8FF);
	if (err)			{ return err; }
	err = TSL2591_Set_AIHT(0x8FF);
	if (err)			{ return err; }

	EN.Bits.AEN = true;					// Turn ON ALS
	EN.Bits.AIEN = TSL2591.cfg.AIEN;	// Turn ON ALS interrupts
	return TSL2591_Write_En(EN.Byte);
}


/****************************************************************/


uint32_t TSL2591_Get_Lux(void) {
	return TSL2591.Lux; }


void TSL2591_Set_CPL(void)
{
	// GA = 1 / Transmitivity -> e.g., for 5% transmissive glass, GA = 1 / 0.05
	const float	GA = 100.0f / TSL2591_GLASS_TRANSMITIVITY;

	// CPL = (ATIME_ms * AGAINx) / (GA * 53)	(Counts per Lux)
	// 6.82f is coef to convert from uW/cm2 to Lux -> https://forums.adafruit.com/viewtopic.php?f=19&t=85343&start=15
	float CPL = (TSL2591_integ_tab[TSL2591.cfg.Integ] * TSL2591_gain_tab[TSL2591.cfg.Gain] /* * 6.82f*/) / /*264.1f*/(GA * 53);

	TSL2591.cfg.DER = 2.0f / CPL;
	TSL2591.cfg.CPkL = (uint32_t) (CPL * 1000);	// convert Counts Per Lux into kCounts
}


#define ADAFRUIT_LUX_TSL2591

/*!\brief Computes an approximation of Illuminance (in lux) from Full and IR values
** \param[in] full - Red value
** \param[in] ir - Green value
** \return FctERR - error code
**/
static FctERR calculateLux(uint16_t full, uint16_t ir)
{
	uint32_t	LUX1, LUX2;
	// SATURATION = 1024 * (256 - ATIME_ms) if ATIME_ms > 172ms
	uint16_t	sat = (TSL2591_integ_tab[TSL2591.cfg.Integ] > 172) ? 65535 : 36863;

	// Check for saturation
	if ((full >= sat) || (ir >= sat))
	{
		TSL2591.Saturation = true;
		return ERROR_OVERFLOW;	// Signal an overflow
	}
	else { TSL2591.Saturation = false; }

	// Check for ripple saturation
	sat = (uint16_t) (sat * 0.75f);
	if ((full >= sat) || (ir >= sat))	{ TSL2591.SaturationRipple = true; }
	else								{ TSL2591.SaturationRipple = false; }

	// LUX = Raw / (gain * (integ / 100)) ????

	// 1.67f factor is obtained as the ratio of Irradiance Responsivity (257.5 / 154.1) or invert ADC count ratio val (1 / 0.598)
	// Lux1 = (C0DATA - 2 * C1DATA) / CPL
	LUX1 = (float) ((1000 * full) - (1670 * ir)) / TSL2591.cfg.CPkL;

	// TODO: find about 2nd segment equation for TSL2591
	// Lux2 = (0.6 * C0DATA - C1DATA) / CPL
	LUX2 = (float) ((600 * full) - (1000 * ir)) / TSL2591.cfg.CPkL;

	// Lux = MAX(Lux1, Lux2, 0)
	TSL2591.Lux = LUX1;// max(LUX1, LUX2);

	// IRF = Lux * CPL / C0DATA;	(Infra Red Factor)
	TSL2591.IRF = (float) TSL2591.Lux * ((float) TSL2591.cfg.CPkL / 1000.0f) / full;

	// MaxLux = IRF * SATURATION / CPL;
	TSL2591.MaxLux = TSL2591.IRF * sat / ((float) TSL2591.cfg.CPkL / 1000.0f);

	#if defined(ADAFRUIT_LUX_TSL2591)
		float		cpl, lux1, lux2;
		uint32_t	lux;

		// Equation from Adafruit libraries
		cpl = (TSL2591_integ_tab[TSL2591.cfg.Integ] * TSL2591_gain_tab[TSL2591.cfg.Gain]) / 408.0f;

		lux1 = ((float) full - (1.6f * (float) ir)) / cpl;
		lux2 = ((0.59f * (float) full) - (0.86f * (float) ir)) / cpl;
		lux = max((uint32_t) lux1, (uint32_t) lux2);

		printf("TSL2591: adafruit lux: %lul\r\n", lux);
	#endif

	return ERROR_OK;
}


__WEAK FctERR TSL2591_handler(void)
{
	uint8_t	DATA[4];
	FctERR	err;

	err = TSL2591_Read(DATA, TSL2591__C0DATAL, 4);
	if (err)	{ return err; }

	TSL2591.Full = MAKEWORD(DATA[0], DATA[1]);
	TSL2591.IR = MAKEWORD(DATA[2], DATA[3]);
	err = calculateLux(TSL2591.Full, TSL2591.IR);

	#if defined(VERBOSE)
		if (err == ERROR_OVERFLOW)	{ printf("TSL2591; Sensor saturation reached!\r\n"); }
		else						{ printf("TSL2591: Full %d IR %d Lux: %lul\r\n", TSL2591.Full, TSL2591.IR, TSL2591.Lux); }
	#endif

	if (TSL2591.cfg.AIEN)
	{
		err = TSL2591_SF_Clear_IT();
		if (err)	{ return err; }
	}

	return ERROR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
