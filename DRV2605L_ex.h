/*!\file DRV2605L.h
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief DRV2605L Driver extensions
** \details DRV2605L: 2 to 5.2V Haptic Driver for LRA and ERM With Effect Library and Smart-Loop Architecture
**/
/****************************************************************/
#ifndef __DRV2605L_EX_H__
	#define __DRV2605L_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "DRV2605L.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\union uDRV_REG_MAP
** \brief DRV2605L Register map union
**/
typedef union uDRV_REG_MAP {
	uint8_t Bytes[35];
	struct PACK__ {
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

/*!\brief Start/Stop haptic pattern
** \param[in] go - 0 OFF, 1 ON
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ DRV2605L_Vibe(const DRV2605L_go go) {
	uDRV_REG__GO GO = { 0 };

	GO.Bits.GO = go;
	return DRV2605L_Write(&GO.Byte, DRV__GO, 1); }


/*!\brief Reset DRV260x chip
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ DRV2605L_Reset(void) {
	uDRV_REG__MODE MODE = { 0 };

	MODE.Bits.DEV_RESET = 1;
	return DRV2605L_Write(&MODE.Byte, DRV__MODE, 1); }


/*!\brief Set haptic actuator type
** \param[in] act - 0 ERM, 1 LRA
** \return FctERR - error code
**/
FctERR DRV2605L_Set_ActType(const DRV2605L_act act);

/*!\brief Set haptic actuator type to ERM
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ DRV2605L_Set_ERM(void) {
	return DRV2605L_Set_ActType(DRV__ACT_ERM); }

/*!\brief Set haptic device actuator type to LRA
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ DRV2605L_Set_LRA(void) {
	return DRV2605L_Set_ActType(DRV__ACT_LRA); }

/*!\brief Set haptic operating mode
** \param[in] mode - haptic input operating mode
** \return FctERR - error code
**/
FctERR DRV2605L_Set_OperatingMode(const DRV2605L_mode mode);

/*!\brief Set haptic device to standby
** \param[in] stdby - 0: Normal operation, 1: standby
** \return FctERR - error code
**/
FctERR DRV2605L_Set_Standby(const bool stdby);

/*!\brief Set haptic device input pin type
** \param[in] input - 0: PWM, 1: Analog
** \return FctERR - error code
**/
FctERR DRV2605L_Set_InputMode(const DRV2605L_input input);

/*!\brief Set haptic device input pin type
** \param[in] loop - 0: ERM Closed Loop, LRA Auto Resonance, 1: Open Loop
** \return FctERR - error code
**/
FctERR DRV2605L_Set_LoopMode(const DRV2605L_loop loop);

/*!\brief Set haptic device library
** \param[in] lib - Internal library used
** \return FctERR - error code
**/
FctERR DRV2605L_Set_Library(const DRV2605L_lib lib);

/*!\brief Set haptic device channel in the waveform sequencer
** \param[in] chan - Waveform sequencer channel
** \param[in] effect - Effect number for haptic feedback
** \param[in] wait - Wait between patterns
** \return FctERR - error code
**/
FctERR DRV2605L_Set_Waveform(const uint16_t chan, const DRV2605L_eff effect, const bool wait);


/*!\brief Set Overdrive time offset
** \param[in] val - Overdrive time offset value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ DRV2605L_Set_OverdriveTimeOffset(const uint8_t val) {
	return DRV2605L_Write((uint8_t *) &val, DRV__OVERDRIVE_TIME_OFFSET, 1); }

/*!\brief Set Sustain time offset
** \param[in] val - Sustain time offset value
** \param[in] pos - 0 Negative, 1 Positive
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ DRV2605L_Set_SustainTimeOffset(const uint8_t val, const bool pos) {
	return DRV2605L_Write((uint8_t *) &val, DRV__SUSTAIN_TIME_OFFSET_NEG - pos, 1); }

/*!\brief Set Brake time offset
** \param[in] val - Brake time offset value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ DRV2605L_Set_BrakeTimeOffset(const uint8_t val) {
	return DRV2605L_Write((uint8_t *) &val, DRV__BRAKE_TIME_OFFSET, 1); }


/*!\brief Set RTP input level
** \param[in] val - RTP input level value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ DRV2605L_Set_RTP(const uint8_t val) {
	return DRV2605L_Write((uint8_t *) &val, DRV__RTP_INPUT, 1); }

/*!\brief Set RTP format
** \param[in] format - 0: Signed, 1: Unsigned
** \return FctERR - error code
**/
FctERR DRV2605L_Set_RTPDataFormat(const DRV2605L_rtp_format format);


/*!\brief Set Audio To Vibe Peak time
** \param[in] peak - Audio To Vibe Peak time value
** \return FctERR - error code
**/
FctERR DRV2605L_Set_ATVPeakTime(const DRV2605L_peak peak);

/*!\brief Set Audio To Vibe Low pass filter
** \param[in] filt - Audio To Vibe Low pass filter value
** \return FctERR - error code
**/
FctERR DRV2605L_Set_ATVLowPassFilter(const DRV2605L_filter filt);

/*!\brief Set Audio To Vibe min/max input level
** \param[in] val - Audio To Vibe Input level value
** \param[in] max - 0 Min, 1 Max
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ DRV2605L_Set_ATVInputLevel_Raw(const uint8_t val, const bool max) {
	return DRV2605L_Write((uint8_t *) &val, DRV__ATV_MIN_INPUT_LEVEL + max, 1); }

/*!\brief Set Audio To Vibe min/max output drive
** \param[in] val - Audio To Vibe Output drive value
** \param[in] max - 0 Min, 1 Max
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ DRV2605L_Set_ATVOutputDrive_Raw(const uint8_t val, const bool max) {
	return DRV2605L_Write((uint8_t *) &val, DRV__ATV_MIN_OUTPUT_DRIVE + max, 1); }

/*!\brief Set Audio To Vibe min/max input voltage
** \param[in] volt - Audio To Vibe Input voltage value
** \param[in] max - 0 Min, 1 Max
** \return FctERR - error code
**/
FctERR DRV2605L_Set_ATVInput_Volt(const float volt, const bool max);

/*!\brief Set Audio To Vibe min/max Drive
** \param[in] perc - Audio To Vibe drive value (in percent)
** \param[in] max - 0 Min, 1 Max
** \return FctERR - error code
**/
FctERR DRV2605L_Set_ATVDrive_Percent(const uint16_t perc, const bool max);


/*!\brief Set haptic Rated voltage
** \param[in] val - Rated voltage value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ DRV2605L_Set_RatedVoltage(const uint8_t val) {
	return DRV2605L_Write((uint8_t *) &val, DRV__RATED_VOLTAGE, 1); }

/*!\brief Set haptic Overdrive Clamp
** \param[in] val - Overdrive clamp value
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ DRV2605L_Set_ODClamp(const uint8_t val) {
	return DRV2605L_Write((uint8_t *) &val, DRV__OVERDRIVE_CLAMP_VOLTAGE, 1); }


/*!\brief Set haptic Blanking time
** \param[in] time - Blanking time value
** \return FctERR - error code
**/
FctERR DRV2605L_Set_BlankingTime(const uint16_t time);

/*!\brief Set haptic Current dissipation time
** \param[in] time - Current dissipation time value
** \return FctERR - error code
**/
FctERR DRV2605L_Set_CurrentDissipationTime(const uint16_t time);

/*!\brief Set haptic LRA Open loop period
** \param[in] val - LRA Open loop period raw
** \return FctERR - error code
**/
__INLINE FctERR INLINE__ DRV2605L_Set_LRAOpenLoopPeriod_Raw(const uint8_t val) {
	return DRV2605L_Write((uint8_t *) &val, DRV__LRA_OPEN_LOOP_PERIOD, 1); }

/*!\brief Set haptic LRA Open loop period
** \param[in] per - LRA Open loop period (in us)
** \return FctERR - error code
**/
FctERR DRV2605L_Set_LRAOpenLoopPeriod_us(const uint16_t per);



/*!\brief Get min/max Audio To Vibe Input voltage
** \param[in,out] volt - pointer to Audio To Vibe Input voltage result
** \param[in] max - 0 Min, 1 Max
** \return FctERR - error code
**/
FctERR NONNULL__ DRV2605L_Get_ATVInput_Volt(float * volt, const bool max);

/*!\brief Get min/max Audio To Vibe Drive
** \param[in,out] perc - pointer to Audio To Vibe Drive result
** \param[in] max - 0 Min, 1 Max
** \return FctERR - error code
**/
FctERR NONNULL__ DRV2605L_Get_ATVDrive_Percent(float * perc, const bool max);

/*!\brief Get haptic Blanking time
** \param[in,out] time - pointer to Blanking time value result
** \return FctERR - error code
**/
FctERR NONNULL__ DRV2605L_Get_BlankingTime(uint16_t * time);

/*!\brief Get haptic Current dissipation time
** \param[in,out] time - pointer to Blanking time value result
** \return FctERR - error code
**/
FctERR NONNULL__ DRV2605L_Get_CurrentDissipationTime(uint16_t * time);

/*!\brief Get haptic Voltage
** \param[in,out] vbat - pointer to Battery voltage result
** \return FctERR - error code
**/
FctERR NONNULL__ DRV2605L_Get_Voltage(uint16_t * vbat);

/*!\brief Get haptic LRA Open loop period
** \param[in,out] per - pointer to LRA open loop period result
** \return FctERR - error code
**/
FctERR NONNULL__ DRV2605L_Get_LRAOpenLoopPeriod(uint16_t * per);

/*!\brief Get haptic LRA Resonance period
** \param[in,out] per - pointer to LRA Resonance period result
** \return FctERR - error code
**/
FctERR NONNULL__ DRV2605L_Get_ResonancePeriod(uint16_t * per);


/*!\brief Get DRV260x chip ID
** \param[in,out] id - pointer to chip ID result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ DRV2605L_Get_ChipID(uint8_t * id) {
	FctERR	err;
	uint8_t	ID;

	err = DRV2605L_Read(&ID, DRV__STATUS, 1);
	if (err)	{ return err; }
	*id = ID & 0xE0;	// Keep only 3 msb
	return err; }


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* __DRV2605L_EX_H__ */
/****************************************************************/
