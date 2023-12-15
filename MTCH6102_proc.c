/*!\file MTCH6102_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
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
#include <stdlib.h>
/****************************************************************/

MTCH6102_t MTCH6102[I2C_MTCH6102_NB] = { 0 };


#if defined(MTCH6102_DEFAULT_CONFIG_DEF)
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
#endif


/****************************************************************/


__WEAK FctERR NONNULL__ MTCH6102_Init_Sequence(MTCH6102_t * const pCpnt)
{
	uint8_t	MTCH_CORE[4];
	FctERR	err;

#if defined(MTCH6102_CARTESIAN_CENTERED)
	MTCH6102_Set_Centered_Coord(pCpnt, true);
	MTCH6102_Set_RxTx_Direction(pCpnt, false, true);
#else
	MTCH6102_Set_Centered_Coord(pCpnt, false);
	MTCH6102_Set_RxTx_Direction(pCpnt, false, false);	// To keep default demo board directions behavior (Tx not complemented)
#endif
	MTCH6102_Set_Rotation(pCpnt, 0);

	// Read Version & ID
	err = MTCH6102_Read(pCpnt->cfg.slave_inst, MTCH_CORE, MTCH__FW_MAJOR, sizeof(MTCH_CORE));
	if (err)	{ return err; }

	pCpnt->cfg.FW_Major = MTCH_CORE[0];
	pCpnt->cfg.FW_Minor = MTCH_CORE[1];
	pCpnt->cfg.APP_ID = MAKEWORD(MTCH_CORE[3], MTCH_CORE[2]);

	// Configure with default values
	err = MTCH6102_Configure(pCpnt, false, 20, 50, Filter_IIR, 1, Filter_Median, 5, 9, 6);
	if (err)	{ return err; }

	MTCH6102_Set_Grid(pCpnt);

	return ERROR_OK;
}


FctERR NONNULL__ MTCH6102_Configure(MTCH6102_t * const pCpnt, const bool store_to_nv,
									const uint16_t active_per, const uint16_t idle_per,
									const MTCH6102_FILTER_TYPE filter, const uint8_t filter_str,
									const MTCH6102_FILTER_TYPE base_filter, const uint8_t base_filter_str,
									const uint8_t rx, const uint8_t tx)
{
	const uint16_t	ACTIVE_PER = perVal2perReg(active_per);
	const uint16_t	IDLE_PER = perVal2perReg(idle_per);
	uint8_t			MTCH_ARRAY[4] = { 0 };
	FctERR			err;

	pCpnt->cfg.nb_rx = rx;
	pCpnt->cfg.nb_tx = tx;

	// Put in standby mode for configuration
	err = MTCH6102_Set_Mode(pCpnt, Standby);
	if (err)	{ return err; }

	// Send configuration parameters (and retry if not configured correctly after first try)
	while ((MTCH_ARRAY[0] != pCpnt->cfg.nb_rx) || (MTCH_ARRAY[1] != pCpnt->cfg.nb_tx))
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
		MTCH_ARRAY[0] = pCpnt->cfg.nb_rx;
		MTCH_ARRAY[1] = pCpnt->cfg.nb_tx;
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

	// Put in Gesture & Touch mode
	return MTCH6102_Set_Mode(pCpnt, Full);
}


void NONNULL__ MTCH6102_Set_Grid(MTCH6102_t * const pCpnt)
{
	pCpnt->cfg.max_rx = pCpnt->cfg.nb_rx * MTCH_RES_STEP;
	pCpnt->cfg.max_tx = pCpnt->cfg.nb_tx * MTCH_RES_STEP;

	// Set min & max possible values
	if (pCpnt->cfg.Centered)
	{
		pCpnt->max_x = pCpnt->cfg.max_rx / 2;
		pCpnt->max_y = pCpnt->cfg.max_tx / 2;
		pCpnt->min_x = -pCpnt->max_x;
		pCpnt->min_y = -pCpnt->max_y;
	}
	else
	{
		pCpnt->max_x = pCpnt->cfg.max_rx;
		pCpnt->max_y = pCpnt->cfg.max_tx;
		pCpnt->min_x = pCpnt->min_y = 0;
	}

	if (pCpnt->cfg.Rotation)
	{
		const sCoord2D original = { pCpnt->max_x, pCpnt->max_y };
		const sCoord2D coords = rotate_2D(original, pCpnt->cfg.Rotation);

		pCpnt->max_x = coords.x;
		pCpnt->max_y = coords.y;

		if (pCpnt->cfg.Centered)
		{
			pCpnt->min_x = -coords.x;
			pCpnt->min_y = -coords.y;
		}

		if (pCpnt->max_x < pCpnt->min_x)	{ SWAP_WORD(pCpnt->max_x, pCpnt->min_x); }
		if (pCpnt->max_y < pCpnt->min_y)	{ SWAP_WORD(pCpnt->max_y, pCpnt->min_y); }
	}
}


FctERR NONNULL__ MTCH6102_Set_Compensation(MTCH6102_t * const pCpnt)
{
	const uint8_t	nb = pCpnt->cfg.nb_rx + pCpnt->cfg.nb_tx;
	uint8_t			SENS_VAL[15];
	uint16_t		average = 0;
	FctERR			err;

	err = MTCH6102_Read(pCpnt->cfg.slave_inst, SENS_VAL, MTCH__SENSOR_VALUE_RX0, nb);
	if (err)	{ return err; }

	for (uintCPU_t i = 0 ; i < nb ; i++)	{ average += SENS_VAL[i]; }
	average /= nb;

	for (uintCPU_t i = 0 ; i < nb ; i++)
	{
		const float temp = (float) SENS_VAL[i] / average;
		SENS_VAL[i] = (uint8_t) ((temp == 1.0) ? 0 : (temp * 64));	// Compensation set to 0 if median is equal to sensor value for faster computing on the channel
	}

	return MTCH6102_Write(pCpnt->cfg.slave_inst, SENS_VAL, MTCH__SENSOR_COMP_RX0, nb);
}


FctERR NONNULL__ MTCH6102_Get_MFG_Results(MTCH6102_t * const pCpnt, uint32_t * const res)
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

	for (uintCPU_t i = 0 ; i < 15 ; i++)
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


FctERR NONNULL__ MTCH6102_decode_touch_datas(MTCH6102_t * const pCpnt, const MTCH6102_raw_gest * const dat)
{
	pCpnt->touch.State = dat->Gest_state;
	pCpnt->touch.Diag = dat->Gest_diag;

	pCpnt->touch.Large = dat->Touch_state.Bits.LRG;
	pCpnt->touch.Gesture = dat->Touch_state.Bits.GES;
	pCpnt->touch.Touch = dat->Touch_state.Bits.TCH;
	pCpnt->touch.Frame = dat->Touch_state.Bits.FRAME;

	pCpnt->touch.Coords.x = (LSHIFT(dat->Touch_x.Byte, 4) & 0x0FF0) | (dat->Touch_lsb.Bits.TOUCHX3_0 & 0x000F);
	pCpnt->touch.Coords.y = (LSHIFT(dat->Touch_y.Byte, 4) & 0x0FF0) | (dat->Touch_lsb.Bits.TOUCHY3_0 & 0x000F);

	if (pCpnt->cfg.RxDownwards)	{ pCpnt->touch.Coords.x = pCpnt->cfg.max_rx - pCpnt->touch.Coords.x; }
	if (pCpnt->cfg.TxDownwards)	{ pCpnt->touch.Coords.y = pCpnt->cfg.max_tx - pCpnt->touch.Coords.y; }

	if (pCpnt->cfg.Centered)
	{
		pCpnt->touch.Coords.x -= pCpnt->cfg.max_rx / 2;
		pCpnt->touch.Coords.y -= pCpnt->cfg.max_tx / 2;
	}

	pCpnt->touch.Coords = rotate_2D(pCpnt->touch.Coords, pCpnt->cfg.Rotation);	// Rotation

	return ERROR_OK;
}


FctERR NONNULL__ MTCH6102_gesture_to_str(char * const str, const MTCH6102_GESTURE_STATE state)
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

FctERR NONNULL__ MTCH6102_diag_to_str(char * const str, const MTCH6102_GESTURE_DIAGNOSTIC diag)
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


__WEAK FctERR NONNULL__ MTCH6102_handler(MTCH6102_t * const pCpnt)
{
	const bool			get_values = false;

	FctERR				err;
	MTCH6102_raw_gest	Gesture;

	err = MTCH6102_Get_Gest(pCpnt, &Gesture);
	if (err)	{ return err; }

	err = MTCH6102_decode_touch_datas(pCpnt, &Gesture);
	if (err)	{ return err; }

	#if defined(VERBOSE)
		const uint8_t	idx = pCpnt - MTCH6102;
		char			str_gest[18], str_diag[64];

		MTCH6102_gesture_to_str(str_gest, pCpnt->touch.State);
		MTCH6102_diag_to_str(str_diag, pCpnt->touch.Diag);

		printf("MTCH6102 id%d: T%d G%d L%d STATE: 0x%02X\t DIAG:0x%02X", idx, pCpnt->touch.Touch, pCpnt->touch.Gesture, pCpnt->touch.Large, pCpnt->touch.State, pCpnt->touch.Diag);
		printf("\tX: %-4ld\tY: %-4ld\t\tFrm: %d", pCpnt->touch.Coords.x, pCpnt->touch.Coords.y, pCpnt->touch.Frame);	// Coords padded to 3 digits with sign
		printf("\tST: %-18s\tDG: %s\r\n", str_gest, str_diag);	// Gesture string padded to 18 chars

		if (get_values)
		{
			MTCH6102_raw_sense SensValues;

			err = MTCH6102_Get_Raw(pCpnt, &SensValues);
			if (err)	{ return err; }

			printf("Sensor Values: ");
			for (uintCPU_t i = 0; i < sizeof(SensValues) ; i++)	{ printf("%04d ", SensValues.sensor[i]); }
			printf("\r\n");
		}
	#endif

	return ERROR_OK;
}


__WEAK FctERR NONNULL__ MTCH6102_handler_it(MTCH6102_t * const pCpnt)
{
	FctERR	err;
	bool	interrupt;

	err = MTCH6102_INT_GPIO_Get(pCpnt, &interrupt);
	if ((!err) && interrupt)	{ err = MTCH6102_handler(pCpnt); }

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/

