/*!\file DRV2605L.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief DRV2605L Driver extensions declarations
** \details DRV2605L: 2 to 5.2V Haptic Driver for LRA and ERM With Effect Library and Smart-Loop Architecture
**/
/****************************************************************/
#ifndef __DRV2605L_EX_H__
	#define __DRV2605L_EX_H__

#include "DRV2605L.h"

#include "sarmfsw.h"
#include CMSIS_INC
#include CMSIS_CFG

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/


// *****************************************************************************
// Section: Types
// *****************************************************************************
typedef union uDRV_REG_MAP {
	uint8_t Bytes[0x21];
	struct {
		uDRV_REG__STATUS				Status;
		uDRV_REG__MODE					Mode;
		uint8_t							RTP_Input;
		uDRV_REG__LIBRARY_SELECTION		Library_Selection;
		uDRV_REG__WAVEFORM_SEQUENCER	Waveform_Sequencer[8];
		uDRV_REG__GO					Go;
		uint8_t							ODT;
		uint8_t							SPT;
		uint8_t							SNT;
		uint8_t							BRT;
		uDRV_REG__ATV_CONTROL			ATV_Control;
		uint8_t							ATH_MIN_INPUT;
		uint8_t							ATH_MAX_INPUT;
		uint8_t							ATH_MIN_DRIVE;
		uint8_t							ATH_MAX_DRIVE;
		uint8_t							RATED_VOLTAGE;
		uint8_t							OD_CLAMP;
		uint8_t							A_CAL_COMP;
		uint8_t							A_CAL_BEMF;
		uDRV_REG__FEEDBACK_CONTROL		Feedback_Control;
		uDRV_REG__CONTROL_1				Control_1;
		uDRV_REG__CONTROL_2				Control_2;
		uDRV_REG__CONTROL_3				Control_3;
		uDRV_REG__CONTROL_4				Control_4;
		uDRV_REG__CONTROL_5				Control_5;
		uint8_t							OL_LRA_PERIOD;
		uint8_t							VBAT;
		uint8_t							LRA_PERIOD;
	} Reg;
} uDRV_REG_MAP;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

__INLINE FctERR INLINE__ DRV2605L_Vibe(DRV2605L_go go) {
	uDRV_REG__GO GO;
	GO.Byte = 0;
	GO.Bits.GO = go;
	return DRV2605L_Write(&GO.Byte, DRV__GO, 1); }


__INLINE FctERR INLINE__ DRV2605L_Reset(void) {
	uDRV_REG__MODE MODE;
	MODE.Byte = 0;
	MODE.Bits.DEV_RESET = 1;
	return DRV2605L_Write(&MODE.Byte, DRV__MODE, 1); }


FctERR DRV2605L_Set_ActType(DRV2605L_act act);

__INLINE FctERR INLINE__ DRV2605L_Set_LRA(void) {
	return DRV2605L_Set_ActType(DRV__ACT_LRA); }

__INLINE FctERR INLINE__ DRV2605L_Set_ERM(void) {
	return DRV2605L_Set_ActType(DRV__ACT_ERM); }

FctERR DRV2605L_Set_OperatingMode(DRV2605L_mode mode);
FctERR DRV2605L_Set_Standby(bool stdby);

FctERR DRV2605L_Set_InputMode(DRV2605L_input input);
FctERR DRV2605L_Set_LoopMode(DRV2605L_loop loop);

FctERR DRV2605L_Set_Library(DRV2605L_lib lib);
FctERR DRV2605L_Set_Waveform(uint16_t chan, DRV2605L_eff effect, bool wait);


__INLINE FctERR INLINE__ DRV2605L_Set_OverdriveTimeOffset(uint8_t val) {
	return DRV2605L_Write(&val, DRV__OVERDRIVE_TIME_OFFSET, 1); }

__INLINE FctERR INLINE__ DRV2605L_Set_SustainTimeOffset(uint8_t val, bool pos) {
	return DRV2605L_Write(&val, DRV__SUSTAIN_TIME_OFFSET_NEG - pos, 1); }

__INLINE FctERR INLINE__ DRV2605L_Set_BrakeTimeOffset(uint8_t val) {
	return DRV2605L_Write(&val, DRV__BRAKE_TIME_OFFSET, 1); }


__INLINE FctERR INLINE__ DRV2605L_Set_RTP(uint8_t val) {
	return DRV2605L_Write(&val, DRV__RTP_INPUT, 1); }

FctERR DRV2605L_Set_RTPDataFormat(DRV2605L_rtp_format format);


FctERR DRV2605L_Set_ATVPeakTime(DRV2605L_peak val);
FctERR DRV2605L_Set_ATVLowPassFilter(DRV2605L_filter val);

__INLINE FctERR INLINE__ DRV2605L_Set_ATVInputLevel_Raw(uint8_t val, bool max) {
	return DRV2605L_Write(&val, DRV__ATV_MIN_INPUT_LEVEL + max, 1); }

__INLINE FctERR INLINE__ DRV2605L_Set_ATVOutputDrive_Raw(uint8_t val, bool max) {
	return DRV2605L_Write(&val, DRV__ATV_MIN_OUTPUT_DRIVE + max, 1); }

FctERR DRV2605L_Set_ATVInput_Volt(float volt, bool max);
FctERR DRV2605L_Set_ATVDrive_Percent(uint16_t perc, bool max);


__INLINE FctERR INLINE__ DRV2605L_Set_RatedVoltage(uint8_t val) {
	return DRV2605L_Write(&val, DRV__RATED_VOLTAGE, 1); }

__INLINE FctERR INLINE__ DRV2605L_Set_ODClamp(uint8_t val) {
	return DRV2605L_Write(&val, DRV__OVERDRIVE_CLAMP_VOLTAGE, 1); }


FctERR DRV2605L_Set_BlankingTime(uint16_t time);
FctERR DRV2605L_Set_CurrentDissipationTime(uint16_t time);

__INLINE FctERR INLINE__ DRV2605L_Set_LRAOpenLoopPeriod_Raw(uint8_t val) {
	return DRV2605L_Write(&val, DRV__LRA_OPEN_LOOP_PERIOD, 1); }

FctERR DRV2605L_Set_LRAOpenLoopPeriod_us(uint16_t per);



FctERR DRV2605L_Get_ATVInput_Volt(float * volt, bool max);
FctERR DRV2605L_Get_ATVDrive_Percent(float * perc, bool max);

FctERR DRV2605L_Get_BlankingTime(uint16_t * time);
FctERR DRV2605L_Get_CurrentDissipationTime(uint16_t * time);

FctERR DRV2605L_Get_Voltage(uint16_t * vbat);
FctERR DRV2605L_Get_LRAOpenLoopPeriod(uint16_t * per);
FctERR DRV2605L_Get_ResonancePeriod(uint16_t * per);


__INLINE FctERR INLINE__ DRV2605L_Get_ChipID(uint8_t * id) {
	FctERR	err;
	uint8_t	ID;

	err = DRV2605L_Read(&ID, DRV__STATUS, 1);
	if (err)	{ return err; }
	*id = ID & 0xE0;	// Keep only 3 msb
	return err; }


/****************************************************************/
#endif
#endif /* __DRV2605L_EX_H__ */
/****************************************************************/
