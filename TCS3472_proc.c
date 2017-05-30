/*!\file TCS3472_proc.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief TCS3472 Driver procedures code
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
// TODO: doxygen for static functions


TCS3472_proc TCS3472 = { 0, 0, 0, 0, 0, 0, 0.0f, 0.0f, false, false, { TCS3472__LOW_GAIN, 200, 1000, 0x8FF, 0x8FF, true, true, 0 } };

const uint16_t TCS3472_gain_tab[4] = { 1, 4, 16, 60 };


/****************************************************************/


FctERR TCS3472_Init_Sequence(void)
{
	uTCS3472_REG__ENABLE	EN;
	FctERR					err = ERR_OK;

	// get ID & check against values for TCS3472
	err = TCS3472_Get_ChipID(&TCS3472.cfg.Id);
	if (err)			{ return err; }

	if (	(TCS3472.cfg.Id != TCS34725_CHIP_ID)
		&&	(TCS3472.cfg.Id != TCS34727_CHIP_ID))
	{ return ERR_COMMON; }	// Unknown device

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


/**!\brief  Converts the RGB values to color temperature in degrees Kelvin
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
		return ERR_OVERFLOW;	// Saturation reached
	}
	else { TCS3472.Saturation = false; }

	// Check for ripple saturation
	sat = (uint16_t) (sat * 0.75f);
	if ((r >= sat) || (g >= sat) || (b >= sat))	{ TCS3472.SaturationRipple = true; }
	else										{ TCS3472.SaturationRipple = false; }

	// Map RGB values to XYZ space
	// Based on 6500K fluorescent, 3000K fluorescent and 60W incandescent values for a wide range.
	// Note: Y = Illuminance or lux
	X = (-0.14282f * r) + (1.54924f * g) + (-0.95641f * b);
	Y = (-0.32466f * r) + (1.57837f * g) + (-0.73191f * b);
	Z = (-0.68202f * r) + (0.77073f * g) + ( 0.56332f * b);

	// Calculate the chromaticity coordinates
	xc = X / (X + Y + Z);
	yc = Y / (X + Y + Z);

	// Use McCamy's formula to determine the CCT
	n = (xc - 0.3320f) / (0.1858f - yc);
	TCS3472.Temp = (uint32_t) ((449.0f * powf(n, 3)) + (3525.0f * powf(n, 2)) + (6823.3f * n) + 5520.33f);
	TCS3472.Lux = (uint32_t) Y;

	return ERR_OK;
}


FctERR TCS3472_handler(void)
{
	uint8_t	DATA[8];
	FctERR	err;

	err = TCS3472_Read(DATA, TCS3472__CDATAL, 8);
	if (err)	{ return err; }

	TCS3472.Clear = (DATA[1] * 0x100) + DATA[0];
	TCS3472.Red = (DATA[3] * 0x100) + DATA[2];
	TCS3472.Green = (DATA[5] * 0x100) + DATA[4];
	TCS3472.Blue = (DATA[7] * 0x100) + DATA[6];
	err = TCS3472_calc(TCS3472.Red, TCS3472.Green, TCS3472.Blue);

	#if defined(VERBOSE)
		if (err == ERR_OVERFLOW)	{ printf("TCS3472: Sensor saturation reached!"); }
		else						{ printf("TCS3472: C%d R%d G%d B%d Lux: %lul Temp: %luK\r\n", TCS3472.Clear, TCS3472.Red, TCS3472.Green, TCS3472.Blue, TCS3472.Lux, TCS3472.Temp); }
	#endif

	if (TCS3472.cfg.AIEN)
	{
		TCS3472_SF_Clear_IT();
		if (err)	{ return err; }
	}

	return ERR_OK;
}

/****************************************************************/
#endif
#endif
/****************************************************************/
