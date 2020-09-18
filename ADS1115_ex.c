/*!\file ADS1115_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief ADS1115 Driver extensions
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

/*!\brief ADS1115 step value in µV (following configured gain)
** \note Depending the configured gain \ref ADS1115_gain
**/
static const float ADS1115_step[8] = {
	187.5f,		// ±6.144V gain
	125.0f,		// ±4.096V gain
	62.5f,		// ±2.048V gain
	31.25f,		// ±1.024V gain
	15.625f,	// ±0.512V gain
	7.8125f,	// ±0.256V gain
	7.8125f,	// ±0.256V gain
	7.8125f		// ±0.256V gain
};


float ADS1115_convert_to_uV(const int16_t val, const ADS1115_gain gain)
{
	if (gain > ADS1115__FS_256mV_3)	{ return 0.0f; }

	return (float) (val * ADS1115_step[gain]);
}


/****************************************************************/
#endif
#endif
/****************************************************************/
