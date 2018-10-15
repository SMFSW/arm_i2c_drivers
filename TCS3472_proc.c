/*!\file TCS3472_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief TCS3472 Driver procedures
** \details TCS3472: Color light-to-digital converter with IR filter
**/
/****************************************************************/
#include "TCS3472.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TCS3472)
/****************************************************************/
// std libs
#include <math.h>
/****************************************************************/


TCS3472_t TCS3472[I2C_TCS3472_NB];

const uint16_t TCS3472_gain_tab[4] = { 1, 4, 16, 60 };


/****************************************************************/


__WEAK FctERR NONNULL__ TCS3472_Init_Sequence(TCS3472_t * pCpnt)
{
	uTCS3472_REG__ENABLE	EN;
	FctERR					err = ERROR_OK;

	pCpnt->cfg.Gain = TCS3472__LOW_GAIN;
	pCpnt->cfg.Integ = 200;
	pCpnt->cfg.Wait = 1000;
	pCpnt->cfg.LowThreshold = 0x8FF;
	pCpnt->cfg.HighThreshold = 0x8FF;
	pCpnt->cfg.AIEN = true;
	pCpnt->cfg.WEN = true;

	// get ID & check against values for TCS3472
	err = TCS3472_Get_ChipID(pCpnt, &pCpnt->cfg.ID);
	if (err)			{ return err; }

	if (	(pCpnt->cfg.ID != TCS34725_CHIP_ID)
		&&	(pCpnt->cfg.ID != TCS34727_CHIP_ID))
	{ return ERROR_COMMON; }	// Unknown device

	EN.Byte = 0;
	EN.Bits.PON = true;		// Turn ON Osc
	err = TCS3472_Write_En(pCpnt, EN.Byte);
	if (err)			{ return err; }

	err = TCS3472_Set_Gain(pCpnt, pCpnt->cfg.Gain);
	if (err)			{ return err; }
	err = TCS3472_Set_Integration_Time(pCpnt, pCpnt->cfg.Integ);
	if (err)			{ return err; }
	err = TCS3472_Set_Wait_Time(pCpnt, pCpnt->cfg.Wait);
	if (err)			{ return err; }

	err = TCS3472_Set_AILT(pCpnt, pCpnt->cfg.LowThreshold);
	if (err)			{ return err; }
	err = TCS3472_Set_AIHT(pCpnt, pCpnt->cfg.HighThreshold);
	if (err)			{ return err; }

	EN.Bits.AEN = true;					// Turn ON ALS
	EN.Bits.AIEN = pCpnt->cfg.AIEN;		// Turn ALS interrupts depending cfg
	EN.Bits.WEN = pCpnt->cfg.WEN;		// Turn WAIT on depending cfg
	return TCS3472_Write_En(pCpnt, EN.Byte);
}


/****************************************************************/


/*!\brief Converts the RGB values to color temperature in Kelvin degrees
** \param[in] r - Red value
** \param[in] g - Green value
** \param[in] b - Blue value
** \return FctERR - error code
**/
static FctERR NONNULL__ TCS3472_calc(TCS3472_t * pCpnt, uint16_t r, uint16_t g, uint16_t b)
{
	float		X, Y, Z;	// RGB to XYZ correlation
	float		xc, yc;		// Chromaticity coordinates
	float		n;			// McCamy's formula
	// SATURATION = 1024 * (256 - ATIME_ms) if ATIME_ms > 153ms
	uint16_t	sat = (pCpnt->cfg.Integ > 153) ? 65535 : 1024 * (256 - pCpnt->cfg.Integ);

	// Check for saturation
	if ((r >= sat) || (g >= sat) || (b >= sat))
	{
		pCpnt->Saturation = true;
		return ERROR_OVERFLOW;	// Saturation reached
	}
	else { pCpnt->Saturation = false; }

	// Check for ripple saturation
	sat = (uint16_t) (sat * 0.75f);
	if ((r >= sat) || (g >= sat) || (b >= sat))	{ pCpnt->SaturationRipple = true; }
	else										{ pCpnt->SaturationRipple = false; }

	// Convert RGB to XYZ (based on TAOS DN25 application note)
	// These equations are the result of a transformation matrix composed of correlations at different light sources
	X = (-0.14282f * r) + (1.54924f * g) + (-0.95641f * b);
	Y = (-0.32466f * r) + (1.57837f * g) + (-0.73191f * b);		// Note: Y = Illuminance (lux)
	Z = (-0.68202f * r) + (0.77073f * g) + ( 0.56332f * b);

	// Calculate the chromaticity coordinates
	xc = X / (X + Y + Z);
	yc = Y / (X + Y + Z);

	// Use McCamy's formula to determine the CCT (original formula, not taken from TAOS DN25 application note)
	n = (xc - 0.3320f) / (yc - 0.1858f);
	pCpnt->Temp = (uint32_t) ((449.0 * powf(n, 3)) + (3525.0 * powf(n, 2)) + (6823.3 * n) + 5520.33);
	//pCpnt->Temp = (uint32_t) ((6253.80338 * exp(-n / 0.92159)) + (28.70599 * exp(-n / 0.20039)) + (0.00004 * exp(-n / 0.07125)) - 949.86315);
	pCpnt->Lux = (uint32_t) Y;

	return ERROR_OK;
}


__WEAK FctERR NONNULL__ TCS3472_handler(TCS3472_t * pCpnt)
{
	uint8_t	DATA[8];
	FctERR	err;

	err = TCS3472_Read(pCpnt->cfg.slave_inst, DATA, TCS3472__CDATAL, 8);
	if (err)	{ return err; }

	pCpnt->Clear = MAKEWORD(DATA[0], DATA[1]);
	pCpnt->Red = MAKEWORD(DATA[2], DATA[3]);
	pCpnt->Green = MAKEWORD(DATA[4], DATA[5]);
	pCpnt->Blue = MAKEWORD(DATA[6], DATA[7]);
	err = TCS3472_calc(pCpnt, pCpnt->Red, pCpnt->Green, pCpnt->Blue);

	#if defined(VERBOSE)
		if (err == ERROR_OVERFLOW)	{ printf("TCS3472: Sensor saturation reached!\r\n"); }
		else						{ printf("TCS3472: C%d R%d G%d B%d Lux: %lul Temp: %luK\r\n", pCpnt->Clear, pCpnt->Red, pCpnt->Green, pCpnt->Blue, pCpnt->Lux, pCpnt->Temp); }
	#endif

	if (pCpnt->cfg.AIEN)
	{
		err = TCS3472_SF_Clear_IT(pCpnt);
		if (err)	{ return err; }
	}

	return ERROR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
