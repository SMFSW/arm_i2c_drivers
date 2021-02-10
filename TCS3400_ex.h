/*!\file TCS3400_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief TCS3400 Driver extensions
** \details TCS3400: Color Light-to-Digital Converter
**/
/****************************************************************/
#ifndef __TCS3400_EX_H__
	#define __TCS3400_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "TCS3400.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum TCS3400_chan
** \brief Light type channels of TCS3400
**/
typedef enum PACK__ TCS3400_chan {
	TCS3400__CHAN_CLEAR = 0,	//!< Clear light channel
	TCS3400__CHAN_RED,			//!< Red light channel
	TCS3400__CHAN_GREEN,		//!< Green light channel
	TCS3400__CHAN_BLUE			//!< Blue light channel
} TCS3400_chan;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

/*** Configuration ***/
/*!\brief Write TCS3400 Enable register
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3400_Write_En(TCS3400_t * pCpnt, const uint8_t en) {
	return TCS3400_Write(pCpnt->cfg.slave_inst, (uint8_t *) &en, TCS3400__ENABLE, 1); }

/*!\brief Write TCS3400 Config register
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in] cfg - Configuration register value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3400_Write_Cfg(TCS3400_t * pCpnt, const uint8_t cfg) {
	return TCS3400_Write(pCpnt->cfg.slave_inst, (uint8_t *) &cfg, TCS3400__CONFIG, 1); }

/*!\brief Write TCS3400 Control register
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in] ctl - Control register value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3400_Write_Ctl(TCS3400_t * pCpnt, const uint8_t ctl) {
	return TCS3400_Write(pCpnt->cfg.slave_inst, (uint8_t *) &ctl, TCS3400__CONTROL, 1); }


/*!\brief Oscillator Enable / Disable
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3400_Set_PON(TCS3400_t * pCpnt, const bool en);

/*!\brief Clear module Enable / Disable
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3400_Set_AEN(TCS3400_t * pCpnt, const bool en);

/*!\brief Clear interrupt module Enable / Disable
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3400_Set_AIEN(TCS3400_t * pCpnt, const bool en);

/*!\brief WAIT module Enable / Disable
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3400_Set_WEN(TCS3400_t * pCpnt, const bool en);

/*!\brief Sleep After Interrupt Enable / Disable
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3400_Set_SAI(TCS3400_t * pCpnt, const bool en);


/*!\brief Gain configuration
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in] gain - Gain value
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3400_Set_Gain(TCS3400_t * pCpnt, const TCS3400_gain gain);


/*!\brief ALS Persistence configuration
** \param[in] pCpnt - Pointer to TCS3472 component
** \param[in] persist - Persistence value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3400_Set_Pesistence(TCS3400_t * pCpnt, const TCS3400_it_persist persist) {
	uTCS3400_REG__PERSIST PERS = { .Bits.PERSIST = persist };
	return TCS3400_Write(pCpnt->cfg.slave_inst, &PERS.Byte, TCS3400__PERSIST, 1); }


/*!\brief Integration time configuration
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in] integ - Integration time value (from 3ms to 712ms)
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3400_Set_Integration_Time(TCS3400_t * pCpnt, const uint16_t integ);

/*!\brief Wait time configuration
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in] wait - Wait time value (from 3ms to 8540ms)
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3400_Set_Wait_Time(TCS3400_t * pCpnt, const uint16_t wait);


/*!\brief ALS interrupt low threshold configuration
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in] thr - Low threshold value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3400_Set_AILT(TCS3400_t * pCpnt, const uint16_t thr) {
	return TCS3400_Write_Word(pCpnt->cfg.slave_inst, (uint16_t *) &thr, TCS3400__AILTL); }

/*!\brief ALS interrupt high threshold configuration
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in] thr - High threshold value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3400_Set_AIHT(TCS3400_t * pCpnt, const uint16_t thr) {
	return TCS3400_Write_Word(pCpnt->cfg.slave_inst, (uint16_t *) &thr, TCS3400__AIHTL); }

/*!\brief ALS interrupt thresholds configuration
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in] lthr - Low threshold value
** \param[in] hthr - High threshold value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3400_Set_AIT(TCS3400_t * pCpnt, const uint16_t lthr, const uint16_t hthr) {
	uint8_t DAT[4] = { LOBYTE(lthr), HIBYTE(lthr), LOBYTE(hthr), HIBYTE(hthr) };
	return TCS3400_Write(pCpnt->cfg.slave_inst, DAT, TCS3400__AILTL, sizeof(DAT)); }


/*** Interrupts Functions ***/
/*!\brief Force interruption
** \param[in] pCpnt - Pointer to TCS3400 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3400_Force_IT(TCS3400_t * pCpnt) {
	uint8_t REG = 0xFF;
	return TCS3400_Write(pCpnt->cfg.slave_inst, &REG, TCS3400__IFORCE, 1); }

/*!\brief Clear pending IR interruption
** \warning if event pin enabled, shall be called after read to reset pin
** \param[in] pCpnt - Pointer to TCS3400 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3400_Clear_IR_IT(TCS3400_t * pCpnt) {
	uint8_t REG = 0xFF;
	return TCS3400_Write(pCpnt->cfg.slave_inst, &REG, TCS3400__CICLEAR, 1); }

/*!\brief Clear All pending interruptions
** \warning if event pin enabled, shall be called after read to reset pin
** \param[in] pCpnt - Pointer to TCS3400 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3400_Clear_All_IT(TCS3400_t * pCpnt) {
	uint8_t REG = 0xFF;
	return TCS3400_Write(pCpnt->cfg.slave_inst, &REG, TCS3400__AICLEAR, 1); }


/*** Operation ***/
/*!\brief Get TCS3400 Revision ID
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in,out] id - pointer to Revision ID result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3400_Get_RevID(TCS3400_t * pCpnt, uint8_t * id) {
	return TCS3400_Read(pCpnt->cfg.slave_inst, id, TCS3400__REVID, 1); }

/*!\brief Get TCS3400 Device ID
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in,out] id - pointer to Device ID result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3400_Get_DeviceID(TCS3400_t * pCpnt, uint8_t * id) {
	return TCS3400_Read(pCpnt->cfg.slave_inst, id, TCS3400__ID, 1); }


/*!\brief Get All channels conversions
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in,out] buf - pointer to conversions tab result
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3400_Get_Channels(TCS3400_t * pCpnt, uint16_t buf[]);

/*!\brief Get Specific channel conversion
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in,out] buf - pointer to conversions result
** \param[in] chan - Channel to get
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3400_Get_Channel(TCS3400_t * pCpnt, uint16_t * buf, const TCS3400_chan chan) {
	if (chan > TCS3400__CHAN_BLUE)	{ return ERROR_VALUE; }	// Unknown channel
	return TCS3400_Read_Word(pCpnt->cfg.slave_inst, buf, TCS3400__CDATAL + (2 * chan)); }

/*!\brief Get Clear channel conversion
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in,out] clr - pointer to Clear conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3400_Get_Clear(TCS3400_t * pCpnt, uint16_t * clr) {
	return TCS3400_Read_Word(pCpnt->cfg.slave_inst, clr, TCS3400__CDATAL); }

/*!\brief Get Red channel conversion
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in,out] r - pointer to Red conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3400_Get_Red(TCS3400_t * pCpnt, uint16_t * r) {
	return TCS3400_Read_Word(pCpnt->cfg.slave_inst, r, TCS3400__RDATAL); }

/*!\brief Get Green channel conversion
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in,out] g - pointer to Green conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3400_Get_Green(TCS3400_t * pCpnt, uint16_t * g) {
	return TCS3400_Read_Word(pCpnt->cfg.slave_inst, g, TCS3400__GDATAL); }

/*!\brief Get Blue channel conversion
** \param[in] pCpnt - Pointer to TCS3400 component
** \param[in,out] b - pointer to Blue conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3400_Get_Blue(TCS3400_t * pCpnt, uint16_t * b) {
	return TCS3400_Read_Word(pCpnt->cfg.slave_inst, b, TCS3400__BDATAL); }


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* __TCS3400_EX_H__ */
/****************************************************************/
