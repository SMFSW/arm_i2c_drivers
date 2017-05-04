/*!\file DRV2605L_ex.c
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief TI Haptic Driver extensions code
**/
/****************************************************************/
#include "DRV2605L.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_DRV2605L)
/****************************************************************/
// TODO: doxygen


FctERR DRV2605L_Vibe(bool on)
{
	uDRV_REG__GO go;
	go.Byte = 0;
	go.Bits.GO = on;
	return DRV2605L_Write(&go.Byte, DRV__GO, 1);
}


/****************************************************************/
/****************************************************************/


typedef union uDRV_CFG {
	uint8_t Bytes[6];
	struct {
		uDRV_REG__FEEDBACK_CONTROL	fdbck_ctl;
		uDRV_REG__CONTROL_1			ctl1;
		uDRV_REG__CONTROL_2			ctl2;
		uDRV_REG__CONTROL_3			ctl3;
		uDRV_REG__CONTROL_4			ctl4;
		uDRV_REG__CONTROL_5			ctl5;
	} Reg;
} uDRV_CFG;


static const uint16_t DRV2605L_time_table_ERM[4] = { 45, 75, 150, 225 };
static const uint16_t DRV2605L_time_table_LRA[16] = { 15, 25, 50 , 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 235, 260, 285 };


__STATIC_INLINE FctERR INLINE__ _read_cfg(uDRV_CFG * cfg) {
	return DRV2605L_Read(cfg->Bytes, DRV__FEEDBACK_CONTROL, sizeof(uDRV_CFG)); }

__STATIC_INLINE FctERR INLINE__ _write_cfg(uDRV_CFG * cfg) {
	return DRV2605L_Write(cfg->Bytes, DRV__FEEDBACK_CONTROL, sizeof(uDRV_CFG)); }


FctERR DRV2605L_GetBlankingTime(uint16_t * time)
{
	FctERR		err;
	uint8_t		idx;
	uDRV_CFG	cfg;

	err = _read_cfg(&cfg);
	if (err)	{ return err; }

	if (cfg.Reg.fdbck_ctl.Bits.N_ERM_LRA)
	{// LRA mode
		idx = cfg.Reg.ctl2.Bits.BLANKING_TIME | (cfg.Reg.ctl5.Bits.BLANKING_TIME << 2);
		*time = DRV2605L_time_table_LRA[idx];
	}
	else
	{// ERM mode
		idx = cfg.Reg.ctl2.Bits.BLANKING_TIME;
		*time = DRV2605L_time_table_ERM[idx];
	}

	return err;
}

FctERR DRV2605L_GetCurrentDissipationTime(uint16_t * time)
{
	FctERR		err;
	uint8_t		idx;
	uDRV_CFG	cfg;

	err = _read_cfg(&cfg);
	if (err)	{ return err; }

	if (cfg.Reg.fdbck_ctl.Bits.N_ERM_LRA)
	{// LRA mode
		idx = cfg.Reg.ctl2.Bits.IDISS_TIME | (cfg.Reg.ctl5.Bits.IDISS_TIME << 2);
		*time = DRV2605L_time_table_LRA[idx];
	}
	else
	{// ERM mode
		idx = cfg.Reg.ctl2.Bits.IDISS_TIME;
		*time = DRV2605L_time_table_ERM[idx];
	}

	return err;
}


FctERR DRV2605L_SetCurrentDissipationTime(uint16_t time)
{
	FctERR		err;
	int8_t		idx;
	uDRV_CFG	cfg;

	err = _read_cfg(&cfg);
	if (err)	{ return err; }

	if (cfg.Reg.fdbck_ctl.Bits.N_ERM_LRA)
	{// LRA mode
		for (idx = sizeof(DRV2605L_time_table_LRA) / sizeof(uint16_t) ; ; idx--)
		{ if (time <= DRV2605L_time_table_LRA[idx] || (idx == 0))	{ break; } }
	}
	else
	{// ERM mode
		for (idx = sizeof(DRV2605L_time_table_ERM) / sizeof(uint16_t) ; ; idx--)
		{ if (time <= DRV2605L_time_table_ERM[idx] || (idx == 0))	{ break; } }
	}

	cfg.Reg.ctl2.Bits.IDISS_TIME = idx & 0x02;
	cfg.Reg.ctl5.Bits.IDISS_TIME = (idx >> 2) & 0x02;

	return _write_cfg(&cfg);
}

FctERR DRV2605L_SetBlankingTime(uint16_t time)
{
	FctERR		err;
	int8_t		idx;
	uDRV_CFG	cfg;

	err = _read_cfg(&cfg);
	if (err)	{ return err; }

	if (cfg.Reg.fdbck_ctl.Bits.N_ERM_LRA)
	{// LRA mode
		for (idx = sizeof(DRV2605L_time_table_LRA) ; ; idx--)
		{ if (time <= DRV2605L_time_table_LRA[idx] || (idx == 0))	{ break; } }
	}
	else
	{// ERM mode
		for (idx = sizeof(DRV2605L_time_table_ERM) ; ; idx--)
		{ if (time <= DRV2605L_time_table_ERM[idx] || (idx == 0))	{ break; } }
	}

	cfg.Reg.ctl2.Bits.BLANKING_TIME = idx & 0x02;
	cfg.Reg.ctl5.Bits.BLANKING_TIME = (idx >> 2) & 0x02;

	return _write_cfg(&cfg);
}


/*!\brief get DRV2605L voltage (in V)
**/
FctERR DRV2605L_GetVoltage(uint16_t * vbat)
{
	FctERR	err;
	uint8_t Reg = 0;

	err = DRV2605L_Read(&Reg, DRV__VBAT_VOLTAGE_MONITOR, 1);
	*vbat = (uint16_t) ((Reg * 5.6f) / 255);
	return err;
}

/*!\brief get DRV2605L LRA Open Loop period (in us)
**/
FctERR DRV2605L_GetLRAOpenLoopPeriod(uint16_t * per)
{
	FctERR	err;
	uint8_t Reg = 0;

	err = DRV2605L_Read(&Reg, DRV__LRA_OPEN_LOOP_PERIOD, 1);
	*per = (uint16_t) (Reg * 98.46f);
	return err;
}

/*!\brief get DRV2605L resonance period (in us)
**/
FctERR DRV2605L_GetResonancePeriod(uint16_t * per)
{
	FctERR	err;
	uint8_t Reg = 0;

	err = DRV2605L_Read(&Reg, DRV__LRA_RESONNANCE_PERIOD, 1);
	*per = (uint16_t) (Reg * 98.46f);
	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
