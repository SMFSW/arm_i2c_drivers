/*!\file TCS3400_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief TCS3400 Driver procedures
** \details TCS3400: Color Light-to-Digital Converter
**/
/****************************************************************/
#include "TCS3400.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TCS3400)
/****************************************************************/


TCS3400_t TCS3400[I2C_TCS3400_NB] = { 0 };

const uint16_t TCS3400_gain_tab[4] = { 1U, 4U, 16U, 64U };


/****************************************************************/


__WEAK FctERR NONNULL__ TCS3400_Init_Sequence(TCS3400_t * const pCpnt)
{
	uTCS3400_REG__ENABLE	EN = { 0 };
	FctERR					err;

	// get IDs & check against values for TCS3400
	err = TCS3400_Get_RevID(pCpnt, &pCpnt->cfg.RevID);
	if (err != ERROR_OK)	{ goto ret; }
	err = TCS3400_Get_DeviceID(pCpnt, &pCpnt->cfg.ChipID);
	if (err != ERROR_OK)	{ goto ret; }
	if (	(pCpnt->cfg.ChipID != TCS34005_CHIP_ID)
		&&	(pCpnt->cfg.ChipID != TCS34007_CHIP_ID))	{ err = ERROR_COMMON; goto ret; }	// Unknown device

	pCpnt->cfg.Gain = TCS3400__LOW_GAIN;
	pCpnt->cfg.Integ = 200U;
	pCpnt->cfg.Wait = 1000U;
	pCpnt->cfg.LowThreshold = 0x8FFU;
	pCpnt->cfg.HighThreshold = 0x8FFU;
	pCpnt->cfg.AIEN = true;
	pCpnt->cfg.WEN = true;

	UNUSED_RET memcpy(&pCpnt->cfg.mat, &CLS_RGB2XYZ_Default, sizeof(pCpnt->cfg.mat));

	EN.Bits.PON = true;		// Turn ON Osc

	err = TCS3400_Write_En(pCpnt, EN.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	err = TCS3400_Set_Gain(pCpnt, pCpnt->cfg.Gain);
	if (err != ERROR_OK)	{ goto ret; }

	err = TCS3400_Set_Integration_Time(pCpnt, pCpnt->cfg.Integ);
	if (err != ERROR_OK)	{ goto ret; }

	err = TCS3400_Set_Wait_Time(pCpnt, pCpnt->cfg.Wait);
	if (err != ERROR_OK)	{ goto ret; }

	err = TCS3400_Set_AIT(pCpnt, pCpnt->cfg.LowThreshold, pCpnt->cfg.HighThreshold);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.WEN = pCpnt->cfg.WEN;		// Turn ON WAIT following cfg
	EN.Bits.AEN = pCpnt->cfg.AIEN;		// Turn ON ALS following cfg
	EN.Bits.AIEN = pCpnt->cfg.AIEN;		// Turn ON ALS interrupts following cfg
	err = TCS3400_Write_En(pCpnt, EN.Byte);

	ret:
	return err;
}


/****************************************************************/


/*!\brief Converts the RGB values to color temperature in degrees Kelvin
** \param[in] r - Red value
** \param[in] g - Green value
** \param[in] b - Blue value
** \return FctERR - error code
**/
static FctERR NONNULL__ TCS3400_calc(TCS3400_t * const pCpnt, const uint16_t r, const uint16_t g, const uint16_t b)
{
	FctERR err;

	// SATURATION = 1024 * (256 - ATIME) if ATIME > 192 (<178ms)
	uint16_t sat = (pCpnt->cfg.Integ_reg <= 192U) ? 65535U : 1024U * (256U - pCpnt->cfg.Integ_reg);

	// Check for saturation
	if ((r >= sat) || (g >= sat) || (b >= sat))
	{
		pCpnt->Saturation = true;
		err = ERROR_OVERFLOW;	// Saturation reached
	}
	else
	{
		pCpnt->Saturation = false;

		// Check for ripple saturation
		sat = (uint16_t) (sat * 0.75f);
		pCpnt->SaturationRipple = binEval((r >= sat) || (g >= sat) || (b >= sat));

		err = CLS_get_chromacity(pCpnt->xy, &pCpnt->Lux, pCpnt->cfg.mat, r, g, b);
		err |= CLS_get_CCT(&pCpnt->Temp, pCpnt->xy);
	}

	return err;
}


__WEAK FctERR NONNULL__ TCS3400_handler(TCS3400_t * const pCpnt)
{
	uint8_t								DATA[9];
	const uTCS3400_REG__STATUS * const	ST = (const uTCS3400_REG__STATUS *) DATA;

	FctERR err = TCS3400_Read(pCpnt->cfg.slave_inst, DATA, TCS3400__STATUS, sizeof(DATA));
	if (err != ERROR_OK)	{ goto ret; }

	if ((ST->Bits.AINT) && (ST->Bits.AVALID))
	{
		pCpnt->Clear = MAKEWORD(DATA[1], DATA[2]);
		pCpnt->Red = MAKEWORD(DATA[3], DATA[4]);
		pCpnt->Green = MAKEWORD(DATA[5], DATA[6]);
		pCpnt->Blue = MAKEWORD(DATA[7], DATA[8]);
		err = TCS3400_calc(pCpnt, pCpnt->Red, pCpnt->Green, pCpnt->Blue);

		#if defined(VERBOSE)
			const uint8_t idx = pCpnt - TCS3400;
			if (err == ERROR_OVERFLOW)	{ UNUSED_RET printf("TCS3400 id%d: Sensor saturation reached!\r\n", idx); }
			else						{ UNUSED_RET printf("TCS3400 id%d: C%d R%d G%d B%d x%d.%04ld y%d.%04ld Lux: %lul Temp: %luK\r\n",
															idx, pCpnt->Clear, pCpnt->Red, pCpnt->Green, pCpnt->Blue,
															(uint16_t) pCpnt->xy[0], get_fp_dec(pCpnt->xy[0], 4U),
															(uint16_t) pCpnt->xy[1], get_fp_dec(pCpnt->xy[1], 4U),
															pCpnt->Lux, pCpnt->Temp); }
		#endif
	}

	if (ST->Bits.AINT)	{ err = TCS3400_Clear_All_IT(pCpnt); }

	ret:
	return err;
}


__WEAK FctERR NONNULL__ TCS3400_handler_it(TCS3400_t * const pCpnt)
{
	FctERR err = ERROR_OK;

	if (TCS3400_INT_GPIO_Get(pCpnt))	{ err = TCS3400_handler(pCpnt); }

	return err;
}


FctERR TCS3400_handler_all(void)
{
	FctERR err = ERROR_OK;

	for (TCS3400_t * pCpnt = TCS3400 ; pCpnt < &TCS3400[I2C_TCS3400_NB] ; pCpnt++)
	{
		err |= TCS3400_handler(pCpnt);
	}

	return err;
}

FctERR TCS3400_handler_it_all(void)
{
	FctERR err = ERROR_OK;

	for (TCS3400_t * pCpnt = TCS3400 ; pCpnt < &TCS3400[I2C_TCS3400_NB] ; pCpnt++)
	{
		err |= TCS3400_handler_it(pCpnt);
	}

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
