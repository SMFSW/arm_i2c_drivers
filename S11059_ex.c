/*!\file S11059_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief S11059 Driver extensions
** \details S11059: I2C interface-compatible color sensor
**/
/****************************************************************/
#include "S11059.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_S11059)
/****************************************************************/


FctERR NONNULL__ S11059_Set_ADC(S11059_t * const pCpnt, const S11059_adc_reset adc)
{
	uS11059_REG__CONTROL	CTL;
	FctERR					err;

	err = S11059_Read(pCpnt->cfg.slave_inst, &CTL.Byte, S11059__CONTROL, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	CTL.Bits.ADC_RESET = adc;

	err = S11059_Write_Ctl(pCpnt->cfg.slave_inst, CTL.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.ADCMode = adc;

	ret:
	return err;
}


FctERR NONNULL__ S11059_Set_Standby(S11059_t * const pCpnt, const S11059_standby_fct standby)
{
	uS11059_REG__CONTROL	CTL;
	FctERR					err;

	err = S11059_Read(pCpnt->cfg.slave_inst, &CTL.Byte, S11059__CONTROL, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	CTL.Bits.STANDBY_FUNCTION = standby;

	err = S11059_Write_Ctl(pCpnt->cfg.slave_inst, CTL.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.Standby = standby;

	ret:
	return err;
}


FctERR NONNULL__ S11059_Set_Gain(S11059_t * const pCpnt, const S11059_gain gain)
{
	uS11059_REG__CONTROL	CTL;
	FctERR					err;

	err = S11059_Read(pCpnt->cfg.slave_inst, &CTL.Byte, S11059__CONTROL, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	CTL.Bits.GAIN_SELECTION = gain;

	err = S11059_Write_Ctl(pCpnt->cfg.slave_inst, CTL.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.GainSelection = gain;

	ret:
	return err;
}


FctERR NONNULL__ S11059_Set_Integration(S11059_t * const pCpnt, const S11059_integ mode, const S11059_prescaler prescaler)
{
	uS11059_REG__CONTROL	CTL;
	FctERR					err;

	err = S11059_Read(pCpnt->cfg.slave_inst, &CTL.Byte, S11059__CONTROL, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	CTL.Bits.INTEG_MODE = mode;
	CTL.Bits.INTEG_PRESCL = prescaler;

	err = S11059_Write_Ctl(pCpnt->cfg.slave_inst, CTL.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.IntegrationMode = mode;
	pCpnt->cfg.IntegrationPrescaler = prescaler;

	ret:
	return err;
}


FctERR NONNULL__ S11059_Set_Integration_Multiplier(S11059_t * const pCpnt, const uint16_t mult)
{
	FctERR err;

	err = S11059_Write_Word(pCpnt->cfg.slave_inst, &mult, S11059__MANUAL_TIMING_MSB);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.IntegrationTimeMult = mult;

	ret:
	return err;
}


FctERR NONNULL__ S11059_Get_Standby(S11059_t * const pCpnt, S11059_standby_fct * const standby)
{
	uS11059_REG__CONTROL	CTL;
	FctERR					err;

	err = S11059_Read(pCpnt->cfg.slave_inst, &CTL.Byte, S11059__CONTROL, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	*standby = CTL.Bits.STANDBY_MONITOR;
	pCpnt->cfg.Standby = CTL.Bits.STANDBY_MONITOR;

	ret:
	return err;
}


FctERR NONNULL__ S11059_Get_Channel(S11059_t * const pCpnt, uint16_t * const val, const S11059_chan chan)
{
	FctERR err;

	if (chan > S11059__CHAN_IR)	{ err = ERROR_VALUE; }	// Unknown channel
	else						{ err = S11059_Read_Word(pCpnt->cfg.slave_inst, val, S11059__RED_DATA_MSB + (2U * chan)); }

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
