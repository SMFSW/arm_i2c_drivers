/*!\file APDS9930_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief APDS9930 Driver extensions
** \details APDS9930: Digital Proximity and Ambient Light Sensor
**/
/****************************************************************/
#ifndef APDS9930_EX_H__
	#define APDS9930_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "APDS9930.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/
// TODO: add Configuration register write of AGL & PDL
// TODO: add functions to write Persistence

// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum APDS9930_chan
** \brief Light types of APDS9930
**/
typedef enum PACK__ APDS9930_chan {
	APDS9930__CHAN_FULL = 0,	//!< Full light spectrum channel (channel 0)
	APDS9930__CHAN_IR,			//!< Infra Red light channel (channel 1)
	APDS9930__CHAN_PROXIMITY,	//!< Proximity channel
	APDS9930__CHAN_VISIBLE		//!< Visible light (channel 0 - channel 1)
} APDS9930_chan;


/*!\union uAPDS9930_REG_MAP
** \brief Union of APDS9930 registry map
**/
typedef union uAPDS9930_REG_MAP {
	uint8_t Bytes[31];
	struct PACK__ {
		uAPDS9930_REG__ENABLE	ENABLE;		//!< Enables of states and interrupts
		uint8_t					ATIME;		//!< ALS ADC time
		uint8_t					PTIME;		//!< Proximity ADC time
		uint8_t					WTIME;		//!< Wait time
		uint8_t					AILTL;		//!< ALS interrupt low threshold low byte
		uint8_t					AILTH;		//!< ALS interrupt low threshold high byte
		uint8_t					AIHTL;		//!< ALS interrupt high threshold low byte
		uint8_t					AIHTH;		//!< ALS interrupt high threshold high byte
		uint8_t					PILTL;		//!< Proximity interrupt low threshold low byte
		uint8_t					PILTH;		//!< Proximity interrupt low threshold high byte
		uint8_t					PIHTL;		//!< Proximity interrupt high threshold low byte
		uint8_t					PIHTH;		//!< Proximity interrupt high threshold high byte
		uAPDS9930_REG__PERSIST	PERS;		//!< Interrupt persistence filters
		uAPDS9930_REG__CONFIG	CONFIG;		//!< Configuration
		uint8_t					PPULSE;		//!< Proximity pulse count
		uAPDS9930_REG__CONTROL	CONTROL;	//!< Gain control register
		uint8_t					:8;
		uint8_t					:8;
		uint8_t					ID;			//!< Device ID
		uAPDS9930_REG__STATUS	STATUS;		//!< Device status
		uint8_t					CH0DATAL;	//!< CH0 ADC low data byte (Full)
		uint8_t					CH0DATAH;	//!< CH0 ADC high data byte (Full)
		uint8_t					CH1DATAL;	//!< CH1 ADC low data byte (IR)
		uint8_t					CH1DATAH;	//!< CH1 ADC high data byte (IR)
		uint8_t					PDATAL;		//!< Proximity ADC low data register
		uint8_t					PDATAH;		//!< Proximity ADC high data register
		uint8_t					:8;
		uint8_t					:8;
		uint8_t					:8;
		uint8_t					:8;
		int8_t					POFFSET;	//!< Proximity offset register
	} Reg;
} uAPDS9930_REG_MAP;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

/*** Configuration ***/
/*!\brief Write APDS9930 Enable register
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] en - Enable register value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9930_Write_En(APDS9930_t * const pCpnt, const uint8_t en) {
	return APDS9930_Write(pCpnt->cfg.slave_inst, (uint8_t *) &en, APDS9930__ENABLE, 1); }

/*!\brief Write APDS9930 Config
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] cfg - Configuration register value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9930_Write_Cfg(APDS9930_t * const pCpnt, const uint8_t cfg) {
	return APDS9930_Write(pCpnt->cfg.slave_inst, (uint8_t *) &cfg, APDS9930__CONFIG, 1); }


/*!\brief Write APDS9930 Control
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] ctrl - Control register value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9930_Write_Ctrl(APDS9930_t * const pCpnt, const uint8_t ctrl) {
	uAPDS9930_REG__CONTROL CTRL;
	CTRL.Byte = ctrl;
	CTRL.Bits.PDIODE = APDS9930__PDIODE_CH1;
	return APDS9930_Write(pCpnt->cfg.slave_inst, &CTRL.Byte, APDS9930__CONTROL, 1); }


/*!\brief Oscillator Enable / Disable
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Set_PON(APDS9930_t * const pCpnt, const bool en);

/*!\brief ALS module Enable / Disable
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Set_AEN(APDS9930_t * const pCpnt, const bool en);

/*!\brief Proximity module Enable / Disable
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Set_PEN(APDS9930_t * const pCpnt, const bool en);

/*!\brief ALS interrupt module Enable / Disable
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Set_AIEN(APDS9930_t * const pCpnt, const bool en);

/*!\brief Proximity interrupt module Enable / Disable
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Set_PIEN(APDS9930_t * const pCpnt, const bool en);

/*!\brief WAIT module Enable / Disable
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Set_WEN(APDS9930_t * const pCpnt, const bool en);

/*!\brief Sleep After Interrupt Enable / Disable
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Set_SAI(APDS9930_t * const pCpnt, const bool en);


/*!\brief ALS Gain configuration
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] gain - Gain value
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Set_ALS_Gain(APDS9930_t * const pCpnt, const APDS9930_als_gain_ex gain);

/*!\brief Proximity Gain configuration
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] gain - Gain value
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Set_Prox_Gain(APDS9930_t * const pCpnt, const APDS9930_prox_gain gain);


/*!\brief Proximity Drive configuration
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] pdrive - Proximity Drive Strength value
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Set_Prox_Drive_Strength(APDS9930_t * const pCpnt, const APDS9930_drive_strength_ex pdrive);


/*!\brief ALS Persistence configuration
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] persist - Persistence value
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Set_ALS_Pesistence(APDS9930_t * const pCpnt, const APDS9930_als_it_persist persist);

/*!\brief Proximity Persistence configuration
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] persist - Persistence value
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Set_Prox_Pesistence(APDS9930_t * const pCpnt, const APDS9930_prox_it_persist persist);


/*!\brief ALS Integration time configuration
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] integ - Integration time value (from 3ms to 699ms)
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Set_ALS_Integration_Time(APDS9930_t * const pCpnt, const uint16_t integ);

/*!\brief Proximity Integration time configuration
** \note It is recommended that this register be programmed to a value of 0xff (1 cycle, 1023 bits). -> integ = 3
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] integ - Integration time value (from 3ms to 699ms)
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Set_Prox_Integration_Time(APDS9930_t * const pCpnt, const uint16_t integ);

/*!\brief Wait time configuration
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] wait - Wait time value (from 3ms to 8400ms)
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Set_Wait_Time(APDS9930_t * const pCpnt, const uint16_t wait);


/*!\brief ALS interrupt low threshold configuration
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] thr - Low threshold value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9930_Set_AILT(APDS9930_t * const pCpnt, const uint16_t thr) {
	return APDS9930_Write_Word(pCpnt->cfg.slave_inst, (uint16_t *) &thr, APDS9930__AILTL); }

/*!\brief ALS interrupt high threshold configuration
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] thr - High threshold value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9930_Set_AIHT(APDS9930_t * const pCpnt, const uint16_t thr) {
	return APDS9930_Write_Word(pCpnt->cfg.slave_inst, (uint16_t *) &thr, APDS9930__AIHTL); }

/*!\brief ALS interrupt thresholds configuration
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] lthr - Low threshold value
** \param[in] hthr - High threshold value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9930_Set_AIT(APDS9930_t * const pCpnt, const uint16_t lthr, const uint16_t hthr) {
	uint8_t DAT[4] = { LOBYTE(lthr), HIBYTE(lthr), LOBYTE(hthr), HIBYTE(hthr) };
	return APDS9930_Write(pCpnt->cfg.slave_inst, DAT, APDS9930__AILTL, sizeof(DAT)); }


/*!\brief Proximity interrupt Persist Low threshold filter configuration
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] thr - Persist Low threshold filter value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9930_Set_PILT(APDS9930_t * const pCpnt, const uint16_t thr) {
	return APDS9930_Write_Word(pCpnt->cfg.slave_inst, (uint16_t *) &thr, APDS9930__PILTL); }

/*!\brief Proximity interrupt Persist High threshold filter configuration
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] thr - Persist High threshold filter value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9930_Set_PIHT(APDS9930_t * const pCpnt, const uint16_t thr) {
	return APDS9930_Write_Word(pCpnt->cfg.slave_inst, (uint16_t *) &thr, APDS9930__PIHTL); }

/*!\brief Proximity interrupt Persist threshold filters configuration
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] lthr - Low threshold filter value
** \param[in] hthr - High threshold filter value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9930_Set_PIT(APDS9930_t * const pCpnt, const uint16_t lthr, const uint16_t hthr) {
	uint8_t DAT[4] = { LOBYTE(lthr), HIBYTE(lthr), LOBYTE(hthr), HIBYTE(hthr) };
	return APDS9930_Write(pCpnt->cfg.slave_inst, DAT, APDS9930__PILTL, sizeof(DAT)); }

/*** Special Functions ***/
/*!\brief Clear pending ALS interruption
** \warning if event pin enabled, shall be called after read to reset pin
** \param[in] pCpnt - Pointer to APDS9930 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9930_SF_Clear_ALS_IT(APDS9930_t * const pCpnt) {
	return APDS9930_Write_Special(pCpnt->cfg.slave_inst, APDS9930__SF_CLR_ALS_IT); }

/*!\brief Clear pending Proximity interruption
** \warning if event pin enabled, shall be called after read to reset pin
** \param[in] pCpnt - Pointer to APDS9930 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9930_SF_Clear_PROX_IT(APDS9930_t * const pCpnt) {
	return APDS9930_Write_Special(pCpnt->cfg.slave_inst, APDS9930__SF_CLR_PROX_IT); }

/*!\brief Clear pending ALS & Proximity interruptions
** \warning if event pin enabled, shall be called after read to reset pin
** \param[in] pCpnt - Pointer to APDS9930 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9930_SF_Clear_ALS_PROX_IT(APDS9930_t * const pCpnt) {
	return APDS9930_Write_Special(pCpnt->cfg.slave_inst, APDS9930__SF_CLR_PROX_ALS_IT); }


/*** Operation ***/
/*!\brief Get APDS9930 chip ID
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in,out] id - pointer to chip ID result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9930_Get_ChipID(APDS9930_t * const pCpnt, uint8_t * id) {
	return APDS9930_Read(pCpnt->cfg.slave_inst, id, APDS9930__ID, 1); }

/*!\brief Get Full conversion (Channel 0)
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in,out] full - pointer to Full conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9930_Get_Full(APDS9930_t * const pCpnt, uint16_t * full) {
	return APDS9930_Read_Word(pCpnt->cfg.slave_inst, full, APDS9930__CH0DATAL); }

/*!\brief Get IR conversion (Channel 1)
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in,out] ir - pointer to IR conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9930_Get_IR(APDS9930_t * const pCpnt, uint16_t * ir) {
	return APDS9930_Read_Word(pCpnt->cfg.slave_inst, ir, APDS9930__CH1DATAL); }

/*!\brief Get Proximity conversion
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in,out] prox - pointer to Proximity conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9930_Get_Prox(APDS9930_t * const pCpnt, uint16_t * prox) {
	return APDS9930_Read_Word(pCpnt->cfg.slave_inst, prox, APDS9930__PDATAL); }


/*******************/
/*** GPIO access ***/
/*******************/

/*!\brief Interrupt GPIO pin init for APDS9930
** \weak APDS9930 Interrupt GPIO pin init may be user implemented if needed
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in] GPIOx - INT port
** \param[in] GPIO_Pin - INT pin
** \param[in] GPIO_Active: INT pin active state
**/
void NONNULL__ APDS9930_INT_GPIO_Init(APDS9930_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active);

/*!\brief Interrupt GPIO pin getter for APDS9930
** \weak APDS9930 Interrupt GPIO pin getter may be user implemented if needed
** \param[in] pCpnt - Pointer to APDS9930 component
** \param[in,out] pState - Pointer to INT pin state variable (0: inactive, 1: active)
**/
void NONNULL__ APDS9930_INT_GPIO_Get(APDS9930_t * const pCpnt, bool * const pState);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* APDS9930_EX_H__ */
/****************************************************************/
