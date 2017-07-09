/*!\file TCS3472_ex.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief TCS3472 Driver extensions declarations
** \details TCS3472: Color light-to-digital converter with IR filter
**/
/****************************************************************/
#ifndef __TCS3472_EX_H__
	#define __TCS3472_EX_H__

#include "TCS3472.h"

#include "sarmfsw.h"
#include CMSIS_INC
#include CMSIS_CFG

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
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
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3472_Write_En(uint8_t en) {
	return TCS3472_Write(&en, TCS3472__ENABLE, 1); }

/*!\brief Write TCS3472 Config register
** \param[in] cfg - Configuration register value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3472_Write_Cfg(uint8_t cfg) {
	return TCS3472_Write(&cfg, TCS3472__CONFIG, 1); }

/*!\brief Write TCS3472 Control register
** \param[in] ctl - Control register value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3472_Write_Ctl(uint8_t ctl) {
	return TCS3472_Write(&ctl, TCS3472__CONTROL, 1); }


/*!\brief oscillator Enable / Disable
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR TCS3472_Set_PON(bool en);

/*!\brief Clear module Enable / Disable
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR TCS3472_Set_AEN(bool en);

/*!\brief Clear interrupt module Enable / Disable
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR TCS3472_Set_AIEN(bool en);

/*!\brief WAIT module Enable / Disable
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR TCS3472_Set_WEN(bool en);


/*!\brief Gain configuration
** \param[in] gain - Gain value
** \return FctERR - error code
**/
FctERR TCS3472_Set_Gain(TCS3472_gain gain);

/*!\brief Integration time configuration
** \param[in] integ - Integration time value
** \return FctERR - error code
**/
FctERR TCS3472_Set_Integration_Time(uint16_t integ);

/*!\brief Wait time configuration
** \param[in] wait - Wait time value
** \return FctERR - error code
**/
FctERR TCS3472_Set_Wait_Time(uint16_t wait);


/*!\brief ALS interrupt low threshold configuration
** \param[in] thr - Low threshold value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3472_Set_AILT(uint16_t thr) {
	return TCS3472_Write_Word(&thr, TCS3472__AILTL); }

/*!\brief ALS interrupt high threshold configuration
** \param[in] thr - High threshold value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3472_Set_AIHT(uint16_t thr) {
	return TCS3472_Write_Word(&thr, TCS3472__AIHTL); }

/*!\brief ALS interrupt thresholds configuration
** \param[in] lthr - Low threshold value
** \param[in] hthr - High threshold value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3472_Set_AIT(uint16_t lthr, uint16_t hthr) {
	uint8_t DAT[4] = { (hthr & 0xFF), (hthr / 0x100), (lthr & 0xFF), (lthr / 0x100) };
	return TCS3472_Write(DAT, TCS3472__AILTL, sizeof(DAT)); }


/*** Special Functions ***/
/*!\brief Clear pending interruption
** \warning if event pin enabled, shall be called after read to reset pin
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3472_SF_Clear_IT(void) {
	return TCS3472_Write_Special(TCS3472__SF_CLR_IT); }


/*** Operation ***/
/*!\brief Get TCS3472 chip ID
** \param[in,out] id - pointer to chip ID result
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3472_Get_ChipID(uint8_t * id) {
	return TCS3472_Read(id, TCS3472__ID, 1); }


/*!\brief Get All channels conversions
** \param[in,out] tab - pointer to conversions tab result
** \return FctERR - error code
**/
FctERR TCS3472_Get_Channels(uint16_t tab[]);

/*!\brief Get Specific channel conversion
** \param[in,out] buf - pointer to conversions result
** \param[in] chan - Channel to get
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3472_Get_Channel(uint16_t * buf, TCS3472_chan chan) {
	if (chan > TCS3472__CHAN_BLUE)	{ return ERR_VALUE; }	// Unknown channel
	return TCS3472_Read_Word(buf, TCS3472__CDATAL + (2 * chan)); }

/*!\brief Get Clear channel conversion
** \param[in,out] clr - pointer to Clear conversion result
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3472_Get_Clear(uint16_t * clr) {
	return TCS3472_Read_Word(clr, TCS3472__CDATAL); }

/*!\brief Get Red channel conversion
** \param[in,out] r - pointer to Red conversion result
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3472_Get_Red(uint16_t * r) {
	return TCS3472_Read_Word(r, TCS3472__RDATAL); }

/*!\brief Get Green channel conversion
** \param[in,out] g - pointer to Green conversion result
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3472_Get_Green(uint16_t * g) {
	return TCS3472_Read_Word(g, TCS3472__GDATAL); }

/*!\brief Get Blue channel conversion
** \param[in,out] b - pointer to Blue conversion result
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3472_Get_Blue(uint16_t * b) {
	return TCS3472_Read_Word(b, TCS3472__BDATAL); }


/****************************************************************/
#endif
#endif /* __TCS3472_EX_H__ */
/****************************************************************/
