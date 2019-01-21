/*!\file MTCH6102_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
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

MTCH6102_t MTCH6102[I2C_MTCH6102_NB];


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


/*!\brief Set MTCH6102 grid min & max values
** \param[in,out] pCpnt - Pointer to MTCH6102 component
**/
static void NONNULL__ MTCH6102_Set_Grid(MTCH6102_t * pCpnt)
{
	// Set min & max possible values
	if (pCpnt->cfg.Centered)
	{
		pCpnt->max_x = (pCpnt->cfg.nb_x * MTCH_RES_STEP) / 2;
		pCpnt->min_x = -pCpnt->max_x;
		pCpnt->max_y = (pCpnt->cfg.nb_y * MTCH_RES_STEP) / 2;
		pCpnt->min_y = -pCpnt->max_y;
	}
	else
	{
		pCpnt->max_x = pCpnt->cfg.nb_x * MTCH_RES_STEP;
		pCpnt->max_y = pCpnt->cfg.nb_y * MTCH_RES_STEP;
		pCpnt->min_x = pCpnt->min_y = 0;
	}
}


__WEAK FctERR NONNULL__ MTCH6102_Init_Sequence(MTCH6102_t * pCpnt)
{
	uint8_t	MTCH_CORE[4];
	FctERR	err;

	pCpnt->cfg.Centered = false;

	// Read Version & ID
	err = MTCH6102_Read(pCpnt->cfg.slave_inst, MTCH_CORE, MTCH__FW_MAJOR, sizeof(MTCH_CORE));
	if (err)	{ return err; }

	pCpnt->cfg.FW_Major = MTCH_CORE[0];
	pCpnt->cfg.FW_Minor = MTCH_CORE[1];
	pCpnt->cfg.APP_ID = MAKEWORD(MTCH_CORE[3], MTCH_CORE[2]);

	// Configure with default values
	return MTCH6102_Configure(pCpnt, false, 20, 50, Filter_IIR, 1, Filter_Median, 5, 9, 6);
}


FctERR NONNULL__ MTCH6102_Configure(MTCH6102_t * pCpnt, const bool store_to_nv,
									const uint16_t active_per, const uint16_t idle_per,
									const MTCH6102_FILTER_TYPE filter, const uint8_t filter_str,
									const MTCH6102_FILTER_TYPE base_filter, const uint8_t base_filter_str,
									const uint8_t rx, const uint8_t tx)
{
	uint8_t		MTCH_ARRAY[4] = { 0, 0, 0, 0 };
	uint16_t	ACTIVE_PER = perVal2perReg(active_per);
	uint16_t	IDLE_PER = perVal2perReg(idle_per);
	FctERR		err;

	pCpnt->cfg.nb_x = rx;
	pCpnt->cfg.nb_y = tx;

	// Put in standby mode for configuration
	err = MTCH6102_Set_Mode(pCpnt, Standby);
	if (err)	{ return err; }

	// Send configuration parameters (and retry if not configured correctly after first try)
	while ((MTCH_ARRAY[0] != pCpnt->cfg.nb_x) || (MTCH_ARRAY[1] != pCpnt->cfg.nb_y))
	{
		// Set ACTIVEPERIOD / IDLEPERIOD
		MTCH_ARRAY[0] = LOBYTE(ACTIVE_PER);
		MTCH_ARRAY[1] = HIBYTE(ACTIVE_PER);
		MTCH_ARRAY[2] = LOBYTE(IDLE_PER);
		MTCH_ARRAY[3] = HIBYTE(IDLE_PER);
		err = MTCH6102_Write(pCpnt->cfg.slave_inst, MTCH_ARRAY, MTCH__ACTIVE_PERIOD_L, 4);
		if (err)	{ return err; }

		// Set FILTERTYPE / FILTERSTRENGTH + BASEFILTERTYPE / BASEFILTERSTRENGTH
		MTCH_ARRAY[0] = filter;
		MTCH_ARRAY[1] = filter_str;
		MTCH_ARRAY[2] = base_filter;
		MTCH_ARRAY[3] = base_filter_str;
		err = MTCH6102_Write(pCpnt->cfg.slave_inst, MTCH_ARRAY, MTCH__FILTER_TYPE, 4);
		if (err)	{ return err; }

		// Set NUMBEROFXCHANNELS / NUMBEROFYCHANNELS
		MTCH_ARRAY[0] = pCpnt->cfg.nb_x;
		MTCH_ARRAY[1] = pCpnt->cfg.nb_y;
		err = MTCH6102_Write(pCpnt->cfg.slave_inst, MTCH_ARRAY, MTCH__NUMBER_OF_X_CHANNELS, 2);
		if (err)	{ return err; }

		// Send configuration request
		err = MTCH6102_Configuration_Request(pCpnt);
		if (err)	{ return err; }

		MTCH_ARRAY[0] = 0;
		MTCH_ARRAY[1] = 0;
		err = MTCH6102_Read(pCpnt->cfg.slave_inst, MTCH_ARRAY, MTCH__NUMBER_OF_X_CHANNELS, 2);
		if (err)	{ return err; }
	}

	if (store_to_nv)
	{
		err = MTCH6102_Store_To_Non_Volatile(pCpnt);
		if (err)	{ return err; }
	}

	MTCH6102_Set_Grid(pCpnt);

	// Put in Gesture & Touch mode
	return MTCH6102_Set_Mode(pCpnt, Full);
}


FctERR NONNULL__ MTCH6102_Set_Compensation(MTCH6102_t * pCpnt)
{
	uint8_t		SENS_VAL[15];
	uint16_t	average = 0;
	FctERR		err;

	err = MTCH6102_Read(pCpnt->cfg.slave_inst, SENS_VAL, MTCH__SENSOR_VALUE_RX0, sizeof(SENS_VAL));
	if (err)	{ return err; }

	for (unsigned int i = 0 ; i < sizeof(SENS_VAL) ; i++)	{ average += SENS_VAL[i]; }
	average /= sizeof(SENS_VAL);

	for (unsigned int i = 0 ; i < sizeof(SENS_VAL) ; i++)
	{
		float temp = (float) SENS_VAL[i] / average;

		if (temp == 1.0)	{ SENS_VAL[i] = 0; }	// Compensation set to 0 if median is equal to sensor value for faster computing on the channel
		else				{ SENS_VAL[i] = (uint8_t) (temp * 64); }
	}

	return MTCH6102_Write(pCpnt->cfg.slave_inst, SENS_VAL, MTCH__SENSOR_COMP_RX0, sizeof(SENS_VAL));
}


FctERR NONNULL__ MTCH6102_Get_MFG_Results(MTCH6102_t * pCpnt, uint32_t * res)
{
	MTCH6102_MODE	mode;
	uint8_t			RES[6];
	uint32_t		vdd = 0, gnd = 0, result = 0;
	FctERR			err;

	// Get MTCH6102 decoding mode
	err = MTCH6102_Get_Mode(pCpnt, &mode);
	if (err)	{ return err; }

	// Put in standby mode for test
	err = MTCH6102_Set_Mode(pCpnt, Standby);
	if (err)	{ return err; }

	// Execute manufacturing test
	err = MTCH6102_Manufacturing_Test(pCpnt);
	if (err)	{ return err; }

	// Read Results
	err = MTCH6102_Read(pCpnt->cfg.slave_inst, RES, MTCH__RAW_ADC_00, sizeof(RES));
	if (err)	{ return err; }

	for (unsigned int i = 0 ; i < 15 ; i++)
	{
		if (i <= 2)
		{
			vdd = (RES[4] & (0x20 << i)) ? 1 : 0;
			gnd = (RES[5] & (0x20 << i)) ? 1 : 0;
		}
		else if (i <= 8)
		{
			vdd = (RES[2] & (0x01 << i)) ? 1 : 0;
			gnd = (RES[3] & (0x01 << i)) ? 1 : 0;
		}
		else if (i <= 12)
		{
			vdd = (RES[0] & (0x01 << i)) ? 1 : 0;
			gnd = (RES[1] & (0x01 << i)) ? 1 : 0;
		}
		else if (i == 13)
		{
			vdd = (RES[0] & 0x20) ? 1 : 0;
			gnd = (RES[1] & 0x20) ? 1 : 0;
		}
		else if (i == 14)
		{
			vdd = (RES[4] & 0x04) ? 1 : 0;
			gnd = (RES[5] & 0x04) ? 1 : 0;
		}

		result |= LSHIFT(gnd | LSHIFT(vdd, 16), i);
	}

	*res = result;	// Store MFG result

	// Set MTCH6102 decoding mode back
	return MTCH6102_Set_Mode(pCpnt, mode);
}


/****************************************************************/


FctERR NONNULL__ MTCH6102_decode_touch_datas(MTCH6102_t * pCpnt, MTCH6102_gesture * touch, const MTCH6102_raw_gest * dat)
{
	touch->Large = dat->Touch_state.Bits.LRG;
	touch->Gesture = dat->Touch_state.Bits.GES;
	touch->Touch = dat->Touch_state.Bits.TCH;
	touch->Frame = dat->Touch_state.Bits.FRAME;

	touch->Coords.x = LSHIFT(dat->Touch_x.Byte, 4) & 0x0FF0;
	touch->Coords.x |= dat->Touch_lsb.Bits.TOUCHX3_0 & 0x000F;

	touch->Coords.y = LSHIFT(dat->Touch_y.Byte, 4) & 0x0FF0;
	touch->Coords.y |= dat->Touch_lsb.Bits.TOUCHY3_0 & 0x000F;

	if (pCpnt->cfg.Centered)
	{
		// Translating 0,0 point at the center of the ring (thus allowing centered rotation of the point afterwards)
		touch->Coords.x -= ((pCpnt->cfg.nb_x * MTCH_RES_STEP) / 2);
		touch->Coords.y -= ((pCpnt->cfg.nb_y * MTCH_RES_STEP) / 2);
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


FctERR NONNULL__ MTCH6102_gesture_to_str(char * str, const MTCH6102_GESTURE_STATE state)
{
	switch (state)
	{
		case NoGesture:			strcpy(str, "No Gesture");			break;
		case SingleClick:		strcpy(str, "Single Click");		break;
		case ClickNHold:		strcpy(str, "Click n Hold");		break;
		case DoubleClick:		strcpy(str, "Double click");		break;
		case DownSwipe:			strcpy(str, "Down Swipe");			break;
		case DownSwipeNHold:	strcpy(str, "Down Swipe n Hold");	break;
		case RightSwipe:		strcpy(str, "Right Swipe");			break;
		case RightSwipeNHold:	strcpy(str, "Right Swipe n Hold");	break;
		case UpSwipe:			strcpy(str, "Up Swipe");			break;
		case UpSwipeNHold:		strcpy(str, "Up Swipe n Hold");		break;
		case LeftSwipe:			strcpy(str, "Left Swipe");			break;
		case LeftSwipeNHold:	strcpy(str, "Left Swipe n Hold");	break;
		default:				strcpy(str, "");					return ERROR_VALUE;
	}

	return ERROR_OK;
}

FctERR NONNULL__ MTCH6102_diag_to_str(char * str, const MTCH6102_GESTURE_DIAGNOSTIC diag)
{
	switch (diag)
	{
		case ClickTimeout:					strcpy(str, "Click Timeout");									break;
		case SwipeTimeout:					strcpy(str, "Swipe Timeout");									break;
		case GeneralTimeout:				strcpy(str, "General Timeout");									break;
		case ClickThreshExceed:				strcpy(str, "Click threshold exceeded");						break;
		case SwipeThreshExceed:				strcpy(str, "Swipe threshold exceeded");						break;
		case SwipeNHoldThreshExceed:		strcpy(str, "Swipe and Hold threshold exceeded");				break;
		case SwipeOppositeDirThreshExceed:	strcpy(str, "Swipe Opposite Direction threshold exceeded");		break;
		case Reserved:						strcpy(str, "Reserved");										break;
		case SwipeNHoldValExceed:			strcpy(str, "Swipe and Hold value exceeded");					break;
		case OutsideSwipeAngle:				strcpy(str, "Outside Swipe Angle");								break;
		default:							strcpy(str, "");												return ERROR_VALUE;
	}

	return ERROR_OK;
}


__WEAK FctERR NONNULL__ MTCH6102_handler(MTCH6102_t * pCpnt)
{
	const bool			get_values = false;

	FctERR				err;
	MTCH6102_raw_gest	Gesture;
	MTCH6102_raw_sense	SensValues;
	MTCH6102_gesture	touch;
	char				str_gest[18];
	char				str_diag[64];

	memset(&Gesture, 0, sizeof(Gesture));
	memset(&touch, 0, sizeof(touch));

	if (get_values)
	{
		memset(&SensValues, 0, sizeof(SensValues));
		err = MTCH6102_Read(pCpnt->cfg.slave_inst, (uint8_t *) &SensValues, MTCH__SENSOR_VALUE_RX0, sizeof(SensValues));
		if (err)	{ return err; }
	}

	err = MTCH6102_Read(pCpnt->cfg.slave_inst, (uint8_t *) &Gesture, MTCH__TOUCH_STATE, sizeof(Gesture));
	if (err)	{ return err; }

	err = MTCH6102_decode_touch_datas(pCpnt, &touch, &Gesture);
	if (err)	{ return err; }

	MTCH6102_gesture_to_str(str_gest, touch.State);
	MTCH6102_diag_to_str(str_diag, touch.Diag);

	#if defined(VERBOSE)
		printf("%d %d %d STATE: 0x%X\t DIAG:0x%X", touch.Touch, touch.Gesture, touch.Large, touch.State, touch.Diag);
		printf("\tX: %d\tY: %d\tFrm: %d", touch.Coords.x, touch.Coords.y, touch.Frame);
		printf("\tST: %s\tDG: %s\r\n", str_gest, str_diag);

		if (get_values)
		{
			printf("Sensor Values:");
			for (int i = 0; i < sizeof(SensValues) ; i++)	{ printf("%d ", SensValues.sensor[i]); }
			printf("\r\n");
		}
	#endif

	return ERROR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/

