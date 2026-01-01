/*!\file APDS9960_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
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

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9960__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.PON = en;

	err = APDS9960_Write_En(pCpnt, EN.Byte);

	ret:
	return err;
}


FctERR NONNULL__ APDS9960_Set_AEN(APDS9960_t * const pCpnt, const bool en)
{
	uAPDS9960_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9960__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.AEN = en;

	err = APDS9960_Write_En(pCpnt, EN.Byte);

	ret:
	return err;
}


FctERR NONNULL__ APDS9960_Set_PEN(APDS9960_t * const pCpnt, const bool en)
{
	uAPDS9960_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9960__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.PEN = en;

	err = APDS9960_Write_En(pCpnt, EN.Byte);

	ret:
	return err;
}


FctERR NONNULL__ APDS9960_Set_AIEN(APDS9960_t * const pCpnt, const bool en)
{
	uAPDS9960_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9960__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.AIEN = en;

	err = APDS9960_Write_En(pCpnt, EN.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.PIEN = en;

	ret:
	return err;
}


FctERR NONNULL__ APDS9960_Set_PIEN(APDS9960_t * const pCpnt, const bool en)
{
	uAPDS9960_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9960__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.PIEN = en;

	err = APDS9960_Write_En(pCpnt, EN.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.PIEN = en;

	ret:
	return err;
}


FctERR NONNULL__ APDS9960_Set_WEN(APDS9960_t * const pCpnt, const bool en)
{
	uAPDS9960_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9960__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.WEN = en;

	err = APDS9960_Write_En(pCpnt, EN.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.WEN = en;

	ret:
	return err;
}


FctERR NONNULL__ APDS9960_Set_GEN(APDS9960_t * const pCpnt, const bool en)
{
	uAPDS9960_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9960__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.GEN = en;

	err = APDS9960_Write_En(pCpnt, EN.Byte);

	ret:
	return err;
}


FctERR NONNULL__ APDS9960_Set_ALS_Pesistence(APDS9960_t * const pCpnt, const APDS9960_als_it_persist persist)
{
	uAPDS9960_REG__PERSIST	PERS;
	FctERR					err = ERROR_OK;

	if (persist > APDS9960__PERSIST_60_ALS_PERIODS)	{ err = ERROR_VALUE; }	// Unknown persistence
	if (err != ERROR_OK)							{ goto ret; }

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &PERS.Byte, APDS9960__PERS, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	PERS.Bits.APERS = persist;

	err = APDS9960_Write(pCpnt->cfg.slave_inst, &PERS.Byte, APDS9960__PERS, 1U);

	ret:
	return err;
}


FctERR NONNULL__ APDS9960_Set_Prox_Pesistence(APDS9960_t * const pCpnt, const APDS9960_prox_it_persist persist)
{
	uAPDS9960_REG__PERSIST	PERS;
	FctERR					err = ERROR_OK;

	if (persist > APDS9960__PERSIST_15_PROX_PERIODS)	{ err = ERROR_VALUE; }	// Unknown persistence
	if (err != ERROR_OK)								{ goto ret; }

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &PERS.Byte, APDS9960__PERS, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	PERS.Bits.PPERS = persist;

	err = APDS9960_Write(pCpnt->cfg.slave_inst, &PERS.Byte, APDS9960__PERS, 1U);

	ret:
	return err;
}


FctERR NONNULL__ APDS9960_Set_Prox_Pulse_Length(APDS9960_t * const pCpnt, const APDS9960_pulse_length length)
{
	uAPDS9960_REG__PULSE	PPULSE;
	FctERR					err = ERROR_OK;

	if (length > APDS9960__PL_32US)	{ err = ERROR_VALUE; }	// Unknown pulse length
	if (err != ERROR_OK)			{ goto ret; }

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &PPULSE.Byte, APDS9960__PPULSE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	PPULSE.Bits.PLEN = length;

	err = APDS9960_Write(pCpnt->cfg.slave_inst, &PPULSE.Byte, APDS9960__PPULSE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.Prox_Length = length;

	ret:
	return err;
}


FctERR NONNULL__ APDS9960_Set_Prox_Pulse_Count(APDS9960_t * const pCpnt, const uint8_t cnt)
{
	uAPDS9960_REG__PULSE	PPULSE;
	FctERR					err = ERROR_OK;

	if (cnt > 63U)			{ err = ERROR_VALUE; }	// Unknown pulse count
	if (err != ERROR_OK)	{ goto ret; }

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &PPULSE.Byte, APDS9960__PPULSE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	PPULSE.Bits.PULSE = cnt;

	err = APDS9960_Write(pCpnt->cfg.slave_inst, &PPULSE.Byte, APDS9960__PPULSE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.Prox_Pulses = cnt;

	ret:
	return err;
}


FctERR NONNULL__ APDS9960_Set_ALS_Gain(APDS9960_t * const pCpnt, const APDS9960_als_gain gain)
{
	uAPDS9960_REG__CONTROL	CTL;
	FctERR					err = ERROR_OK;

	if (gain > APDS9960__ALS_64X_GAIN)	{ err = ERROR_VALUE; }	// Unknown gain
	if (err != ERROR_OK)				{ goto ret; }

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &CTL.Byte, APDS9960__CONTROL, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	CTL.Bits.AGAIN = gain;

	err = APDS9960_Write_Ctrl(pCpnt, CTL.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.ADC_Gain = gain;

	ret:
	return err;
}


FctERR NONNULL__ APDS9960_Set_Prox_Gain(APDS9960_t * const pCpnt, const APDS9960_prox_gain gain)
{
	uAPDS9960_REG__CONTROL	CTL;
	FctERR					err = ERROR_OK;

	if (gain > APDS9960__PROX_8X_GAIN)	{ err = ERROR_VALUE; }	// Unknown gain
	if (err != ERROR_OK)				{ goto ret; }

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &CTL.Byte, APDS9960__CONTROL, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	CTL.Bits.PGAIN = gain;

	err = APDS9960_Write_Ctrl(pCpnt, CTL.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.Prox_Gain = gain;

	ret:
	return err;
}


FctERR NONNULL__ APDS9960_Set_Prox_Drive_Strength(APDS9960_t * const pCpnt, const APDS9960_drive_strength ldrive)
{
	uAPDS9960_REG__CONTROL	CTL;
	FctERR					err = ERROR_OK;

	if (ldrive > APDS9960__DS_12_5MA)	{ err = ERROR_VALUE; }	// Unknown drive strength
	if (err != ERROR_OK)				{ goto ret; }

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &CTL.Byte, APDS9960__CONTROL, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	CTL.Bits.LDRIVE = ldrive;

	err = APDS9960_Write_Ctrl(pCpnt, CTL.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.Prox_Strength = ldrive;

	ret:
	return err;
}


FctERR NONNULL__ APDS9960_Set_ADC_Integration_Time(APDS9960_t * const pCpnt, const uint16_t integ)
{
	uint8_t	ATIME;
	FctERR	err = ERROR_OK;

	if ((integ < 3U) || (integ > 712U))	{ err = ERROR_RANGE; }	// Integration time out of range
	if (err != ERROR_OK)				{ goto ret; }

	// 2.78ms (0xFF) to 712ms (0x00)
	//ATIME = (uint8_t) ((integ - 2.78f) * (0x00 - 0xFF) / (712.0f - 2.78f) + 0xFF);
	ATIME = (uint8_t) (256U - (uintCPU_t) ((float) integ / 2.78f));

	err = APDS9960_Write(pCpnt->cfg.slave_inst, &ATIME, APDS9960__ATIME, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.ADC_Integ = integ;
	pCpnt->cfg.ADC_Integ_reg = ATIME;

	ret:
	return err;
}


FctERR NONNULL__ APDS9960_Set_Wait_Time(APDS9960_t * const pCpnt, const uint16_t wait)
{
	uAPDS9960_REG__CONFIG1	CFG1;
	uint8_t					WAIT;
	FctERR					err = ERROR_OK;

	if ((wait < 3U) || (wait > 8540U))	{ err = ERROR_RANGE; }	// Wait time out of range
	if (err != ERROR_OK)				{ goto ret; }

	err = APDS9960_Read(pCpnt->cfg.slave_inst, &CFG1.Byte, APDS9960__CONFIG1, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	if (wait <= 712U)
	{
		// 2.78ms (0xFF) to 712ms (0x00)
		//WAIT = (uint8_t) ((wait - 2.78f) * (0x00 - 0xFF) / (712.0f - 2.78f) + 0xFF);
		WAIT = (uint8_t) (256U - (uintCPU_t) ((float) wait / 2.78f));
		CFG1.Bits.WLONG = 0;
	}
	else
	{
		// 30ms (0xFF) to 8.54s (0x00)
		//WAIT = (uint8_t) ((wait - 30.0f) * (0x00 - 0xFF) / (8540.0f - 30.0f) + 0xFF);
		WAIT = (uint8_t) (256U - (uintCPU_t) (wait / 30.0f));
		CFG1.Bits.WLONG = 1U;
	}

	err = APDS9960_Write_Cfg1(pCpnt, CFG1.Byte);
	if (err != ERROR_OK)	{ goto ret; }
	err = APDS9960_Write(pCpnt->cfg.slave_inst, &WAIT, APDS9960__WTIME, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.Wait = wait;

	ret:
	return err;
}

/****************************************************************/


__WEAK void NONNULL__ APDS9960_INT_GPIO_Init(APDS9960_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&pCpnt->cfg.INT_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK bool NONNULL__ APDS9960_INT_GPIO_Get(const APDS9960_t * const pCpnt) {
	return I2C_peripheral_GPIO_get(&pCpnt->cfg.INT_GPIO); }


/****************************************************************/
#endif
#endif
/****************************************************************/
