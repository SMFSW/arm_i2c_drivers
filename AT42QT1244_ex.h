/*!\file AT42QT1244_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief AT42QT1244 Driver extensions
** \details AT42QT1244: 24-key QMatrix FMEA IEC/EN/UL60730 Touch Sensor
**/
/****************************************************************/
#ifndef __AT42QT1244_EX_H__
	#define __AT42QT1244_EX_H__

#include "sarmfsw.h"
#include "AT42QT1244.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/
// TODO: doxygen


// *****************************************************************************
// Section: Types
// *****************************************************************************
typedef union uAT42QT_REG_MAP {
	uint8_t Bytes[251];
	struct {
		uint8_t						Res_1;
		uint8_t						Res_2;
		uint8_t						Counter_100ms;
		uint8_t						Counter_Signal_Fail;
		uint8_t						Counter_Matrix_Scan;
		uAT42QT_REG__DEVICE_STATUS	Device_Status;
		uint8_t						Detect_Status_0_7;
		uint8_t						Detect_Status_8_15;
		uint8_t						Detect_Status_16_23;
		uint8_t						Res_3;
		uint8_t						Current_Frequency;
		uint8_t						Current_Pulse_Spacing;
		uAT42QT_REG__KEY_DATA		KeyData[24];
		uint8_t						Res_4[31];
		uint8_t						Control_Command;
		uAT42QT_REG__SETUP_141_164	Setup_Key_Thr[24];
		uAT42QT_REG__SETUP_165_188	Setup_Key_Mode[24];
		uint8_t						Setup_Calib_Frequency_Offset_1[24];
		uint8_t						Setup_Calib_Frequency_Offset_2[24];
		uint8_t						Setup_Negative_Recall_Delay;
		uAT42QT_REG__SETUP_238		Setup_Debug_MSync;
		uint8_t						Setup_Awake_Timeout;
		uint8_t						Setup_Drift_Hold_Time;
		uAT42QT_REG__SETUP_241		Setup_Drift;
		uAT42QT_REG__SETUP_242		Setup_Lower_Signal_Limit_1;
		uAT42QT_REG__SETUP_243		Setup_Lower_Signal_Limit_2;
		uAT42QT_REG__SETUP_244		Setup_Freq_Hoping_Dwell;
		uint8_t						Setup_Frequency_0;
		uint8_t						Setup_Frequency_1;
		uint8_t						Setup_Frequency_2;
		uAT42QT_REG__SETUP_248		Setup_Noise;
		uint8_t						Setup_Host_CRC[2];	//!< Host CRC (LSB at addr 249, MSB at addr 250)
		uint8_t						Setup_Positive_Recalibration_Delay;
	} Reg;
} uAT42QT_REG_MAP;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

FctERR AT42QT1244_Send_Command(const AT42QT_cmd cmd);
FctERR AT42QT1244_Send_Setup(const uint8_t * setup, const uint8_t addr, const uint8_t nb);

FctERR AT42QT1244_Setup_Key(const uint8_t Key, const bool use);
FctERR AT42QT1244_Setup_FHM(const AT42QT_FHM FHM);

FctERR AT42QT1244_Get_Keys(uint32_t * Keys);

int AT42QT1244_is_Calib_Pending(void);


__INLINE FctERR INLINE__ AT42QT1244_Get_Key_Data(uAT42QT_REG__KEY_DATA * Key_Data, const uint8_t Key_Num) {
	return AT42QT1244_Read((uint8_t *) Key_Data, AT42QT__DATA_KEY_1 + (Key_Num * sizeof(uAT42QT_REG__KEY_DATA)), sizeof(uAT42QT_REG__KEY_DATA)); }


__INLINE FctERR INLINE__ AT42QT1244_Get_Status(uAT42QT_REG__DEVICE_STATUS * st) {
	return AT42QT1244_Read((uint8_t *) st, AT42QT__DEVICE_STATUS, 1); }


__INLINE FctERR INLINE__ AT42QT1244_Reset(void) {
	return AT42QT1244_Send_Command(AT42QT__RESET_DEVICE); }


__INLINE FctERR INLINE__ AT42QT1244_Key_Enable(const uint8_t Key) {
	return AT42QT1244_Setup_Key(Key, true); }

__INLINE FctERR INLINE__ AT42QT1244_Key_Disable(const uint8_t Key) {
	return AT42QT1244_Setup_Key(Key, false); }


/****************************************************************/
#endif
#endif /* __AT42QT1244_EX_H__ */
/****************************************************************/
