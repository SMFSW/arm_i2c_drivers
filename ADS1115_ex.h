/*!\file ADS1115_ex.h
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
#ifndef __ADS1115_EX_H__
	#define __ADS1115_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "ADS1115.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\union uADS1115_REG_MAP
** \brief Union of ADS1115 registry map
**/
typedef union uADS1115_REG_MAP {
	uint8_t		Bytes[8];
	uint16_t	Words[4];
	struct PACK__ {
		int16_t				Conversion;	// Conversion register
		uADS1115_REG__CFG	Config;		// Config register
		int16_t				Lo_Thresh;	// Lo_thresh register
		int16_t				Hi_Thresh;	// Hi_thresh register
	} Reg;
} uADS1115_REG_MAP;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

/*!\brief Convert ADS1115 value to µV
** \param[in] val - ADS1115 value
** \param[in] gain - ADS1115 configured gain
** \return Converted value
**/
float ADS1115_convert_to_uV(const int16_t val, const ADS1115_gain gain);

/*!\brief Convert ADS1115 value to V
** \param[in] val - ADS1115 value
** \param[in] gain - ADS1115 configured gain
** \return Converted value
**/
__INLINE float INLINE__ ADS1115_convert_to_V(const int16_t val, const ADS1115_gain gain)
{
	return (ADS1115_convert_to_uV(val, gain) * 1000000);
}

/*!\brief Convert ADS1115 value to mV
** \param[in] val - ADS1115 value
** \param[in] gain - ADS1115 configured gain
** \return Converted value
**/
__INLINE float INLINE__ ADS1115_convert_to_mV(const int16_t val, const ADS1115_gain gain)
{
	return (ADS1115_convert_to_uV(val, gain) * 1000);
}

/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __ADS1115_EX_H__ */
/****************************************************************/
