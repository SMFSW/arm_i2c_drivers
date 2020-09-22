/*!\file ADS1115_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
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
/****************************************************************/


ADS1115_t ADS1115[I2C_ADS1115_NB];


/****************************************************************/


__WEAK FctERR NONNULL__ ADS1115_Init_Sequence(ADS1115_t * pCpnt)
{
	pCpnt->cfg.Lo_Thresh = 0x8000;		// - max Default value
	pCpnt->cfg.Hi_Thresh = 0x7FFF;		// + max Default value

	pCpnt->cfg.Config.Word = 0x8583;	// Default value
	pCpnt->cfg.Config.Bits.PGA = ADS1115__FS_4096mV;

	return ADS1115_Set_Function(pCpnt, ADS1115__FUNC_SINGLE_ENDED, ADS1115__MODE_CONTINUOUS, 4);
}


/****************************************************************/


__WEAK FctERR NONNULL__ ADS1115_handler(ADS1115_t * pCpnt)
{
	FctERR err;

	//! \note In case of ADS1115__MODE_CONTINUOUS, conversion time need to be respected (thus ADS1115_get_conv_ms may be used to help ensure that)

	if (pCpnt->cfg.mode == ADS1115__MODE_SINGLE_SHOT)
	{
		err = ADS1115_Start_NextConversion(pCpnt);
		if (err)	{ return err; }
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


/****************************************************************/
#endif
#endif
/****************************************************************/
