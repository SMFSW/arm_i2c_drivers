/*!\file S11059_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
** \brief S11059 Driver procedures
** \details S11059: I2C interface-compatible color sensor
**/
/****************************************************************/
#include "S11059.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_S11059)
/****************************************************************/


S11059_t S11059[I2C_S11059_NB] = { 0 };

static float S11059_Integ_tab[4] = { 87.5f, 1400.0f, 22400.0f, 179200.0f };		//!< S11059 integration time table (in us)


/****************************************************************/


__WEAK FctERR NONNULL__ S11059_Init_Sequence(S11059_t * const pCpnt)
{
	uS11059_REG__CONTROL	CTL = { 0 };
	FctERR					err;

	pCpnt->cfg.IntegrationPrescaler = S11059__INTEG_22_4MS;
	pCpnt->cfg.IntegrationMode = S11059__FIXED_PERIOD_INTEGRATION;
	pCpnt->cfg.GainSelection = S11059__LOW_GAIN;
	pCpnt->cfg.Standby = S11059__OPERATING_MODE;
	pCpnt->cfg.ADCMode = S11059__ADC_OPERATION;

	CTL.Bits.ADC_RESET = S11059__ADC_RESET;
	CTL.Bits.STANDBY_FUNCTION = pCpnt->cfg.Standby;
	CTL.Bits.GAIN_SELECTION = pCpnt->cfg.GainSelection;
	CTL.Bits.INTEG_MODE = pCpnt->cfg.IntegrationMode;
	CTL.Bits.INTEG_PRESCL = pCpnt->cfg.IntegrationPrescaler;

	err = S11059_Write_Ctl(pCpnt->cfg.slave_inst, CTL.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.FullIntegrationTime = S11059_Get_Full_Integration_Time(pCpnt->cfg.IntegrationMode, pCpnt->cfg.IntegrationPrescaler, pCpnt->cfg.IntegrationTimeMult);

	CTL.Bits.ADC_RESET = pCpnt->cfg.ADCMode;
	err = S11059_Write_Ctl(pCpnt->cfg.slave_inst, CTL.Byte);
	if (err)	{ return err; }

	pCpnt->hStartConversion = HAL_GetTick();

	return err;
}


/****************************************************************/


uint32_t S11059_Get_Full_Integration_Time(const S11059_integ mode, const S11059_prescaler prescaler, const uint16_t mult)
{
	if (mode > S11059__FIXED_PERIOD_INTEGRATION)	{ return ERROR_VALUE; }
	if (prescaler > S11059__INTEG_179_2MS)			{ return ERROR_VALUE; }

	float		t = S11059_Integ_tab[prescaler] * 4;		// Prescaler multiplied by 4 channels
	uint16_t	m = (!mult) ? 1 : mult;

	if (mode == S11059__MANUAL_INTEGRATION)	{ t *= 2 * m; }

	return (uint32_t) t;
}


__WEAK FctERR NONNULL__ S11059_handler(S11059_t * const pCpnt)
{
	uint8_t	DATA[8];
	FctERR	err;

	if (TPSSUP_MS(pCpnt->hStartConversion, (uint32_t) ((pCpnt->cfg.FullIntegrationTime / 1000) + 1)))	// true at least after 1ms in the worst case (fixed period set to 87.5us)
	{
		pCpnt->hStartConversion = HAL_GetTick();

		err = S11059_Read(pCpnt->cfg.slave_inst, DATA, S11059__RED_DATA_MSB, sizeof(DATA));
		if (err)	{ return err; }

		pCpnt->Red = MAKEWORD(DATA[1], DATA[0]);
		pCpnt->Green = MAKEWORD(DATA[3], DATA[2]);
		pCpnt->Blue = MAKEWORD(DATA[5], DATA[4]);
		pCpnt->IR = MAKEWORD(DATA[7], DATA[6]);

		#if defined(VERBOSE)
			const uint8_t idx = pCpnt - S11059;
			printf("S11059 id%d: R%d G%d B%d IR%d\r\n", idx, pCpnt->Red, pCpnt->Green, pCpnt->Blue, pCpnt->IR);
		#endif
	}

	return ERROR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
