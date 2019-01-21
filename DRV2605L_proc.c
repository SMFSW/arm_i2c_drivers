/*!\file DRV2605L_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
** \brief DRV2605L Driver procedures
** \details DRV2605L: 2 to 5.2V Haptic Driver for LRA and ERM With Effect Library and Smart-Loop Architecture
**/
/****************************************************************/
#include "DRV2605L.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_DRV2605L)
/****************************************************************/


DRV2605L_t DRV2605L = { { &DRV2605_hal, DRV__MODE_INTERNAL_TRIGGER, DRV__ACT_ERM, DRV__OPEN_LOOP, DRV__IN_PWM, DRV__RTP_SIGNED, 0 } };


/****************************************************************/


__WEAK FctERR DRV2605L_Init_Sequence(void)
{
	uDRV_REG__CONTROL_1	CTL1;
	uint8_t				EFFECT[3] = { DRV__EFF_TRANS_RAMP_DWN_LONG_SMOOTH_1, 0, 0 };
	FctERR				err = ERROR_OK;

	err = DRV2605L_Set_Standby(false);
	if (err)										{ return err; }

	err = DRV2605L_Get_ChipID(&DRV2605L.cfg.Id);
	if (err)										{ return err; }
	if (	(DRV2605L.cfg.Id != DRV2604_CHIP_ID)
		&&	(DRV2605L.cfg.Id != DRV2604L_CHIP_ID)
		&&	(DRV2605L.cfg.Id != DRV2605_CHIP_ID)
		&&	(DRV2605L.cfg.Id != DRV2605L_CHIP_ID))	{ return ERROR_COMMON; }	// Unknown device

	// TODO: write library selection reg if using lib
	err = DRV2605L_Write(EFFECT, DRV__WAVEFORM_SEQUENCER_1, 2);
	if (err)										{ return err; }

	// Write no Overdrive time offset & Sustain Pos/Neg
	err = DRV2605L_Set_OverdriveTimeOffset(0);
	if (err)										{ return err; }
	err = DRV2605L_Set_SustainTimeOffset(0, false);
	if (err)										{ return err; }
	err = DRV2605L_Set_SustainTimeOffset(0, true);
	if (err)										{ return err; }
	err = DRV2605L_Set_BrakeTimeOffset(0);
	if (err)										{ return err; }

	err = DRV2605L_Set_ATVInputLevel_Raw(0x64, true);
	if (err)										{ return err; }

	CTL1.Byte = 0;
	CTL1.Bits.STARTUP_BOOST = 1;
	CTL1.Bits.DRIVE_TIME = 20;

	err = DRV2605L_Write(&CTL1.Byte, DRV__CONTROL_1, 1);
	if (err)	{ return err; }

	// TODO: write CTL1,2,3 for operating mode selection
	err = DRV2605L_Set_ActType(DRV2605L.cfg.ERM_LRA);
	if (err)										{ return err; }
	err = DRV2605L_Set_LoopMode(DRV2605L.cfg.Open_Loop);
	if (err)										{ return err; }

	//MODE.Bits.STANDBY = 1;
	return DRV2605L_Set_OperatingMode(DRV2605L.cfg.Mode);
}


/****************************************************************/


FctERR DRV2605L_Auto_Calib(void)
{
	uDRV_REG__STATUS			ST;
	uDRV_REG__MODE				MODE;
	uDRV_REG__FEEDBACK_CONTROL	FB_CTL;
	uDRV_REG__CONTROL_1			CTL1;
	uDRV_REG__CONTROL_2			CTL2;
	uDRV_REG__CONTROL_4			CTL4;
	uint8_t						RATED_VOLTAGE, OD_CLAMP;
	FctERR						err = ERROR_OK;

	MODE.Byte = 0;

	err = DRV2605L_Write(&MODE.Byte, DRV__MODE, 1);
	if (err)	{ return err; }

	err = DRV2605L_Read(&FB_CTL.Byte, DRV__FEEDBACK_CONTROL, 1);
	if (err)	{ return err; }
	err = DRV2605L_Read(&CTL1.Byte, DRV__CONTROL_1, 1);
	if (err)	{ return err; }
	err = DRV2605L_Read(&CTL2.Byte, DRV__CONTROL_2, 1);
	if (err)	{ return err; }
	err = DRV2605L_Read(&CTL4.Byte, DRV__CONTROL_4, 1);
	if (err)	{ return err; }

	FB_CTL.Bits.N_ERM_LRA = DRV2605L.cfg.ERM_LRA;
	FB_CTL.Bits.FB_BRAKE_FACTOR = 2;
	FB_CTL.Bits.LOOP_GAIN = 2;

	CTL1.Bits.DRIVE_TIME = 20;

	CTL2.Bits.SAMPLE_TIME = 3;
	CTL2.Bits.BLANKING_TIME = 1;
	CTL2.Bits.IDISS_TIME = 1;

	CTL4.Bits.ZC_DET_TIME = 0;
	CTL4.Bits.AUTO_CAL_TIME = 3;

	RATED_VOLTAGE = 100;
	OD_CLAMP = 100;

	// call auto-calibration procedure
	MODE.Bits.MODE = DRV__MODE_AUTO_CALIBRATION;
	err = DRV2605L_Write(&MODE.Byte, DRV__MODE, 1);
	if (err)	{ return err; }


	err = DRV2605L_Write(&FB_CTL.Byte, DRV__FEEDBACK_CONTROL, 1);
	if (err)	{ return err; }
	err = DRV2605L_Write(&RATED_VOLTAGE, DRV__RATED_VOLTAGE, 1);
	if (err)	{ return err; }
	err = DRV2605L_Write(&OD_CLAMP, DRV__OVERDRIVE_CLAMP_VOLTAGE, 1);
	if (err)	{ return err; }
	err = DRV2605L_Write(&CTL1.Byte, DRV__CONTROL_1, 1);
	if (err)	{ return err; }
	err = DRV2605L_Write(&CTL2.Byte, DRV__CONTROL_2, 1);
	if (err)	{ return err; }
	err = DRV2605L_Write(&CTL4.Byte, DRV__CONTROL_4, 1);
	if (err)	{ return err; }


	err = DRV2605L_Vibe(DRV__START);
	if (err)	{ return err; }

	err = DRV2605L_Read(&ST.Byte, DRV__STATUS, 1);
	if (err)	{ return err; }

	MODE.Bits.STANDBY = 1;
	MODE.Bits.MODE = DRV__MODE_INTERNAL_TRIGGER;

	return DRV2605L_Set_OperatingMode(DRV__MODE_INTERNAL_TRIGGER);
}


/****************************************************************/
#endif
#endif
/****************************************************************/
