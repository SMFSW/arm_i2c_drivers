/*!\file DRV2605L_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief DRV2605L Driver extensions
** \details DRV2605L: 2 to 5.2V Haptic Driver for LRA and ERM With Effect Library and Smart-Loop Architecture
**/
/****************************************************************/
#include "DRV2605L.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_DRV2605L)
/****************************************************************/


/*!\union uDRV_CFG
** \brief DRV2605L Config registers union
**/
typedef union {
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


static const uint16_t DRV2605L_time_table_ERM[4] = { 45U, 75U, 150U, 225U };
//! Time table for ERM actuators

static const uint16_t DRV2605L_time_table_LRA[16] = { 15U, 25U, 50U, 75U, 90U, 105U, 120U, 135U, 150U, 165U, 180U, 195U, 210U, 235U, 260U, 285U };
//! Time table for LRA actuators


/*!\brief Read configuration block of DRV2605L peripheral
** \param[in,out] cfg - pointer to DRV2605L configuration in RAM to write to
** \return FctERR - error code
**/
__STATIC_INLINE FctERR NONNULL_INLINE__ DRV2605_read_cfg(uDRV_CFG * const cfg) {
	return DRV2605L_Read(cfg->Bytes, DRV__FEEDBACK_CONTROL, sizeof(uDRV_CFG)); }

/*!\brief Write configuration block of DRV2605L peripheral
** \param[in,out] cfg - pointer to DRV2605L configuration in RAM to read from
** \return FctERR - error code
**/
__STATIC_INLINE FctERR NONNULL_INLINE__ DRV2605_write_cfg(const uDRV_CFG * const cfg) {
	return DRV2605L_Write(cfg->Bytes, DRV__FEEDBACK_CONTROL, sizeof(uDRV_CFG)); }


FctERR DRV2605L_Set_ActType(const DRV2605L_act act)
{
	uDRV_REG__FEEDBACK_CONTROL	FB_CTL;
	FctERR						err = ERROR_OK;

	if (act > DRV__ACT_LRA)	{ err = ERROR_VALUE; }	// Unknown actuator type
	if (err != ERROR_OK)	{ goto ret; }

	err = DRV2605L_Read(&FB_CTL.Byte, DRV__FEEDBACK_CONTROL, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	FB_CTL.Bits.N_ERM_LRA = act;

	err = DRV2605L_Write(&FB_CTL.Byte, DRV__FEEDBACK_CONTROL, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	DRV2605L.cfg.ERM_LRA = act;

	ret:
	return err;
}


FctERR DRV2605L_Set_OperatingMode(const DRV2605L_mode mode)
{
	uDRV_REG__MODE	MODE;
	FctERR			err = ERROR_OK;

	if (mode > DRV__MODE_AUTO_CALIBRATION)	{ err = ERROR_VALUE; }	// Unknown mode
	if (err != ERROR_OK)					{ goto ret; }

	err = DRV2605L_Read(&MODE.Byte, DRV__MODE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	MODE.Bits.MODE = mode;

	err = DRV2605L_Write(&MODE.Byte, DRV__MODE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	DRV2605L.cfg.Mode = mode;

	ret:
	return err;
}


FctERR DRV2605L_Set_Standby(const bool stdby)
{
	uDRV_REG__MODE	MODE;
	FctERR			err;

	err = DRV2605L_Read(&MODE.Byte, DRV__MODE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	MODE.Bits.STANDBY = stdby;

	err = DRV2605L_Write(&MODE.Byte, DRV__MODE, 1U);

	ret:
	return err;
}


FctERR DRV2605L_Set_InputMode(const DRV2605L_input input)
{
	uDRV_REG__CONTROL_3	CTL3;
	FctERR				err = ERROR_OK;

	if (input > DRV__IN_ANALOG)	{ err = ERROR_VALUE; }	// Unknown input type
	if (err != ERROR_OK)		{ goto ret; }

	err = DRV2605L_Read(&CTL3.Byte, DRV__CONTROL_3, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	CTL3.Bits.N_PWM_ANALOG = input;
	err = DRV2605L_Write(&CTL3.Byte, DRV__CONTROL_3, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	DRV2605L.cfg.Input_Mode = input;

	ret:
	return err;
}


FctERR DRV2605L_Set_LoopMode(const DRV2605L_loop loop)
{
	uDRV_REG__CONTROL_3	CTL3;
	FctERR				err = ERROR_OK;

	if (loop > DRV__OPEN_LOOP)	{ err = ERROR_VALUE; }	// Unknown loop mode
	if (err != ERROR_OK)		{ goto ret; }

	err = DRV2605L_Read(&CTL3.Byte, DRV__CONTROL_3, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	if (DRV2605L.cfg.ERM_LRA == DRV__ACT_ERM)	{ CTL3.Bits.ERM_OPEN_LOOP = loop; }
	else										{ CTL3.Bits.LRA_OPEN_LOOP = loop; }

	err = DRV2605L_Write(&CTL3.Byte, DRV__CONTROL_3, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	DRV2605L.cfg.Open_Loop = loop;

	ret:
	return err;
}


FctERR DRV2605L_Set_Library(const DRV2605L_lib lib)
{
	uDRV_REG__LIBRARY_SELECTION LIB = { 0 };
	FctERR						err = ERROR_OK;

	if (lib > DRV__LIB_TS2200_LIBRARY_F)	{ err = ERROR_VALUE; }	// Unknown library
	if (err != ERROR_OK)					{ goto ret; }

	LIB.Bits.LIBRARY_SEL = lib;
	err = DRV2605L_Write(&LIB.Byte, DRV__LIBRARY_SELECTION, 1U);

	ret:
	return err;
}


FctERR DRV2605L_Set_Waveform(const uint16_t chan, const DRV2605L_eff effect, const bool wait)
{
	uDRV_REG__WAVEFORM_SEQUENCER	WAVEFORM;
	FctERR							err = ERROR_OK;

	if (chan > 7U)						{ err = ERROR_VALUE; }	// Unknown channel
	if (effect > DRV__EFF_SMOOTH_HUM_5)	{ err = ERROR_VALUE; }	// Unknown effect
	if (err != ERROR_OK)				{ goto ret; }

	WAVEFORM.Bits.WAIT = wait;
	WAVEFORM.Bits.WAV_FRM_SEQ = effect;

	err = DRV2605L_Write(&WAVEFORM.Byte, DRV__WAVEFORM_SEQUENCER_1 + chan, 1U);

	ret:
	return err;
}


FctERR DRV2605L_Set_RTPDataFormat(const DRV2605L_rtp_format format)
{
	uDRV_REG__CONTROL_3	CTL3;
	FctERR				err = ERROR_OK;

	if (format > DRV__RTP_UNSIGNED)	{ err = ERROR_VALUE; }	// Unknown RTP data format
	if (err != ERROR_OK)			{ goto ret; }

	err = DRV2605L_Read(&CTL3.Byte, DRV__CONTROL_3, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	CTL3.Bits.DATA_FORMAT_RTP = format;

	err = DRV2605L_Write(&CTL3.Byte, DRV__CONTROL_3, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	DRV2605L.cfg.RTP_Format = format;

	ret:
	return err;
}


FctERR DRV2605L_Set_ATVPeakTime(const DRV2605L_peak peak)
{
	uDRV_REG__ATV_CONTROL	ATV;
	FctERR					err = ERROR_OK;

	if (peak > DRV__PEAK_40MS)	{ err = ERROR_VALUE; }	// Unknown peak time
	if (err != ERROR_OK)		{ goto ret; }

	err = DRV2605L_Read(&ATV.Byte, DRV__ATV_CONTROL, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	ATV.Bits.ATH_PEAK_TIME = peak;
	err = DRV2605L_Write(&ATV.Byte, DRV__ATV_CONTROL, 1U);

	ret:
	return err;
}


FctERR DRV2605L_Set_ATVLowPassFilter(const DRV2605L_filter filt)
{
	uDRV_REG__ATV_CONTROL	ATV;
	FctERR					err = ERROR_OK;

	if (filt > DRV__FILTER_200HZ)	{ err = ERROR_VALUE; }	// Unknown low pass filter frequency
	if (err != ERROR_OK)			{ goto ret; }

	err = DRV2605L_Read(&ATV.Byte, DRV__ATV_CONTROL, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	ATV.Bits.ATH_FILTER = filt;
	err = DRV2605L_Write(&ATV.Byte, DRV__ATV_CONTROL, 1U);

	ret:
	return err;
}


FctERR DRV2605L_Set_ATVInput_Volt(const float volt, const bool maxi)
{
	const float	tmp = (volt * 255.0f) / 1.8f;
	FctERR		err = ERROR_OK;

	if (tmp > 255.0f)	{ err = ERROR_VALUE; }	// voltage desired is too high
	else
	{
		const uint8_t VAL = (uint8_t) tmp;
		err = DRV2605L_Set_ATVInputLevel_Raw(VAL, maxi);
	}

	return err;
}


FctERR DRV2605L_Set_ATVDrive_Percent(const uint16_t perc, const bool maxi)
{
	FctERR err = ERROR_OK;

	if (perc > 100U)	{ err = ERROR_VALUE; }	// percent desired is too high
	else
	{
		const uint8_t VAL = (uint8_t) ((perc * 255.0f) / 100.0f);
		err = DRV2605L_Set_ATVOutputDrive_Raw(VAL, maxi);
	}

	return err;
}


FctERR DRV2605L_Set_BlankingTime(const uint16_t time)
{
	FctERR		err;
	uintCPU_t	idx;
	uDRV_CFG	CFG;

	err = DRV2605_read_cfg(&CFG);
	if (err != ERROR_OK)	{ goto ret; }

	if (CFG.Reg.fdbck_ctl.Bits.N_ERM_LRA)
	{// LRA mode
		for (idx = SZ_ARRAY(DRV2605L_time_table_LRA) - 1U ; idx ; idx--)
		{
			if (time <= DRV2605L_time_table_LRA[idx])	{ break; }
		}
	}
	else
	{// ERM mode
		for (idx = SZ_ARRAY(DRV2605L_time_table_ERM) - 1U ; idx ; idx--)
		{
			if (time <= DRV2605L_time_table_ERM[idx])	{ break; }
		}
	}

	CFG.Reg.ctl2.Bits.BLANKING_TIME = idx & 0x02U;
	CFG.Reg.ctl5.Bits.BLANKING_TIME = RSHIFT(idx, 2U) & 0x02U;

	err = DRV2605_write_cfg(&CFG);

	ret:
	return err;
}


FctERR DRV2605L_Set_CurrentDissipationTime(const uint16_t time)
{
	FctERR		err;
	uintCPU_t	idx;
	uDRV_CFG	CFG;

	err = DRV2605_read_cfg(&CFG);
	if (err != ERROR_OK)	{ goto ret; }

	if (CFG.Reg.fdbck_ctl.Bits.N_ERM_LRA)
	{// LRA mode
		for (idx = SZ_ARRAY(DRV2605L_time_table_LRA) - 1U ; idx ; idx--)
		{
			if (time <= DRV2605L_time_table_LRA[idx])	{ break; }
		}
	}
	else
	{// ERM mode
		for (idx = SZ_ARRAY(DRV2605L_time_table_ERM) - 1U ; idx ; idx--)
		{
			if (time <= DRV2605L_time_table_ERM[idx])	{ break; }
		}
	}

	CFG.Reg.ctl2.Bits.IDISS_TIME = idx & 0x02U;
	CFG.Reg.ctl5.Bits.IDISS_TIME = RSHIFT(idx, 2U) & 0x02U;

	err = DRV2605_write_cfg(&CFG);

	ret:
	return err;
}


FctERR DRV2605L_Set_LRAOpenLoopPeriod_us(const uint16_t per)
{
	FctERR err = ERROR_OK;

	if (per > 25107U)	{ err = ERROR_VALUE; }	// Period is out of range
	else
	{
		const uint8_t PER = (uint8_t) (per / 98.46f);
		err = DRV2605L_Write(&PER, DRV__LRA_OPEN_LOOP_PERIOD, 1U);
	}

	return err;
}


/****************************************************************/


FctERR NONNULL__ DRV2605L_Get_BlankingTime(uint16_t * const time)
{
	FctERR		err;
	uint8_t		idx;
	uDRV_CFG	CFG;

	err = DRV2605_read_cfg(&CFG);
	if (err != ERROR_OK)	{ goto ret; }

	if (CFG.Reg.fdbck_ctl.Bits.N_ERM_LRA)
	{// LRA mode
		idx = CFG.Reg.ctl2.Bits.BLANKING_TIME | LSHIFT(CFG.Reg.ctl5.Bits.BLANKING_TIME, 2U);
		*time = DRV2605L_time_table_LRA[idx];
	}
	else
	{// ERM mode
		idx = CFG.Reg.ctl2.Bits.BLANKING_TIME;
		*time = DRV2605L_time_table_ERM[idx];
	}

	ret:
	return err;
}


FctERR NONNULL__ DRV2605L_Get_ATVInput_Volt(float * const volt, const bool maxi)
{
	uint8_t	VAL;
	FctERR	err;

	err = DRV2605L_Read(&VAL, DRV__ATV_MIN_INPUT_LEVEL + maxi, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	*volt = (VAL * 1.8f) / 255.0f;

	ret:
	return err;
}


FctERR NONNULL__ DRV2605L_Get_ATVDrive_Percent(float * const perc, const bool maxi)
{
	uint8_t	VAL;
	FctERR	err;

	err = DRV2605L_Read(&VAL, DRV__ATV_MIN_OUTPUT_DRIVE + maxi, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	*perc = (VAL / 255.0f) * 100.0;

	ret:
	return err;
}


FctERR NONNULL__ DRV2605L_Get_CurrentDissipationTime(uint16_t * const time)
{
	uDRV_CFG	CFG;
	FctERR		err;
	uint8_t		idx;

	err = DRV2605_read_cfg(&CFG);
	if (err != ERROR_OK)	{ goto ret; }

	if (CFG.Reg.fdbck_ctl.Bits.N_ERM_LRA)
	{// LRA mode
		idx = CFG.Reg.ctl2.Bits.IDISS_TIME | LSHIFT(CFG.Reg.ctl5.Bits.IDISS_TIME, 2U);
		*time = DRV2605L_time_table_LRA[idx];
	}
	else
	{// ERM mode
		idx = CFG.Reg.ctl2.Bits.IDISS_TIME;
		*time = DRV2605L_time_table_ERM[idx];
	}

	ret:
	return err;
}


FctERR NONNULL__ DRV2605L_Get_Voltage(uint16_t * const vbat)
{
	uint8_t VBAT;

	FctERR err = DRV2605L_Read(&VBAT, DRV__VBAT_VOLTAGE_MONITOR, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	*vbat = (uint16_t) ((VBAT * 5.6f) / 255U);

	ret:
	return err;
}


FctERR NONNULL__ DRV2605L_Get_LRAOpenLoopPeriod(uint16_t * const per)
{
	uint8_t PER;

	FctERR err = DRV2605L_Read(&PER, DRV__LRA_OPEN_LOOP_PERIOD, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	*per = (uint16_t) (PER * 98.46f);

	ret:
	return err;
}


FctERR NONNULL__ DRV2605L_Get_ResonancePeriod(uint16_t * const per)
{
	uint8_t PER;

	FctERR err = DRV2605L_Read(&PER, DRV__LRA_RESONANCE_PERIOD, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	*per = (uint16_t) (PER * 98.46f);

	ret:
	return err;
}


FctERR NONNULL__ DRV2605L_Get_ChipID(uint8_t * const id)
{
	uint8_t	ID;

	FctERR err = DRV2605L_Read(&ID, DRV__STATUS, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	*id = ID & 0xE0U;	// Keep only 3 msb

	ret:
	return err;
}


/****************************************************************/


__WEAK void NONNULL__ DRV2605L_EN_GPIO_Init(GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&DRV2605L.cfg.EN_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK void DRV2605L_EN_GPIO_Set(const bool state) {
	I2C_peripheral_GPIO_set(&DRV2605L.cfg.EN_GPIO, state); }


__WEAK void NONNULL__ DRV2605L_TRIG_GPIO_Init(GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&DRV2605L.cfg.TRIG_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK void DRV2605L_TRIG_GPIO_Set(const bool state) {
	I2C_peripheral_GPIO_set(&DRV2605L.cfg.TRIG_GPIO, state); }


/****************************************************************/
#endif
#endif
/****************************************************************/
