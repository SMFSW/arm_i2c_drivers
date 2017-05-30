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
// TODO: doxygen


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
		uTCS3472_REG__ENABLE	ENABLE;
		uint8_t					ATIME;
		uint8_t					:8;
		uint8_t					WTIME;
		uint8_t					AILTL;
		uint8_t					AILTH;
		uint8_t					AIHTL;
		uint8_t					AIHTH;
		uint8_t					:8;
		uint8_t					:8;
		uint8_t					:8;
		uint8_t					:8;
		uTCS3472_REG__PERSIST	PERS;
		uTCS3472_REG__CONFIG	CONFIG;
		uint8_t					:8;
		uTCS3472_REG__CONTROL	CONTROL;
		uint8_t					:8;
		uint8_t					:8;
		uint8_t					ID;
		uTCS3472_REG__STATUS	STATUS;
		uint8_t					CDATAL;
		uint8_t					CDATAH;
		uint8_t					RDATAL;
		uint8_t					RDATAH;
		uint8_t					GDATAL;
		uint8_t					GDATAH;
		uint8_t					BDATAL;
		uint8_t					BDATAH;
		} Reg;
} uTCS3472_REG_MAP;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

/*** Configuration ***/
__INLINE FctERR INLINE__ TCS3472_Write_En(uint8_t en) {
	return TCS3472_Write(&en, TCS3472__ENABLE, 1); }

__INLINE FctERR INLINE__ TCS3472_Write_Cfg(uint8_t cfg) {
	return TCS3472_Write(&cfg, TCS3472__CONFIG, 1); }

__INLINE FctERR INLINE__ TCS3472_Write_Ctl(uint8_t ctl) {
	return TCS3472_Write(&ctl, TCS3472__CONTROL, 1); }


/*!\brief TCS3472 oscillator Enable / Disable
**/
FctERR TCS3472_Set_PON(bool en);

/*!\brief TCS3472 Clear module Enable / Disable
**/
FctERR TCS3472_Set_AEN(bool en);

/*!\brief TCS3472 Clear interrupt module Enable / Disable
**/
FctERR TCS3472_Set_AIEN(bool en);

/*!\brief TCS3472 WAIT module Enable / Disable
**/
FctERR TCS3472_Set_WEN(bool en);


/*!\brief TCS3472 gain configuration
**/
FctERR TCS3472_Set_Gain(TCS3472_gain gain);

/*!\brief TCS3472 integration time configuration
**/
FctERR TCS3472_Set_Integration_Time(uint16_t integ);

/*!\brief TCS3472 wait time configuration
**/
FctERR TCS3472_Set_Wait_Time(uint16_t wait);


__INLINE FctERR INLINE__ TCS3472_Set_AILT(uint16_t thr) {
	return TCS3472_Write_Word(&thr, TCS3472__AILTL); }

__INLINE FctERR INLINE__ TCS3472_Set_AIHT(uint16_t thr) {
	return TCS3472_Write_Word(&thr, TCS3472__AIHTL); }

__INLINE FctERR INLINE__ TCS3472_Set_AIT(uint16_t lthr, uint16_t hthr) {
	uint8_t DAT[4] = { (hthr & 0xFF), (hthr / 0x100), (lthr & 0xFF), (lthr / 0x100) };
	return TCS3472_Write(DAT, TCS3472__AILTL, sizeof(DAT)); }


/*** Special Functions ***/
__INLINE FctERR INLINE__ TCS3472_SF_Clear_IT(void) {
	return TCS3472_Write_Special(TCS3472__SF_CLR_IT); }


/*** Operation ***/
__INLINE FctERR INLINE__ TCS3472_Get_ChipID(uint8_t * id) {
	return TCS3472_Read(id, TCS3472__ID, 1); }


FctERR TCS3472_Get_Channels(uint16_t buf[]);

__INLINE FctERR INLINE__ TCS3472_Get_Channel(uint16_t * buf, TCS3472_chan chan) {
	if (chan > TCS3472__CHAN_BLUE)	{ return ERR_VALUE; }	// Unknown channel
	return TCS3472_Read_Word(buf, TCS3472__CDATAL + (2 * chan)); }

__INLINE FctERR INLINE__ TCS3472_Get_Clear(uint16_t * buf) {
	return TCS3472_Read_Word(buf, TCS3472__CDATAL); }

__INLINE FctERR INLINE__ TCS3472_Get_Red(uint16_t * buf) {
	return TCS3472_Read_Word(buf, TCS3472__RDATAL); }

__INLINE FctERR INLINE__ TCS3472_Get_Green(uint16_t * buf) {
	return TCS3472_Read_Word(buf, TCS3472__GDATAL); }

__INLINE FctERR INLINE__ TCS3472_Get_Blue(uint16_t * buf) {
	return TCS3472_Read_Word(buf, TCS3472__BDATAL); }


/****************************************************************/
#endif
#endif /* __TCS3472_EX_H__ */
/****************************************************************/
