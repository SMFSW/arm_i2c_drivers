/*!\file DRV2605L.h
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief TI Haptic Driver declarations
**/
/****************************************************************/
#ifndef __DRV2605L_H__
	#define __DRV2605L_H__

#include "I2C_component.h"
#include "FctERR.h"

#include "sarmfsw.h"
#include CMSIS_INC
#include CMSIS_CFG

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/
// TODO: doxygen


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define DRV2605L_BASE_ADDR		(0x5A << 1)		//!< DRV2605L Base address


// *****************************************************************************
// Section: Types
// *****************************************************************************
typedef enum PACK__ DRV2605L_reg_map {
	DRV__STATUS = 0,						// 0x0E
	DRV__MODE,								// 0x40
	DRV__RTP_INPUT,							// 0x00
	DRV__LIBRARY_SELECTION,					// 0x01
	DRV__WAVEFORM_SEQUENCER_1,				// 0x01
	DRV__WAVEFORM_SEQUENCER_2,				// 0x00
	DRV__WAVEFORM_SEQUENCER_3,				// 0x00
	DRV__WAVEFORM_SEQUENCER_4,				// 0x00
	DRV__WAVEFORM_SEQUENCER_5,				// 0x00
	DRV__WAVEFORM_SEQUENCER_6,				// 0x00
	DRV__WAVEFORM_SEQUENCER_7,				// 0x00
	DRV__WAVEFORM_SEQUENCER_8,				// 0x00
	DRV__GO,								// 0x00
	DRV__OVERDRIVE_TIME_OFFSET,				// 0x00
	DRV__SUSTAIN_TIME_OFFSET_POS,			// 0x00
	DRV__SUSTAIN_TIME_OFFSET_NEG,			// 0x00
	DRV__BRAKE_TIME_OFFSET,					// 0x00
	DRV__ATV_CONTROL,						// 0x05
	DRV__ATV_MIN_INPUT_LEVEL,				// 0x19
	DRV__ATV_MAX_INPUT_LEVEL,				// 0xFF
	DRV__ATV_MIN_OUTPUT_DRIVE,				// 0x19
	DRV__ATV_MAX_OUTPUT_DRIVE,				// 0xFF
	DRV__RATED_VOLTAGE,						// 0x3E
	DRV__OVERDRIVE_CLAMP_VOLTAGE,			// 0x8C
	DRV__AUTO_CALIB_COMPENSATION_RESULT,	// 0x0C
	DRV__AUTO_CALIB_BACK_EMF_RESULT,		// 0x6C
	DRV__FEEDBACK_CONTROL,					// 0x36
	DRV__CONTROL_1,							// 0x93
	DRV__CONTROL_2,							// 0xF5
	DRV__CONTROL_3,							// 0xA0
	DRV__CONTROL_4,							// 0x20
	DRV__CONTROL_5,							// 0x80
	DRV__LRA_OPEN_LOOP_PERIOD,				// 0x33
	DRV__VBAT_VOLTAGE_MONITOR,				// 0x00
	DRV__LRA_RESONNANCE_PERIOD				// 0x00
} DRV2605L_reg;


typedef enum PACK__ DRV2605L_effects {
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
	DRV__EFF_ALERT_750MS = 15,							//!< 750 ms Alert 100%
	DRV__EFF_ALERT_1000MS = 16,							//!< 1000 ms Alert 100%
	DRV__EFF_STRONG_CLK_1 = 17,							//!< Strong Click 1 - 100%
	DRV__EFF_STRONG_CLK_2 = 18,							//!< Strong Click 2 - 80%
	DRV__EFF_STRONG_CLK_3 = 19,							//!< Strong Click 3 - 60%
	DRV__EFF_STRONG_CLK_4 = 20,							//!< Strong Click 4 - 30%
	DRV__EFF_MEDIUM_CLK_1 = 21,							//!< Medium Click 1 - 100%
	DRV__EFF_MEDIUM_CLK_2 = 22,							//!< Medium Click 2 - 80%
	DRV__EFF_MEDIUM_CLK_3 = 23,							//!< Medium Click 3 - 60%
	DRV__EFF_SHARP_TICK_1 = 24,							//!< Sharp Tick 1 - 100%
	DRV__EFF_SHARP_TICK_2 = 25,							//!< Sharp Tick 2 - 80%
	DRV__EFF_SHARP_TICK_3 = 26,							//!< Sharp Tick 3 – 60%
	DRV__EFF_SHORT_DBL_CLK_STRONG_1 = 27,				//!< Short Double Click Strong 1 – 100%
	DRV__EFF_SHORT_DBL_CLK_STRONG_2 = 28,				//!< Short Double Click Strong 2 – 80%
	DRV__EFF_SHORT_DBL_CLK_STRONG_3 = 29,				//!< Short Double Click Strong 3 – 60%
	DRV__EFF_SHORT_DBL_CLK_STRONG_4 = 30,				//!< Short Double Click Strong 4 – 30%
	DRV__EFF_SHORT_DBL_CLK_MEDIUM_1 = 31,				//!< Short Double Click Medium 1 – 100%
	DRV__EFF_SHORT_DBL_CLK_MEDIUM_2 = 32,				//!< Short Double Click Medium 2 – 80%
	DRV__EFF_SHORT_DBL_CLK_MEDIUM_3 = 33,				//!< Short Double Click Medium 3 – 60%
	DRV__EFF_SHORT_DBL_SHARP_TICK_1 = 34,				//!< Short Double Sharp Tick 1 – 100%
	DRV__EFF_SHORT_DBL_SHARP_TICK_2 = 35,				//!< Short Double Sharp Tick 2 – 80%
	DRV__EFF_SHORT_DBL_SHARP_TICK_3 = 36,				//!< Short Double Sharp Tick 3 – 60%
	DRV__EFF_LONG_DBL_CLK_STRONG_1 = 37,				//!< Long Double Sharp Click Strong 1 – 100%
	DRV__EFF_LONG_DBL_CLK_STRONG_2 = 38,				//!< Long Double Sharp Click Strong 2 – 80%
	DRV__EFF_LONG_DBL_CLK_STRONG_3 = 39,				//!< Long Double Sharp Click Strong 3 – 60%
	DRV__EFF_LONG_DBL_CLK_STRONG_4 = 40,				//!< Long Double Sharp Click Strong 4 – 30%
	DRV__EFF_LONG_DBL_CLK_MEDIUM_1 = 41,				//!< Long Double Sharp Click Medium 1 – 100%
	DRV__EFF_LONG_DBL_CLK_MEDIUM_2 = 42,				//!< Long Double Sharp Click Medium 2 – 80%
	DRV__EFF_LONG_DBL_CLK_MEDIUM_3 = 43,				//!< Long Double Sharp Click Medium 3 – 60%
	DRV__EFF_LONG_DBL_SHARP_TICK_1 = 44,				//!< Long Double Sharp Tick 1 – 100%
	DRV__EFF_LONG_DBL_SHARP_TICK_2 = 45,				//!< Long Double Sharp Tick 2 – 80%
	DRV__EFF_LONG_DBL_SHARP_TICK_3 = 46,				//!< Long Double Sharp Tick 3 – 60%
	DRV__EFF_BUZZ_1 = 47,								//!< Buzz 1 – 100%
	DRV__EFF_BUZZ_2 = 48,								//!< Buzz 2 – 80%
	DRV__EFF_BUZZ_3 = 49,								//!< Buzz 3 – 60%
	DRV__EFF_BUZZ_4 = 50,								//!< Buzz 4 – 40%
	DRV__EFF_BUZZ_5 = 51,								//!< Buzz 5 – 20%
	DRV__EFF_PULSING_STRONG_1 = 52,						//!< Pulsing Strong 1 – 100%
	DRV__EFF_PULSING_STRONG_2 = 53,						//!< Pulsing Strong 2 – 60%
	DRV__EFF_PULSING_MEDIUM_1 = 54,						//!< Pulsing Medium 1 – 100%
	DRV__EFF_PULSING_MEDIUM_2 = 55,						//!< Pulsing Medium 2 – 60%
	DRV__EFF_PULSING_SHARP_1 = 56,						//!< Pulsing Sharp 1 – 100%
	DRV__EFF_PULSING_SHARP_2 = 57,						//!< Pulsing Sharp 2 – 60%
	DRV__EFF_TRANS_CLK_1 = 58,							//!< Transition Click 1 – 100%
	DRV__EFF_TRANS_CLK_2 = 59,							//!< Transition Click 2 – 80%
	DRV__EFF_TRANS_CLK_3 = 60,							//!< Transition Click 3 – 60%
	DRV__EFF_TRANS_CLK_4 = 61,							//!< Transition Click 4 – 40%
	DRV__EFF_TRANS_CLK_5 = 62,							//!< Transition Click 5 – 20%
	DRV__EFF_TRANS_CLK_6 = 63,							//!< Transition Click 6 – 10%
	DRV__EFF_TRANS_HUM_1 = 64,							//!< Transition Hum 1 – 100%
	DRV__EFF_TRANS_HUM_2 = 65,							//!< Transition Hum 2 – 80%
	DRV__EFF_TRANS_HUM_3 = 66,							//!< Transition Hum 3 – 60%
	DRV__EFF_TRANS_HUM_4 = 67,							//!< Transition Hum 4 – 40%
	DRV__EFF_TRANS_HUM_5 = 68,							//!< Transition Hum 5 – 20%
	DRV__EFF_TRANS_HUM_6 = 69,							//!< Transition Hum 6 – 10%
	DRV__EFF_TRANS_RAMP_DWN_LONG_SMOOTH_1 = 70,			//!< Transition Ramp Down Long Smooth 1 – 100 to 0%
	DRV__EFF_TRANS_RAMP_DWN_LONG_SMOOTH_2 = 71,			//!< Transition Ramp Down Long Smooth 2 – 100 to 0%
	DRV__EFF_TRANS_RAMP_DWN_MEDIUM_SMOOTH_1 = 72,		//!< Transition Ramp Down Medium Smooth 1 – 100 to 0%
	DRV__EFF_TRANS_RAMP_DWN_MEDIUM_SMOOTH_2 = 73,		//!< Transition Ramp Down Medium Smooth 2 – 100 to 0%
	DRV__EFF_TRANS_RAMP_DWN_SHORT_SMOOTH_1 = 74,		//!< Transition Ramp Down Short Smooth 1 – 100 to 0%
	DRV__EFF_TRANS_RAMP_DWN_SHORT_SMOOTH_2 = 75,		//!< Transition Ramp Down Short Smooth 2 – 100 to 0%
	DRV__EFF_TRANS_RAMP_DWN_LONG_SHARP_1 = 76,			//!< Transition Ramp Down Long Sharp 1 – 100 t0 0%
	DRV__EFF_TRANS_RAMP_DWN_LONG_SHARP_2 = 77,			//!< Transition Ramp Down Long Sharp 2 – 100 to 0%
	DRV__EFF_TRANS_RAMP_DWN_MEDIUM_SHARP_1 = 78,		//!< Transition Ramp Down Medium Sharp 1 – 100 to 0%
	DRV__EFF_TRANS_RAMP_DWN_MEDIUM_SHARP_2 = 79,		//!< Transition Ramp Down Medium Sharp 2 – 100 to 0%
	DRV__EFF_TRANS_RAMP_DWN_SHORT_SHARP_1 = 80,			//!< Transition Ramp Down Short Sharp 1 – 100 to 0%
	DRV__EFF_TRANS_RAMP_DWN_SHORT_SHARP_2 = 81,			//!< Transition Ramp Down Short Sharp 2 – 100 to 0%
	DRV__EFF_TRANS_RAMP_UP_LONG_SMOOTH_1 = 82,			//!< Transition Ramp Up Long Smooth 1 – 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_LONG_SMOOTH_2 = 83,			//!< Transition Ramp Up Long Smooth 2 – 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_MEDIUM_SMOOTH_1 = 84,		//!< Transition Ramp Up Medium Smooth 1 – 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_MEDIUM_SMOOTH_2 = 85,		//!< Transition Ramp Up Medium Smooth 2 – 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_SHORT_SMOOTH_1 = 86,			//!< Transition Ramp Up Short Smooth 1 – 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_SHORT_SMOOTH_2 = 87,			//!< Transition Ramp Up Short Smooth 2 – 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_LONG_SHARP_1 = 88,			//!< Transition Ramp Up Long Sharp 1 – 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_LONG_SHARP_2 = 89,			//!< Transition Ramp Up Long Sharp 2 – 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_MEDIUM_SHARP_1 = 90,			//!< Transition Ramp Up Medium Sharp 1 – 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_MEDIUM_SHARP_2 = 91,			//!< Transition Ramp Up Medium Sharp 2 – 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_SHORT_SHARP_1 = 92,			//!< Transition Ramp Up Short Sharp 1 – 0 to 100%
	DRV__EFF_TRANS_RAMP_UP_SHORT_SHARP_2 = 93,			//!< Transition Ramp Up Short Sharp 2 – 0 to 100%
	DRV__EFF_TRANS_RAMP_DWN_LONG_SMOOTH_1_LOW = 94,		//!< Transition Ramp Down Long Smooth 1 – 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_LONG_SMOOTH_2_LOW = 95,		//!< Transition Ramp Down Long Smooth 2 – 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_MEDIUM_SMOOTH_1_LOW = 96,	//!< Transition Ramp Down Medium Smooth 1 – 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_MEDIUM_SMOOTH_2_LOW = 97,	//!< Transition Ramp Down Medium Smooth 2 – 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_SHORT_SMOOTH_1_LOW = 98,	//!< Transition Ramp Down Short Smooth 1 – 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_SHORT_SMOOTH_2_LOW = 99,	//!< Transition Ramp Down Short Smooth 2 – 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_LONG_SHARP_1_LOW = 100,		//!< Transition Ramp Down Long Sharp 1 – 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_LONG_SHARP_2_LOW = 101,		//!< Transition Ramp Down Long Sharp 2 – 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_MEDIUM_SHARP_1_LOW = 102,	//!< Transition Ramp Down Medium Sharp 1 – 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_MEDIUM_SHARP_2_LOW = 103,	//!< Transition Ramp Down Medium Sharp 2 – 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_SHORT_SHARP_1_LOW = 104,	//!< Transition Ramp Down Short Sharp 1 – 50 to 0%
	DRV__EFF_TRANS_RAMP_DWN_SHORT_SHARP_2_LOW = 105,	//!< Transition Ramp Down Short Sharp 2 – 50 to 0%
	DRV__EFF_TRANS_RAMP_UP_LONG_SMOOTH_1_LOW = 106,		//!< Transition Ramp Up Long Smooth 1 – 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_LONG_SMOOTH_2_LOW = 107,		//!< Transition Ramp Up Long Smooth 2 – 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_MEDIUM_SMOOTH_1_LOW = 108,	//!< Transition Ramp Up Medium Smooth 1 – 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_MEDIUM_SMOOTH_2_LOW = 109,	//!< Transition Ramp Up Medium Smooth 2 – 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_SHORT_SMOOTH_1_LOW = 110,	//!< Transition Ramp Up Short Smooth 1 – 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_SHORT_SMOOTH_2_LOW = 111,	//!< Transition Ramp Up Short Smooth 2 – 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_LONG_SHARP_1_LOW = 112,		//!< Transition Ramp Up Long Sharp 1 – 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_LONG_SHARP_2_LOW = 113,		//!< Transition Ramp Up Long Sharp 2 – 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_MEDIUM_SHARP_1_LOW = 114,	//!< Transition Ramp Up Medium Sharp 1 – 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_MEDIUM_SHARP_2_LOW = 115,	//!< Transition Ramp Up Medium Sharp 2 – 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_SHORT_SHARP_1_LOW = 116,		//!< Transition Ramp Up Short Sharp 1 – 0 to 50%
	DRV__EFF_TRANS_RAMP_UP_SHORT_SHARP_2_LOW = 117,		//!< Transition Ramp Up Short Sharp 2 – 0 to 50%
	DRV__EFF_LONG_BUZZ = 118,							//!< Long buzz for programmatic stopping – 100%
	DRV__EFF_SMOOTH_HUM_1 = 119,						//!< Smooth Hum 1 (No kick or brake pulse) – 50%
	DRV__EFF_SMOOTH_HUM_2 = 120,						//!< Smooth Hum 2 (No kick or brake pulse) – 40%
	DRV__EFF_SMOOTH_HUM_3 = 121,						//!< Smooth Hum 3 (No kick or brake pulse) – 30%
	DRV__EFF_SMOOTH_HUM_4 = 122,						//!< Smooth Hum 4 (No kick or brake pulse) – 20%
	DRV__EFF_SMOOTH_HUM_5 = 123							//!< Smooth Hum 5 (No kick or brake pulse) – 10%
} DRV2605L_eff;


typedef union uDRV_REG__STATUS {
	uint8_t Byte;
	struct {
		uint8_t DEVICE_ID		:2;		//!< Device identifier
		uint8_t 				:1;
		uint8_t DIAG_RESULT		:1;		//!< Auto Calibration routine diagnostic
		uint8_t					:1;
		uint8_t OVER_TEMP		:1;		//!< Latching over temperature detection flag
		uint8_t OC_DETECT		:1;		//!< Latching over current detection flag
	} Bits;
} uDRV_REG__STATUS;

typedef union uDRV_REG__MODE {
	uint8_t Byte;
	struct {
		uint8_t DEV_RESET		:1;		//!< Device reset
		uint8_t STANDBY			:1;		//!< Software standby
		uint8_t 				:3;
		uint8_t	MODE			:3;		//!< Working mode
	} Bits;
} uDRV_REG__MODE;

typedef union uDRV_REG__LIBRARY_SELECTION {
	uint8_t Byte;
	struct {
		uint8_t 				:3;
		uint8_t HI_Z			:1;		//!< True high impedance state
		uint8_t 				:1;
		uint8_t	LIBRARY_SEL		:3;		//!< Waveform library selection value
	} Bits;
} uDRV_REG__LIBRARY_SELECTION;

typedef union uDRV_REG__WAVEFORM_SEQUENCER {
	uint8_t Byte;
	struct {
		uint8_t			WAIT		:1;		//!< Delay between waveforms
		DRV2605L_eff	WAV_FRM_SEQ	:7;		//!< Waveform sequence value
	} Bits;
} uDRV_REG__WAVEFORM_SEQUENCER;

typedef union uDRV_REG__GO {
	uint8_t Byte;
	struct {
		uint8_t 				:7;
		uint8_t GO				:1;		//!< Fire processes
	} Bits;
} uDRV_REG__GO;

typedef union uDRV_REG__ATV_CONTROL {
	uint8_t Byte;
	struct {
		uint8_t 				:4;
		uint8_t ATH_PEAK_TIME	:2;		//!< Peak detection time
		uint8_t ATH_FILTER		:2;		//!< Low pass filter frequency
	} Bits;
} uDRV_REG__ATV_CONTROL;

typedef union uDRV_REG__FEEDBACK_CONTROL {
	uint8_t Byte;
	struct {
		uint8_t N_ERM_LRA		:1;		//!< ERM//LRA Mode
		uint8_t FB_BRAKE_FACTOR	:2;		//!< Feedback gain ratio between braking gain and driving gain
		uint8_t LOOP_GAIN		:2;		//!< Loop gain for the feedback control
		uint8_t BEMF_GAIN		:2;		//!< Analog gain of the back-EMF amplifier
	} Bits;
} uDRV_REG__FEEDBACK_CONTROL;

typedef union uDRV_REG__CONTROL_1 {
	uint8_t Byte;
	struct {
		uint8_t STARTUP_BOOST	:1;		//!< Higher loop gain during overdrive
		uint8_t 				:1;
		uint8_t AC_COUPLE		:1;		//!< Apply 0.9V to IN_TRIG pin (only useful for analog input mode)
		uint8_t DRIVE_TIME		:5;		//!< LRA: sets initial guess for LRA drive-time / ERM: sets the sample rate for the back-EMF detection
	} Bits;
} uDRV_REG__CONTROL_1;

typedef union uDRV_REG__CONTROL_2 {
	uint8_t Byte;
	struct {
		uint8_t BIDIR_INPUT			:1;		//!< Input mode (uni/bi directional)
		uint8_t BRAKE_STABILIZER	:1;		//!< Reduced gain when braking is almost complete to improve loop stability
		uint8_t SAMPLE_TIME			:2;		//!< LRA auto-resonance sampling time
		uint8_t BLANKING_TIME		:2;		//!< Blanking time before the back-EMF AD makes a conversion
		uint8_t IDISS_TIME			:2;		//!< Current dissipation time
	} Bits;
} uDRV_REG__CONTROL_2;

typedef union uDRV_REG__CONTROL_3 {
	uint8_t Byte;
	struct {
		uint8_t NG_THRESH		:2;		//!< Noise gate threshold for PWM and analog inputs (Disabled/2%/4%/8%)
		uint8_t ERM_OPEN_LOOP	:1;		//!< Mode of operation while in ERM mode (closed/open loop)
		uint8_t SUPPLY_COMP_DIS	:1;		//!< Disable supply compensation
		uint8_t DATA_FORMAT_RTP	:1;		//!< Mode of input data for RTP (signed/unsigned)
		uint8_t LRA_DRIVE_MODE	:1;		//!< Drive mode for the LRA algorithm (Once/Twice per cycle)
		uint8_t N_PWM_ANALOG	:1;		//!< Input mode for IN_TRIG when MODE[2:0]=3 (PWM/Analog input)
		uint8_t LRA_OPEN_LOOP	:1;		//!< Open-loop drive option for LRA mode (auto-resonance/open loop)
	} Bits;
} uDRV_REG__CONTROL_3;

typedef union uDRV_REG__CONTROL_4 {
	uint8_t Byte;
	struct {
		uint8_t ZC_DET_TIME		:2;		//!< Minimum length of time devoted for detecting a zero crossing (100/200/300/390 us)
		uint8_t AUTO_CAL_TIME	:2;		//!< Sets the length of the auto-calibration time (150/250/500/1000 ms)
		uint8_t 				:1;
		uint8_t OTP_STATUS		:1;		//!< OTP memory status (not programmed/programmed)
		uint8_t 				:1;
		uint8_t OTP_PROGRAM		:1;		//!< Launch programming process of OTP
	} Bits;
} uDRV_REG__CONTROL_4;

typedef union uDRV_REG__CONTROL_5 {
	uint8_t Byte;
	struct {
		uint8_t AUTO_OL_CNT			:2;		//!< Number of cycles required to attempt synchronization (3/4/5/6 attempts)
		uint8_t LRA_AUTO_OPEN_LOOP	:1;		//!< Auto transition to open-loop drive when no back-EMF signal detected (never/auto)
		uint8_t PLAYBACK_INTERVAL	:1;		//!< Memory playback interval (5ms/1ms)
		uint8_t BLANKING_TIME		:2;		//!< MSB of BLANKING_TIME from CONTROL_2
		uint8_t IDISS_TIME			:2;		//!< MSB of IDISS_TIME from CONTROL_2
	} Bits;
} uDRV_REG__CONTROL_5;


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
// Slave init
FctERR DRV2605L_Init(void);

// Low level access
FctERR DRV2605L_Write(uint8_t * Buffer, uint16_t Addr, uint16_t nb);
FctERR DRV2605L_Read(uint8_t * Buffer, uint16_t Addr, uint16_t nb);


/****************************************************************/
#include "DRV2605L_ex.h"
#endif
#endif /* __DRV2605L_H__ */
/****************************************************************/
