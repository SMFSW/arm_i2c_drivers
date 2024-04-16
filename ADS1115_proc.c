/*!\file ADS1115_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
** \brief ADS1115 Driver procedures
** \details ADS1115: Ultra-Small, Low-Power, 16-Bit Analog-to-Digital Converter with Internal Reference
** \note	Compatibility with:
**				- ADS1113 (restrictions given in datasheet)
**				- ADS1114 (restrictions given in datasheet)
**				- ADS1115
** \note	Compatibility should be assured with:
**				- ADS1x1x (check datasheets for compatibility informations)
**/
/****************************************************************/
#include "ADS1115.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_ADS1115)

// HARMcksL libs
#include "tick_utils.h"
/****************************************************************/


ADS1115_t ADS1115[I2C_ADS1115_NB] = { 0 };


/****************************************************************/


__WEAK FctERR NONNULL__ ADS1115_Init_Sequence(ADS1115_t * const pCpnt)
{
	pCpnt->cfg.Lo_Thresh = 0x8000;		// - max Default value
	pCpnt->cfg.Hi_Thresh = 0x7FFF;		// + max Default value

	pCpnt->cfg.Config.Word = 0x8583;	// Default value
	pCpnt->cfg.Config.Bits.PGA = ADS1115__FS_4096mV;

	return ADS1115_Set_Function(pCpnt, ADS1115__FUNC_SINGLE_ENDED, ADS1115__MODE_CONTINUOUS, 4);
}


/****************************************************************/


__WEAK FctERR NONNULL__ ADS1115_handler(ADS1115_t * const pCpnt)
{
	FctERR err;

	/**!\note Conversion time need to be respected,
	**	\ref ADS1115_handler_it may be used to handle GPIO pin interrupt, or
	**	\ref ADS1115_Get_conv_ms may be used to help ensure that without using interrupt pin.
	**/

	/**!\note If single shot is required, \ref ADS1115_Start_NextConversion can be called once at init, using \ref ADS1115_handler_it afterwards.
	**	\ref ADS1115_handler shall be user implemented in this case, only to get converted value when interrupt pin triggers
	**	(and eventually start another conversion with \ref ADS1115_Start_NextConversion).
	**/
	if (pCpnt->cfg.mode == ADS1115__MODE_SINGLE_SHOT)
	{
		err = ADS1115_Start_NextConversion(pCpnt);
		if (err)	{ return err; }

		Delay_us(ADS1115_Get_conv_us(pCpnt));
	}

	err = ADS1115_Get_Conversion(pCpnt);
	if (err)	{ return err; }

	#if defined(VERBOSE)
		const uint8_t idx = pCpnt - ADS1115;
		const uint8_t chan = pCpnt->AIN_idx;
		printf("ADS1115 id%d: Chan%d %-6d %ldmV\r\n", idx, chan,
				ADS1115_Get_raw_value(pCpnt, chan),
				(int32_t) ADS1115_Get_converted_value_mV(pCpnt, chan));
	#endif

	if (pCpnt->cfg.mode != ADS1115__MODE_SINGLE_SHOT)
	{
		err = ADS1115_Start_NextConversion(pCpnt);
	}

	return err;
}


__WEAK FctERR NONNULL__ ADS1115_handler_it(ADS1115_t * const pCpnt)
{
	FctERR	err;
	bool	interrupt;

	err = ADS1115_RDY_GPIO_Get(pCpnt, &interrupt);
	if ((!err) && interrupt)	{ err = ADS1115_handler(pCpnt); }

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
