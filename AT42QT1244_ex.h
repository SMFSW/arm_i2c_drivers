/*!\file AT42QT1244_ex.h
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Atmel Capacitive Driver extensions declarations
**/
/****************************************************************/
#ifndef __AT42QT1244_EX_H__
	#define __AT42QT1244_EX_H__

#include "AT42QT1244.h"

#include "sarmfsw.h"
#include CMSIS_INC
#include CMSIS_CFG

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/
// TODO: doxygen


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// High level functions
FctERR AT42QT1244_Send_Command(AT42QT_cmd cmd);
FctERR AT42QT1244_Send_Setup(uint8_t * setup, uint8_t Addr, uint8_t nb);

FctERR AT42QT1244_Setup_Key(uint8_t Key, bool use);
FctERR AT42QT1244_Setup_FHM(AT42QT_FHM FHM);

FctERR AT42QT1244_Get_Keys(uint32_t * Keys);

int AT42QT1244_is_Calib_Pending(void);

FctERR AT42QT1244_Calibrate_Freq_Offset(void);


__INLINE FctERR INLINE__ AT42QT1244_Get_Key_Data(uAT42QT_REG__KEY_DATA * Key_Data, uint8_t Key_Num) {
	return AT42QT1244_Read((uint8_t *) Key_Data, AT42QT__DATA_KEY_1 + (Key_Num * sizeof(uAT42QT_REG__KEY_DATA)), sizeof(uAT42QT_REG__KEY_DATA)); }


__INLINE FctERR INLINE__ AT42QT1244_Get_Status(uAT42QT_REG__DEVICE_STATUS * st) {
	return AT42QT1244_Read((uint8_t *) st, AT42QT__DEVICE_STATUS, 1); }


__INLINE FctERR INLINE__ AT42QT1244_Reset(void) {
	return AT42QT1244_Send_Command(AT42QT__RESET_DEVICE); }

__INLINE FctERR INLINE__ AT42QT1244_Calibrate_Low_Level(void) {
	if (AT42QT1244_is_Calib_Pending())	{ return ERR_BUSY; }
	return AT42QT1244_Send_Command(AT42QT__LOW_LEVEL_CALIBRATION); }

__INLINE FctERR INLINE__ AT42QT1244_Calibrate_All_Keys(void) {
	if (AT42QT1244_is_Calib_Pending())	{ return ERR_BUSY; }
	return AT42QT1244_Send_Command(AT42QT__CALIBRATE_ALL_KEYS); }

__INLINE FctERR INLINE__ AT42QT1244_Calibrate_Key(uint8_t Key) {
	if (Key > AT42QT__CALIBRATE_KEY_23)	{ return ERR_VALUE; }
	if (AT42QT1244_is_Calib_Pending())	{ return ERR_BUSY; }
	return AT42QT1244_Send_Command(Key); }


__INLINE FctERR INLINE__ AT42QT1244_Key_Enable(uint8_t Key) {
	return AT42QT1244_Setup_Key(Key, true); }

__INLINE FctERR INLINE__ AT42QT1244_Key_Disable(uint8_t Key) {
	return AT42QT1244_Setup_Key(Key, false); }


/****************************************************************/
#endif
#endif /* __AT42QT1244_EX_H__ */
/****************************************************************/
