/*!\file shared_APS.h
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief Ambient Pressure Sensing shared
**/
/****************************************************************/
#ifndef __SHARED_APS_H__
	#define __SHARED_APS_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#include "I2C_component.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


#ifndef SEA_LEVEL_PRESSURE
//! \note Define SEA_LEVEL_PRESSURE to use a custom average SeaLevel pressure value (hard value)
//! \note It is highly recommended to implement \ref Get_SeaLevel_Pressure function in project instead.
#define SEA_LEVEL_PRESSURE	1013.25f		//!< Average sea level pressure (hPa / mbar)
#endif


/**!\brief Sea Level pressure getter (in hPa / mbar)
**	\weak Get_SeaLevel_Pressure is recommended to be user implemented (average value returned by weak implementation)
**	\return Sea Level pressure in hPa/mbar
**/
float Get_SeaLevel_Pressure(void);


/**!\brief Calculates the altitude (in meters) using international barometric formula
**	from the specified atmospheric pressure (in hPa / mbar), and sea-level pressure (in hPa / mbar).
**	\note A pressure change of ∆p = 1hPa corresponds to 8.43m at sea level
**	\param[in] pressure - Atmospheric pressure in hPa / mbar
**	\param[in] sea_level_pressure - Sea Level pressure in hPa / mbar
**	\return Altitude in meters
**/
float Atmospheric_Pressure_To_Altitude(const float pressure, const float sea_level_pressure);


/**!\brief Calculates the pressure at sea level (in hPa / mbar) using international barometric formula
**	from the specified altitude (in meters), and atmospheric pressure (in hPa / mbar).
**	\note A difference in altitude of ∆altitude = 10m corresponds to 1.2hPa pressure change at sea level
**	\param[in] pressure - Atmospheric pressure in hPa / mbar
**	\param[in] altitude - Altitude in meters
**	\return Sea Level pressure estimation in hPa / mbar
**/
float Altitude_To_SeaLevel_Pressure(const float pressure, const float altitude);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* __SHARED_APS_H__ */
/****************************************************************/
