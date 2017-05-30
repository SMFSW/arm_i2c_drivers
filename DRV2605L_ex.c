/*!\file DRV2605L_ex.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief DRV2605L Driver extensions code
** \details DRV2605L: 2 to 5.2V Haptic Driver for LRA and ERM With Effect Library and Smart-Loop Architecture
**/
/****************************************************************/
#include "DRV2605L.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_DRV2605L)
/****************************************************************/


extern DRV2605L_proc DRV2605L;


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


FctERR DRV2605L_Set_ActType(DRV2605L_act act)
{
	uDRV_REG__FEEDBACK_CONTROL	FB_CTL;
	FctERR						err;

	if (act > DRV__ACT_LRA)	{ return ERR_VALUE; }	// Unknown actuator type

	err = DRV2605L_Read(&FB_CTL.Byte, DRV__FEEDBACK_CONTROL, 1);
	if (err)	{ return err; }

	FB_CTL.Bits.N_ERM_LRA = act;
	err = DRV2605L_Write(&FB_CTL.Byte, DRV__FEEDBACK_CONTROL, 1);
	if (err)	{ return err; }

	DRV2605L.cfg.ERM_LRA = act;
	return err;
}


FctERR DRV2605L_Set_OperatingMode(DRV2605L_mode mode)
{
	uDRV_REG__MODE	MODE;
	FctERR			err;

	if (mode > DRV__MODE_AUTO_CALIBRATION)	{ return ERR_VALUE; }	// Unknown mode

	err = DRV2605L_Read(&MODE.Byte, DRV__MODE, 1);
	if (err)	{ return err; }

	MODE.Bits.MODE = mode;
	err = DRV2605L_Write(&MODE.Byte, DRV__MODE, 1);
	if (err)	{ return err; }

	DRV2605L.cfg.Mode = mode;
	return err;
}


FctERR DRV2605L_Set_Standby(bool stdby)
{
	uDRV_REG__MODE	MODE;
	FctERR			err;

	err = DRV2605L_Read(&MODE.Byte, DRV__MODE, 1);
	if (err)	{ return err; }

	MODE.Bits.STANDBY = stdby;
	return  DRV2605L_Write(&MODE.Byte, DRV__MODE, 1);
}


FctERR DRV2605L_Set_InputMode(DRV2605L_input input)
{
	uDRV_REG__CONTROL_3	CTL3;
	FctERR				err;

	if (input > DRV__IN_ANALOG)		{ return ERR_VALUE; }	// Unknown input type

	err = DRV2605L_Read(&CTL3.Byte, DRV__CONTROL_3, 1);
	if (err)	{ return err; }

	CTL3.Bits.N_PWM_ANALOG = input;
	err = DRV2605L_Write(&CTL3.Byte, DRV__CONTROL_3, 1);
	if (err)	{ return err; }

	DRV2605L.cfg.Input_Mode = input;
	return err;
}


FctERR DRV2605L_Set_LoopMode(DRV2605L_loop loop)
{
	uDRV_REG__CONTROL_3	CTL3;
	FctERR				err;

	if (loop > DRV__OPEN_LOOP)		{ return ERR_VALUE; }	// Unknown loop mode

	err = DRV2605L_Read(&CTL3.Byte, DRV__CONTROL_3, 1);
	if (err)	{ return err; }

	if (DRV2605L.cfg.ERM_LRA == DRV__ACT_ERM)	{ CTL3.Bits.ERM_OPEN_LOOP = loop; }
	else										{ CTL3.Bits.LRA_OPEN_LOOP = loop; }

	err = DRV2605L_Write(&CTL3.Byte, DRV__CONTROL_3, 1);
	if (err)	{ return err; }

	DRV2605L.cfg.Open_Loop = loop;
	return err;
}


FctERR DRV2605L_Set_Library(DRV2605L_lib lib)
{
	uDRV_REG__LIBRARY_SELECTION LIB;

	if (lib > DRV__LIB_TS2200_LIBRARY_F)	{ return ERR_VALUE; }	// Unknown library

	LIB.Byte = 0;
	LIB.Bits.LIBRARY_SEL = lib;
	return DRV2605L_Write(&LIB.Byte, DRV__LIBRARY_SELECTION, 1);
}


FctERR DRV2605L_Set_Waveform(uint16_t chan, DRV2605L_eff effect, bool wait)
{
	uDRV_REG__WAVEFORM_SEQUENCER WAVEFORM;

	if (chan > 7)						{ return ERR_VALUE; }	// Unknown channel
	if (effect > DRV__EFF_SMOOTH_HUM_5)	{ return ERR_VALUE; }	// Unknown effect

	WAVEFORM.Bits.WAIT = wait;
	WAVEFORM.Bits.WAV_FRM_SEQ = effect;
	return DRV2605L_Write(&WAVEFORM.Byte, DRV__WAVEFORM_SEQUENCER_1 + chan, 1);
}


FctERR DRV2605L_Set_RTPDataFormat(DRV2605L_rtp_format format)
{
	uDRV_REG__CONTROL_3	CTL3;
	FctERR				err;

	if (format > DRV__RTP_UNSIGNED)	{ return ERR_VALUE; }	// Unknown RTP data format

	err = DRV2605L_Read(&CTL3.Byte, DRV__CONTROL_3, 1);
	if (err)	{ return err; }

	CTL3.Bits.DATA_FORMAT_RTP = format;

	err = DRV2605L_Write(&CTL3.Byte, DRV__CONTROL_3, 1);
	if (err)	{ return err; }

	DRV2605L.cfg.RTP_Format = format;
	return err;
}


FctERR DRV2605L_Set_ATVPeakTime(DRV2605L_peak val)
{
	uDRV_REG__ATV_CONTROL	ATV;
	FctERR					err;

	if (val > DRV__PEAK_40MS)	{ return ERR_VALUE; }	// Unknown peak time

	err = DRV2605L_Read(&ATV.Byte, DRV__ATV_CONTROL, 1);
	if (err)	{ return err; }

	ATV.Bits.ATH_PEAK_TIME = val;
	return DRV2605L_Write(&ATV.Byte, DRV__ATV_CONTROL, 1);
}


FctERR DRV2605L_Set_ATVLowPassFilter(DRV2605L_filter val)
{
	uDRV_REG__ATV_CONTROL	ATV;
	FctERR					err;

	if (val > DRV__FILTER_200HZ)	{ return ERR_VALUE; }	// Unknown low pass filter frequency

	err = DRV2605L_Read(&ATV.Byte, DRV__ATV_CONTROL, 1);
	if (err)	{ return err; }

	ATV.Bits.ATH_FILTER = val;
	return DRV2605L_Write(&ATV.Byte, DRV__ATV_CONTROL, 1);
}


FctERR DRV2605L_Set_ATVInput_Volt(float volt, bool max)
{
	float	tmp = (volt * 255.0f) / 1.8f;
	uint8_t	VAL;

	if (tmp > 255.0f)	{ return ERR_VALUE; }	// voltage desired is too high

	VAL = (uint8_t) tmp;
	return DRV2605L_Set_ATVInputLevel_Raw(VAL, max);
}


FctERR DRV2605L_Set_ATVDrive_Percent(uint16_t perc, bool max)
{
	uint8_t	VAL;

	if (perc > 100)	{ return ERR_VALUE; }	// percent desired is too high

	VAL = (uint8_t) ((perc * 255.0f) / 100.0f);
	return DRV2605L_Set_ATVOutputDrive_Raw(VAL, max);
}


FctERR DRV2605L_Set_BlankingTime(uint16_t time)
{
	FctERR		err;
	int8_t		idx;
	uDRV_CFG	CFG;

	err = _read_cfg(&CFG);
	if (err)	{ return err; }

	if (CFG.Reg.fdbck_ctl.Bits.N_ERM_LRA)
	{// LRA mode
		for (idx = sizeof(DRV2605L_time_table_LRA) ; ; idx--)
		{ if (time <= DRV2605L_time_table_LRA[idx] || (idx == 0))	{ break; } }
	}
	else
	{// ERM mode
		for (idx = sizeof(DRV2605L_time_table_ERM) ; ; idx--)
		{ if (time <= DRV2605L_time_table_ERM[idx] || (idx == 0))	{ break; } }
	}

	CFG.Reg.ctl2.Bits.BLANKING_TIME = idx & 0x02;
	CFG.Reg.ctl5.Bits.BLANKING_TIME = (idx >> 2) & 0x02;

	return _write_cfg(&CFG);
}


FctERR DRV2605L_Set_CurrentDissipationTime(uint16_t time)
{
	FctERR		err;
	int8_t		idx;
	uDRV_CFG	CFG;

	err = _read_cfg(&CFG);
	if (err)	{ return err; }

	if (CFG.Reg.fdbck_ctl.Bits.N_ERM_LRA)
	{// LRA mode
		for (idx = sizeof(DRV2605L_time_table_LRA) / sizeof(uint16_t) ; ; idx--)
		{ if (time <= DRV2605L_time_table_LRA[idx] || (idx == 0))	{ break; } }
	}
	else
	{// ERM mode
		for (idx = sizeof(DRV2605L_time_table_ERM) / sizeof(uint16_t) ; ; idx--)
		{ if (time <= DRV2605L_time_table_ERM[idx] || (idx == 0))	{ break; } }
	}

	CFG.Reg.ctl2.Bits.IDISS_TIME = idx & 0x02;
	CFG.Reg.ctl5.Bits.IDISS_TIME = (idx >> 2) & 0x02;

	return _write_cfg(&CFG);
}


FctERR DRV2605L_Set_LRAOpenLoopPeriod_us(uint16_t per)
{
	uint8_t PER;

	if (per > 25107)	{ return ERR_VALUE; }	// Period is out of range

	PER = (uint8_t) (per / 98.46f);
	return DRV2605L_Write(&PER, DRV__LRA_OPEN_LOOP_PERIOD, 1);
}


/****************************************************************/
/****************************************************************/


FctERR DRV2605L_Get_BlankingTime(uint16_t * time)
{
	FctERR		err;
	uint8_t		idx;
	uDRV_CFG	CFG;

	err = _read_cfg(&CFG);
	if (err)	{ return err; }

	if (CFG.Reg.fdbck_ctl.Bits.N_ERM_LRA)
	{// LRA mode
		idx = CFG.Reg.ctl2.Bits.BLANKING_TIME | (CFG.Reg.ctl5.Bits.BLANKING_TIME << 2);
		*time = DRV2605L_time_table_LRA[idx];
	}
	else
	{// ERM mode
		idx = CFG.Reg.ctl2.Bits.BLANKING_TIME;
		*time = DRV2605L_time_table_ERM[idx];
	}

	return err;
}


FctERR DRV2605L_Get_ATVInput_Volt(float * volt, bool max)
{
	uint8_t	VAL;
	FctERR	err;

	err = DRV2605L_Read(&VAL, DRV__ATV_MIN_INPUT_LEVEL + max, 1);
	if (err)	{ return err; }

	*volt = (VAL * 1.8f) / 255.0f;
	return err;
}


FctERR DRV2605L_Get_ATVDrive_Percent(float * perc, bool max)
{
	uint8_t	VAL;
	FctERR	err;

	err = DRV2605L_Read(&VAL, DRV__ATV_MIN_OUTPUT_DRIVE + max, 1);
	if (err)	{ return err; }

	*perc = (VAL / 255.0f) * 100.0;
	return err;
}


FctERR DRV2605L_Get_CurrentDissipationTime(uint16_t * time)
{
	uDRV_CFG	CFG;
	FctERR		err;
	uint8_t		idx;

	err = _read_cfg(&CFG);
	if (err)	{ return err; }

	if (CFG.Reg.fdbck_ctl.Bits.N_ERM_LRA)
	{// LRA mode
		idx = CFG.Reg.ctl2.Bits.IDISS_TIME | (CFG.Reg.ctl5.Bits.IDISS_TIME << 2);
		*time = DRV2605L_time_table_LRA[idx];
	}
	else
	{// ERM mode
		idx = CFG.Reg.ctl2.Bits.IDISS_TIME;
		*time = DRV2605L_time_table_ERM[idx];
	}

	return err;
}


/*!\brief get DRV2605L voltage (in V)
** \param[in,out] vbat - pointer to voltage variable
**/
FctERR DRV2605L_Get_Voltage(uint16_t * vbat)
{
	uint8_t VBAT = 0;
	FctERR	err;

	err = DRV2605L_Read(&VBAT, DRV__VBAT_VOLTAGE_MONITOR, 1);
	*vbat = (uint16_t) ((VBAT * 5.6f) / 255);
	return err;
}


/*!\brief get DRV2605L LRA Open Loop period (in us)
** \param[in,out] per - pointer to open loop period variable
**/
FctERR DRV2605L_Get_LRAOpenLoopPeriod(uint16_t * per)
{
	uint8_t PER = 0;
	FctERR	err;

	err = DRV2605L_Read(&PER, DRV__LRA_OPEN_LOOP_PERIOD, 1);
	*per = (uint16_t) (PER * 98.46f);
	return err;
}


/*!\brief get DRV2605L resonance period (in us)
** \param[in,out] per - pointer to period variable
**/
FctERR DRV2605L_Get_ResonancePeriod(uint16_t * per)
{
	uint8_t PER = 0;
	FctERR	err;

	err = DRV2605L_Read(&PER, DRV__LRA_RESONANCE_PERIOD, 1);
	*per = (uint16_t) (PER * 98.46f);
	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
