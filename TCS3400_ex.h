/*!\file TCS3400_ex.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief TCS3400 Driver extensions declarations
** \details TCS3400: Color Light-to-Digital Converter
**/
/****************************************************************/
#ifndef __TCS3400_EX_H__
	#define __TCS3400_EX_H__

#include "TCS3400.h"

#include "sarmfsw.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
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
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3400_Write_En(uint8_t en) {
	return TCS3400_Write(&en, TCS3400__ENABLE, 1); }

/*!\brief Write TCS3400 Config register
** \param[in] cfg - Configuration register value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3400_Write_Cfg(uint8_t cfg) {
	return TCS3400_Write(&cfg, TCS3400__CONFIG, 1); }

/*!\brief Write TCS3472 Control register
** \param[in] ctl - Control register value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3400_Write_Ctl(uint8_t ctl) {
	return TCS3400_Write(&ctl, TCS3400__CONTROL, 1); }


/*!\brief oscillator Enable / Disable
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR TCS3400_Set_PON(bool en);

/*!\brief Clear module Enable / Disable
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR TCS3400_Set_AEN(bool en);

/*!\brief Clear interrupt module Enable / Disable
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR TCS3400_Set_AIEN(bool en);

/*!\brief WAIT module Enable / Disable
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR TCS3400_Set_WEN(bool en);

/*!\brief Sleep After Interrupt Enable / Disable
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR TCS3400_Set_SAI(bool en);


/*!\brief Gain configuration
** \param[in] gain - Gain value
** \return FctERR - error code
**/
FctERR TCS3400_Set_Gain(TCS3400_gain gain);

/*!\brief Integration time configuration
** \param[in] integ - Integration time value
** \return FctERR - error code
**/
FctERR TCS3400_Set_Integration_Time(uint16_t integ);

/*!\brief Wait time configuration
** \param[in] wait - Wait time value
** \return FctERR - error code
**/
FctERR TCS3400_Set_Wait_Time(uint16_t wait);


/*!\brief ALS interrupt low threshold configuration
** \param[in] thr - Low threshold value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3400_Set_AILT(uint16_t thr) {
	return TCS3400_Write_Word(&thr, TCS3400__AILTL); }

/*!\brief ALS interrupt high threshold configuration
** \param[in] thr - High threshold value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3400_Set_AIHT(uint16_t thr) {
	return TCS3400_Write_Word(&thr, TCS3400__AIHTL); }

/*!\brief ALS interrupt thresholds configuration
** \param[in] lthr - Low threshold value
** \param[in] hthr - High threshold value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3400_Set_AIT(uint16_t lthr, uint16_t hthr) {
	uint8_t DAT[4] = { (hthr & 0xFF), (hthr / 0x100), (lthr & 0xFF), (lthr / 0x100) };
	return TCS3400_Write(DAT, TCS3400__AILTL, sizeof(DAT)); }


/*** Interrupts Functions ***/
/*!\brief Force interruption
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3400_Force_IT(void) {
	uint8_t REG = 0xFF;
	return TCS3400_Write(&REG, TCS3400__IFORCE, 1); }

/*!\brief Clear pending IR interruption
** \warning if event pin enabled, shall be called after read to reset pin
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3400_Clear_IR_IT(void) {
	uint8_t REG = 0xFF;
	return TCS3400_Write(&REG, TCS3400__CICLEAR, 1); }

/*!\brief Clear All pending interruptions
** \warning if event pin enabled, shall be called after read to reset pin
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3400_Clear_All_IT(void) {
	uint8_t REG = 0xFF;
	return TCS3400_Write(&REG, TCS3400__AICLEAR, 1); }


/*** Operation ***/
/*!\brief Get TCS3400 Revision ID
** \param[in,out] id - pointer to Revision ID result
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3400_Get_RevID(uint8_t * id) {
	return TCS3400_Read(id, TCS3400__REVID, 1); }

/*!\brief Get TCS3400 Device ID
** \param[in,out] id - pointer to Device ID result
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3400_Get_DeviceID(uint8_t * id) {
	return TCS3400_Read(id, TCS3400__ID, 1); }


/*!\brief Get All channels conversions
** \param[in,out] buf - pointer to conversions tab result
** \return FctERR - error code
**/
FctERR TCS3400_Get_Channels(uint16_t buf[]);

/*!\brief Get Specific channel conversion
** \param[in,out] buf - pointer to conversions result
** \param[in] chan - Channel to get
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3400_Get_Channel(uint16_t * buf, TCS3400_chan chan) {
	if (chan > TCS3400__CHAN_BLUE)	{ return ERR_VALUE; }	// Unknown channel
	return TCS3400_Read_Word(buf, TCS3400__CDATAL + (2 * chan)); }

/*!\brief Get Clear channel conversion
** \param[in,out] clr - pointer to Clear conversion result
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3400_Get_Clear(uint16_t * clr) {
	return TCS3400_Read_Word(clr, TCS3400__CDATAL); }

/*!\brief Get Red channel conversion
** \param[in,out] r - pointer to Red conversion result
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3400_Get_Red(uint16_t * r) {
	return TCS3400_Read_Word(r, TCS3400__RDATAL); }

/*!\brief Get Green channel conversion
** \param[in,out] g - pointer to Green conversion result
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3400_Get_Green(uint16_t * g) {
	return TCS3400_Read_Word(g, TCS3400__GDATAL); }

/*!\brief Get Blue channel conversion
** \param[in,out] b - pointer to Blue conversion result
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ TCS3400_Get_Blue(uint16_t * b) {
	return TCS3400_Read_Word(b, TCS3400__BDATAL); }


/****************************************************************/
#endif
#endif /* __TCS3400_EX_H__ */
/****************************************************************/
