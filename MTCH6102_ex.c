/*!\file MTCH6102_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief MTCH6102 Driver extensions
** \details MTCH6102: Low-Power Projected Capacitive Touch Controller
**/
/****************************************************************/
#include "MTCH6102.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MTCH6102)
/****************************************************************/


FctERR NONNULL__ MTCH6102_Command(const MTCH6102_t * const pCpnt, const MTCH6102_COMMAND cmd)
{
	uMTCH_REG__CMD	MTCH_CMD = { 0 }, MEM_MTCH_CMD;
	FctERR			err;

	switch (cmd)
	{
		case MTCH_StoreToNV:
			MTCH_CMD.Bits.NV = 1;
			break;

		case MTCH_RestoreDefaults:
			MTCH_CMD.Bits.DEF = 1;
			break;

		case MTCH_Configure:
			MTCH_CMD.Bits.CFG = 1;
			break;

		case MTCH_ManufacturingTest:
			MTCH_CMD.Bits.MFG = 1;
			break;

		case MTCH_ForceBaseline:
			MTCH_CMD.Bits.BS = 1;
			break;

		default:
			return ERROR_VALUE;
	}

	MEM_MTCH_CMD = MTCH_CMD;

	// Send command
	err = MTCH6102_Write(pCpnt->cfg.slave_inst, &MTCH_CMD.Byte, MTCH__CMD, 1);
	if (err)	{ return err; }

	// Wait for command to complete
	while ((MTCH_CMD.Byte & MEM_MTCH_CMD.Byte) != 0)
	{
		err = MTCH6102_Read(pCpnt->cfg.slave_inst, &MTCH_CMD.Byte, MTCH__CMD, 1);
		if (err)	{ return err; }
		//HAL_Delay(1);
	}

	return err;
}


FctERR NONNULL__ MTCH6102_Get_Active_Period(const MTCH6102_t * const pCpnt, uint16_t * const period)
{
	uint8_t DAT[2];
	FctERR	err;

	err = MTCH6102_Read(pCpnt->cfg.slave_inst, DAT, MTCH__ACTIVE_PERIOD_L, sizeof(DAT));
	if (err)	{ return err; }

	*period = perReg2perVal(LSHIFT(DAT[1], 8) + DAT[0]);
	return ERROR_OK;
}


FctERR NONNULL__ MTCH6102_Get_Idle_Period(const MTCH6102_t * const pCpnt, uint16_t * const period)
{
	uint8_t DAT[2];
	FctERR	err;

	err = MTCH6102_Read(pCpnt->cfg.slave_inst, DAT, MTCH__IDLE_PERIOD_L, sizeof(DAT));
	if (err)	{ return err; }

	*period = perReg2perVal(LSHIFT(DAT[1], 8) + DAT[0]);
	return ERROR_OK;
}


FctERR NONNULL__ MTCH6102_Set_Filter(const MTCH6102_t * const pCpnt, const MTCH6102_FILTER_TYPE type, const uint8_t strength, const bool baseline_filter)
{
	uint8_t		MTCH_FILTER[2];
	FctERR		err;

	switch (type)
	{
		case Filter_None:
			MTCH_FILTER[0] = Filter_None;
			MTCH_FILTER[1] = 0;
			break;

		case Filter_Median:
			MTCH_FILTER[0] = Filter_Median;
			if ((strength >= 3) && (strength <= 9) && (strength & 0x01))	MTCH_FILTER[1] = strength;
			else															MTCH_FILTER[1] = 5;		// Default Median strength
			break;

		case Filter_IIR:
			MTCH_FILTER[0] = Filter_IIR;
			if ((strength > 0) && (strength <= 3))	MTCH_FILTER[1] = strength;
			else									MTCH_FILTER[1] = 1;		// Default IIR strength
			break;

		case Filter_Average:
			MTCH_FILTER[0] = Filter_Average;
			if ((strength > 0) && (strength <= 3))	MTCH_FILTER[1] = strength;
			else									MTCH_FILTER[1] = 1;		// Default Average strength
			break;

		default:
			return ERROR_VALUE;
	}

	// Send configuration parameters
	err = MTCH6102_Write(pCpnt->cfg.slave_inst, &MTCH_FILTER[0], baseline_filter ? MTCH__BASE_FILTER_TYPE : MTCH__FILTER_TYPE, 2);
	if (err)	{ return err; }

	// Send configuration request
	err = MTCH6102_Configuration_Request(pCpnt);

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/

