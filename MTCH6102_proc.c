/*!\file MTCH6102_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
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
uint8_t MTCH6102_default_cfg[MTCH__I2CADDR - MTCH__NUMBER_OF_X_CHANNELS + 1U] = {
	0x09U,	// NUMBER_OF_X_CHANNELS @ 0x20
	0x06U,	// NUMBER_OF_Y_CHANNELS
	0x06U,	// SCAN_COUNT
	0x37U,	// TOUCH_THRESH_X
	0x28U,	// TOUCH_THRESH_Y
	0x85U,	// ACTIVE_PERIOD_L
	0x02U,	// ACTIVE_PERIOD_H
	0x4CU,	// IDLE_PERIOD_L
	0x06U,	// IDLE_PERIOD_H
	0x10U,	// IDLE_TIMEOUT
	0x04U,	// HYSTERESIS
	0x01U,	// DEBOUNCE_UP
	0x01U,	// DEBOUNCE_DOWN
	0x0AU,	// BASE_INTERVAL_L
	0x00U,	// BASE_INTERVAL_H
	0x14U,	// BASE_POS_FILTER
	0x14U,	// BASE_NEG_FILTER
	0x02U,	// FILTER_TYPE
	0x01U,	// FILTER_STRENGTH
	0x01U,	// BASE_FILTER_TYPE
	0x05U,	// BASE_FILTER_STRENGTH
	0x00U,	// LARGE_ACTIVATION_THRESH_L
	0x00U,	// LARGE_ACTIVATION_THRESH_H
	0x40U,	// HORIZONTAL_SWIPE_DISTANCE
	0x40U,	// VERTICAL_SWIPE_DISTANCE
	0x19U,	// SWIPE_HOLD_BOUNDARY
	0x19U,	// TAP_DISTANCE
	0x40U,	// DISTANCE_BETWEEN_TAPS
	0x32U,	// TAP_HOLD_TIME_L
	0x09U,	// TAP_HOLD_TIME_H
	0x0CU,	// GESTURE_CLICK_TIME
	0x20U,	// SWIPE_HOLD_THRESH
	0x04U,	// MIN_SWIPE_VELOCITY
	0x2DU,	// HORIZONTAL_GESTURE_ANGLE
	0x2DU,	// VERTICAL_GESTURE_ANGLE
	0x25U,	// I2CADDR
};
#endif


/****************************************************************/


__WEAK FctERR NONNULL__ MTCH6102_Init_Sequence(MTCH6102_t * const pCpnt)
{
	uint8_t	MTCH_CORE[4];

	// Read Version & ID
	FctERR err = MTCH6102_Read(pCpnt->cfg.slave_inst, MTCH_CORE, MTCH__FW_MAJOR, sizeof(MTCH_CORE));
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.FW_Major = MTCH_CORE[0];
	pCpnt->cfg.FW_Minor = MTCH_CORE[1];
	pCpnt->cfg.APP_ID = MAKEWORD(MTCH_CORE[3], MTCH_CORE[2]);

	// Configure with default values
	err = MTCH6102_Configure(pCpnt, false, 20U, 50U, Filter_IIR, 1U, Filter_Median, 5U, 9U, 6U);
	if (err != ERROR_OK)	{ goto ret; }

#if defined(MTCH6102_CARTESIAN_CENTERED)
	MTCH6102_Set_Centered_Coord(pCpnt, true);
	MTCH6102_Set_RxTx_Direction(pCpnt, false, true);
#else
	MTCH6102_Set_Centered_Coord(pCpnt, false);
	MTCH6102_Set_RxTx_Direction(pCpnt, false, false);	// To keep default demo board directions behavior (Tx not complemented)
#endif

	MTCH6102_Set_Rotation(pCpnt, 0);
	MTCH6102_Set_Grid(pCpnt);

	ret:
	return err;
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
	if (err != ERROR_OK)	{ goto ret; }

	// Send configuration parameters (and retry if not configured correctly after first try)
	while ((MTCH_ARRAY[0] != pCpnt->cfg.nb_rx) || (MTCH_ARRAY[1] != pCpnt->cfg.nb_tx))
	{
		// Set ACTIVEPERIOD / IDLEPERIOD
		MTCH_ARRAY[0] = LOBYTE(ACTIVE_PER);
		MTCH_ARRAY[1] = HIBYTE(ACTIVE_PER);
		MTCH_ARRAY[2] = LOBYTE(IDLE_PER);
		MTCH_ARRAY[3] = HIBYTE(IDLE_PER);
		err = MTCH6102_Write(pCpnt->cfg.slave_inst, MTCH_ARRAY, MTCH__ACTIVE_PERIOD_L, 4U);
		if (err != ERROR_OK)	{ goto ret; }

		// Set FILTERTYPE / FILTERSTRENGTH + BASEFILTERTYPE / BASEFILTERSTRENGTH
		MTCH_ARRAY[0] = filter;
		MTCH_ARRAY[1] = filter_str;
		MTCH_ARRAY[2] = base_filter;
		MTCH_ARRAY[3] = base_filter_str;
		err = MTCH6102_Write(pCpnt->cfg.slave_inst, MTCH_ARRAY, MTCH__FILTER_TYPE, 4U);
		if (err != ERROR_OK)	{ goto ret; }

		// Set NUMBEROFXCHANNELS / NUMBEROFYCHANNELS
		MTCH_ARRAY[0] = pCpnt->cfg.nb_rx;
		MTCH_ARRAY[1] = pCpnt->cfg.nb_tx;
		err = MTCH6102_Write(pCpnt->cfg.slave_inst, MTCH_ARRAY, MTCH__NUMBER_OF_X_CHANNELS, 2U);
		if (err != ERROR_OK)	{ goto ret; }

		// Send configuration request
		err = MTCH6102_Configuration_Request(pCpnt);
		if (err != ERROR_OK)	{ goto ret; }

		MTCH_ARRAY[0] = 0;
		MTCH_ARRAY[1] = 0;
		err = MTCH6102_Read(pCpnt->cfg.slave_inst, MTCH_ARRAY, MTCH__NUMBER_OF_X_CHANNELS, 2U);
		if (err != ERROR_OK)	{ goto ret; }
	}

	if (store_to_nv)
	{
		err = MTCH6102_Store_To_Non_Volatile(pCpnt);
		if (err != ERROR_OK)	{ goto ret; }
	}

	// Put in Gesture & Touch mode
	err = MTCH6102_Set_Mode(pCpnt, Full);

	ret:
	return err;
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
		pCpnt->min_x = 0;
		pCpnt->min_y = 0;
	}

	if (pCpnt->cfg.Rotation != 0)
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
	if (err != ERROR_OK)	{ goto ret; }

	for (uintCPU_t i = 0 ; i < nb ; i++)	{ average += SENS_VAL[i]; }
	average /= nb;

	for (uintCPU_t i = 0 ; i < nb ; i++)
	{
		const float temp = (float) SENS_VAL[i] / average;
		SENS_VAL[i] = (uint8_t) ((temp == 1.0f) ? 0 : (temp * 64.0f));	// Compensation set to 0 if median is equal to sensor value for faster computing on the channel
	}

	err = MTCH6102_Write(pCpnt->cfg.slave_inst, SENS_VAL, MTCH__SENSOR_COMP_RX0, nb);

	ret:
	return err;
}


FctERR NONNULL__ MTCH6102_Get_MFG_Results(MTCH6102_t * const pCpnt, uint32_t * const res)
{
	MTCH6102_MODE	mode;
	uint8_t			RES[6];
	uint32_t		result = 0;
	FctERR			err;

	// Get MTCH6102 decoding mode
	err = MTCH6102_Get_Mode(pCpnt, &mode);
	if (err != ERROR_OK)	{ goto ret; }

	// Put in standby mode for test
	err = MTCH6102_Set_Mode(pCpnt, Standby);
	if (err != ERROR_OK)	{ goto ret; }

	// Execute manufacturing test
	err = MTCH6102_Manufacturing_Test(pCpnt);
	if (err != ERROR_OK)	{ goto ret; }

	// Read Results
	err = MTCH6102_Read(pCpnt->cfg.slave_inst, RES, MTCH__RAW_ADC_00, sizeof(RES));
	if (err != ERROR_OK)	{ goto ret; }

	for (uintCPU_t i = 0 ; i < 15U ; i++)
	{
		uint32_t vdd = 0, gnd = 0;

		if (i <= 2U)
		{
			vdd = (RES[4] & LSHIFT(0x20U, i)) ? 1U : 0U;
			gnd = (RES[5] & LSHIFT(0x20U, i)) ? 1U : 0U;
		}
		else if (i <= 8U)
		{
			vdd = (RES[2] & LSHIFT(0x01U, i)) ? 1U : 0U;
			gnd = (RES[3] & LSHIFT(0x01U, i)) ? 1U : 0U;
		}
		else if (i <= 12U)
		{
			vdd = (RES[0] & LSHIFT(0x01U, i)) ? 1U : 0U;
			gnd = (RES[1] & LSHIFT(0x01U, i)) ? 1U : 0U;
		}
		else if (i <= 13U)
		{
			vdd = (RES[0] & 0x20U) ? 1U : 0U;
			gnd = (RES[1] & 0x20U) ? 1U : 0U;
		}
		else /*if (i <= 14U)*/
		{
			vdd = (RES[4] & 0x04U) ? 1U : 0U;
			gnd = (RES[5] & 0x04U) ? 1U : 0U;
		}

		result |= LSHIFT(gnd | LSHIFT(vdd, 16U), i);
	}

	*res = result;	// Store MFG result

	// Set MTCH6102 decoding mode back
	err = MTCH6102_Set_Mode(pCpnt, mode);

	ret:
	return err;
}


/****************************************************************/


void NONNULL__ MTCH6102_decode_touch_datas(MTCH6102_t * const pCpnt, const MTCH6102_raw_gest * const dat)
{
	pCpnt->touch.State = dat->Gest_state;
	pCpnt->touch.Diag = dat->Gest_diag;

	pCpnt->touch.Large = dat->Touch_state.Bits.LRG;
	pCpnt->touch.Gesture = dat->Touch_state.Bits.GES;
	pCpnt->touch.Touch = dat->Touch_state.Bits.TCH;
	pCpnt->touch.Frame = dat->Touch_state.Bits.FRAME;

	pCpnt->touch.Coords.x = (LSHIFT(dat->Touch_x.Byte, 4U) & 0x0FF0U) | (dat->Touch_lsb.Bits.TOUCHX3_0 & 0x000FU);
	pCpnt->touch.Coords.y = (LSHIFT(dat->Touch_y.Byte, 4U) & 0x0FF0U) | (dat->Touch_lsb.Bits.TOUCHY3_0 & 0x000FU);

	if (pCpnt->cfg.RxDownwards)	{ pCpnt->touch.Coords.x = pCpnt->cfg.max_rx - pCpnt->touch.Coords.x; }
	if (pCpnt->cfg.TxDownwards)	{ pCpnt->touch.Coords.y = pCpnt->cfg.max_tx - pCpnt->touch.Coords.y; }

	if (pCpnt->cfg.Centered)
	{
		pCpnt->touch.Coords.x -= pCpnt->cfg.max_rx / 2;
		pCpnt->touch.Coords.y -= pCpnt->cfg.max_tx / 2;
	}

	pCpnt->touch.Coords = rotate_2D(pCpnt->touch.Coords, pCpnt->cfg.Rotation);	// Rotation
}


FctERR NONNULL__ MTCH6102_gesture_to_str(char * const str, const MTCH6102_GESTURE_STATE state)
{
	FctERR err = ERROR_OK;

	switch (state)
	{
		case NoGesture:			UNUSED_RET strcpy(str, "No Gesture");			break;
		case SingleClick:		UNUSED_RET strcpy(str, "Single Click");			break;
		case ClickNHold:		UNUSED_RET strcpy(str, "Click n Hold");			break;
		case DoubleClick:		UNUSED_RET strcpy(str, "Double click");			break;
		case DownSwipe:			UNUSED_RET strcpy(str, "Down Swipe");			break;
		case DownSwipeNHold:	UNUSED_RET strcpy(str, "Down Swipe n Hold");	break;
		case RightSwipe:		UNUSED_RET strcpy(str, "Right Swipe");			break;
		case RightSwipeNHold:	UNUSED_RET strcpy(str, "Right Swipe n Hold");	break;
		case UpSwipe:			UNUSED_RET strcpy(str, "Up Swipe");				break;
		case UpSwipeNHold:		UNUSED_RET strcpy(str, "Up Swipe n Hold");		break;
		case LeftSwipe:			UNUSED_RET strcpy(str, "Left Swipe");			break;
		case LeftSwipeNHold:	UNUSED_RET strcpy(str, "Left Swipe n Hold");	break;
		default:				UNUSED_RET strcpy(str, ""); err = ERROR_VALUE;	break;
	}

	return err;
}

FctERR NONNULL__ MTCH6102_diag_to_str(char * const str, const MTCH6102_GESTURE_DIAGNOSTIC diag)
{
	FctERR err = ERROR_OK;

	switch (diag)
	{
		case ClickTimeout:					UNUSED_RET strcpy(str, "Click Timeout");								break;
		case SwipeTimeout:					UNUSED_RET strcpy(str, "Swipe Timeout");								break;
		case GeneralTimeout:				UNUSED_RET strcpy(str, "General Timeout");								break;
		case ClickThreshExceed:				UNUSED_RET strcpy(str, "Click threshold exceeded");						break;
		case SwipeThreshExceed:				UNUSED_RET strcpy(str, "Swipe threshold exceeded");						break;
		case SwipeNHoldThreshExceed:		UNUSED_RET strcpy(str, "Swipe and Hold threshold exceeded");			break;
		case SwipeOppositeDirThreshExceed:	UNUSED_RET strcpy(str, "Swipe Opposite Direction threshold exceeded");	break;
		case Reserved:						UNUSED_RET strcpy(str, "Reserved");										break;
		case SwipeNHoldValExceed:			UNUSED_RET strcpy(str, "Swipe and Hold value exceeded");				break;
		case OutsideSwipeAngle:				UNUSED_RET strcpy(str, "Outside Swipe Angle");							break;
		default:							UNUSED_RET strcpy(str, ""); err = ERROR_VALUE;							break;
	}

	return err;
}


__WEAK FctERR NONNULL__ MTCH6102_handler(MTCH6102_t * const pCpnt)
{
	MTCH6102_raw_gest Gesture;

	FctERR err = MTCH6102_Get_Gest(pCpnt, &Gesture);
	if (err != ERROR_OK)	{ goto ret; }

	MTCH6102_decode_touch_datas(pCpnt, &Gesture);

	#if defined(VERBOSE)
		const bool		get_values = false;
		const uint8_t	idx = pCpnt - MTCH6102;
		char			str_gest[18], str_diag[64];

		err = MTCH6102_gesture_to_str(str_gest, pCpnt->touch.State);
		err |= MTCH6102_diag_to_str(str_diag, pCpnt->touch.Diag);

		UNUSED_RET printf("MTCH6102 id%d: T%d G%d L%d STATE: %#02X\t DIAG:%#02X", idx, pCpnt->touch.Touch, pCpnt->touch.Gesture,
							pCpnt->touch.Large, pCpnt->touch.State, pCpnt->touch.Diag);
		UNUSED_RET printf("\tX: %-4ld\tY: %-4ld\t\tFrm: %d", pCpnt->touch.Coords.x, pCpnt->touch.Coords.y, pCpnt->touch.Frame);	// Coords padded to 3 digits with sign
		UNUSED_RET printf("\tST: %-18s\tDG: %s\r\n", str_gest, str_diag);	// Gesture string padded to 18 chars

		if (get_values)
		{
			MTCH6102_raw_sense SensValues;

			err |= MTCH6102_Get_Raw(pCpnt, &SensValues);
			if (err != ERROR_OK)	{ goto ret; }

			UNUSED_RET printf("Sensor Values: ");
			for (uintCPU_t i = 0 ; i < sizeof(SensValues) ; i++)	{ UNUSED_RET printf("%04d ", SensValues.sensor[i]); }
			UNUSED_RET printf("\r\n");
		}
	#endif

	ret:
	return err;
}


__WEAK FctERR NONNULL__ MTCH6102_handler_it(MTCH6102_t * const pCpnt)
{
	FctERR	err = ERROR_OK;

	if (MTCH6102_INT_GPIO_Get(pCpnt))	{ err = MTCH6102_handler(pCpnt); }

	return err;
}


FctERR MTCH6102_handler_all(void)
{
	FctERR err = ERROR_OK;

	for (MTCH6102_t * pCpnt = MTCH6102 ; pCpnt < &MTCH6102[I2C_MTCH6102_NB] ; pCpnt++)
	{
		err |= MTCH6102_handler(pCpnt);
	}

	return err;
}

FctERR MTCH6102_handler_it_all(void)
{
	FctERR err = ERROR_OK;

	for (MTCH6102_t * pCpnt = MTCH6102 ; pCpnt < &MTCH6102[I2C_MTCH6102_NB] ; pCpnt++)
	{
		err |= MTCH6102_handler_it(pCpnt);
	}

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/

