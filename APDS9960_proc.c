/*!\file APDS9960_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief APDS9960 Driver procedures
** \details APDS9960: Digital Proximity, Ambient Light, RGB and Gesture Sensor
**/
/****************************************************************/
#include "APDS9960.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_APDS9960)
/****************************************************************/


APDS9960_t APDS9960[I2C_APDS9960_NB] = { 0 };

const uint8_t APDS9960_ALS_gain_tab[4] = { 1, 4, 16, 64 };
const uint8_t APDS9960_Prox_gain_tab[4] = { 1, 2, 4, 8 };


/****************************************************************/


__WEAK FctERR NONNULL__ APDS9960_Init_Sequence(APDS9960_t * const pCpnt)
{
	uAPDS9960_REG__ENABLE	EN = { 0 };
	FctERR					err = ERROR_OK;

	//pCpnt->cfg.GA = APDS9960_GLASS_ATTENUATION;

	pCpnt->cfg.ADC_Gain = APDS9960__ALS_16X_GAIN;
	pCpnt->cfg.Prox_Gain = APDS9960__PROX_2X_GAIN;
	pCpnt->cfg.ADC_Integ = 100;
	pCpnt->cfg.Wait = 1000;
	pCpnt->cfg.ADC_LowThreshold = 0x8FF;
	pCpnt->cfg.ADC_HighThreshold = 0x8FF;
	pCpnt->cfg.Prox_LowThreshold = 0x8F;
	pCpnt->cfg.Prox_HighThreshold = 0x8F;
	pCpnt->cfg.Prox_Pulses = 8;
	pCpnt->cfg.Prox_Length = APDS9960__PL_8US;
	pCpnt->cfg.Prox_Strength = APDS9960__DS_12_5MA;
	pCpnt->cfg.AIEN = true;
	pCpnt->cfg.PIEN = true;
	pCpnt->cfg.GIEN = false;
	pCpnt->cfg.WEN = true;

	memcpy(&pCpnt->cfg.mat, &CLS_RGB2XYZ_Default, sizeof(pCpnt->cfg.mat));

	// get ID & check against values for APDS9960
	err = APDS9960_Get_ChipID(pCpnt, &pCpnt->cfg.Id);
	if (err)								{ return err; }
	if (pCpnt->cfg.Id != APDS9960_CHIP_ID)	{ return ERROR_COMMON; }	// Unknown device

	EN.Bits.PON = true;		// Turn ON Osc
	err = APDS9960_Write_En(pCpnt, EN.Byte);
	if (err)	{ return err; }

	err = APDS9960_Set_ALS_Gain(pCpnt, pCpnt->cfg.ADC_Gain);
	if (err)	{ return err; }
	err = APDS9960_Set_Prox_Gain(pCpnt, pCpnt->cfg.Prox_Gain);
	if (err)	{ return err; }

	err = APDS9960_Set_Prox_Drive_Strength(pCpnt, pCpnt->cfg.Prox_Strength);
	if (err)	{ return err; }
	err = APDS9960_Write(pCpnt->cfg.slave_inst, &pCpnt->cfg.Prox_Pulses, APDS9960__PPULSE, 1);
	if (err)	{ return err; }

	err = APDS9960_Set_ADC_Integration_Time(pCpnt, pCpnt->cfg.ADC_Integ);
	if (err)	{ return err; }
	err = APDS9960_Set_Wait_Time(pCpnt, pCpnt->cfg.Wait);
	if (err)	{ return err; }

	err = APDS9960_Set_AIT(pCpnt, pCpnt->cfg.ADC_LowThreshold, pCpnt->cfg.ADC_HighThreshold);
	if (err)	{ return err; }
	err = APDS9960_Set_PIT(pCpnt, pCpnt->cfg.Prox_LowThreshold, pCpnt->cfg.Prox_HighThreshold);
	if (err)	{ return err; }

	EN.Bits.WEN = pCpnt->cfg.WEN;	// Turn ON Wait following cfg
	EN.Bits.AEN = pCpnt->cfg.AIEN;	// Turn ON ALS following cfg
	EN.Bits.PEN = pCpnt->cfg.PIEN;	// Turn ON Proximity following cfg
	EN.Bits.GEN = pCpnt->cfg.GIEN;	// Turn ON Gesture following cfg
	EN.Bits.AIEN = pCpnt->cfg.AIEN;	// Turn ON ALS interrupts following cfg
	EN.Bits.PIEN = pCpnt->cfg.PIEN;	// Turn ON Proximity interrupts following cfg
	return APDS9960_Write_En(pCpnt, EN.Byte);
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
	CLS_get_chromacity(pCpnt->xy, &pCpnt->Lux, pCpnt->cfg.mat, r, g, b);
	CLS_get_CCT(&pCpnt->Temp, pCpnt->xy);

	return ERROR_OK;
}


__WEAK FctERR NONNULL__ APDS9960_handler(APDS9960_t * const pCpnt)
{
	uint8_t					DATA[10];
	uAPDS9960_REG__STATUS *	ST = (uAPDS9960_REG__STATUS *) DATA;
	FctERR					err;
	#if defined(VERBOSE)
		const uint8_t		idx = pCpnt - APDS9960;
	#endif

	err = APDS9960_Read(pCpnt->cfg.slave_inst, DATA, APDS9960__STATUS, sizeof(DATA));
	if (err)	{ return err; }

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
				printf("APDS9960 id%d: C%d R%d G%d B%d x%d.%04ld y%d.%04ld Lux: %lul Temp: %luK\r\n",
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
				printf("APDS9960 id%d: Prox %d\r\n", idx, pCpnt->Prox);
			#endif
		}
	}

	if ((pCpnt->cfg.GIEN) && (ST->Bits.GINT))
	{
		uint8_t						GDATA[2], GDATAS[4];
		uAPDS9960_REG__GSTATUS *	GST = (uAPDS9960_REG__GSTATUS *) &GDATA[1];

		err = APDS9960_Read(pCpnt->cfg.slave_inst, GDATA, APDS9960__GFLVL, sizeof(GDATA));
		if (err)	{ return err; }

		if (GST->Bits.GVALID)
		{
			// Read FIFO datas (GDATA[0] holding number of FIFO items)
			for (unsigned int i = 0 ; i < GDATA[0] ; i++)
			{
				err = APDS9960_Read(pCpnt->cfg.slave_inst, GDATAS, APDS9960__GFIFO_U, sizeof(GDATAS));
				if (err)	{ return err; }

				// Do something with datas
			}
		}
	}

	if ((pCpnt->cfg.AIEN) && (pCpnt->cfg.PIEN))	{ return APDS9960_SF_Clear_ALS_PROX_IT(pCpnt); }
	else if (pCpnt->cfg.AIEN)					{ return APDS9960_SF_Clear_ALS_IT(pCpnt); }
	else if (pCpnt->cfg.PIEN)					{ return APDS9960_SF_Clear_PROX_IT(pCpnt); }

	return ERROR_OK;
}


__WEAK FctERR NONNULL__ APDS9960_handler_it(APDS9960_t * const pCpnt)
{
	FctERR	err;
	bool	interrupt;

	err = APDS9960_INT_GPIO_Get(pCpnt, &interrupt);
	if ((!err) && interrupt)	{ err = APDS9960_handler(pCpnt); }

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
