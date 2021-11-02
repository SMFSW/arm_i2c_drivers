/*!\file APDS9930_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief APDS9930 Driver extensions
** \details APDS9930: Digital Proximity and Ambient Light Sensor
**/
/****************************************************************/
#include "APDS9930.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_APDS9930)
/****************************************************************/


FctERR NONNULL__ APDS9930_Set_PON(APDS9930_t * const pCpnt, const bool en)
{
	uAPDS9930_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9930__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.PON = en;
	return APDS9930_Write_En(pCpnt, EN.Byte);
}


FctERR NONNULL__ APDS9930_Set_AEN(APDS9930_t * const pCpnt, const bool en)
{
	uAPDS9930_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9930__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.AEN = en;
	return APDS9930_Write_En(pCpnt, EN.Byte);
}


FctERR NONNULL__ APDS9930_Set_PEN(APDS9930_t * const pCpnt, const bool en)
{
	uAPDS9930_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9930__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.PEN = en;
	return APDS9930_Write_En(pCpnt, EN.Byte);
}


FctERR NONNULL__ APDS9930_Set_AIEN(APDS9930_t * const pCpnt, const bool en)
{
	uAPDS9930_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9930__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.AIEN = en;
	err = APDS9930_Write_En(pCpnt, EN.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.PIEN = en;
	return err;
}


FctERR NONNULL__ APDS9930_Set_PIEN(APDS9930_t * const pCpnt, const bool en)
{
	uAPDS9930_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9930__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.PIEN = en;
	err = APDS9930_Write_En(pCpnt, EN.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.PIEN = en;
	return err;
}


FctERR NONNULL__ APDS9930_Set_WEN(APDS9930_t * const pCpnt, const bool en)
{
	uAPDS9930_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9930__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.WEN = en;
	err = APDS9930_Write_En(pCpnt, EN.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.WEN = en;
	return err;
}


FctERR NONNULL__ APDS9930_Set_SAI(APDS9930_t * const pCpnt, const bool en)
{
	uAPDS9930_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9930__ENABLE, 1);
	if (err)	{ return err; }

	EN.Bits.SAI = en;
	return APDS9930_Write_En(pCpnt, EN.Byte);
}


FctERR NONNULL__ APDS9930_Set_ALS_Gain(APDS9930_t * const pCpnt, const APDS9930_als_gain_ex gain)
{
	uAPDS9930_REG__CONTROL	CTL;
	uAPDS9930_REG__CONFIG	CFG;
	FctERR					err;

	if (gain > APDS9930__ALS_GAIN_8ON6X)	{ return ERROR_VALUE; }	// Unknown gain

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &CFG.Byte, APDS9930__CONFIG, 1);
	if (err)	{ return err; }

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &CTL.Byte, APDS9930__CONTROL, 1);
	if (err)	{ return err; }

	CTL.Bits.AGAIN = gain % APDS9930__ALS_GAIN_1ON6X;
	CFG.Bits.AGL = (gain >= APDS9930__ALS_GAIN_1ON6X) ? true : false;

	err = APDS9930_Write_Cfg(pCpnt, CFG.Byte);
	if (err)	{ return err; }
	err = APDS9930_Write_Ctrl(pCpnt, CTL.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.ALS_Gain = gain;
	APDS9930_Set_LPC(pCpnt);

	return err;
}


FctERR NONNULL__ APDS9930_Set_Prox_Gain(APDS9930_t * const pCpnt, const APDS9930_prox_gain gain)
{
	uAPDS9930_REG__CONTROL	CTL;
	FctERR					err;

	if (gain > APDS9930__PROX_8X_GAIN)	{ return ERROR_VALUE; }	// Unknown gain

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &CTL.Byte, APDS9930__CONTROL, 1);
	if (err)	{ return err; }

	CTL.Bits.PGAIN = gain;
	err = APDS9930_Write_Ctrl(pCpnt, CTL.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.Prox_Gain = gain;

	return err;
}


FctERR NONNULL__ APDS9930_Set_Prox_Drive_Strength(APDS9930_t * const pCpnt, const APDS9930_drive_strength_ex pdrive)
{
	uAPDS9930_REG__CONTROL	CTL;
	uAPDS9930_REG__CONFIG	CFG;
	FctERR					err;

	if (pdrive > APDS9930__STRENGTH_1_4MA)	{ return ERROR_VALUE; }	// Unknown drive strength

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &CFG.Byte, APDS9930__CONFIG, 1);
	if (err)	{ return err; }

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &CTL.Byte, APDS9930__CONTROL, 1);
	if (err)	{ return err; }

	CTL.Bits.PDRIVE = pdrive % APDS9930__STRENGTH_11_1MA;
	CFG.Bits.PDL = (pdrive >= APDS9930__STRENGTH_11_1MA) ? true : false;

	err = APDS9930_Write_Cfg(pCpnt, CFG.Byte);
	if (err)	{ return err; }
	err = APDS9930_Write_Ctrl(pCpnt, CTL.Byte);
	if (err)	{ return err; }

	pCpnt->cfg.Prox_Strength = pdrive;

	return err;
}


FctERR NONNULL__ APDS9930_Set_ALS_Pesistence(APDS9930_t * const pCpnt, const APDS9930_als_it_persist persist)
{
	uAPDS9930_REG__PERSIST	PERS;
	FctERR					err;

	if (persist > APDS9930__PERSIST_60_ALS_PERIODS)		{ return ERROR_VALUE; }	// Unknown persistence

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &PERS.Byte, APDS9930__PERS, 1);
	if (err)	{ return err; }

	PERS.Bits.APERS = persist;
	err = APDS9930_Write(pCpnt->cfg.slave_inst, &PERS.Byte, APDS9930__PERS, 1);
	if (err)	{ return err; }

	return err;
}


FctERR NONNULL__ APDS9930_Set_Prox_Pesistence(APDS9930_t * const pCpnt, const APDS9930_prox_it_persist persist)
{
	uAPDS9930_REG__PERSIST	PERS;
	FctERR					err;

	if (persist > APDS9930__PERSIST_15_PROX_PERIODS)	{ return ERROR_VALUE; }	// Unknown persistence

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &PERS.Byte, APDS9930__PERS, 1);
	if (err)	{ return err; }

	PERS.Bits.PPERS = persist;
	err = APDS9930_Write(pCpnt->cfg.slave_inst, &PERS.Byte, APDS9930__PERS, 1);
	if (err)	{ return err; }

	return err;
}


FctERR NONNULL__ APDS9930_Set_ALS_Integration_Time(APDS9930_t * const pCpnt, const uint16_t integ)
{
	uint8_t	ATIME;
	FctERR	err;

	if ((integ < 3) || (integ > 699))	{ return ERROR_RANGE; }	// Integration time out of range

	// 2.73ms (0xFF) to 699ms (0x00)
	//ATIME = (uint8_t) ((integ - 2.73f) * (0x00 - 0xFF) / (699.0f - 2.73f) + 0xFF);
	ATIME = 256 - (uint8_t) ((float) integ / 2.73f);
	err = APDS9930_Write(pCpnt->cfg.slave_inst, &ATIME, APDS9930__ATIME, 1);
	if (err)	{ return err; }

	pCpnt->cfg.ALS_Integ = integ;
	pCpnt->cfg.ALS_Integ_reg = ATIME;
	APDS9930_Set_LPC(pCpnt);

	return err;
}


FctERR NONNULL__ APDS9930_Set_Prox_Integration_Time(APDS9930_t * const pCpnt, const uint16_t integ)
{
	uint8_t	ATIME;
	FctERR	err;

	if ((integ < 3) || (integ > 699))	{ return ERROR_RANGE; }	// Integration time out of range

	// 2.73ms (0xFF) to 699ms (0x00)
	//ATIME = (uint8_t) ((integ - 2.73f) * (0x00 - 0xFF) / (699.0f - 2.73f) + 0xFF);
	ATIME = 256 - (uint8_t) ((float) integ / 2.73f);
	err = APDS9930_Write(pCpnt->cfg.slave_inst, &ATIME, APDS9930__PTIME, 1);
	if (err)	{ return err; }

	pCpnt->cfg.Prox_Integ = integ;

	return err;
}


FctERR NONNULL__ APDS9930_Set_Wait_Time(APDS9930_t * const pCpnt, const uint16_t wait)
{
	uAPDS9930_REG__CONFIG	CFG;
	uint8_t					WAIT;
	FctERR					err;

	if ((wait < 3) || (wait > 8400))	{ return ERROR_RANGE; }	// Wait time out of range

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &CFG.Byte, APDS9930__CONFIG, 1);
	if (err)	{ return err; }

	if (wait <= 699)
	{
		// 2.73ms (0xFF) to 699ms (0x00)
		//WAIT = (uint8_t) ((wait - 2.73f) * (0x00 - 0xFF) / (699.0f - 2.73f) + 0xFF);
		WAIT = 256 - (uint8_t) ((float) wait / 2.73f);
		CFG.Bits.WLONG = 0;
	}
	else
	{
		// 33ms (0xFF) to 8.4s (0x00)
		//WAIT = (uint8_t) ((wait - 33.0f) * (0x00 - 0xFF) / (8400.0f - 33.0f) + 0xFF);
		WAIT = 256 - (uint8_t) (wait / 33.0f);
		CFG.Bits.WLONG = 1;
	}

	err = APDS9930_Write_Cfg(pCpnt, CFG.Byte);
	if (err)	{ return err; }
	err = APDS9930_Write(pCpnt->cfg.slave_inst, &WAIT, APDS9930__WTIME, 1);
	if (err)	{ return err; }

	pCpnt->cfg.Wait = wait;

	return err;
}


/****************************************************************/


__WEAK FctERR NONNULL__ APDS9930_INT_GPIO_Init(APDS9930_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	return I2C_peripheral_GPIO_init(&pCpnt->cfg.INT_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK FctERR NONNULL__ APDS9930_INT_GPIO_Get(APDS9930_t * const pCpnt, bool * const pState) {
	return I2C_peripheral_GPIO_get(&pCpnt->cfg.INT_GPIO, pState); }


/****************************************************************/
#endif
#endif
/****************************************************************/
