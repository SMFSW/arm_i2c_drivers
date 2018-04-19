/*!\file S11059_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief S11059 Driver procedures
** \details S11059: I2C interface-compatible color sensor
**/
/****************************************************************/
#include "S11059.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_S11059)
/****************************************************************/


S11059_proc S11059 = { 0, 0, 0, 0, 0, { 0, S11059__INTEG_22_4MS, 0, S11059__FIXED_PERIOD_INTEGRATION, S11059__LOW_GAIN, S11059__OPERATING_MODE, S11059__ADC_OPERATION } };

static float S11059_Integ_tab[4] = { 87.5f, 1400.0f, 22400.0f, 179200.0f };		//!< S11059 integration time table (in us)


/****************************************************************/


__WEAK FctERR S11059_Init_Sequence(void)
{
	uS11059_REG__CONTROL	CTL;
	FctERR					err;

	CTL.Byte = 0;
	CTL.Bits.ADC_RESET = S11059__ADC_RESET;
	CTL.Bits.STANDBY_FUNCTION = S11059.cfg.Standby;
	CTL.Bits.GAIN_SELECTION = S11059.cfg.GainSelection;
	CTL.Bits.INTEG_MODE = S11059.cfg.IntegrationMode;
	CTL.Bits.INTEG_PRESCL = S11059.cfg.IntegrationPrescaler;

	err = S11059_Write_Ctl(CTL.Byte);
	if (err)	{ return err; }

	S11059.cfg.FullIntegrationTime = get_Full_Integration_Time(S11059.cfg.IntegrationMode, S11059.cfg.IntegrationPrescaler, S11059.cfg.IntegrationTimeMult);

	CTL.Bits.ADC_RESET = S11059.cfg.ADCMode;
	err = S11059_Write_Ctl(CTL.Byte);
	if (err)	{ return err; }

	S11059.hStartConversion = HAL_GetTick();

	return err;
}


/****************************************************************/


uint32_t get_Full_Integration_Time(const S11059_integ mode, const S11059_prescaler prescaler, const uint16_t mult)
{
	if (mode > S11059__FIXED_PERIOD_INTEGRATION)	{ return ERROR_VALUE; }
	if (prescaler > S11059__INTEG_179_2MS)			{ return ERROR_VALUE; }

	float		t = S11059_Integ_tab[prescaler] * 4;		// Prescaler multiplied by 4 channels
	uint16_t	m = (!mult) ? 1 : mult;

	if (mode == S11059__MANUAL_INTEGRATION)	{ t *= 2 * m; }

	return (uint32_t) t;
}


__WEAK FctERR S11059_handler(void)
{
	uint8_t	DATA[8];
	FctERR	err;

	if (TPSSUP_MS(S11059.hStartConversion, (uint32_t) ((S11059.cfg.FullIntegrationTime / 1000) + 1)))	// true at least after 1ms in the worst case (fixed period set to 87.5us)
	{
		S11059.hStartConversion = HAL_GetTick();

		err = S11059_Read(DATA, S11059__RED_DATA_MSB, sizeof(DATA));
		if (err)	{ return err; }

		S11059.Red = MAKEWORD(DATA[1], DATA[0]);
		S11059.Green = MAKEWORD(DATA[3], DATA[2]);
		S11059.Blue = MAKEWORD(DATA[5], DATA[4]);
		S11059.IR = MAKEWORD(DATA[7], DATA[6]);

		#if defined(VERBOSE)
			printf("S11059: R%d G%d B%d IR%d\r\n", S11059.Red, S11059.Green, S11059.Blue, S11059.IR);
		#endif
	}

	return ERROR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
