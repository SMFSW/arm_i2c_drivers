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


TCS3472_t TCS3472[I2C_TCS3472_NB];

const uint16_t TCS3472_gain_tab[4] = { 1, 4, 16, 60 };


/****************************************************************/


__WEAK FctERR NONNULL__ TCS3472_Init_Sequence(TCS3472_t * pCpnt)
{
	uTCS3472_REG__ENABLE	EN = { 0 };
	FctERR					err = ERROR_OK;

	pCpnt->cfg.Gain = TCS3472__LOW_GAIN;
	pCpnt->cfg.Integ = 200;
	pCpnt->cfg.Wait = 1000;
	pCpnt->cfg.LowThreshold = 0x8FF;
	pCpnt->cfg.HighThreshold = 0x8FF;
	pCpnt->cfg.AIEN = true;
	pCpnt->cfg.WEN = true;

	memcpy(&pCpnt->cfg.mat, &CLS_RGB2XYZ_Default, sizeof(pCpnt->cfg.mat));

	// get ID & check against values for TCS3472
	err = TCS3472_Get_ChipID(pCpnt, &pCpnt->cfg.ID);
	if (err)			{ return err; }

	if (	(pCpnt->cfg.ID != TCS34725_CHIP_ID)
		&&	(pCpnt->cfg.ID != TCS34727_CHIP_ID))
	{ return ERROR_COMMON; }	// Unknown device

	EN.Bits.PON = true;		// Turn ON Osc
	err = TCS3472_Write_En(pCpnt, EN.Byte);
	if (err)			{ return err; }

	err = TCS3472_Set_Gain(pCpnt, pCpnt->cfg.Gain);
	if (err)			{ return err; }
	err = TCS3472_Set_Integration_Time(pCpnt, pCpnt->cfg.Integ);
	if (err)			{ return err; }
	err = TCS3472_Set_Wait_Time(pCpnt, pCpnt->cfg.Wait);
	if (err)			{ return err; }

	err = TCS3472_Set_AIT(pCpnt, pCpnt->cfg.LowThreshold, pCpnt->cfg.HighThreshold);
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
static FctERR NONNULL__ TCS3472_calc(TCS3472_t * pCpnt, const uint16_t r, const uint16_t g, const uint16_t b)
{
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

	CLS_get_chromacity(pCpnt->xy, &pCpnt->Lux, pCpnt->cfg.mat, r, g, b);
	CLS_get_CCT(&pCpnt->Temp, pCpnt->xy);

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
		else						{ printf("TCS3472: C%d R%d G%d B%d x%d.%04d y%d.%04d Lux: %lul Temp: %luK\r\n",
											pCpnt->Clear, pCpnt->Red, pCpnt->Green, pCpnt->Blue,
											(uint16_t) pCpnt->xy[0], get_fp_dec(pCpnt->xy[0], 4),
											(uint16_t) pCpnt->xy[1], get_fp_dec(pCpnt->xy[1], 4),
											pCpnt->Lux, pCpnt->Temp); }
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
