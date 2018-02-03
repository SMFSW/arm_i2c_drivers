/*!\file TCS3472_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief TCS3472 Driver procedures
** \details TCS3472: Color light-to-digital converter with IR filter
**/
/****************************************************************/
#include "TCS3472.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TCS3472)
/****************************************************************/
// std libs
#include <math.h>
#include <string.h>
/****************************************************************/


TCS3472_proc TCS3472 = { 0, 0, 0, 0, 0, 0, 0.0f, 0.0f, false, false, { TCS3472__LOW_GAIN, 200, 1000, 0x8FF, 0x8FF, true, true, 0 } };

const uint16_t TCS3472_gain_tab[4] = { 1, 4, 16, 60 };


/****************************************************************/


__WEAK FctERR TCS3472_Init_Sequence(void)
{
	uTCS3472_REG__ENABLE	EN;
	FctERR					err = ERROR_OK;

	// get ID & check against values for TCS3472
	err = TCS3472_Get_ChipID(&TCS3472.cfg.Id);
	if (err)			{ return err; }

	if (	(TCS3472.cfg.Id != TCS34725_CHIP_ID)
		&&	(TCS3472.cfg.Id != TCS34727_CHIP_ID))
	{ return ERROR_COMMON; }	// Unknown device

	EN.Byte = 0;
	EN.Bits.PON = true;		// Turn ON Osc
	err = TCS3472_Write_En(EN.Byte);
	if (err)			{ return err; }

	err = TCS3472_Set_Gain(TCS3472.cfg.Gain);
	if (err)			{ return err; }
	err = TCS3472_Set_Integration_Time(TCS3472.cfg.Integ);
	if (err)			{ return err; }
	err = TCS3472_Set_Wait_Time(TCS3472.cfg.Wait);
	if (err)			{ return err; }

	err = TCS3472_Set_AILT(TCS3472.cfg.LowThreshold);
	if (err)			{ return err; }
	err = TCS3472_Set_AIHT(TCS3472.cfg.HighThreshold);
	if (err)			{ return err; }

	EN.Bits.AEN = true;					// Turn ON ALS
	EN.Bits.AIEN = TCS3472.cfg.AIEN;	// Turn ALS interrupts depending cfg
	EN.Bits.WEN = TCS3472.cfg.WEN;		// Turn WAIT on depending cfg
	return TCS3472_Write_En(EN.Byte);
}


/****************************************************************/


uint32_t TCS3472_Get_Temp(void) {
	return TCS3472.Temp; }

uint32_t TCS3472_Get_Lux(void) {
	return TCS3472.Lux; }


/*!\brief Converts the RGB values to color temperature in degrees Kelvin
** \param[in] r - Red value
** \param[in] g - Green value
** \param[in] b - Blue value
** \return FctERR - error code
**/
static FctERR TCS3472_calc(uint16_t r, uint16_t g, uint16_t b)
{
	float		X, Y, Z;	// RGB to XYZ correlation
	float		xc, yc;		// Chromaticity coordinates
	float		n;			// McCamy's formula
	// SATURATION = 1024 * (256 - ATIME_ms) if ATIME_ms > 153ms
	uint16_t	sat = (TCS3472.cfg.Integ > 153) ? 65535 : 1024 * (256 - TCS3472.cfg.Integ);

	// Check for saturation
	if ((r >= sat) || (g >= sat) || (b >= sat))
	{
		TCS3472.Saturation = true;
		return ERROR_OVERFLOW;	// Saturation reached
	}
	else { TCS3472.Saturation = false; }

	// Check for ripple saturation
	sat = (uint16_t) (sat * 0.75f);
	if ((r >= sat) || (g >= sat) || (b >= sat))	{ TCS3472.SaturationRipple = true; }
	else										{ TCS3472.SaturationRipple = false; }

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
	TCS3472.Temp = (uint32_t) ((-449.0 * powf(n, 3)) + (3525.0 * powf(n, 2)) - (6823.3 * n) + 5520.33);
	TCS3472.Lux = (uint32_t) Y;

	return ERROR_OK;
}


__WEAK FctERR TCS3472_handler(void)
{
	uint8_t	DATA[8];
	FctERR	err;

	err = TCS3472_Read(DATA, TCS3472__CDATAL, 8);
	if (err)	{ return err; }

	TCS3472.Clear = MAKEWORD(DATA[0], DATA[1]);
	TCS3472.Red = MAKEWORD(DATA[2], DATA[3]);
	TCS3472.Green = MAKEWORD(DATA[4], DATA[5]);
	TCS3472.Blue = MAKEWORD(DATA[6], DATA[7]);
	err = TCS3472_calc(TCS3472.Red, TCS3472.Green, TCS3472.Blue);

	#if defined(VERBOSE)
		if (err == ERROR_OVERFLOW)	{ printf("TCS3472: Sensor saturation reached!\r\n"); }
		else						{ printf("TCS3472: C%d R%d G%d B%d Lux: %lul Temp: %luK\r\n", TCS3472.Clear, TCS3472.Red, TCS3472.Green, TCS3472.Blue, TCS3472.Lux, TCS3472.Temp); }
	#endif

	if (TCS3472.cfg.AIEN)
	{
		TCS3472_SF_Clear_IT();
		if (err)	{ return err; }
	}

	return ERROR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
