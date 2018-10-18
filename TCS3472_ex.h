/*!\file TCS3472_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief TCS3472 Driver extensions
** \details TCS3472: Color light-to-digital converter with IR filter
**/
/****************************************************************/
#ifndef __TCS3472_EX_H__
	#define __TCS3472_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "TCS3472.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum TCS3472_chan
** \brief Light type channels of TCS3472
**/
typedef enum PACK__ TCS3472_chan {
	TCS3472__CHAN_CLEAR = 0,	//!< Clear light channel
	TCS3472__CHAN_RED,			//!< Red light channel
	TCS3472__CHAN_GREEN,		//!< Green light channel
	TCS3472__CHAN_BLUE			//!< Blue light channel
} TCS3472_chan;


/*!\union uTCS3472_REG_MAP
** \brief Union of TCS3472 registry map
**/
typedef union uTCS3472_REG_MAP {
	uint8_t Bytes[28];
	struct {
		uTCS3472_REG__ENABLE	ENABLE;		//!< Enables states and interrupts
		uint8_t					ATIME;		//!< RGBC time
		uint8_t					:8;
		uint8_t					WTIME;		//!< Wait time
		uint8_t					AILTL;		//!< Clear interrupt low threshold low byte
		uint8_t					AILTH;		//!< Clear interrupt low threshold high byte
		uint8_t					AIHTL;		//!< Clear interrupt high threshold low byte
		uint8_t					AIHTH;		//!< Clear interrupt high threshold high byte
		uint8_t					:8;
		uint8_t					:8;
		uint8_t					:8;
		uint8_t					:8;
		uTCS3472_REG__PERSIST	PERS;		//!< Interrupt persistence filter
		uTCS3472_REG__CONFIG	CONFIG;		//!< Configuration
		uint8_t					:8;
		uTCS3472_REG__CONTROL	CONTROL;	//!< Control
		uint8_t					:8;
		uint8_t					:8;
		uint8_t					ID;			//!< Device ID
		uTCS3472_REG__STATUS	STATUS;		//!< Device status
		uint8_t					CDATAL;		//!< Clear data low byte
		uint8_t					CDATAH;		//!< Clear data high byte
		uint8_t					RDATAL;		//!< Red data low byte
		uint8_t					RDATAH;		//!< Red data high byte
		uint8_t					GDATAL;		//!< Green data low byte
		uint8_t					GDATAH;		//!< Green data high byte
		uint8_t					BDATAL;		//!< Blue data low byte
		uint8_t					BDATAH;		//!< Blue data high byte
	} Reg;
} uTCS3472_REG_MAP;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

/*** Configuration ***/
/*!\brief Write TCS3472 Enable register
** \param[in] pCpnt - Pointer to TCS3472 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3472_Write_En(TCS3472_t * pCpnt, const uint8_t en) {
	return TCS3472_Write(pCpnt->cfg.slave_inst, (uint8_t *) &en, TCS3472__ENABLE, 1); }

/*!\brief Write TCS3472 Config register
** \param[in] pCpnt - Pointer to TCS3472 component
** \param[in] cfg - Configuration register value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3472_Write_Cfg(TCS3472_t * pCpnt, const uint8_t cfg) {
	return TCS3472_Write(pCpnt->cfg.slave_inst, (uint8_t *) &cfg, TCS3472__CONFIG, 1); }

/*!\brief Write TCS3472 Control register
** \param[in] pCpnt - Pointer to TCS3472 component
** \param[in] ctl - Control register value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3472_Write_Ctl(TCS3472_t * pCpnt, const uint8_t ctl) {
	return TCS3472_Write(pCpnt->cfg.slave_inst, (uint8_t *) &ctl, TCS3472__CONTROL, 1); }


/*!\brief Oscillator Enable / Disable
** \param[in] pCpnt - Pointer to TCS3472 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3472_Set_PON(TCS3472_t * pCpnt, const bool en);

/*!\brief Clear module Enable / Disable
** \param[in] pCpnt - Pointer to TCS3472 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3472_Set_AEN(TCS3472_t * pCpnt, const bool en);

/*!\brief Clear interrupt module Enable / Disable
** \param[in] pCpnt - Pointer to TCS3472 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3472_Set_AIEN(TCS3472_t * pCpnt, const bool en);

/*!\brief WAIT module Enable / Disable
** \param[in] pCpnt - Pointer to TCS3472 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3472_Set_WEN(TCS3472_t * pCpnt, const bool en);


/*!\brief Gain configuration
** \param[in] pCpnt - Pointer to TCS3472 component
** \param[in] gain - Gain value
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3472_Set_Gain(TCS3472_t * pCpnt, const TCS3472_gain gain);

/*!\brief Integration time configuration
** \param[in] pCpnt - Pointer to TCS3472 component
** \param[in] integ - Integration time value
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3472_Set_Integration_Time(TCS3472_t * pCpnt, const uint16_t integ);

/*!\brief Wait time configuration
** \param[in] pCpnt - Pointer to TCS3472 component
** \param[in] wait - Wait time value
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3472_Set_Wait_Time(TCS3472_t * pCpnt, const uint16_t wait);


/*!\brief ALS interrupt low threshold configuration
** \param[in] pCpnt - Pointer to TCS3472 component
** \param[in] thr - Low threshold value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3472_Set_AILT(TCS3472_t * pCpnt, const uint16_t thr) {
	return TCS3472_Write_Word(pCpnt->cfg.slave_inst, (uint16_t *) &thr, TCS3472__AILTL); }

/*!\brief ALS interrupt high threshold configuration
** \param[in] pCpnt - Pointer to TCS3472 component
** \param[in] thr - High threshold value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3472_Set_AIHT(TCS3472_t * pCpnt, const uint16_t thr) {
	return TCS3472_Write_Word(pCpnt->cfg.slave_inst, (uint16_t *) &thr, TCS3472__AIHTL); }

/*!\brief ALS interrupt thresholds configuration
** \param[in] pCpnt - Pointer to TCS3472 component
** \param[in] lthr - Low threshold value
** \param[in] hthr - High threshold value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3472_Set_AIT(TCS3472_t * pCpnt, const uint16_t lthr, const uint16_t hthr) {
	uint8_t DAT[4] = { LOBYTE(hthr), HIBYTE(hthr), LOBYTE(lthr), HIBYTE(lthr) };
	return TCS3472_Write(pCpnt->cfg.slave_inst, DAT, TCS3472__AILTL, sizeof(DAT)); }


/*** Special Functions ***/
/*!\brief Clear pending interruption
** \warning if event pin enabled, shall be called after read to reset pin
** \param[in] pCpnt - Pointer to TCS3472 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3472_SF_Clear_IT(TCS3472_t * pCpnt) {
	return TCS3472_Write_Special(pCpnt->cfg.slave_inst, TCS3472__SF_CLR_IT); }


/*** Operation ***/
/*!\brief Get TCS3472 chip ID
** \param[in] pCpnt - Pointer to TCS3472 component
** \param[in,out] id - pointer to chip ID result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3472_Get_ChipID(TCS3472_t * pCpnt, uint8_t * id) {
	return TCS3472_Read(pCpnt->cfg.slave_inst, id, TCS3472__ID, 1); }


/*!\brief Get All channels conversions
** \param[in] pCpnt - Pointer to TCS3472 component
** \param[in,out] tab - pointer to conversions tab result
** \return FctERR - error code
**/
FctERR NONNULL__ TCS3472_Get_Channels(TCS3472_t * pCpnt, uint16_t tab[]);

/*!\brief Get Specific channel conversion
** \param[in] pCpnt - Pointer to TCS3472 component
** \param[in,out] buf - pointer to conversions result
** \param[in] chan - Channel to get
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3472_Get_Channel(TCS3472_t * pCpnt, uint16_t * buf, const TCS3472_chan chan) {
	if (chan > TCS3472__CHAN_BLUE)	{ return ERROR_VALUE; }	// Unknown channel
	return TCS3472_Read_Word(pCpnt->cfg.slave_inst, buf, TCS3472__CDATAL + (2 * chan)); }

/*!\brief Get Clear channel conversion
** \param[in] pCpnt - Pointer to TCS3472 component
** \param[in,out] clr - pointer to Clear conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3472_Get_Clear(TCS3472_t * pCpnt, uint16_t * clr) {
	return TCS3472_Read_Word(pCpnt->cfg.slave_inst, clr, TCS3472__CDATAL); }

/*!\brief Get Red channel conversion
** \param[in] pCpnt - Pointer to TCS3472 component
** \param[in,out] r - pointer to Red conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3472_Get_Red(TCS3472_t * pCpnt, uint16_t * r) {
	return TCS3472_Read_Word(pCpnt->cfg.slave_inst, r, TCS3472__RDATAL); }

/*!\brief Get Green channel conversion
** \param[in] pCpnt - Pointer to TCS3472 component
** \param[in,out] g - pointer to Green conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3472_Get_Green(TCS3472_t * pCpnt, uint16_t * g) {
	return TCS3472_Read_Word(pCpnt->cfg.slave_inst, g, TCS3472__GDATAL); }

/*!\brief Get Blue channel conversion
** \param[in] pCpnt - Pointer to TCS3472 component
** \param[in,out] b - pointer to Blue conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TCS3472_Get_Blue(TCS3472_t * pCpnt, uint16_t * b) {
	return TCS3472_Read_Word(pCpnt->cfg.slave_inst, b, TCS3472__BDATAL); }


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __TCS3472_EX_H__ */
/****************************************************************/
