/*!\file APDS9960_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief APDS9960 Driver procedures
** \details APDS9960: Digital Proximity, Ambient Light, RGB and Gesture Sensor
**/
/****************************************************************/
#include "APDS9960.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_APDS9960)
/****************************************************************/


APDS9960_t APDS9960[I2C_APDS9960_NB] = { 0 };

const uint8_t APDS9960_ALS_gain_tab[4] = { 1U, 4U, 16U, 64U };
const uint8_t APDS9960_Prox_gain_tab[4] = { 1U, 2U, 4U, 8U };


/****************************************************************/


__WEAK FctERR NONNULL__ APDS9960_Init_Sequence(APDS9960_t * const pCpnt)
{
	uAPDS9960_REG__ENABLE	EN = { 0 };
	FctERR					err = ERROR_OK;

	// get ID & check against values for APDS9960
	err = APDS9960_Get_ChipID(pCpnt, &pCpnt->cfg.ChipID);
	if (err != ERROR_OK)						{ goto ret; }
	if (pCpnt->cfg.ChipID != APDS9960_CHIP_ID)	{ err = ERROR_COMMON; goto ret; }	// Unknown device

	pCpnt->cfg.ADC_Gain = APDS9960__ALS_16X_GAIN;
	pCpnt->cfg.Prox_Gain = APDS9960__PROX_2X_GAIN;
	pCpnt->cfg.ADC_Integ = 100U;
	pCpnt->cfg.Wait = 1000U;
	pCpnt->cfg.ADC_LowThreshold = 0x8FFU;
	pCpnt->cfg.ADC_HighThreshold = 0x8FFU;
	pCpnt->cfg.Prox_LowThreshold = 0x8FU;
	pCpnt->cfg.Prox_HighThreshold = 0x8FU;
	pCpnt->cfg.Prox_Pulses = 8U;
	pCpnt->cfg.Prox_Length = APDS9960__PL_8US;
	pCpnt->cfg.Prox_Strength = APDS9960__DS_12_5MA;
	pCpnt->cfg.AIEN = true;
	pCpnt->cfg.PIEN = true;
	pCpnt->cfg.GIEN = false;
	pCpnt->cfg.WEN = true;

	UNUSED_RET memcpy(&pCpnt->cfg.mat, &CLS_RGB2XYZ_Default, sizeof(pCpnt->cfg.mat));

	EN.Bits.PON = true;		// Turn ON Osc
	err = APDS9960_Write_En(pCpnt, EN.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	err = APDS9960_Set_ALS_Gain(pCpnt, pCpnt->cfg.ADC_Gain);
	if (err != ERROR_OK)	{ goto ret; }

	err = APDS9960_Set_Prox_Gain(pCpnt, pCpnt->cfg.Prox_Gain);
	if (err != ERROR_OK)	{ goto ret; }

	err = APDS9960_Set_Prox_Drive_Strength(pCpnt, pCpnt->cfg.Prox_Strength);
	if (err != ERROR_OK)	{ goto ret; }

	err = APDS9960_Write(pCpnt->cfg.slave_inst, &pCpnt->cfg.Prox_Pulses, APDS9960__PPULSE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	err = APDS9960_Set_ADC_Integration_Time(pCpnt, pCpnt->cfg.ADC_Integ);
	if (err != ERROR_OK)	{ goto ret; }

	err = APDS9960_Set_Wait_Time(pCpnt, pCpnt->cfg.Wait);
	if (err != ERROR_OK)	{ goto ret; }

	err = APDS9960_Set_AIT(pCpnt, pCpnt->cfg.ADC_LowThreshold, pCpnt->cfg.ADC_HighThreshold);
	if (err != ERROR_OK)	{ goto ret; }

	err = APDS9960_Set_PIT(pCpnt, pCpnt->cfg.Prox_LowThreshold, pCpnt->cfg.Prox_HighThreshold);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.WEN = pCpnt->cfg.WEN;	// Turn ON Wait following cfg
	EN.Bits.AEN = pCpnt->cfg.AIEN;	// Turn ON ALS following cfg
	EN.Bits.PEN = pCpnt->cfg.PIEN;	// Turn ON Proximity following cfg
	EN.Bits.GEN = pCpnt->cfg.GIEN;	// Turn ON Gesture following cfg
	EN.Bits.AIEN = pCpnt->cfg.AIEN;	// Turn ON ALS interrupts following cfg
	EN.Bits.PIEN = pCpnt->cfg.PIEN;	// Turn ON Proximity interrupts following cfg
	err = APDS9960_Write_En(pCpnt, EN.Byte);

	ret:
	return err;
}


/****************************************************************/


/*!\brief Converts the RGB values to color temperature in Kelvin degrees
** \param[in] r - Red value
** \param[in] g - Green value
** \param[in] b - Blue value
** \return FctERR - error code
**/
static FctERR NONNULL__ APDS9960_calc(APDS9960_t * const pCpnt, const uint16_t r, const uint16_t g, const uint16_t b)
{
	FctERR err;

	err = CLS_get_chromacity(pCpnt->xy, &pCpnt->Lux, pCpnt->cfg.mat, r, g, b);
	err |= CLS_get_CCT(&pCpnt->Temp, pCpnt->xy);

	return err;
}


__WEAK FctERR NONNULL__ APDS9960_handler(APDS9960_t * const pCpnt)
{
	uint8_t								DATA[10];
	const uAPDS9960_REG__STATUS * const	ST = (const uAPDS9960_REG__STATUS *) DATA;
	#if defined(VERBOSE)
		const uint8_t					idx = pCpnt - APDS9960;
	#endif

	FctERR err = APDS9960_Read(pCpnt->cfg.slave_inst, DATA, APDS9960__STATUS, sizeof(DATA));
	if (err != ERROR_OK)	{ goto ret; }

	if (pCpnt->cfg.AIEN)
	{
		if ((ST->Bits.AINT) && (ST->Bits.AVALID))
		{
			pCpnt->Clear = MAKEWORD(DATA[1], DATA[2]);
			pCpnt->Red = MAKEWORD(DATA[3], DATA[4]);
			pCpnt->Green = MAKEWORD(DATA[5], DATA[6]);
			pCpnt->Blue = MAKEWORD(DATA[7], DATA[8]);
			err = APDS9960_calc(pCpnt, pCpnt->Red, pCpnt->Green, pCpnt->Blue);

			#if defined(VERBOSE)
				UNUSED_RET printf("APDS9960 id%d: C%d R%d G%d B%d x%d.%04ld y%d.%04ld Lux: %lul Temp: %luK\r\n",
									idx, pCpnt->Clear, pCpnt->Red, pCpnt->Green, pCpnt->Blue,
									(uint16_t) pCpnt->xy[0], get_fp_dec(pCpnt->xy[0], 4),
									(uint16_t) pCpnt->xy[1], get_fp_dec(pCpnt->xy[1], 4),
									pCpnt->Lux, pCpnt->Temp);
			#endif
		}
	}

	if (pCpnt->cfg.PIEN)
	{
		if ((ST->Bits.PINT) && (ST->Bits.PVALID))
		{
			pCpnt->Prox = DATA[9];

			#if defined(VERBOSE)
				UNUSED_RET printf("APDS9960 id%d: Prox %d\r\n", idx, pCpnt->Prox);
			#endif
		}
	}

	if ((pCpnt->cfg.GIEN) && (ST->Bits.GINT))
	{
		uint8_t									GDATA[2], GDATAS[4];
		const uAPDS9960_REG__GSTATUS * const	GST = (const uAPDS9960_REG__GSTATUS *) &GDATA[1];

		err = APDS9960_Read(pCpnt->cfg.slave_inst, GDATA, APDS9960__GFLVL, sizeof(GDATA));
		if (err != ERROR_OK)	{ goto ret; }

		if (GST->Bits.GVALID)
		{
			// Read FIFO datas (GDATA[0] holding number of FIFO items)
			for (uintCPU_t i = 0 ; i < GDATA[0] ; i++)
			{
				err = APDS9960_Read(pCpnt->cfg.slave_inst, GDATAS, APDS9960__GFIFO_U, sizeof(GDATAS));
				if (err != ERROR_OK)	{ goto ret; }

				// Do something with datas
			}
		}
	}

	if ((pCpnt->cfg.AIEN) && (pCpnt->cfg.PIEN))	{ err = APDS9960_SF_Clear_ALS_PROX_IT(pCpnt); }
	else if (pCpnt->cfg.AIEN)					{ err = APDS9960_SF_Clear_ALS_IT(pCpnt); }
	else if (pCpnt->cfg.PIEN)					{ err = APDS9960_SF_Clear_PROX_IT(pCpnt); }

	ret:
	return err;
}


__WEAK FctERR NONNULL__ APDS9960_handler_it(APDS9960_t * const pCpnt)
{
	FctERR err = ERROR_OK;

	if (APDS9960_INT_GPIO_Get(pCpnt))	{ err = APDS9960_handler(pCpnt); }

	return err;
}


FctERR APDS9960_handler_all(void)
{
	FctERR err = ERROR_OK;

	for (APDS9960_t * pCpnt = APDS9960 ; pCpnt < &APDS9960[I2C_APDS9960_NB] ; pCpnt++)
	{
		err |= APDS9960_handler(pCpnt);
	}

	return err;
}

FctERR APDS9960_handler_it_all(void)
{
	FctERR err = ERROR_OK;

	for (APDS9960_t * pCpnt = APDS9960 ; pCpnt < &APDS9960[I2C_APDS9960_NB] ; pCpnt++)
	{
		err |= APDS9960_handler_it(pCpnt);
	}

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
