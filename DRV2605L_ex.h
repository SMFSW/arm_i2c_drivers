/*!\file DRV2605L.h
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief TI Haptic Driver extensions declarations
**/
/****************************************************************/
#ifndef __DRV2605L_EX_H__
	#define __DRV2605L_EX_H__

#include "DRV2605L.h"

#include "sarmfsw.h"
#include CMSIS_INC
#include CMSIS_CFG

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// High level functions
FctERR DRV2605L_Vibe(bool on);

FctERR DRV2605L_GetBlankingTime(uint16_t * time);
FctERR DRV2605L_GetCurrentDissipationTime(uint16_t * time);

FctERR DRV2605L_SetBlankingTime(uint16_t time);
FctERR DRV2605L_SetCurrentDissipationTime(uint16_t time);

FctERR DRV2605L_GetVoltage(uint16_t * vbat);
FctERR DRV2605L_GetLRAOpenLoopPeriod(uint16_t * per);
FctERR DRV2605L_GetResonancePeriod(uint16_t * per);


/****************************************************************/
#endif
#endif /* __DRV2605L_EX_H__ */
/****************************************************************/
