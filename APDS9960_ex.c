/*!\file APDS9960_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief APDS9960 Driver extensions
** \details APDS9960: Digital Proximity, Ambient Light, RGB and Gesture Sensor
**/
/****************************************************************/
#include "APDS9960.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_APDS9960)
/****************************************************************/


FctERR NONNULL__ APDS9960_Set_PON(APDS9960_t * const pCpnt, const bool en)
{
	uAPDS9960_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9960__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.PON = en;
	return APDS9960_Write_En(pCpnt, EN.Byte);
}


FctERR NONNULL__ APDS9960_Set_AEN(APDS9960_t * const pCpnt, const bool en)
{
	uAPDS9960_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9960__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.AEN = en;
	return APDS9960_Write_En(pCpnt, EN.Byte);
}


FctERR NONNULL__ APDS9960_Set_PEN(APDS9960_t * const pCpnt, const bool en)
{
	uAPDS9960_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9960__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.PEN = en;
	return APDS9960_Write_En(pCpnt, EN.Byte);
}


FctERR NONNULL__ APDS9960_Set_AIEN(APDS9960_t * const pCpnt, const bool en)
{
	uAPDS9960_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9960__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.AIEN = en;
	err = APDS9960_Write_En(pCpnt, EN.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.PIEN = en;
	return err;
}


FctERR NONNULL__ APDS9960_Set_PIEN(APDS9960_t * const pCpnt, const bool en)
{
	uAPDS9960_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9960__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.PIEN = en;
	err = APDS9960_Write_En(pCpnt, EN.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.PIEN = en;
	return err;
}


FctERR NONNULL__ APDS9960_Set_WEN(APDS9960_t * const pCpnt, const bool en)
{
	uAPDS9960_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9960__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.WEN = en;
	err = APDS9960_Write_En(pCpnt, EN.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.WEN = en;
	return err;
}


FctERR NONNULL__ APDS9960_Set_GEN(APDS9960_t * const pCpnt, const bool en)
{
	uAPDS9960_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9960__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.GEN = en;
	return APDS9960_Write_En(pCpnt, EN.Byte);
}


FctERR NONNULL__ APDS9960_Set_ALS_Pesistence(APDS9960_t * const pCpnt, const APDS9960_als_it_persist persist)
{
	uAPDS9960_REG__PERSIST	PERS;
	FctERR					err;

	if (persist > APDS9960__PERSIST_60_ALS_PERIODS)		{ return ERROR_VALUE; }	// Unknown persistence

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &PERS.Byte, APDS9960__PERS, 1);
	if (err)	{ return err; }

	PERS.Bits.APERS = persist;
	err = APDS9960_Write(pCpnt->cfg.slave_inst, &PERS.Byte, APDS9960__PERS, 1);
	if (err)	{ return err; }

	return err;
}


FctERR NONNULL__ APDS9960_Set_Prox_Pesistence(APDS9960_t * const pCpnt, const APDS9960_prox_it_persist persist)
{
	uAPDS9960_REG__PERSIST	PERS;
	FctERR					err;

	if (persist > APDS9960__PERSIST_15_PROX_PERIODS)	{ return ERROR_VALUE; }	// Unknown persistence

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &PERS.Byte, APDS9960__PERS, 1);
	if (err)	{ return err; }

	PERS.Bits.PPERS = persist;
	err = APDS9960_Write(pCpnt->cfg.slave_inst, &PERS.Byte, APDS9960__PERS, 1);
	if (err)	{ return err; }

	return err;
}


FctERR NONNULL__ APDS9960_Set_Prox_Pulse_Length(APDS9960_t * const pCpnt, const APDS9960_pulse_length length)
{
	uAPDS9960_REG__PULSE	PPULSE;
	FctERR					err;

	if (length > APDS9960__PL_32US)	{ return ERROR_VALUE; }	// Unknown pulse length

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &PPULSE.Byte, APDS9960__PPULSE, 1);
	if (err)	{ return err; }

	PPULSE.Bits.PLEN = length;
	err = APDS9960_Write(pCpnt->cfg.slave_inst, &PPULSE.Byte, APDS9960__PPULSE, 1);
	if (err)	{ return err; }

	pCpnt->cfg.Prox_Length = length;

	return err;
}


FctERR NONNULL__ APDS9960_Set_Prox_Pulse_Count(APDS9960_t * const pCpnt, const uint8_t cnt)
{
	uAPDS9960_REG__PULSE	PPULSE;
	FctERR					err;

	if (cnt > 63)	{ return ERROR_VALUE; }	// Unknown pulse count

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &PPULSE.Byte, APDS9960__PPULSE, 1);
	if (err)	{ return err; }

	PPULSE.Bits.PULSE = cnt;
	err = APDS9960_Write(pCpnt->cfg.slave_inst, &PPULSE.Byte, APDS9960__PPULSE, 1);
	if (err)	{ return err; }

	pCpnt->cfg.Prox_Pulses = cnt;

	return err;
}


FctERR NONNULL__ APDS9960_Set_ALS_Gain(APDS9960_t * const pCpnt, const APDS9960_als_gain gain)
{
	uAPDS9960_REG__CONTROL	CTL;
	FctERR					err;

	if (gain > APDS9960__ALS_64X_GAIN)	{ return ERROR_VALUE; }	// Unknown gain

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &CTL.Byte, APDS9960__CONTROL, 1);
	if (err)	{ return err; }

	CTL.Bits.AGAIN = gain;
	err = APDS9960_Write_Ctrl(pCpnt, CTL.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.ADC_Gain = gain;

	return err;
}


FctERR NONNULL__ APDS9960_Set_Prox_Gain(APDS9960_t * const pCpnt, const APDS9960_prox_gain gain)
{
	uAPDS9960_REG__CONTROL	CTL;
	FctERR					err;

	if (gain > APDS9960__PROX_8X_GAIN)	{ return ERROR_VALUE; }	// Unknown gain

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &CTL.Byte, APDS9960__CONTROL, 1);
	if (err)	{ return err; }

	CTL.Bits.PGAIN = gain;
	err = APDS9960_Write_Ctrl(pCpnt, CTL.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.Prox_Gain = gain;

	return err;
}


FctERR NONNULL__ APDS9960_Set_Prox_Drive_Strength(APDS9960_t * const pCpnt, const APDS9960_drive_strength ldrive)
{
	uAPDS9960_REG__CONTROL	CTL;
	FctERR					err;

	if (ldrive > APDS9960__DS_12_5MA)	{ return ERROR_VALUE; }	// Unknown drive strength

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &CTL.Byte, APDS9960__CONTROL, 1);
	if (err)	{ return err; }

	CTL.Bits.LDRIVE = ldrive;
	err = APDS9960_Write_Ctrl(pCpnt, CTL.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.Prox_Strength = ldrive;

	return err;
}


FctERR NONNULL__ APDS9960_Set_ADC_Integration_Time(APDS9960_t * const pCpnt, const uint16_t integ)
{
	uint8_t	ATIME;
	FctERR	err;

	if ((integ < 3) || (integ > 712))	{ return ERROR_RANGE; }	// Integration time out of range

	// 2.78ms (0xFF) to 712ms (0x00)
	//ATIME = (uint8_t) ((integ - 2.78f) * (0x00 - 0xFF) / (712.0f - 2.78f) + 0xFF);
	ATIME = 256 - (uint8_t) ((float) integ / 2.78f);
	err = APDS9960_Write(pCpnt->cfg.slave_inst, &ATIME, APDS9960__ATIME, 1);
	if (err)	{ return err; }

	pCpnt->cfg.ADC_Integ = integ;
	pCpnt->cfg.ADC_Integ_reg = ATIME;

	return err;
}


FctERR NONNULL__ APDS9960_Set_Wait_Time(APDS9960_t * const pCpnt, const uint16_t wait)
{
	uAPDS9960_REG__CONFIG1	CFG1;
	uint8_t					WAIT;
	FctERR					err;

	if ((wait < 3) || (wait > 8540))	{ return ERROR_RANGE; }	// Wait time out of range

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &CFG1.Byte, APDS9960__CONFIG1, 1);
	if (err)	{ return err; }

	if (wait <= 712)
	{
		// 2.78ms (0xFF) to 712ms (0x00)
		//WAIT = (uint8_t) ((wait - 2.78f) * (0x00 - 0xFF) / (712.0f - 2.78f) + 0xFF);
		WAIT = 256 - (uint8_t) ((float) wait / 2.78f);
		CFG1.Bits.WLONG = 0;
	}
	else
	{
		// 30ms (0xFF) to 8.54s (0x00)
		//WAIT = (uint8_t) ((wait - 30.0f) * (0x00 - 0xFF) / (8540.0f - 30.0f) + 0xFF);
		WAIT = 256 - (uint8_t) (wait / 30.0f);
		CFG1.Bits.WLONG = 1;
	}

	err = APDS9960_Write_Cfg1(pCpnt, CFG1.Byte);
	if (err)	{ return err; }
	err = APDS9960_Write(pCpnt->cfg.slave_inst, &WAIT, APDS9960__WTIME, 1);
	if (err)	{ return err; }

	pCpnt->cfg.Wait = wait;

	return err;
}

/****************************************************************/


__WEAK FctERR NONNULL__ APDS9960_INT_GPIO_Init(APDS9960_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	return I2C_peripheral_GPIO_init(&pCpnt->cfg.INT_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK FctERR NONNULL__ APDS9960_INT_GPIO_Get(APDS9960_t * const pCpnt, bool * const pState) {
	return I2C_peripheral_GPIO_get(&pCpnt->cfg.INT_GPIO, pState); }


/****************************************************************/
#endif
#endif
/****************************************************************/
