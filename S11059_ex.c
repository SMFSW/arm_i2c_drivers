/*!\file S11059_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief S11059 Driver extensions
** \details S11059: I2C interface-compatible color sensor
**/
/****************************************************************/
#include "S11059.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_S11059)
/****************************************************************/


FctERR S11059_Set_ADC(const S11059_adc_reset adc)
{
	uS11059_REG__CONTROL	CTL;
	FctERR					err;

	err = S11059_Read(&CTL.Byte, S11059__CONTROL, 1);
	if (err)	{ return err; }

	CTL.Bits.ADC_RESET = adc;
	err = S11059_Write_Ctl(CTL.Byte);
	if (err)	{ return err; }

	S11059.cfg.ADCMode = adc;

	return err;
}


FctERR S11059_Set_Standby(const S11059_standby_fct standby)
{
	uS11059_REG__CONTROL	CTL;
	FctERR					err;

	err = S11059_Read(&CTL.Byte, S11059__CONTROL, 1);
	if (err)	{ return err; }

	CTL.Bits.STANDBY_FUNCTION = standby;
	err = S11059_Write_Ctl(CTL.Byte);
	if (err)	{ return err; }

	S11059.cfg.Standby = standby;

	return err;
}


FctERR S11059_Set_Gain(const S11059_gain gain)
{
	uS11059_REG__CONTROL	CTL;
	FctERR					err;

	err = S11059_Read(&CTL.Byte, S11059__CONTROL, 1);
	if (err)	{ return err; }

	CTL.Bits.GAIN_SELECTION = gain;
	err =  S11059_Write_Ctl(CTL.Byte);
	if (err)	{ return err; }

	S11059.cfg.GainSelection = gain;

	return err;
}


FctERR S11059_Set_Integration(const S11059_integ mode, const S11059_prescaler prescaler)
{
	uS11059_REG__CONTROL	CTL;
	FctERR					err;

	err = S11059_Read(&CTL.Byte, S11059__CONTROL, 1);
	if (err)	{ return err; }

	CTL.Bits.INTEG_MODE = mode;
	CTL.Bits.INTEG_PRESCL = prescaler;
	err = S11059_Write_Ctl(CTL.Byte);
	if (err)	{ return err; }

	S11059.cfg.IntegrationMode = mode;
	S11059.cfg.IntegrationPrescaler = prescaler;

	return err;
}


FctERR S11059_Set_Integration_Multiplier(const uint16_t mult)
{
	FctERR	err;

	err = S11059_Write_Word(&mult, S11059__MANUAL_TIMING_MSB);
	if (err)	{ return err; }

	S11059.cfg.IntegrationTimeMult = mult;

	return err;
}


FctERR NONNULL__ S11059_Get_Standby(S11059_standby_fct * standby)
{
	uS11059_REG__CONTROL	CTL;
	FctERR					err;

	err = S11059_Read((uint8_t *) &CTL, S11059__CONTROL, 1);
	if (err)	{ return err; }

	*standby = CTL.Bits.STANDBY_MONITOR;
	S11059.cfg.Standby = CTL.Bits.STANDBY_MONITOR;

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
