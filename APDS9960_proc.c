/*!\file APDS9960_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
** \brief APDS9960 Driver procedures
** \details APDS9960: Digital Proximity, Ambient Light, RGB and Gesture Sensor
**/
/****************************************************************/
#include "APDS9960.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_APDS9960)
/****************************************************************/


APDS9960_t APDS9960[I2C_APDS9960_NB];

const uint8_t APDS9960_ALS_gain_tab[4] = { 1, 4, 16, 64 };
const uint8_t APDS9960_Prox_gain_tab[4] = { 1, 2, 4, 8 };


/****************************************************************/


__WEAK FctERR NONNULL__ APDS9960_Init_Sequence(APDS9960_t * pCpnt)
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
	pCpnt->cfg.GIEN = true;
	pCpnt->cfg.WEN = true;

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
	err = APDS9960_Set_PIT(pCpnt, pCpnt->cfg.Prox_LowThreshold, pCpnt->cfg.Prox_HighThreshold);

	EN.Bits.WEN = pCpnt->cfg.WEN;	// Turn ON Wait following cfg
	EN.Bits.AEN = pCpnt->cfg.AIEN;	// Turn ON ALS following cfg
	EN.Bits.PEN = pCpnt->cfg.PIEN;	// Turn ON Proximity following cfg
	EN.Bits.GEN = pCpnt->cfg.GIEN;	// Turn ON Gesture following cfg
	EN.Bits.AIEN = pCpnt->cfg.AIEN;	// Turn ON ALS interrupts following cfg
	EN.Bits.PIEN = pCpnt->cfg.PIEN;	// Turn ON Proximity interrupts following cfg
	return APDS9960_Write_En(pCpnt, EN.Byte);
}


/****************************************************************/


__WEAK FctERR NONNULL__ APDS9960_handler(APDS9960_t * pCpnt)
{
	return ERROR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
