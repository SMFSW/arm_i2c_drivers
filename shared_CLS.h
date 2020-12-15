/*!\file shared_CLS.h
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief Color Light Sensing shared
**/
/****************************************************************/
#ifndef __SHARED_CLS_H__
	#define __SHARED_CLS_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#include "I2C_Component.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


extern const float CLS_RGB2XYZ_Default[3][3];


/*!\brief Determining chromacity following RGB values
** \param[in,out] xy - Pointer to chromacity coordinates result
** \param[in,out] illum - Pointer to illuminance result
** \param[in] r - Red value
** \param[in] g - Green value
** \param[in] b - Blue value
** \return FctERR - error code
**/
FctERR NONNULL__ CLS_get_chromacity(float xy[2], uint32_t * const illum, const float mat[3][3], const uint16_t r, const uint16_t g, const uint16_t b);

/*!\brief Determining CCT (correlated temperature) following xy values
** \param[in,out] cct - Pointer to CCT result
** \param[in] xy - Pointer to chromacity coordinates
** \return FctERR - error code
**/
FctERR NONNULL__ CLS_get_CCT(uint32_t * const cct, const float xy[2]);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* __SHARED_CLS_H__ */
/****************************************************************/
