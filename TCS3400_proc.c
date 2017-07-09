/*!\file TCS3400_proc.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief TCS3400 Driver procedures code
** \details TCS3400: Color Light-to-Digital Converter
**/
/****************************************************************/
#include "TCS3400.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TCS3400)
/****************************************************************/
// std libs
#include <math.h>
#include <string.h>
/****************************************************************/


TCS3400_proc TCS3400 = { 0, 0, 0, 0, 0, 0, 0.0f, 0.0f, false, false, { TCS3400__LOW_GAIN, 200, 1000, 0x8FF, 0x8FF,  true, true, 0, 0 } };

const uint16_t TCS3400_gain_tab[4] = { 1, 4, 16, 64 };


/****************************************************************/


FctERR TCS3400_Init_Sequence(void)
{
	uTCS3400_REG__ENABLE	EN;
	FctERR					err = ERR_OK;

	err = TCS3400_Get_RevID(&TCS3400.cfg.Revision_Id);
	if (err)			{ return err; }

	// get ID & check against values for TCS3400
	err = TCS3400_Get_DeviceID(&TCS3400.cfg.Device_Id);
	if (err)			{ return err; }

	if (	(TCS3400.cfg.Device_Id != TCS34005_CHIP_ID)
		&&	(TCS3400.cfg.Device_Id != TCS34007_CHIP_ID))
	{ return ERR_COMMON; }	// Unknown device

	EN.Byte = 0;
	EN.Bits.PON = true;		// Turn ON Osc
	err = TCS3400_Write_En(EN.Byte);
	if (err)			{ return err; }

	err = TCS3400_Set_Gain(TCS3400.cfg.Gain);
	if (err)			{ return err; }
	err = TCS3400_Set_Integration_Time(TCS3400.cfg.Integ);
	if (err)			{ return err; }
	err = TCS3400_Set_Wait_Time(TCS3400.cfg.Wait);
	if (err)			{ return err; }

	err = TCS3400_Set_AILT(TCS3400.cfg.LowThreshold);
	if (err)			{ return err; }
	err = TCS3400_Set_AIHT(TCS3400.cfg.HighThreshold);
	if (err)			{ return err; }

	EN.Bits.AEN = true;					// Turn ON ALS
	EN.Bits.AIEN = TCS3400.cfg.AIEN;	// Turn ON ALS interrupts
	EN.Bits.WEN = TCS3400.cfg.WEN;		// Turn WAIT on depending cfg
	return TCS3400_Write_En(EN.Byte);
}


/****************************************************************/


uint32_t TCS3400_Get_Temp(void) {
	return TCS3400.Temp; }

uint32_t TCS3400_Get_Lux(void) {
	return TCS3400.Lux; }


/*!\brief Converts the RGB values to color temperature in degrees Kelvin
** \param[in] r - Red value
** \param[in] g - Green value
** \param[in] b - Blue value
** \return FctERR - error code
**/
static FctERR TCS3400_calc(uint16_t r, uint16_t g, uint16_t b)
{
	float		X, Y, Z;	// RGB to XYZ correlation
	float		xc, yc;		// Chromaticity coordinates
	float		n;			// McCamy's formula
	// SATURATION = 1024 * (256 - ATIME_ms) if ATIME_ms > 192ms
	uint16_t	sat = (TCS3400.cfg.Integ > 192) ? 65535 : 1024 * (256 - TCS3400.cfg.Integ);

	// Check for saturation
	if ((r >= sat) || (g >= sat) || (b >= sat))
	{
		TCS3400.Saturation = true;
		return ERR_OVERFLOW;	// Saturation reached
	}
	else { TCS3400.Saturation = false; }

	// Check for ripple saturation
	sat = (uint16_t) (sat * 0.75f);
	if ((r >= sat) || (g >= sat) || (b >= sat))	{ TCS3400.SaturationRipple = true; }
	else										{ TCS3400.SaturationRipple = false; }

	// Convert RGB to XYZ (based on TAOS DN25 application note)
	// These equations are the result of a transformation matrix composed of correlations at different light sources
	X = (-0.14282f * r) + (1.54924f * g) + (-0.95641f * b);
	Y = (-0.32466f * r) + (1.57837f * g) + (-0.73191f * b);	// Note: Y = Illuminance (lux)
	Z = (-0.68202f * r) + (0.77073f * g) + ( 0.56332f * b);

	// Calculate chromaticity coordinates (from XYZ)
	xc = X / (X + Y + Z);
	yc = Y / (X + Y + Z);

	// Use McCamy's formula to determine the CCT (original formula, not taken from TAOS DN25 application note)
	n = (xc - 0.3320f) / (yc - 0.1858f);
	TCS3400.Temp = (uint32_t) ((-449.0 * powf(n, 3)) + (3525.0 * powf(n, 2)) - (6823.3 * n) + 5520.33);
	TCS3400.Lux = (uint32_t) Y;

	return ERR_OK;
}


FctERR TCS3400_handler(void)
{
	uint8_t	DATA[8];
	FctERR	err;

	err = TCS3400_Read(DATA, TCS3400__CDATAL, 8);
	if (err)	{ return err; }

	TCS3400.Clear = (DATA[1] * 0x100) + DATA[0];
	TCS3400.Red = (DATA[3] * 0x100) + DATA[2];
	TCS3400.Green = (DATA[5] * 0x100) + DATA[4];
	TCS3400.Blue = (DATA[7] * 0x100) + DATA[6];
	err = TCS3400_calc(TCS3400.Red, TCS3400.Green, TCS3400.Blue);

	#if defined(VERBOSE)
		if (err == ERR_OVERFLOW)	{ printf("TCS3400: Sensor saturation reached!\r\n"); }
		else						{ printf("TCS3400: C%d R%d G%d B%d Lux: %lul Temp: %luK\r\n", TCS3400.Clear, TCS3400.Red, TCS3400.Green, TCS3400.Blue, TCS3400.Lux, TCS3400.Temp); }
	#endif

	if (TCS3400.cfg.AIEN)
	{
		TCS3400_Clear_All_IT();
		if (err)	{ return err; }
	}

	return ERR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
