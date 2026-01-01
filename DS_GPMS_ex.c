/*!\file DS_GPMS_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief DS-GPM.S Driver extensions
** \details DS-GPM.S: 99 Channel Positioning System (GPS + GLONASS) Shield
**/
/****************************************************************/
#include "DS_GPMS.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_GPMS)
/****************************************************************/
// std libs
//#include <math.h>
//#include <stdlib.h>
/****************************************************************/


FctERR NONNULL__ GPMS_Get_IO_Value(uint8_t * const io, const uint8_t chan)
{
	FctERR err;

	if (chan > 4)	{ err = ERROR_VALUE; }	// Unknown analogic input
	else			{ err = GPMS_Read(io, GPMS__AN0_VALUE + chan, 1U); }

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
