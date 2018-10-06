/*!\file DRV2605L.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief DRV2605L Driver
** \details DRV2605L: 2 to 5.2V Haptic Driver for LRA and ERM With Effect Library and Smart-Loop Architecture
**/
/****************************************************************/
#ifndef __DRV2605L_H__
	#define __DRV2605L_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "I2C_component.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define DRV2605L_BASE_ADDR		0x5A		//!< DRV2605L Base address


// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern I2C_slave_t DRV2605_hal;	//!< DRV2605L Slave structure


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum DRV2605L_reg_map
** \brief Register map enum of DRV2605L
**/
typedef enum PACK__ DRV2605L_reg_map {
	DRV__STATUS = 0,						//!< Status register
	DRV__MODE,								//!< Mode register
	DRV__RTP_INPUT,							//!< Real Time Playback Input register
	DRV__LIBRARY_SELECTION,					//!< Library Selection register
	DRV__WAVEFORM_SEQUENCER_1,				//!< Waveform Sequencer 1 register
	DRV__WAVEFORM_SEQUENCER_2,				//!< Waveform Sequencer 2 register
	DRV__WAVEFORM_SEQUENCER_3,				//!< Waveform Sequencer 3 register
	DRV__WAVEFORM_SEQUENCER_4,				//!< Waveform Sequencer 4 register
	DRV__WAVEFORM_SEQUENCER_5,				//!< Waveform Sequencer 5 register
	DRV__WAVEFORM_SEQUENCER_6,				//!< Waveform Sequencer 6 register
	DRV__WAVEFORM_SEQUENCER_7,				//!< Waveform Sequencer 7 register
	DRV__WAVEFORM_SEQUENCER_8,				//!< Waveform Sequencer 8 register
	DRV__GO,								//!< Go register
	DRV__OVERDRIVE_TIME_OFFSET,				//!< Overdrive Time Offset register
	DRV__SUSTAIN_TIME_OFFSET_POS,			//!< Sustain Time Offset positive register
	DRV__SUSTAIN_TIME_OFFSET_NEG,			//!< Sustain Time Offset negative register
	DRV__BRAKE_TIME_OFFSET,					//!< Brake Time Offset register
	DRV__ATV_CONTROL,						//!< Audio-To-Vibe Control register
	DRV__ATV_MIN_INPUT_LEVEL,				//!< Audio-To-Vibe Minimum Input Level register
	DRV__ATV_MAX_INPUT_LEVEL,				//!< Audio-To-Vibe Maximum Input Level register
	DRV__ATV_MIN_OUTPUT_DRIVE,				//!< Audio-To-Vibe Minimum Output Drive Level register
	DRV__ATV_MAX_OUTPUT_DRIVE,				//!< Audio-To-Vibe Maximum Output Drive Level register
	DRV__RATED_VOLTAGE,						//!< Rated Voltage register
	DRV__OVERDRIVE_CLAMP_VOLTAGE,			//!< Overdrive Clamp Voltage register
	DRV__AUTO_CALIB_COMPENSATION_RESULT,	//!< Auto-Calibration Compensation Result register
	DRV__AUTO_CALIB_BACK_EMF_RESULT,		//!< Auto-Calibration Back EMF Result register
	DRV__FEEDBACK_CONTROL,					//!< Feedback Control register
	DRV__CONTROL_1,							//!< Control 1 register
	DRV__CONTROL_2,							//!< Control 2 register
	DRV__CONTROL_3,							//!< Control 3 register
	DRV__CONTROL_4,							//!< Control 4 register
	DRV__CONTROL_5,							//!< Control 5 register
	DRV__LRA_OPEN_LOOP_PERIOD,				//!< LRA Open Loop Period register
	DRV__VBAT_VOLTAGE_MONITOR,				//!< VBAT Voltage Monitor register
	DRV__LRA_RESONANCE_PERIOD				//!< LRA Resonance Period register
} DRV2605L_reg;


/*!\enum DRV2605L_lib
** \brief Waveform library selection value.
**/
typedef enum PACK__ DRV2605L_lib {
	DRV__LIB_EMPTY = 0,			//!< Empty
	DRV__LIB_TS2200_LIBRARY_A,	//!< TS2200 Library A
	DRV__LIB_TS2200_LIBRARY_B,	//!< TS2200 Library B
	DRV__LIB_TS2200_LIBRARY_C,	//!< TS2200 Library C
	DRV__LIB_TS2200_LIBRARY_D,	//!< TS2200 Library D
	DRV__LIB_TS2200_LIBRARY_E,	//!< TS2200 Library E
	DRV__LIB_LRA_LIBRARY,		//!< LRA Library
	DRV__LIB_TS2200_LIBRARY_F	//!< TS2200 Library F
} DRV2605L_lib;


/*!\enum DRV2605L_mode
** \brief DRV2605L operating mode
**/
typedef enum PACK__ DRV2605L_mode {
	DRV__MODE_INTERNAL_TRIGGER = 0,		//!< Internal trigger
	DRV__MODE_EXTERNAL_TRIGGER_EDGE,	//!< External trigger (edge mode)
	DRV__MODE_EXTERNAL_TRIGGER_LEVEL,	//!< External trigger (level mode)
	DRV__MODE_PWM_ANA_INPUT,			//!< PWM input and analog input
	DRV__MODE_AUDIO_TO_VIBE,			//!< Audio-to-vibe
	DRV__MODE_REAL_TIME_PLAYBACK,		//!< Real-time playback (RTP mode)
	DRV__MODE_DIAGNOSTICS,				//!< Diagnostics
	DRV__MODE_AUTO_CALIBRATION			//!< Auto calibration
} DRV2605L_mode;

/*!\enum DRV2605L_actuator
** \brief DRV2605L actuator
**/
typedef enum PACK__ DRV2605L_actuator {
	DRV__ACT_ERM = 0,		//!< Eccentric Rotating Mass actuator
	DRV__ACT_LRA			//!< Linear Resonant Actuators actuator
} DRV2605L_act;

/*!\enum DRV2605L_go
** \brief DRV2605L Go mode
**/
typedef enum PACK__ DRV2605L_go {
	DRV__STOP = 0,	//!< Start haptic
	DRV__START		//!< Stop haptic
} DRV2605L_go;

/*!\enum DRV2605L_peak
** \brief Peak detection time for the audio-to-vibe signal path
**/
typedef enum PACK__ DRV2605L_peak {
	DRV__PEAK_10MS = 0,		//!< 10ms peak time
	DRV__PEAK_20MS,			//!< 20ms peak time
	DRV__PEAK_30MS,			//!< 30ms peak time
	DRV__PEAK_40MS			//!< 40ms peak time
} DRV2605L_peak;

/*!\enum DRV2605L_filter
** \brief Low-pass filter frequency for the audio-to-vibe signal path
**/
typedef enum PACK__ DRV2605L_filter {
	DRV__FILTER_100HZ = 0,	//!< 100Hz low-pass filter
	DRV__FILTER_125HZ,		//!< 125Hz low-pass filter
	DRV__FILTER_150HZ,		//!< 150Hz low-pass filter
	DRV__FILTER_200HZ		//!< 200Hz low-pass filter
} DRV2605L_filter;

/*!\enum DRV2605L_loop_mode
** \brief DRV2605L loop mode
**/
typedef enum PACK__ DRV2605L_loop_mode {
	DRV__CLOSED_LOOP = 0,	//!< ERM: Close Loop / LRA: Auto resonance
	DRV__OPEN_LOOP			//!< Open Loop
} DRV2605L_loop;

/*!\enum DRV2605L_input_mode
** \brief DRV2605L input pin mode
**/
typedef enum PACK__ DRV2605L_input_mode {
	DRV__IN_PWM = 0,	//!< PWM input
	DRV__IN_ANALOG		//!< Analog input
} DRV2605L_input;


/*!\enum DRV2605L_input_mode
** \brief DRV2605L input pin mode
**/
typedef enum PACK__ DRV2605L_rtp_format {
	DRV__RTP_SIGNED = 0,	//!< RTP signed format
	DRV__RTP_UNSIGNED		//!< RTP unsigned format
} DRV2605L_rtp_format;


/*!\enum DRV2605L_effect
** \brief Waveform Library Effects List
**/
typedef enum PACK__ DRV2605L_effect {
	DRV__EFF_STRONG_CLK_100 = 1,						//!< Strong Click - 100%
	DRV__EFF_STRONG_CLK_60 = 2,							//!< Strong Click - 60%
	DRV__EFF_STRONG_CLK_30 = 3,							//!< Strong Click - 30%
	DRV__EFF_SHARP_CLK_100 = 4,							//!< Sharp Click - 100%
	DRV__EFF_SHARP_CLK_60 = 5,							//!< Sharp Click - 60%
	DRV__EFF_SHARP_CLK_30 = 6,							//!< Sharp Click - 30%
	DRV__EFF_SOFT_BUMP_100 = 7,							//!< Soft Bump - 100%
	DRV__EFF_SOFT_BUMP_60 = 8,							//!< Soft Bump - 60%
	DRV__EFF_SOFT_BUMP_30 = 9,							//!< Soft Bump - 30%
	DRV__EFF_DBL_CLK_100 = 10,							//!< Double Click - 100%
	DRV__EFF_DBL_CLK_60 = 11,							//!< Double Click - 60%
	DRV__EFF_DBL_CLK_30 = 12,							//!< Triple Click - 100%
	DRV__EFF_SOFT_FUZZ_60 = 13,							//!< Soft Fuzz - 60%
	DRV__EFF_STRONG_BUZZ_100 = 14,						//!< Strong Buzz - 100%
	DRV__EFF_ALERT_750MS = 15,							//!< 750ms Alert 100%
	DRV__EFF_ALERT_1000MS = 16,							//!< 1000ms Alert 100%
	DRV__EFF_STRONG_CLK_1 = 17,							//!< Strong Click 1 - 100%
	DRV__EFF_STRONG_CLK_2 = 18,							//!< Strong Click 2 - 80%
	DRV__EFF_STRONG_CLK_3 = 19,							//!< Strong Click 3 - 60%
	DRV__EFF_STRONG_CLK_4 = 20,							//!< Strong Click 4 - 30%
	DRV__EFF_MEDIUM_CLK_1 = 21,							//!< Medium Click 1 - 100%
	DRV__EFF_MEDIUM_CLK_2 = 22,							//!< Medium Click 2 - 80%
	DRV__EFF_MEDIUM_CLK_3 = 23,							//!< Medium Click 3 - 60%
	DRV__EFF_SHARP_TICK_1 = 24,							//!< Sharp Tick 1 - 100%
	DRV__EFF_SHARP_TICK_2 = 25,							//!< Sharp Tick 2 - 80%
	DRV__EFF_SHARP_TICK_3 = 26,							//!< Sharp Tick 3 - 60%
	DRV__EFF_SHORT_DBL_CLK_STRONG_1 = 27,				//!< Short Double Click Strong 1 - 100%
	DRV__EFF_SHORT_DBL_CLK_STRONG_2 = 28,				//!< Short Double Click Strong 2 - 80%
	DRV__EFF_SHORT_DBL_CLK_STRONG_3 = 29,				//!< Short Double Click Strong 3 - 60%
	DRV__EFF_SHORT_DBL_CLK_STRONG_4 = 30,				//!< Short Double Click Strong 4 - 30%
	DRV__EFF_SHORT_DBL_CLK_MEDIUM_1 = 31,				//!< Short Double Click Medium 1 - 100%
	DRV__EFF_SHORT_DBL_CLK_MEDIUM_2 = 32,				//!< Short Double Click Medium 2 - 80%
	DRV__EFF_SHORT_DBL_CLK_MEDIUM_3 = 33,				//!< Short Double Click Medium 3 - 60%
	DRV__EFF_SHORT_DBL_SHARP_TICK_1 = 34,				//!< Short Double Sharp Tick 1 - 100%
	DRV__EFF_SHORT_DBL_SHARP_TICK_2 = 35,				//!< Short Double Sharp Tick 2 - 80%
	DRV__EFF_SHORT_DBL_SHARP_TICK_3 = 36,				//!< Short Double Sharp Tick 3 - 60%
	DRV__EFF_LONG_DBL_CLK_STRONG_1 = 37,				//!< Long Double Sharp Click Strong 1 - 100%
	DRV__EFF_LONG_DBL_CLK_STRONG_2 = 38,				//!< Long Double Sharp Click Strong 2 - 80%
	DRV__EFF_LONG_DBL_CLK_STRONG_3 = 39,				//!< Long Double Sharp Click Strong 3 - 60%
	DRV__EFF_LONG_DBL_CLK_STRONG_4 = 40,				//!< Long Double Sharp Click Strong 4 - 30%
	DRV__EFF_LONG_DBL_CLK_MEDIUM_1 = 41,				//!< Long Double Sharp Click Medium 1 - 100%
	DRV__EFF_LONG_DBL_CLK_MEDIUM_2 = 42,				//!< Long Double Sharp Click Medium 2 - 80%
	DRV__EFF_LONG_DBL_CLK_MEDIUM_3 = 43,				//!< Long Double Sharp Click Medium 3 - 60%
	DRV__EFF_LONG_DBL_SHARP_TICK_1 = 44,				//!< Long Double Sharp Tick 1 - 100%
	DRV__EFF_LONG_DBL_SHARP_TICK_2 = 45,				//!< Long Double Sharp Tick 2 - 80%
	DRV__EFF_LONG_DBL_SHARP_TICK_3 = 46,				//!< Long Double Sharp Tick 3 - 60%
	DRV__EFF_BUZZ_1 = 47,								//!< Buzz 1 - 100%
	DRV__EFF_BUZZ_2 = 48,								//!< Buzz 2 - 80%
	DRV__EFF_BUZZ_3 = 49,								//!< Buzz 3 - 60%
	DRV__EFF_BUZZ_4 = 50,								//!< Buzz 4 - 40%
	DRV__EFF_BUZZ_5 = 51,								//!< Buzz 5 - 20%
	DRV__EFF_PULSING_STRONG_1 = 52,						//!< Pulsing Strong 1 - 100%
	DRV__EFF_PULSING_STRONG_2 = 53,						//!< Pulsing Strong 2 - 60%
	DRV__EFF_PULSING_MEDIUM_1 = 54,						//!< Pulsing Medium 1 - 100%
	DRV__EFF_PULSING_MEDIUM_2 = 55,						//!< Pulsing Medium 2 - 60%
	DRV__EFF_PULSING_SHARP_1 = 56,						//!< Pulsing Sharp 1 - 100%
	DRV__EFF_PULSING_SHARP_2 = 57,						//!< Pulsing Sharp 2 - 60%
	DRV__EFF_TRANS_CLK_1 = 58,							//!< Transition Click 1 - 100%
	DRV__EFF_TRANS_CLK_2 = 59,							//!< Transition Click 2 - 80%
	DRV__EFF_TRANS_CLK_3 = 60,							//!< Transition Click 3 - 60%
	DRV__EFF_TRANS_CLK_4 = 61,							//!< Transition Click 4 - 40%
	DRV__EFF_TRANS_CLK_5 = 62,							//!< Transition Click 5 - 20%
	DRV__EFF_TRANS_CLK_6 = 63,							//!< Transition Click 6 - 10%
	DRV__EFF_TRANS_HUM_1 = 64,							//!< Transition Hum 1 - 100%
	DRV__EFF_TRANS_HUM_2 = 65,							//!< Transition Hum 2 - 80%
	DRV__EFF_TRANS_HUM_3 = 66,							//!< Transition Hum 3 - 60%
	DRV__EFF_TRANS_HUM_4 = 67,							//!< Transition Hum 4 - 40%
	DRV__EFF_TRANS_HUM_5 = 68,							//!< Transition Hum 5 - 20%
	DRV__EFF_TRANS_HUM_6 = 69,							//!< Transition Hum 6 - 10%
	DRV__EFF_TRANS_RAMP_DWN_LONG_SMOOTH_1 = 70,			//!< Transition Ramp Down Long Smooth 1 - 100 to 0%
	DRV__EFF_TRANS_RAMP_DWN_LONG_SMOOTH_2 = 71,			//!< Transition Ramp Down Long Smooth 2 - 100 to 0%
	DRV__EFF_TRANS_RAMP_DWN_MEDIUM_SMOOTH_1 = 72,		//!< Transition Ramp Down Medium Smooth 1 - 100 to 0%
	DRV__EFF_TRANS_RAMP_DWN_MEDIUM_SMOOTH_2 = 73,		//!< Transition Ramp Down Medium Smooth 2 - 100 to 0%
	DRV__EFF_TRANS_RAMP_DWN_SHORT_SMOOTH_1 = 74,		//!< Transition Ramp Down Short Smooth 1 - 100 to 0%
	DRV__EFF_TRANS_RAMP_DWN_SHORT_SMOOTH_2 = 75,		//!< Transition Ramp Down Short Smooth 2 - 100 to 0%
	DRV__EFF_TRANS_RAMP_DWN_LONG_SHARP_1 = 76,			//!< Transition Ramp Down Long Sharp 1 - 100 t0 0%
	DRV__EFF_TRANS_RAMP_DWN_LONG_SHARP_2 = 77,			//!< Transition Ramp Down Long Sharp 2 - 100 to 0%
	DRV__EFF_TRANS_RAMP_DWN_MEDIUM_SHARP_1 = 78,		//!< Transition Ramp Down Medium Sharp 1 - 100 to 0%
	DRV__EFF_TRANS_RAMP_DWN_MEDIUM_SHARP_2 = 79,		//!< Transition Ramp Down Medium Sharp 2 - 100 to 0%
	DRV__EFF_TRANS_RAMP_DWN_SHORT_SHARP_1 = 80,			//!< Transition Ramp Down Short Sharp 1 - 100 to 0%
	DRV__EFF_TRANS_RAMP_DWN_SHORT_SHARP_2 = 81,			//!< Transition Ramp Down Short Sharp 2 - 100 to 0%
	DRV__EFF_TRANS_RAMP_UP_LONG_SMOOTH_1 = 82,			//!< Transition Ramp Up Long Smooth 1 - 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_LONG_SMOOTH_2 = 83,			//!< Transition Ramp Up Long Smooth 2 - 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_MEDIUM_SMOOTH_1 = 84,		//!< Transition Ramp Up Medium Smooth 1 - 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_MEDIUM_SMOOTH_2 = 85,		//!< Transition Ramp Up Medium Smooth 2 - 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_SHORT_SMOOTH_1 = 86,			//!< Transition Ramp Up Short Smooth 1 - 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_SHORT_SMOOTH_2 = 87,			//!< Transition Ramp Up Short Smooth 2 - 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_LONG_SHARP_1 = 88,			//!< Transition Ramp Up Long Sharp 1 - 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_LONG_SHARP_2 = 89,			//!< Transition Ramp Up Long Sharp 2 - 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_MEDIUM_SHARP_1 = 90,			//!< Transition Ramp Up Medium Sharp 1 - 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_MEDIUM_SHARP_2 = 91,			//!< Transition Ramp Up Medium Sharp 2 - 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_SHORT_SHARP_1 = 92,			//!< Transition Ramp Up Short Sharp 1 - 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_SHORT_SHARP_2 = 93,			//!< Transition Ramp Up Short Sharp 2 - 0 to 100%
	DRV__EFF_TRANS_RAMP_DWN_LONG_SMOOTH_1_LOW = 94,		//!< Transition Ramp Down Long Smooth 1 - 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_LONG_SMOOTH_2_LOW = 95,		//!< Transition Ramp Down Long Smooth 2 - 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_MEDIUM_SMOOTH_1_LOW = 96,	//!< Transition Ramp Down Medium Smooth 1 - 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_MEDIUM_SMOOTH_2_LOW = 97,	//!< Transition Ramp Down Medium Smooth 2 - 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_SHORT_SMOOTH_1_LOW = 98,	//!< Transition Ramp Down Short Smooth 1 - 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_SHORT_SMOOTH_2_LOW = 99,	//!< Transition Ramp Down Short Smooth 2 - 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_LONG_SHARP_1_LOW = 100,		//!< Transition Ramp Down Long Sharp 1 - 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_LONG_SHARP_2_LOW = 101,		//!< Transition Ramp Down Long Sharp 2 - 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_MEDIUM_SHARP_1_LOW = 102,	//!< Transition Ramp Down Medium Sharp 1 - 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_MEDIUM_SHARP_2_LOW = 103,	//!< Transition Ramp Down Medium Sharp 2 - 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_SHORT_SHARP_1_LOW = 104,	//!< Transition Ramp Down Short Sharp 1 - 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_SHORT_SHARP_2_LOW = 105,	//!< Transition Ramp Down Short Sharp 2 - 50 to 0%
	DRV__EFF_TRANS_RAMP_UP_LONG_SMOOTH_1_LOW = 106,		//!< Transition Ramp Up Long Smooth 1 - 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_LONG_SMOOTH_2_LOW = 107,		//!< Transition Ramp Up Long Smooth 2 - 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_MEDIUM_SMOOTH_1_LOW = 108,	//!< Transition Ramp Up Medium Smooth 1 - 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_MEDIUM_SMOOTH_2_LOW = 109,	//!< Transition Ramp Up Medium Smooth 2 - 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_SHORT_SMOOTH_1_LOW = 110,	//!< Transition Ramp Up Short Smooth 1 - 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_SHORT_SMOOTH_2_LOW = 111,	//!< Transition Ramp Up Short Smooth 2 - 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_LONG_SHARP_1_LOW = 112,		//!< Transition Ramp Up Long Sharp 1 - 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_LONG_SHARP_2_LOW = 113,		//!< Transition Ramp Up Long Sharp 2 - 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_MEDIUM_SHARP_1_LOW = 114,	//!< Transition Ramp Up Medium Sharp 1 - 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_MEDIUM_SHARP_2_LOW = 115,	//!< Transition Ramp Up Medium Sharp 2 - 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_SHORT_SHARP_1_LOW = 116,		//!< Transition Ramp Up Short Sharp 1 - 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_SHORT_SHARP_2_LOW = 117,		//!< Transition Ramp Up Short Sharp 2 - 0 to 50%
	DRV__EFF_LONG_BUZZ = 118,							//!< Long buzz for programmatic stopping - 100%
	DRV__EFF_SMOOTH_HUM_1 = 119,						//!< Smooth Hum 1 (No kick or brake pulse) - 50%
	DRV__EFF_SMOOTH_HUM_2 = 120,						//!< Smooth Hum 2 (No kick or brake pulse) - 40%
	DRV__EFF_SMOOTH_HUM_3 = 121,						//!< Smooth Hum 3 (No kick or brake pulse) - 30%
	DRV__EFF_SMOOTH_HUM_4 = 122,						//!< Smooth Hum 4 (No kick or brake pulse) - 20%
	DRV__EFF_SMOOTH_HUM_5 = 123							//!< Smooth Hum 5 (No kick or brake pulse) - 10%
} DRV2605L_eff;


/*!\union uDRV_REG__STATUS
** \brief DRV2605L STATUS register union
**/
typedef union uDRV_REG__STATUS {
	uint8_t Byte;
	struct {
		uint8_t OC_DETECT		:1;		//!< Latching over current detection flag
		uint8_t OVER_TEMP		:1;		//!< Latching over temperature detection flag
		uint8_t					:1;
		uint8_t DIAG_RESULT		:1;		//!< Auto Calibration routine diagnostic
		uint8_t 				:1;
		uint8_t DEVICE_ID		:3;		//!< Device identifier
	} Bits;
} uDRV_REG__STATUS;

/*!\union uDRV_REG__MODE
** \brief DRV2605L MODE register union
**/
typedef union uDRV_REG__MODE {
	uint8_t Byte;
	struct {
		DRV2605L_mode	MODE		:3;		//!< Working mode
		uint8_t						:3;
		uint8_t			STANDBY		:1;		//!< Software standby
		uint8_t			DEV_RESET	:1;		//!< Device reset
	} Bits;
} uDRV_REG__MODE;

/*!\union uDRV_REG__LIBRARY_SELECTION
** \brief DRV2605L LIBRARY SELECTION register union
**/
typedef union uDRV_REG__LIBRARY_SELECTION {
	uint8_t Byte;
	struct {
		DRV2605L_lib	LIBRARY_SEL	:3;		//!< Waveform library selection value
		uint8_t						:1;
		uint8_t			HI_Z		:1;		//!< True high impedance state
		uint8_t						:3;
	} Bits;
} uDRV_REG__LIBRARY_SELECTION;

/*!\union uDRV_REG__WAVEFORM_SEQUENCER
** \brief DRV2605L WAVEFORM SEQUENCER register union
**/
typedef union uDRV_REG__WAVEFORM_SEQUENCER {
	uint8_t Byte;
	struct {
		DRV2605L_eff	WAV_FRM_SEQ	:7;		//!< Waveform sequence value
		uint8_t			WAIT		:1;		//!< Delay between waveforms
	} Bits;
} uDRV_REG__WAVEFORM_SEQUENCER;

/*!\union uDRV_REG__GO
** \brief DRV2605L GO register union
**/
typedef union uDRV_REG__GO {
	uint8_t Byte;
	struct {
		DRV2605L_go GO	:1;		//!< Fire processes
		uint8_t 		:7;
	} Bits;
} uDRV_REG__GO;

/*!\union uDRV_REG__ATV_CONTROL
** \brief DRV2605L audio-to-vibe CONTROL register union
**/
typedef union uDRV_REG__ATV_CONTROL {
	uint8_t Byte;
	struct {
		DRV2605L_filter	ATH_FILTER		:2;		//!< Low pass filter frequency
		DRV2605L_peak	ATH_PEAK_TIME	:2;		//!< Peak detection time
		uint8_t 						:4;
	} Bits;
} uDRV_REG__ATV_CONTROL;

/*!\union uDRV_REG__FEEDBACK_CONTROL
** \brief DRV2605L FEEDBACK CONTROL register union
**/
typedef union uDRV_REG__FEEDBACK_CONTROL {
	uint8_t Byte;
	struct {
		uint8_t			BEMF_GAIN		:2;		//!< Analog gain for back-EMF amplifier
		uint8_t			LOOP_GAIN		:2;		//!< Loop gain for the feedback control
		uint8_t			FB_BRAKE_FACTOR	:3;		//!< Feedback gain ratio between braking gain and driving gain
		DRV2605L_act	N_ERM_LRA		:1;		//!< ERM/LRA Mode
	} Bits;
} uDRV_REG__FEEDBACK_CONTROL;

/*!\union uDRV_REG__CONTROL_1
** \brief DRV2605L CONTROL 1 register union
**/
typedef union uDRV_REG__CONTROL_1 {
	uint8_t Byte;
	struct {
		uint8_t DRIVE_TIME		:5;		//!< LRA: sets initial guess for LRA drive-time / ERM: sets the sample rate for the back-EMF detection
		uint8_t AC_COUPLE		:1;		//!< Apply 0.9V to IN_TRIG pin (only useful for analog input mode)
		uint8_t 				:1;
		uint8_t STARTUP_BOOST	:1;		//!< Higher loop gain during overdrive
	} Bits;
} uDRV_REG__CONTROL_1;

/*!\union uDRV_REG__CONTROL_2
** \brief DRV2605L CONTROL 2 register union
**/
typedef union uDRV_REG__CONTROL_2 {
	uint8_t Byte;
	struct {
		uint8_t IDISS_TIME			:2;		//!< Current dissipation time
		uint8_t BLANKING_TIME		:2;		//!< Blanking time before the back-EMF AD makes a conversion
		uint8_t SAMPLE_TIME			:2;		//!< LRA auto-resonance sampling time
		uint8_t BRAKE_STABILIZER	:1;		//!< Reduced gain when braking is almost complete to improve loop stability
		uint8_t BIDIR_INPUT			:1;		//!< Input mode (uni/bi directional)
	} Bits;
} uDRV_REG__CONTROL_2;

/*!\union uDRV_REG__CONTROL_3
** \brief DRV2605L CONTROL 3 register union
**/
typedef union uDRV_REG__CONTROL_3 {
	uint8_t Byte;
	struct {
		DRV2605L_loop		LRA_OPEN_LOOP	:1;		//!< Open-loop drive option for LRA mode (auto-resonance/open loop)
		DRV2605L_input		N_PWM_ANALOG	:1;		//!< Input mode for IN_TRIG when MODE[2:0]=3 (PWM/Analog input)
		uint8_t				LRA_DRIVE_MODE	:1;		//!< Drive mode for the LRA algorithm (Once/Twice per cycle)
		DRV2605L_rtp_format	DATA_FORMAT_RTP	:1;		//!< Mode of input data for RTP (signed/unsigned)
		uint8_t				SUPPLY_COMP_DIS	:1;		//!< Disable supply compensation
		DRV2605L_loop		ERM_OPEN_LOOP	:1;		//!< Mode of operation while in ERM mode (closed/open loop)
		uint8_t				NG_THRESH		:2;		//!< Noise gate threshold for PWM and analog inputs (Disabled/2%/4%/8%)
	} Bits;
} uDRV_REG__CONTROL_3;

/*!\union uDRV_REG__CONTROL_4
** \brief DRV2605L CONTROL 4 register union
**/
typedef union uDRV_REG__CONTROL_4 {
	uint8_t Byte;
	struct {
		uint8_t OTP_PROGRAM		:1;		//!< Launch programming process of OTP
		uint8_t 				:1;
		uint8_t OTP_STATUS		:1;		//!< OTP memory status (not programmed/programmed)
		uint8_t 				:1;
		uint8_t AUTO_CAL_TIME	:2;		//!< Sets the length for auto-calibration time (150/250/500/1000 ms)
		uint8_t ZC_DET_TIME		:2;		//!< Minimum length of time devoted for detecting a zero crossing (100/200/300/390 us)
	} Bits;
} uDRV_REG__CONTROL_4;

/*!\union uDRV_REG__CONTROL_5
** \brief DRV2605L CONTROL 5 register union
**/
typedef union uDRV_REG__CONTROL_5 {
	uint8_t Byte;
	struct {
		uint8_t IDISS_TIME			:2;		//!< MSB of IDISS_TIME from CONTROL_2
		uint8_t BLANKING_TIME		:2;		//!< MSB of BLANKING_TIME from CONTROL_2
		uint8_t PLAYBACK_INTERVAL	:1;		//!< Memory playback interval (5ms/1ms)
		uint8_t LRA_AUTO_OPEN_LOOP	:1;		//!< Auto transition to open-loop drive when no back-EMF signal detected (never/auto)
		uint8_t AUTO_OL_CNT			:2;		//!< Number of cycles required to attempt synchronization (3/4/5/6 attempts)
	} Bits;
} uDRV_REG__CONTROL_5;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for DRV2605L peripheral
** \weak DRV2605L Base address may be changed if user implemented
** \return FctERR - error code
**/
__WEAK FctERR DRV2605L_Init(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for DRV2605L
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR NONNULL__ DRV2605L_Write(const uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for DRV2605L
**
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ DRV2605L_Read(uint8_t * data, const uint16_t addr, const uint16_t nb);


/****************************************************************/
#include "DRV2605L_ex.h"	// Include extensions
#include "DRV2605L_proc.h"	// Include procedures

#ifdef __cplusplus
	}
#endif

#endif
#endif /* __DRV2605L_H__ */
/****************************************************************/
