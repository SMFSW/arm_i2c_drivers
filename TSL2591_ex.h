/*!\file TSL2591_ex.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief TSL2591 Driver extensions declarations
** \details TSL2591: Very-high sensitivity light-to-digital converter
**/
/****************************************************************/
#ifndef __TSL2591_EX_H__
	#define __TSL2591_EX_H__

#include "TSL2591.h"

#include "sarmfsw.h"
#include CMSIS_INC
#include CMSIS_CFG

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/
// TODO: doxygen


// *****************************************************************************
// Section: Constants
// *****************************************************************************
/*!\enum TSL2591_light
** \brief Light types of TSL2591
**/
typedef enum PACK__ TSL2591_light {
	TSL2591__LIGHT_FULL = 0,	//!< Full light spectrum (channel 0)
	TSL2591__LIGHT_IR,			//!< Infra Red light (channel 1)
	TSL2591__LIGHT_VISIBLE		//!< Visible light (channel 0 - channel 1)
} TSL2591_light;


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\union uTSL2591_REG_MAP
** \brief Union of TSL2591 registry map
**/
typedef union uTSL2591_REG_MAP {
	uint8_t Bytes[23];
	struct {
		uTSL2591_REG__ENABLE	ENABLE;
		uTSL2591_REG__CONFIG	CONFIG;
		uint8_t					:8;
		uint8_t					:8;
		uint8_t					AILTL;
		uint8_t					AILTH;
		uint8_t					AIHTL;
		uint8_t					AIHTH;
		uint8_t					NPAILTl;
		uint8_t					NPAILTH;
		uint8_t					NPAIHTL;
		uint8_t					NPAIHTH;
		uTSL2591_REG__PERSIST	PERSIST;
		uint8_t					PID;
		uint8_t					ID;
		uTSL2591_REG__STATUS	STATUS;
		uint8_t					C0DATAL;
		uint8_t					C0DATAH;
		uint8_t					C1DATAL;
		uint8_t					C1DATAH;
		} Reg;
} uTSL2591_REG_MAP;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

/*** Configuration ***/
__INLINE FctERR INLINE__ TSL2591_Write_En(uint8_t en) {
	return TSL2591_Write(&en, TSL2591__ENABLE, 1); }

__INLINE FctERR INLINE__ TSL2591_Write_Cfg(uint8_t cfg) {
	return TSL2591_Write(&cfg, TSL2591__CONFIG, 1); }


FctERR TSL2591_Set_PON(bool en);
FctERR TSL2591_Set_AEN(bool en);
FctERR TSL2591_Set_AIEN(bool en);

FctERR TSL2591_Set_Gain(TSL2591_gain gain);
FctERR TSL2591_Set_Integration_Time(TSL2591_integ integ);

__INLINE FctERR INLINE__ TSL2591_Set_AILT(uint16_t thr) {
	return TSL2591_Write_Word(&thr, TSL2591__AILTL); }

__INLINE FctERR INLINE__ TSL2591_Set_AIHT(uint16_t thr) {
	return TSL2591_Write_Word(&thr, TSL2591__AIHTL); }

__INLINE FctERR INLINE__ TSL2591_Set_AIT(uint16_t lthr, uint16_t hthr) {
	uint8_t DAT[4] = { (hthr & 0xFF), (hthr / 0x100), (lthr & 0xFF), (lthr / 0x100) };
	return TSL2591_Write(DAT, TSL2591__AILTL, sizeof(DAT)); }


__INLINE FctERR INLINE__ TSL2591_Set_NPAILT(uint16_t thr) {
	return TSL2591_Write_Word(&thr, TSL2591__NPAILTL); }

__INLINE FctERR INLINE__ TSL2591_Set_NPAIHT(uint16_t thr) {
	return TSL2591_Write_Word(&thr, TSL2591__NPAIHTL); }

__INLINE FctERR INLINE__ TSL2591_Set_NPAIT(uint16_t lthr, uint16_t hthr) {
	uint8_t DAT[4] = { (hthr & 0xFF), (hthr / 0x100), (lthr & 0xFF), (lthr / 0x100) };
	return TSL2591_Write(DAT, TSL2591__NPAILTL, sizeof(DAT)); }

/*** Special Functions ***/
__INLINE FctERR INLINE__ TSL2591_SF_Force_IT(void) {
	return TSL2591_Write_Special(TSL2591__SF_FORCE_IT); }

__INLINE FctERR INLINE__ TSL2591_SF_Clear_IT(void) {
	return TSL2591_Write_Special(TSL2591__SF_CLR_ALS_IT); }

__INLINE FctERR INLINE__ TSL2591_SF_Clear_IT_PERS(void) {
	return TSL2591_Write_Special(TSL2591__SF_CLR_ALS_AND_NO_PERS); }

__INLINE FctERR INLINE__ TSL2591_SF_Clear_PERS(void) {
	return TSL2591_Write_Special(TSL2591__SF_CLR_NO_PERS); }


/*** Operation ***/
__INLINE FctERR INLINE__ TSL2591_Get_ChipID(uint8_t * id) {
	return TSL2591_Read(id, TSL2591__ID, 1); }

__INLINE FctERR INLINE__ TSL2591_Reset(void) {
	return TSL2591_Write_En(0x80); }

__INLINE FctERR INLINE__ TSL2591_Get_CH0(uint16_t * buf) {
	return TSL2591_Read_Word(buf, TSL2591__C0DATAL); }

__INLINE FctERR INLINE__ TSL2591_Get_CH1(uint16_t * buf) {
	return TSL2591_Read_Word(buf, TSL2591__C1DATAL); }


/****************************************************************/
#endif
#endif /* __TSL2591_EX_H__ */
/****************************************************************/
