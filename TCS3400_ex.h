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
#include CMSIS_INC
#include CMSIS_CFG

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/
// TODO: doxygen


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
__INLINE FctERR INLINE__ TCS3400_Write_En(uint8_t en) {
	return TCS3400_Write(&en, TCS3400__ENABLE, 1); }

__INLINE FctERR INLINE__ TCS3400_Write_Cfg(uint8_t cfg) {
	return TCS3400_Write(&cfg, TCS3400__CONFIG, 1); }

__INLINE FctERR INLINE__ TCS3400_Write_Ctl(uint8_t ctl) {
	return TCS3400_Write(&ctl, TCS3400__CONTROL, 1); }


/*!\brief TCS3400 oscillator Enable / Disable
**/
FctERR TCS3400_Set_PON(bool en);

/*!\brief TCS3400 Clear module Enable / Disable
**/
FctERR TCS3400_Set_AEN(bool en);

/*!\brief TCS3400 Clear interrupt module Enable / Disable
**/
FctERR TCS3400_Set_AIEN(bool en);

/*!\brief TCS3400 WAIT module Enable / Disable
**/
FctERR TCS3400_Set_WEN(bool en);

/*!\brief TCS3400 Sleep After Interrupt Enable / Disable
**/
FctERR TCS3400_Set_SAI(bool en);


/*!\brief TCS3400 gain configuration
**/
FctERR TCS3400_Set_Gain(TCS3400_gain gain);

/*!\brief TCS3400 integration time configuration
**/
FctERR TCS3400_Set_Integration_Time(uint16_t integ);

/*!\brief TCS3400 wait time configuration
**/
FctERR TCS3400_Set_Wait_Time(uint16_t wait);


__INLINE FctERR INLINE__ TCS3400_Set_AILT(uint16_t thr) {
	return TCS3400_Write_Word(&thr, TCS3400__AILTL); }

__INLINE FctERR INLINE__ TCS3400_Set_AIHT(uint16_t thr) {
	return TCS3400_Write_Word(&thr, TCS3400__AIHTL); }

__INLINE FctERR INLINE__ TCS3400_Set_AIT(uint16_t lthr, uint16_t hthr) {
	uint8_t DAT[4] = { (hthr & 0xFF), (hthr / 0x100), (lthr & 0xFF), (lthr / 0x100) };
	return TCS3400_Write(DAT, TCS3400__AILTL, sizeof(DAT)); }


/*** Interrupts Functions ***/
__INLINE FctERR INLINE__ TCS3400_Force_IT(void) {
	uint8_t REG = 0xFF;
	return TCS3400_Write(&REG, TCS3400__IFORCE, 1); }

__INLINE FctERR INLINE__ TCS3400_Clear_IR_IT(void) {
	uint8_t REG = 0xFF;
	return TCS3400_Write(&REG, TCS3400__CICLEAR, 1); }

__INLINE FctERR INLINE__ TCS3400_Clear_All_IT(void) {
	uint8_t REG = 0xFF;
	return TCS3400_Write(&REG, TCS3400__AICLEAR, 1); }


/*** Operation ***/
__INLINE FctERR INLINE__ TCS3400_Get_RevID(uint8_t * id) {
	return TCS3400_Read(id, TCS3400__REVID, 1); }

__INLINE FctERR INLINE__ TCS3400_Get_DeviceID(uint8_t * id) {
	return TCS3400_Read(id, TCS3400__ID, 1); }


FctERR TCS3400_Get_Channels(uint16_t buf[]);

__INLINE FctERR INLINE__ TCS3400_Get_Channel(uint16_t * buf, TCS3400_chan chan) {
	if (chan > TCS3400__CHAN_BLUE)	{ return ERR_VALUE; }	// Unknown channel
	return TCS3400_Read_Word(buf, TCS3400__CDATAL + (2 * chan)); }

__INLINE FctERR INLINE__ TCS3400_Get_Clear(uint16_t * buf) {
	return TCS3400_Read_Word(buf, TCS3400__CDATAL); }

__INLINE FctERR INLINE__ TCS3400_Get_Red(uint16_t * buf) {
	return TCS3400_Read_Word(buf, TCS3400__RDATAL); }

__INLINE FctERR INLINE__ TCS3400_Get_Green(uint16_t * buf) {
	return TCS3400_Read_Word(buf, TCS3400__GDATAL); }

__INLINE FctERR INLINE__ TCS3400_Get_Blue(uint16_t * buf) {
	return TCS3400_Read_Word(buf, TCS3400__BDATAL); }


/****************************************************************/
#endif
#endif /* __TCS3400_EX_H__ */
/****************************************************************/
