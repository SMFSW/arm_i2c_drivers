/*!\file MTCH6102_t.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief MTCH6102 Driver procedures
** \details MTCH6102: Low-Power Projected Capacitive Touch Controller
**/
/****************************************************************/
#include "MTCH6102.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MTCH6102)
/****************************************************************/
// std libs
#include <math.h>
/****************************************************************/

MTCH6102_t MTCH6102 = { 0, 0, 0, 0, { &MTCH6102_hal, 9, 6, 0, 0, 0, false } };


uint8_t MTCH6102_default_core[MTCH__MODE_CON - MTCH__FW_MAJOR + 1] = {
	0x02,	// FW_MAJOR @ 0x00
	0x00,	// FW_MINOR
	0x00,	// APP_ID_H
	0x12,	// APP_ID_L
	0x00,	// CMD
	0x03,	// MODE
	0x00,	// MODE_CON
};

uint8_t MTCH6102_default_cfg[MTCH__I2CADDR - MTCH__NUMBER_OF_X_CHANNELS + 1] = {
	0x09,	// NUMBER_OF_X_CHANNELS @ 0x20
	0x06,	// NUMBER_OF_Y_CHANNELS
	0x06,	// SCAN_COUNT
	0x37,	// TOUCH_THRESH_X
	0x28,	// TOUCH_THRESH_Y
	0x85,	// ACTIVE_PERIOD_L
	0x02,	// ACTIVE_PERIOD_H
	0x4C,	// IDLE_PERIOD_L
	0x06,	// IDLE_PERIOD_H
	0x10,	// IDLE_TIMEOUT
	0x04,	// HYSTERESIS
	0x01,	// DEBOUNCE_UP
	0x01,	// DEBOUNCE_DOWN
	0x0A,	// BASE_INTERVAL_L
	0x00,	// BASE_INTERVAL_H
	0x14,	// BASE_POS_FILTER
	0x14,	// BASE_NEG_FILTER
	0x02,	// FILTER_TYPE
	0x01,	// FILTER_STRENGTH
	0x01,	// BASE_FILTER_TYPE
	0x05,	// BASE_FILTER_STRENGTH
	0x00,	// LARGE_ACTIVATION_THRESH_L
	0x00,	// LARGE_ACTIVATION_THRESH_H
	0x40,	// HORIZONTAL_SWIPE_DISTANCE
	0x40,	// VERTICAL_SWIPE_DISTANCE
	0x19,	// SWIPE_HOLD_BOUNDARY
	0x19,	// TAP_DISTANCE
	0x40,	// DISTANCE_BETWEEN_TAPS
	0x32,	// TAP_HOLD_TIME_L
	0x09,	// TAP_HOLD_TIME_H
	0x0C,	// GESTURE_CLICK_TIME
	0x20,	// SWIPE_HOLD_THRESH
	0x04,	// MIN_SWIPE_VELOCITY
	0x2D,	// HORIZONTAL_GESTURE_ANGLE
	0x2D,	// VERTICAL_GESTURE_ANGLE
	0x25,	// I2CADDR
};


/****************************************************************/


__WEAK FctERR MTCH6102_Init_Sequence(void)
{
	uint8_t	MTCH_CORE[4];
	uint8_t	MTCH_CFG[2] = { MTCH6102.cfg.nb_x, MTCH6102.cfg.nb_y };
	FctERR	err;

	// Put in standby mode for configuration
	err = MTCH6102_Set_Mode(Standby);
	if (err)	{ return err; }

	// Read Version & ID
	err = MTCH6102_Read(MTCH_CORE, MTCH__FW_MAJOR, sizeof(MTCH_CORE));
	if (err)	{ return err; }

	MTCH6102.cfg.FW_Major = MTCH_CORE[0];
	MTCH6102.cfg.FW_Minor = MTCH_CORE[1];
	MTCH6102.cfg.APP_ID = MAKEWORD(MTCH_CORE[3], MTCH_CORE[2]);

	// Send configuration parameters
	err = MTCH6102_Write(MTCH_CFG, MTCH__NUMBER_OF_X_CHANNELS, sizeof(MTCH_CFG));
	if (err)	{ return err; }

	// Send configuration request
	err = MTCH6102_Configuration_Request();
	if (err)	{ return err; }

	// Put in Gesture & Touch mode
	err = MTCH6102_Set_Mode(Full);
	if (err)	{ return err; }

	// Set min & max possible values
	if (MTCH6102.cfg.Centered)
	{
		MTCH6102.max_x = (MTCH6102.cfg.nb_x * MTCH_RES_STEP) / 2;
		MTCH6102.min_x = -MTCH6102.max_x;
		MTCH6102.max_y = (MTCH6102.cfg.nb_y * MTCH_RES_STEP) / 2;
		MTCH6102.min_y = -MTCH6102.max_y;
	}
	else
	{
		MTCH6102.max_x = MTCH6102.cfg.nb_x * MTCH_RES_STEP;
		MTCH6102.max_y = MTCH6102.cfg.nb_y * MTCH_RES_STEP;
		MTCH6102.min_x = MTCH6102.min_y = 0;
	}

	return err;
}


FctERR MTCH6102_Set_Compensation(void)
{
	uint8_t		SENS_VAL[15];
	uint16_t	average = 0;
	FctERR		err = ERROR_OK;

	err = MTCH6102_Read(SENS_VAL, MTCH__SENSOR_VALUE_RX0, sizeof(SENS_VAL));
	if (err)	{ return err; }

	for (unsigned int i = 0 ; i < sizeof(SENS_VAL) ; i++)	{ average += SENS_VAL[i]; }
	average /= sizeof(SENS_VAL);

	for (unsigned int i = 0 ; i < sizeof(SENS_VAL) ; i++)
	{
		float temp = (float) SENS_VAL[i] / average;

		if (temp == 1.0)	{ SENS_VAL[i] = 0; }	// Compensation set to 0 if median is equal to sensor value for faster computing on the channel
		else				{ SENS_VAL[i] = (uint8_t) (temp * 64); }
	}

	return MTCH6102_Write(SENS_VAL, MTCH__SENSOR_COMP_RX0, sizeof(SENS_VAL));
}


FctERR MTCH6102_Get_MFG_Results(uint32_t * res)
{
	uint8_t			RES[6];
	uint32_t		result = 0;
	MTCH6102_MODE	mode;
	FctERR			err = ERROR_OK;

	// Get MTCH6102 decoding mode
	err = MTCH6102_Get_Mode(&mode);
	if (err)	{ return err; }

	// Put in standby mode for test
	err = MTCH6102_Set_Mode(Standby);
	if (err)	{ return err; }

	// Execute manufacturing test
	err = MTCH6102_Manufacturing_Test();
	if (err)	{ return err; }

	// Read Results
	err = MTCH6102_Read(RES, MTCH__RAW_ADC_00, sizeof(RES));
	if (err)	{ return err; }

	for (unsigned int i = 0 ; i < 15 ; i++)
	{
		uint32_t gnd = 0, vdd = 0;

		switch (i)	// Channel
		{
			case 0:
			case 1:
			case 2:
				vdd |= (RES[4] & (0x20 << i)) ? 1 : 0;
				gnd |= (RES[5] & (0x20 << i)) ? 1 : 0;
				break;

			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				vdd |= (RES[2] & (0x01 << i)) ? 1 : 0;
				gnd |= (RES[3] & (0x01 << i)) ? 1 : 0;
				break;

			case 9:
			case 10:
			case 11:
			case 12:
				vdd |= (RES[0] & (0x01 << i)) ? 1 : 0;
				gnd |= (RES[1] & (0x01 << i)) ? 1 : 0;
				break;

			case 13:
				vdd |= (RES[0] & 0x20) ? 1 : 0;
				gnd |= (RES[1] & 0x20) ? 1 : 0;
				break;

			case 14:
				vdd |= (RES[4] & 0x04) ? 1 : 0;
				gnd |= (RES[5] & 0x04) ? 1 : 0;
				break;

			default:
				break;
		}

		result |= LSHIFT(gnd | LSHIFT(vdd, 16), i);
	}

	*res = result;	// Store MFG result

	// Set MTCH6102 decoding mode back
	return MTCH6102_Set_Mode(mode);
}


/****************************************************************/


FctERR MTCH6102_decode_touch_datas(MTCH6102_gesture * touch, const MTCH6102_raw_gest * dat)
{
	if ((!dat) || (!touch))		{ return ERROR_MEMORY; }	// Null pointer

	touch->Large = dat->Touch_state.Bits.LRG;
	touch->Gesture = dat->Touch_state.Bits.GES;
	touch->Touch = dat->Touch_state.Bits.TCH;
	touch->Frame = dat->Touch_state.Bits.FRAME;

	touch->Coords.x = LSHIFT(dat->Touch_x.Byte, 4) & 0x0FF0;
	touch->Coords.x |= dat->Touch_lsb.Bits.TOUCHX3_0 & 0x000F;

	touch->Coords.y = LSHIFT(dat->Touch_y.Byte, 4) & 0x0FF0;
	touch->Coords.y |= dat->Touch_lsb.Bits.TOUCHY3_0 & 0x000F;

	if (MTCH6102.cfg.Centered)
	{
		// Translating 0,0 point at the center of the ring (thus allowing centered rotation of the point afterwards)
		touch->Coords.x -= ((MTCH6102.cfg.nb_x * MTCH_RES_STEP) / 2);
		touch->Coords.y -= ((MTCH6102.cfg.nb_y * MTCH_RES_STEP) / 2);
	}

	touch->State = dat->Gest_state;
	touch->Diag = dat->Gest_diag;

	return ERROR_OK;
}


MTCH6102_Coord MTCH6102_rotate(const MTCH6102_Coord c, int16_t deg)
{
	float			rad;
	MTCH6102_Coord	r;

	deg %= 360;

	switch (deg)
	{
		case 0:
			return c;

		case 90:
		case -270:
			r.x = -c.y;
			r.y = c.x;
			break;

		case 180:
		case -180:
			r.x = -c.x;
			r.y = -c.y;
			break;

		case 270:
		case -90:
			r.x = c.y;
			r.y = -c.x;
			break;

		default:
			rad = (M_PI * deg) / 180.0f;
			r.x = (int16_t) ((c.x * cos(rad)) - (c.y * sin(rad)));
			r.y = (int16_t) ((c.x * sin(rad)) + (c.y * cos(rad)));
	}

	return r;
}


FctERR MTCH6102_gesture_to_str(char * str, const MTCH6102_GESTURE_STATE state)
{
	if (!str)		{ return ERROR_MEMORY; }	// Null pointer

	switch(state)
	{
		case NoGesture: strcpy(str, "No Gesture"); break;
		case SingleClick: strcpy(str, "Single Click"); break;
		case ClickNHold: strcpy(str, "Click n Hold"); break;
		case DoubleClick: strcpy(str, "Double click"); break;
		case DownSwipe: strcpy(str, "Down Swipe"); break;
		case DownSwipeNHold: strcpy(str, "Down Swipe n Hold"); break;
		case RightSwipe: strcpy(str, "Right Swipe"); break;
		case RightSwipeNHold: strcpy(str, "Right Swipe n Hold"); break;
		case UpSwipe: strcpy(str, "Up Swipe"); break;
		case UpSwipeNHold: strcpy(str, "Up Swipe n Hold"); break;
		case LeftSwipe: strcpy(str, "Left Swipe"); break;
		case LeftSwipeNHold: strcpy(str, "Left Swipe n Hold"); break;
		default: strcpy(str, ""); return ERROR_VALUE;
	}

	return ERROR_OK;
}

FctERR MTCH6102_diag_to_str(char * str, const MTCH6102_GESTURE_DIAGNOSTIC diag)
{
	if (!str)		{ return ERROR_MEMORY; }	// Null pointer

	switch(diag)
	{
		case ClickTimeout: strcpy(str, "Click Timeout"); break;
		case SwipeTimeout: strcpy(str, "Swipe Timeout"); break;
		case GeneralTimeout: strcpy(str, "General Timeout"); break;
		case ClickThreshExceed: strcpy(str, "Click threshold exceeded"); break;
		case SwipeThreshExceed: strcpy(str, "Swipe threshold exceeded"); break;
		case SwipeNHoldThreshExceed: strcpy(str, "Swipe and Hold threshold exceeded"); break;
		case SwipeOppositeDirThreshExceed: strcpy(str, "Swipe Opposite Direction threshold exceeded"); break;
		case Reserved: strcpy(str, "Reserved"); break;
		case SwipeNHoldValExceed: strcpy(str, "Swipe and Hold value exceeded"); break;
		case OutsideSwipeAngle: strcpy(str, "Outside Swipe Angle"); break;
		default: strcpy(str, ""); return ERROR_VALUE;
	}

	return ERROR_OK;
}


__WEAK FctERR MTCH6102_handler(void)
{
	FctERR				err;
	MTCH6102_raw_gest	Gesture;
	MTCH6102_raw_sense	SensValues;
	MTCH6102_gesture	touch;
	char				str_gest[18];
	char				str_diag[64];

	memset(&Gesture, 0, sizeof(Gesture));
	memset(&SensValues, 0, sizeof(SensValues));
	memset(&touch, 0, sizeof(touch));

	err = MTCH6102_Read((uint8_t *) &SensValues, MTCH__SENSOR_VALUE_RX0, sizeof(SensValues));
	if (err)	{ return err; }
	err = MTCH6102_Read((uint8_t *) &Gesture, MTCH__TOUCH_STATE, sizeof(Gesture));
	if (err)	{ return err; }

	err = MTCH6102_decode_touch_datas(&touch, &Gesture);
	if (err)	{ return err; }
	(void) MTCH6102_gesture_to_str(str_gest, touch.State);
	(void) MTCH6102_diag_to_str(str_diag, touch.Diag);

	#if defined(VERBOSE)
		printf("%d %d %d STATE: 0x%X\t DIAG:0x%X", touch.Touch, touch.Gesture, touch.Large, touch.State, touch.Diag);
		printf("\tX: %d\tY: %d\tFrm: %d", touch.Coords.x, touch.Coords.y, touch.Frame);
		printf("\tST: %s\tDG: %s\r\n", str_gest, str_diag);

		printf("Sensor Values:");
		for (int i = 0; i < sizeof(SensValues) ; i++)	{ printf("%d ", SensValues.sensor[i]); }
		printf("\r\n");
	#endif

	return ERROR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/

