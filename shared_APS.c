/*!\file shared_APS.c
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
** \brief Ambient Pressure Sensing shared
**/
/****************************************************************/
#include "shared_APS.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_BMP180) || defined(I2C_BMP388) || defined(I2C_LPS35HW)
/****************************************************************/
// std libs
#include <math.h>
/****************************************************************/


__WEAK float Get_SeaLevel_Pressure(void)
{
	return SEA_LEVEL_PRESSURE;
}


float Atmospheric_Pressure_To_Altitude(const float pressure, const float sea_level_pressure)
{
	return (44330.0f * (1.0f - pow(pressure / sea_level_pressure, (1.0f / 5.255f))));
}


float Altitude_To_SeaLevel_Pressure(const float pressure, const float altitude)
{
	return (pressure / pow(1.0f - (altitude / 44330.0f), 5.255f));
}


/****************************************************************/
#endif
#endif
/****************************************************************/
