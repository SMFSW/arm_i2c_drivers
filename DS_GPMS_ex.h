/*!\file DS_GPMS_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief DS-GPM.S Driver extensions
** \details DS-GPM.S: 99 Channel Positioning System (GPS + GLONASS) Shield
**/
/****************************************************************/
#ifndef __DS_GPMS_EX_H__
	#define __DS_GPMS_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "DS_GPMS.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/
// TODO: doxygen


// *****************************************************************************
// Section: Constants
// *****************************************************************************


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\union uGPMS_WR_REG_MAP
** \brief Union of DS-GPM.S writeable registry map
**/
typedef union uGPMS_WR_REG_MAP {
	uint8_t Bytes[3];
	struct {
		uint8_t		IO_Direction;
		uint8_t		IO_Input_Type;
		uint8_t		IO_Port_Output;
	} Reg;
} uGPMS_WR_REG_MAP;


/*!\union uGPMS_RD_REG_MAP
** \brief Union of DS-GPM.S readable registry map
**/
typedef union uGPMS_RD_REG_MAP {
	uint8_t Bytes[70];
	struct {
		uint8_t				Hours_Tens;
		uint8_t				Hours_Units;
		uint8_t				Minutes_Tens;
		uint8_t				Minutes_Units;
		uint8_t				Seconds_Tens;
		uint8_t				Seconds_Units;
		uint8_t				DOM_Tens;
		uint8_t				DOM_Units;
		uint8_t				Month_Tens;
		uint8_t				Month_Units;
		uint8_t				Years_Thousands;
		uint8_t				Years_Hundreds;
		uint8_t				Years_Tens;
		uint8_t				Years_Units;
		uint8_t				Latitude_Deg_Tens;
		uint8_t				Latitude_Deg_Units;
		uint8_t				Latitude_Minutes_Tens;
		uint8_t				Latitude_Minutes_Units;
		uint8_t				Latitude_Minutes_Tenths;
		uint8_t				Latitude_Minutes_Hundredths;
		uint8_t				Latitude_Minutes_Thousands;
		uint8_t				Latitude_Minutes_Ten_Thousands;
		GPMS_direction		Latitude_Direction_Character;
		uint8_t				Longitude_Deg_Tens;
		uint8_t				Longitude_Deg_Units;
		uint8_t				Longitude_Minutes_Tens;
		uint8_t				Longitude_Minutes_Units;
		uint8_t				Longitude_Minutes_Tenths;
		uint8_t				Longitude_Minutes_Hundredths;
		uint8_t				Longitude_Minutes_Thousands;
		uint8_t				Longitude_Minutes_Ten_Thousands;
		GPMS_direction		Longitude_Direction_Character;
		GPMS_quality		GPS_Quality_Indicator;
		uint8_t				Satellites_In_Use_Tens;
		uint8_t				Satellites_In_Use_Units;
		uint8_t				HDOP_Tens;
		uint8_t				HDOP_Units;
		uint8_t				HDOP_Tenths;
		uint8_t				Altitude_Tens_Of_Thousands;
		uint8_t				Altitude_Thousands;
		uint8_t				Altitude_Hundreds;
		uint8_t				Altitude_Tens;
		uint8_t				Altitude_Units;
		uint8_t				Heading_Deg_True_Hundreds;
		uint8_t				Heading_Deg_True_Tens;
		uint8_t				Heading_Deg_True_Units;
		uint8_t				Heading_Deg_True_Tenths;
		uint8_t				Heading_Mag_True_Hundreds;
		uint8_t				Heading_Mag_True_Tens;
		uint8_t				Heading_Mag_True_Units;
		uint8_t				Heading_Mag_True_Tenths;
		uint8_t				Speed_Hundreds;
		uint8_t				Speed_Tens;
		uint8_t				Speed_Units;
		uint8_t				Speed_Tenths;
		GPMS_mode			GPS_Mode;
		uint8_t				AN0_Input;
		uint8_t				AN1_Input;
		uint8_t				AN2_Input;
		uint8_t				AN3_Input;
		uint8_t				IO_Input;
		uGPMS_REG__STATUS	Status;
	} Reg;
} uGPMS_RD_REG_MAP;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/


__INLINE FctERR INLINE__ GPMS_Set_Mode(const GPMS_mode mode) {
	return GPMS_Write((uint8_t *) &mode, GPMS__GPS_MODE, 1); }

	
__INLINE FctERR INLINE__ GPMS_Get_Mode(GPMS_mode * mode) {
	return GPMS_Read((uint8_t *) mode, GPMS__GPS_MODE, 1); }

__INLINE FctERR INLINE__ GPMS_Get_Signal_Quality(GPMS_quality * quality) {
	return GPMS_Read((uint8_t *) quality, GPMS__GPS_QUALITY_INDICATOR, 1); }

__INLINE FctERR INLINE__ GPMS_Get_Status(uint8_t * status) {
	return GPMS_Read(status, GPMS__STATUS, 1); }

__INLINE FctERR INLINE__ GPMS_Get_IO_Value(uint8_t * io, uint8_t chan) {
	if (chan > 4)	{ return ERROR_VALUE; }	// Unknown analogic input
	return GPMS_Read(io, GPMS__AN0_VALUE + chan, 1); }


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __DS_GPMS_EX_H__ */
/****************************************************************/
