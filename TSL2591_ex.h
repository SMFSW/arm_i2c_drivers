/*!\file TSL2591_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief TSL2591 Driver extensions
** \details TSL2591: Very-high sensitivity light-to-digital converter
**/
/****************************************************************/
#ifndef __TSL2591_EX_H__
	#define __TSL2591_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "TSL2591.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum TSL2591_light
** \brief Light types of TSL2591
**/
typedef enum PACK__ TSL2591_light {
	TSL2591__LIGHT_FULL = 0,	//!< Full light spectrum (channel 0)
	TSL2591__LIGHT_IR,			//!< Infra Red light (channel 1)
	TSL2591__LIGHT_VISIBLE		//!< Visible light (channel 0 - channel 1)
} TSL2591_light;


/*!\union uTSL2591_REG_MAP
** \brief Union of TSL2591 registry map
**/
typedef union uTSL2591_REG_MAP {
	uint8_t Bytes[23];
	struct {
		uTSL2591_REG__ENABLE	ENABLE;		//!< Enables states and interrupts
		uTSL2591_REG__CONFIG	CONFIG;		//!< ALS gain and integration time configuration
		uint8_t					:8;
		uint8_t					:8;
		uint8_t					AILTL;		//!< ALS interrupt low threshold low byte
		uint8_t					AILTH;		//!< ALS interrupt low threshold high byte
		uint8_t					AIHTL;		//!< ALS interrupt high threshold low byte
		uint8_t					AIHTH;		//!< ALS interrupt high threshold high byte
		uint8_t					NPAILTl;	//!< Persist ALS interrupt low threshold low byte
		uint8_t					NPAILTH;	//!< Persist ALS interrupt low threshold high byte
		uint8_t					NPAIHTL;	//!< Persist ALS interrupt high threshold low byte
		uint8_t					NPAIHTH;	//!< Persist ALS interrupt high threshold high byte
		uTSL2591_REG__PERSIST	PERSIST;	//!< Interrupt persistence filter
		uint8_t					PID;		//!< Package ID
		uint8_t					ID;			//!< Device ID
		uTSL2591_REG__STATUS	STATUS;		//!< Device status
		uint8_t					C0DATAL;	//!< CH0 ADC low data byte (Full)
		uint8_t					C0DATAH;	//!< CH0 ADC high data byte (Full)
		uint8_t					C1DATAL;	//!< CH1 ADC low data byte (IR)
		uint8_t					C1DATAH;	//!< CH1 ADC high data byte (IR)
		} Reg;
} uTSL2591_REG_MAP;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

/*** Configuration ***/
/*!\brief Write TSL2591 Enable register
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TSL2591_Write_En(const uint8_t en) {
	return TSL2591_Write((uint8_t *) &en, TSL2591__ENABLE, 1); }

/*!\brief Write TSL2591 Config
** \param[in] cfg - Configuration register value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TSL2591_Write_Cfg(const uint8_t cfg) {
	return TSL2591_Write((uint8_t *) &cfg, TSL2591__CONFIG, 1); }


/*!\brief Oscillator Enable / Disable
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR TSL2591_Set_PON(const bool en);

/*!\brief ALS module Enable / Disable
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR TSL2591_Set_AEN(const bool en);

/*!\brief ALS interrupt module Enable / Disable
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR TSL2591_Set_AIEN(const bool en);

/*!\brief Gain configuration
** \param[in] gain - Gain value
** \return FctERR - error code
**/
FctERR TSL2591_Set_Gain(const TSL2591_gain gain);

/*!\brief Integration time configuration
** \param[in] integ - Integration time value
** \return FctERR - error code
**/
FctERR TSL2591_Set_Integration_Time(const TSL2591_integ integ);


/*!\brief ALS interrupt low threshold configuration
** \param[in] thr - Low threshold value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TSL2591_Set_AILT(const uint16_t thr) {
	return TSL2591_Write_Word((uint16_t *) &thr, TSL2591__AILTL); }

/*!\brief ALS interrupt high threshold configuration
** \param[in] thr - High threshold value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TSL2591_Set_AIHT(const uint16_t thr) {
	return TSL2591_Write_Word((uint16_t *) &thr, TSL2591__AIHTL); }

/*!\brief ALS interrupt thresholds configuration
** \param[in] lthr - Low threshold value
** \param[in] hthr - High threshold value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TSL2591_Set_AIT(const uint16_t lthr, const uint16_t hthr) {
	uint8_t DAT[4] = { LOBYTE(hthr), HIBYTE(hthr), LOBYTE(lthr), HIBYTE(lthr) };
	return TSL2591_Write(DAT, TSL2591__AILTL, sizeof(DAT)); }


/*!\brief ALS interrupt Persist Low threshold filter configuration
** \param[in] thr - Persist Low threshold filter value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TSL2591_Set_NPAILT(const uint16_t thr) {
	return TSL2591_Write_Word((uint16_t *) &thr, TSL2591__NPAILTL); }

/*!\brief ALS interrupt Persist High threshold filter configuration
** \param[in] thr - Persist High threshold filter value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TSL2591_Set_NPAIHT(const uint16_t thr) {
	return TSL2591_Write_Word((uint16_t *) &thr, TSL2591__NPAIHTL); }

/*!\brief ALS interrupt Persist threshold filters configuration
** \param[in] lthr - Low threshold filter value
** \param[in] hthr - High threshold filter value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TSL2591_Set_NPAIT(const uint16_t lthr, const uint16_t hthr) {
	uint8_t DAT[4] = { LOBYTE(hthr), HIBYTE(hthr), LOBYTE(lthr), HIBYTE(lthr) };
	return TSL2591_Write(DAT, TSL2591__NPAILTL, sizeof(DAT)); }

/*** Special Functions ***/
/*!\brief Force ALS interruption
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TSL2591_SF_Force_IT(void) {
	return TSL2591_Write_Special(TSL2591__SF_FORCE_IT); }

/*!\brief Clear pending ALS interruption
** \warning if event pin enabled, shall be called after read to reset pin
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TSL2591_SF_Clear_IT(void) {
	return TSL2591_Write_Special(TSL2591__SF_CLR_ALS_IT); }

/*!\brief Clear pending ALS & Persistence interruptions
** \warning if event pin enabled, shall be called after read to reset pin
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TSL2591_SF_Clear_IT_PERS(void) {
	return TSL2591_Write_Special(TSL2591__SF_CLR_ALS_AND_NO_PERS); }

/*!\brief Clear pending Persistence interruption
** \warning if event pin enabled, shall be called after read to reset pin
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TSL2591_SF_Clear_PERS(void) {
	return TSL2591_Write_Special(TSL2591__SF_CLR_NO_PERS); }


/*** Operation ***/
/*!\brief Get TSL2591 chip ID
** \param[in,out] id - pointer to chip ID result
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TSL2591_Get_ChipID(uint8_t * id) {
	return TSL2591_Read(id, TSL2591__ID, 1); }

/*!\brief Reset TSL2591 chip
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TSL2591_Reset(void) {
	return TSL2591_Write_En(0x80); }

/*!\brief Get Full conversion (Channel 0)
** \param[in,out] full - pointer to Full conversion result
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TSL2591_Get_Full(uint16_t * full) {
	return TSL2591_Read_Word(full, TSL2591__C0DATAL); }

/*!\brief Get IR conversion (Channel 1)
** \param[in,out] ir - pointer to IR conversion result
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TSL2591_Get_IR(uint16_t * ir) {
	return TSL2591_Read_Word(ir, TSL2591__C1DATAL); }


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __TSL2591_EX_H__ */
/****************************************************************/
