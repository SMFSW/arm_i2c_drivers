/*!\file S11059_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief S11059 Driver extensions
** \details S11059: I2C interface-compatible color sensor
**/
/****************************************************************/
#include "S11059.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_S11059)
/****************************************************************/


FctERR NONNULL__ S11059_Set_ADC(S11059_t * pCpnt, const S11059_adc_reset adc)
{
	uS11059_REG__CONTROL	CTL;
	FctERR					err;

	err = S11059_Read(pCpnt->cfg.slave_inst, &CTL.Byte, S11059__CONTROL, 1);
	if (err)	{ return err; }

	CTL.Bits.ADC_RESET = adc;
	err = S11059_Write_Ctl(pCpnt->cfg.slave_inst, CTL.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.ADCMode = adc;

	return err;
}


FctERR NONNULL__ S11059_Set_Standby(S11059_t * pCpnt, const S11059_standby_fct standby)
{
	uS11059_REG__CONTROL	CTL;
	FctERR					err;

	err = S11059_Read(pCpnt->cfg.slave_inst, &CTL.Byte, S11059__CONTROL, 1);
	if (err)	{ return err; }

	CTL.Bits.STANDBY_FUNCTION = standby;
	err = S11059_Write_Ctl(pCpnt->cfg.slave_inst, CTL.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.Standby = standby;

	return err;
}


FctERR NONNULL__ S11059_Set_Gain(S11059_t * pCpnt, const S11059_gain gain)
{
	uS11059_REG__CONTROL	CTL;
	FctERR					err;

	err = S11059_Read(pCpnt->cfg.slave_inst, &CTL.Byte, S11059__CONTROL, 1);
	if (err)	{ return err; }

	CTL.Bits.GAIN_SELECTION = gain;
	err = S11059_Write_Ctl(pCpnt->cfg.slave_inst, CTL.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.GainSelection = gain;

	return err;
}


FctERR NONNULL__ S11059_Set_Integration(S11059_t * pCpnt, const S11059_integ mode, const S11059_prescaler prescaler)
{
	uS11059_REG__CONTROL	CTL;
	FctERR					err;

	err = S11059_Read(pCpnt->cfg.slave_inst, &CTL.Byte, S11059__CONTROL, 1);
	if (err)	{ return err; }

	CTL.Bits.INTEG_MODE = mode;
	CTL.Bits.INTEG_PRESCL = prescaler;
	err = S11059_Write_Ctl(pCpnt->cfg.slave_inst, CTL.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.IntegrationMode = mode;
	pCpnt->cfg.IntegrationPrescaler = prescaler;

	return err;
}


FctERR NONNULL__ S11059_Set_Integration_Multiplier(S11059_t * pCpnt, const uint16_t mult)
{
	FctERR	err;

	err = S11059_Write_Word(pCpnt->cfg.slave_inst, &mult, S11059__MANUAL_TIMING_MSB);
	if (err)	{ return err; }

	pCpnt->cfg.IntegrationTimeMult = mult;

	return err;
}


FctERR NONNULL__ S11059_Get_Standby(S11059_t * pCpnt, S11059_standby_fct * standby)
{
	uS11059_REG__CONTROL	CTL;
	FctERR					err;

	err = S11059_Read(pCpnt->cfg.slave_inst, (uint8_t *) &CTL, S11059__CONTROL, 1);
	if (err)	{ return err; }

	*standby = CTL.Bits.STANDBY_MONITOR;
	pCpnt->cfg.Standby = CTL.Bits.STANDBY_MONITOR;

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
