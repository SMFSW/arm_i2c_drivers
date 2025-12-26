/*!\file APDS9930_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief APDS9930 Driver procedures
** \details APDS9930: Digital Proximity and Ambient Light Sensor
**/
/****************************************************************/
#include "APDS9930.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_APDS9930)
/****************************************************************/


APDS9930_t APDS9930[I2C_APDS9930_NB] = { 0 };

const uint8_t APDS9930_ALS_gain_tab[4] = { 1, 8, 16, 120 };
const uint8_t APDS9930_Prox_gain_tab[4] = { 1, 2, 4, 8 };


/****************************************************************/


__WEAK FctERR NONNULL__ APDS9930_Init_Sequence(APDS9930_t * const pCpnt)
{
	uAPDS9930_REG__ENABLE	EN = { 0 };
	FctERR					err = ERROR_OK;

	pCpnt->cfg.GA = APDS9930_GLASS_ATTENUATION;

	pCpnt->cfg.ALS_Gain = APDS9930__ALS_GAIN_16X;
	pCpnt->cfg.Prox_Gain = APDS9930__PROX_2X_GAIN;
	pCpnt->cfg.ALS_Integ = 100;
	pCpnt->cfg.Prox_Integ = 3;	// Value recommended in datasheet
	pCpnt->cfg.Wait = 1000;
	pCpnt->cfg.ALS_LowThreshold = 0x8FF;
	pCpnt->cfg.ALS_HighThreshold = 0x8FF;
	pCpnt->cfg.Prox_LowThreshold = 0x8FF;
	pCpnt->cfg.Prox_HighThreshold = 0x8FF;
	pCpnt->cfg.Prox_Pulses = APDS9930_DEF_PROX_PULSES;
	pCpnt->cfg.Prox_Strength = APDS9930__STRENGTH_11_1MA;
	pCpnt->cfg.AIEN = true;
	pCpnt->cfg.PIEN = true;
	pCpnt->cfg.WEN = true;

	// get ID & check against values for APDS9930
	err = APDS9930_Get_ChipID(pCpnt, &pCpnt->cfg.Id);
	if (err != ERROR_OK)								{ return err; }
	if (pCpnt->cfg.Id != APDS9930_CHIP_ID)	{ return ERROR_COMMON; }	// Unknown device

	EN.Bits.PON = true;		// Turn ON Osc
	err = APDS9930_Write_En(pCpnt, EN.Byte);
	if (err != ERROR_OK)	{ return err; }

	err = APDS9930_Set_ALS_Gain(pCpnt, pCpnt->cfg.ALS_Gain);
	if (err != ERROR_OK)	{ return err; }
	err = APDS9930_Set_Prox_Gain(pCpnt, pCpnt->cfg.Prox_Gain);
	if (err != ERROR_OK)	{ return err; }

	err = APDS9930_Set_Prox_Drive_Strength(pCpnt, pCpnt->cfg.Prox_Strength);
	if (err != ERROR_OK)	{ return err; }
	err = APDS9930_Write(pCpnt->cfg.slave_inst, &pCpnt->cfg.Prox_Pulses, APDS9930__PPULSE, 1);
	if (err != ERROR_OK)	{ return err; }

	err = APDS9930_Set_ALS_Integration_Time(pCpnt, pCpnt->cfg.ALS_Integ);
	if (err != ERROR_OK)	{ return err; }
	err = APDS9930_Set_Prox_Integration_Time(pCpnt, pCpnt->cfg.Prox_Integ);
	if (err != ERROR_OK)	{ return err; }
	err = APDS9930_Set_Wait_Time(pCpnt, pCpnt->cfg.Wait);
	if (err != ERROR_OK)	{ return err; }

	APDS9930_Set_LPC(pCpnt);

	err = APDS9930_Set_AIT(pCpnt, pCpnt->cfg.ALS_LowThreshold, pCpnt->cfg.ALS_HighThreshold);
	err = APDS9930_Set_PIT(pCpnt, pCpnt->cfg.Prox_LowThreshold, pCpnt->cfg.Prox_HighThreshold);

	EN.Bits.WEN = pCpnt->cfg.WEN;	// Turn ON Wait following cfg
	EN.Bits.AEN = pCpnt->cfg.AIEN;	// Turn ON ALS following cfg
	EN.Bits.PEN = pCpnt->cfg.PIEN;	// Turn ON Proximity following cfg
	EN.Bits.AIEN = pCpnt->cfg.AIEN;	// Turn ON ALS interrupts following cfg
	EN.Bits.PIEN = pCpnt->cfg.PIEN;	// Turn ON Proximity interrupts following cfg
	return APDS9930_Write_En(pCpnt, EN.Byte);
}


/****************************************************************/


void NONNULL__ APDS9930_Set_LPC(APDS9930_t * const pCpnt)
{
	float gain = (float) APDS9930_ALS_gain_tab[pCpnt->cfg.ALS_Gain % APDS9930__ALS_GAIN_1ON6X];
	if (pCpnt->cfg.ALS_Gain >= APDS9930__ALS_GAIN_1ON6X)	{ gain /= 8.0f; }

	// LPC = (GA * DF) / (ATIME_ms * AGAINx)	(Lux per Count)
	pCpnt->cfg.LPC = (pCpnt->cfg.GA * APDS9930_DEVICE_FACTOR) / (pCpnt->cfg.ALS_Integ * gain);
}


/*!\brief Computes an approximation of Illuminance (in lux) from Full and IR values
** \param[in] full - Red value
** \param[in] ir - Green value
** \return FctERR - error code
**/
static NONNULL__ FctERR APDS9930_calc(APDS9930_t * const pCpnt, const uint16_t full, const uint16_t ir)
{
	const float	B = 1.862f, C = 0.746f, D = 1.291f;
	// SATURATION = 1024 * (256 - ATIME) if ATIME > 192 (<175ms)
	uint16_t	sat = (pCpnt->cfg.ALS_Integ_reg <= 192) ? 65535 : (1024 * (256 - pCpnt->cfg.ALS_Integ_reg));

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

	float IAC1 = full - (B * ir);			// IAC1 = (C0DATA - (B * C1DATA))
	float IAC2 = (C * full) - (D * ir);		// IAC2 = (C * C0DATA) - (D * C1DATA)
	float IAC = max(0, max(IAC1, IAC2));	// IAC = MAX(IAC1, IAC2, 0)

	pCpnt->Lux = IAC * pCpnt->cfg.LPC;		// Lux = IAC * LPC
	pCpnt->IRF = IAC / full;				// IRF = IAC / C0DATA (Infra Red Factor)

	return ERROR_OK;
}


__WEAK FctERR NONNULL__ APDS9930_handler(APDS9930_t * const pCpnt)
{
	uint8_t					DATA[7];
	uAPDS9930_REG__STATUS *	ST = (uAPDS9930_REG__STATUS *) DATA;
	#if defined(VERBOSE)
		const uint8_t		idx = pCpnt - APDS9930;
	#endif

	FctERR err = APDS9930_Read(pCpnt->cfg.slave_inst, DATA, APDS9930__STATUS, sizeof(DATA));
	if (err != ERROR_OK)	{ goto ret; }

	if (pCpnt->cfg.AIEN)
	{
		if ((ST->Bits.AINT) && (ST->Bits.AVALID))
		{
			pCpnt->Full = MAKEWORD(DATA[1], DATA[2]);
			pCpnt->IR = MAKEWORD(DATA[3], DATA[4]);
			err = APDS9930_calc(pCpnt, pCpnt->Full, pCpnt->IR);

			#if defined(VERBOSE)
				if (err == ERROR_OVERFLOW)	{ printf("APDS9930 id%d: ALS Sensor saturation reached!\r\n", idx); }
				else						{ printf("APDS9930 id%d: Full %d IR %d Lux: %lul\r\n", idx, pCpnt->Full, pCpnt->IR, pCpnt->Lux); }
			#endif
		}
	}

	if (pCpnt->cfg.PIEN)
	{
		if ((ST->Bits.PINT) && (ST->Bits.PVALID))
		{
			pCpnt->Prox = MAKEWORD(DATA[5], DATA[6]);

			#if defined(VERBOSE)
				printf("APDS9930 id%d: Prox %d\r\n", idx, pCpnt->Prox);
			#endif
		}
	}

	if ((pCpnt->cfg.AIEN) && (pCpnt->cfg.PIEN))	{ err = APDS9930_SF_Clear_ALS_PROX_IT(pCpnt); }
	else if (pCpnt->cfg.AIEN)					{ err = APDS9930_SF_Clear_ALS_IT(pCpnt); }
	else if (pCpnt->cfg.PIEN)					{ err = APDS9930_SF_Clear_PROX_IT(pCpnt); }

	ret:
	return err;
}


__WEAK FctERR NONNULL__ APDS9930_handler_it(APDS9930_t * const pCpnt)
{
	FctERR err = ERROR_OK;

	if (APDS9930_INT_GPIO_Get(pCpnt))	{ err = APDS9930_handler(pCpnt); }

	return err;
}


FctERR APDS9930_handler_all(void)
{
	FctERR err = ERROR_OK;

	for (APDS9930_t * pCpnt = APDS9930 ; pCpnt < &APDS9930[I2C_APDS9930_NB] ; pCpnt++)
	{
		err |= APDS9930_handler(pCpnt);
	}

	return err;
}

FctERR APDS9930_handler_it_all(void)
{
	FctERR err = ERROR_OK;

	for (APDS9930_t * pCpnt = APDS9930 ; pCpnt < &APDS9930[I2C_APDS9930_NB] ; pCpnt++)
	{
		err |= APDS9930_handler_it(pCpnt);
	}

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
