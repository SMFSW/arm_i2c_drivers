/*!\file TSL2591_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
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
		uint8_t					NPAILTL;	//!< Persist ALS interrupt low threshold low byte
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
** \param[in] pCpnt - Pointer to TSL2591 component
** \param[in] en - Enable register value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TSL2591_Write_En(TSL2591_t * pCpnt, const uint8_t en) {
	return TSL2591_Write(pCpnt->cfg.slave_inst, (uint8_t *) &en, TSL2591__ENABLE, 1); }

/*!\brief Write TSL2591 Config
** \param[in] pCpnt - Pointer to TSL2591 component
** \param[in] cfg - Configuration register value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TSL2591_Write_Cfg(TSL2591_t * pCpnt, const uint8_t cfg) {
	return TSL2591_Write(pCpnt->cfg.slave_inst, (uint8_t *) &cfg, TSL2591__CONFIG, 1); }


/*!\brief Oscillator Enable / Disable
** \param[in] pCpnt - Pointer to TSL2591 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ TSL2591_Set_PON(TSL2591_t * pCpnt, const bool en);

/*!\brief ALS module Enable / Disable
** \param[in] pCpnt - Pointer to TSL2591 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ TSL2591_Set_AEN(TSL2591_t * pCpnt, const bool en);

/*!\brief ALS interrupt module Enable / Disable
** \param[in] pCpnt - Pointer to TSL2591 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ TSL2591_Set_AIEN(TSL2591_t * pCpnt, const bool en);

/*!\brief Gain configuration
** \param[in] pCpnt - Pointer to TSL2591 component
** \param[in] gain - Gain value
** \return FctERR - error code
**/
FctERR NONNULL__ TSL2591_Set_Gain(TSL2591_t * pCpnt, const TSL2591_gain gain);

/*!\brief Integration time configuration
** \param[in] pCpnt - Pointer to TSL2591 component
** \param[in] integ - Integration time value
** \return FctERR - error code
**/
FctERR NONNULL__ TSL2591_Set_Integration_Time(TSL2591_t * pCpnt, const TSL2591_integ integ);


/*!\brief ALS Persistence configuration
** \param[in] pCpnt - Pointer to TSL2591 component
** \param[in] persist - Persistence value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TSL2591_Set_ALS_Pesistence(TSL2591_t * pCpnt, const TSL2591_it_persist persist) {
	uTSL2591_REG__PERSIST PERS = { .Bits.PERSIST = persist };
	return TSL2591_Write(pCpnt->cfg.slave_inst, &PERS.Byte, TSL2591__PERSIST, 1); }


/*!\brief ALS interrupt low threshold configuration
** \param[in] pCpnt - Pointer to TSL2591 component
** \param[in] thr - Low threshold value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TSL2591_Set_AILT(TSL2591_t * pCpnt, const uint16_t thr) {
	return TSL2591_Write_Word(pCpnt->cfg.slave_inst, (uint16_t *) &thr, TSL2591__AILTL); }

/*!\brief ALS interrupt high threshold configuration
** \param[in] pCpnt - Pointer to TSL2591 component
** \param[in] thr - High threshold value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TSL2591_Set_AIHT(TSL2591_t * pCpnt, const uint16_t thr) {
	return TSL2591_Write_Word(pCpnt->cfg.slave_inst, (uint16_t *) &thr, TSL2591__AIHTL); }

/*!\brief ALS interrupt thresholds configuration
** \param[in] pCpnt - Pointer to TSL2591 component
** \param[in] lthr - Low threshold value
** \param[in] hthr - High threshold value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TSL2591_Set_AIT(TSL2591_t * pCpnt, const uint16_t lthr, const uint16_t hthr) {
	uint8_t DAT[4] = { LOBYTE(lthr), HIBYTE(lthr), LOBYTE(hthr), HIBYTE(hthr) };
	return TSL2591_Write(pCpnt->cfg.slave_inst, DAT, TSL2591__AILTL, sizeof(DAT)); }


/*!\brief ALS interrupt Persist Low threshold filter configuration
** \param[in] pCpnt - Pointer to TSL2591 component
** \param[in] thr - Persist Low threshold filter value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TSL2591_Set_NPAILT(TSL2591_t * pCpnt, const uint16_t thr) {
	return TSL2591_Write_Word(pCpnt->cfg.slave_inst, (uint16_t *) &thr, TSL2591__NPAILTL); }

/*!\brief ALS interrupt Persist High threshold filter configuration
** \param[in] pCpnt - Pointer to TSL2591 component
** \param[in] thr - Persist High threshold filter value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TSL2591_Set_NPAIHT(TSL2591_t * pCpnt, const uint16_t thr) {
	return TSL2591_Write_Word(pCpnt->cfg.slave_inst, (uint16_t *) &thr, TSL2591__NPAIHTL); }

/*!\brief ALS interrupt Persist threshold filters configuration
** \param[in] pCpnt - Pointer to TSL2591 component
** \param[in] lthr - Low threshold filter value
** \param[in] hthr - High threshold filter value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TSL2591_Set_NPAIT(TSL2591_t * pCpnt, const uint16_t lthr, const uint16_t hthr) {
	uint8_t DAT[4] = { LOBYTE(lthr), HIBYTE(lthr), LOBYTE(hthr), HIBYTE(hthr) };
	return TSL2591_Write(pCpnt->cfg.slave_inst, DAT, TSL2591__NPAILTL, sizeof(DAT)); }

/*** Special Functions ***/
/*!\brief Force ALS interruption
** \param[in] pCpnt - Pointer to TSL2591 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TSL2591_SF_Force_IT(TSL2591_t * pCpnt) {
	return TSL2591_Write_Special(pCpnt->cfg.slave_inst, TSL2591__SF_FORCE_IT); }

/*!\brief Clear pending ALS interruption
** \warning if event pin enabled, shall be called after read to reset pin
** \param[in] pCpnt - Pointer to TSL2591 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TSL2591_SF_Clear_IT(TSL2591_t * pCpnt) {
	return TSL2591_Write_Special(pCpnt->cfg.slave_inst, TSL2591__SF_CLR_ALS_IT); }

/*!\brief Clear pending ALS & Persistence interruptions
** \warning if event pin enabled, shall be called after read to reset pin
** \param[in] pCpnt - Pointer to TSL2591 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TSL2591_SF_Clear_IT_PERS(TSL2591_t * pCpnt) {
	return TSL2591_Write_Special(pCpnt->cfg.slave_inst, TSL2591__SF_CLR_ALS_AND_NO_PERS); }

/*!\brief Clear pending Persistence interruption
** \warning if event pin enabled, shall be called after read to reset pin
** \param[in] pCpnt - Pointer to TSL2591 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TSL2591_SF_Clear_PERS(TSL2591_t * pCpnt) {
	return TSL2591_Write_Special(pCpnt->cfg.slave_inst, TSL2591__SF_CLR_NO_PERS); }


/*** Operation ***/
/*!\brief Get TSL2591 chip ID
** \param[in] pCpnt - Pointer to TSL2591 component
** \param[in,out] id - pointer to chip ID result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TSL2591_Get_ChipID(TSL2591_t * pCpnt, uint8_t * id) {
	return TSL2591_Read(pCpnt->cfg.slave_inst, id, TSL2591__ID, 1); }

/*!\brief Reset TSL2591 chip
** \param[in] pCpnt - Pointer to TSL2591 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TSL2591_Reset(TSL2591_t * pCpnt) {
	return TSL2591_Write_En(pCpnt, 0x80); }

/*!\brief Get Full conversion (Channel 0)
** \param[in] pCpnt - Pointer to TSL2591 component
** \param[in,out] full - pointer to Full conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TSL2591_Get_Full(TSL2591_t * pCpnt, uint16_t * full) {
	return TSL2591_Read_Word(pCpnt->cfg.slave_inst, full, TSL2591__C0DATAL); }

/*!\brief Get IR conversion (Channel 1)
** \param[in] pCpnt - Pointer to TSL2591 component
** \param[in,out] ir - pointer to IR conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TSL2591_Get_IR(TSL2591_t * pCpnt, uint16_t * ir) {
	return TSL2591_Read_Word(pCpnt->cfg.slave_inst, ir, TSL2591__C1DATAL); }


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __TSL2591_EX_H__ */
/****************************************************************/
