/*!\file MTCH6102_ex.c
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Microchip Capacitive Driver extensions code
**/
/****************************************************************/
#include "MTCH6102.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MTCH6102)
/****************************************************************/
// std libs
#include <string.h>
/****************************************************************/
// TODO: doxygen
// TODO: split get raw & get touch


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

FctERR MTCH6102_acquire(void)
{
	FctERR					err;
	uint8_t					Gesture[6];
	uint8_t					SensValue[16];
	DecodedTouch			touch;
	char					str_gest[18];
	char					str_diag[64];

	memset(&Gesture, 0, sizeof(Gesture));
	memset(&SensValue, 0, sizeof(SensValue));
	memset(&touch, 0, sizeof(touch));

	err = MTCH6102_Read(&Gesture[0], MTCH__TOUCH_STATE, sizeof(Gesture));
	if (err)	{ return err; }
	err = MTCH6102_Read(&SensValue[0], MTCH__SENSOR_VALUE_RX0, sizeof(SensValue));
	if (err)	{ return err; }

	err = MTCH6102_decode_touch_datas(&Gesture[0], &touch);
	if (err)	{ return err; }
	(void) MTCH6102_gesture_to_str(str_gest, touch.State);
	//if (err)	{ return err; }
	(void) MTCH6102_diag_to_str(str_diag, touch.Diag);
	//if (err)	{ return err; }

	printf("%i %i %i STATE: 0x%X\t DIAG:0x%X", touch.Touch, touch.Gesture, touch.Large, touch.State, touch.Diag);
	printf("X: %i\t\tY: %i\t\tFrm: %i", touch.X_pos, touch.Y_pos, touch.Frame);
	printf("ST: %s\t\tDG: %s", str_gest, str_diag);

	printf("\nSensor Values:");
	for (int i = 0; i < sizeof(SensValue) ; i++)
	{
		printf("%i ", SensValue[i]);
	}

	return ERR_OK;
}


FctERR MTCH6102_decode_touch_datas(uint8_t * dat, DecodedTouch * touch)
{
	uMTCH_REG__TOUCHSTATE	state;
	uMTCH_REG__TOUCHLSB		lsb;

	if (!dat)	{ return ERR_VALUE; }

	for (int i = 0; i < 6; i++)
	{
		switch(i)
		{
			case 0:
				state.Byte = *dat;
				touch->Large = (bool) state.Bits.LRG;
				touch->Gesture = (bool) state.Bits.GES;
				touch->Touch = (bool) state.Bits.TCH;
				touch->Frame = state.Bits.FRAME;
			break;
			case 1:
				touch->X_pos = ((uint16_t) *dat << 4) & 0x0FF0;
			break;
			case 2:
				touch->Y_pos = ((uint16_t) *dat << 4) & 0x0FF0;
			break;
			case 3:
				lsb.Byte = *dat;
				touch->X_pos |= lsb.Bits.TOUCHX3_0 & 0x000F;
				touch->Y_pos |= lsb.Bits.TOUCHY3_0 & 0x000F;
			break;
			case 4:
				touch->State = (MTCH6102_GESTURE_STATE) *dat;
			break;
			case 5:
				touch->Diag = (MTCH6102_GESTURE_DIAGNOSTIC) *dat;
			break;
			default:
			break;
		}
		dat++;
	}

	return true;
}


FctERR MTCH6102_gesture_to_str(char * str, MTCH6102_GESTURE_STATE state)
{
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
		default: strcpy(str, ""); return ERR_VALUE;
	}

	return ERR_OK;
}

FctERR MTCH6102_diag_to_str(char * str, MTCH6102_GESTURE_DIAGNOSTIC diag)
{
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
		default: strcpy(str, ""); return ERR_VALUE;
	}

	return ERR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/

