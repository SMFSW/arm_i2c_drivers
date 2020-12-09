/*!\file shared_CLS.c
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief Color Light Sensing shared
**/
/****************************************************************/
#include "shared_CLS.h"

#include "I2C_Component.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_APDS9960) || defined(I2C_S11059) || defined(I2C_TCS3400) || defined(I2C_TCS3472)
/****************************************************************/
// std libs
#include <math.h>
/****************************************************************/


const float CLS_RGB2XYZ_Default[3][3] = {
	{ -0.14282f, 1.54924f, -0.95641f },
	{ -0.32466f, 1.57837f, -0.73191f },
	{ -0.68202f, 0.77073f, 0.56332f } };		//!!< This matrix is the result of a transformation composed of correlations at different light sources (based on TAOS DN25 application note)


FctERR NONNULL__ CLS_get_chromacity(float xy[2], uint32_t * const illum, const float mat[3][3], const uint16_t r, const uint16_t g, const uint16_t b)
{
	// Convert RGB to XYZ
	const float X = (mat[0][0] * r) + (mat[0][1] * g) + (mat[0][2] * b);
	const float Y = (mat[1][0] * r) + (mat[1][1] * g) + (mat[1][2] * b);	// Note: Y = Illuminance (lux)
	const float Z = (mat[2][0] * r) + (mat[2][1] * g) + (mat[2][2] * b);

	// Calculate xy (CIE1931) chromaticity coordinates
	const float denominator = X + Y + Z;
	xy[0] = X / denominator;
	xy[1] = Y / denominator;

	*illum = (uint32_t) Y;

	return ERROR_OK;
}


FctERR NONNULL__ CLS_get_CCT(uint32_t * const cct, const float xy[2])
{
	// Use McCamy's formula to determine the CCT (original formula, not taken from TAOS DN25 application note (TCS34xx))
	const float n = (xy[0] - 0.3320f) / (xy[1] - 0.1858f);
	*cct = (uint32_t) ((449.0f * powf(n, 3)) + (3525.0f * powf(n, 2)) + (6823.3f * n) + 5520.33f);
	//*cct = (uint32_t) ((6253.80338 * exp(-n / 0.92159)) + (28.70599 * exp(-n / 0.20039)) + (0.00004 * exp(-n / 0.07125)) - 949.86315);

	return ERROR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
