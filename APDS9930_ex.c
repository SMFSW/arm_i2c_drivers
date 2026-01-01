/*!\file APDS9930_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
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

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9930__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.PON = en;

	err = APDS9930_Write_En(pCpnt, EN.Byte);

	ret:
	return err;
}


FctERR NONNULL__ APDS9930_Set_AEN(APDS9930_t * const pCpnt, const bool en)
{
	uAPDS9930_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9930__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.AEN = en;

	err = APDS9930_Write_En(pCpnt, EN.Byte);

	ret:
	return err;
}


FctERR NONNULL__ APDS9930_Set_PEN(APDS9930_t * const pCpnt, const bool en)
{
	uAPDS9930_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9930__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.PEN = en;

	err = APDS9930_Write_En(pCpnt, EN.Byte);

	ret:
	return err;
}


FctERR NONNULL__ APDS9930_Set_AIEN(APDS9930_t * const pCpnt, const bool en)
{
	uAPDS9930_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9930__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.AIEN = en;

	err = APDS9930_Write_En(pCpnt, EN.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.PIEN = en;

	ret:
	return err;
}


FctERR NONNULL__ APDS9930_Set_PIEN(APDS9930_t * const pCpnt, const bool en)
{
	uAPDS9930_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9930__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.PIEN = en;

	err = APDS9930_Write_En(pCpnt, EN.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.PIEN = en;

	ret:
	return err;
}


FctERR NONNULL__ APDS9930_Set_WEN(APDS9930_t * const pCpnt, const bool en)
{
	uAPDS9930_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9930__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.WEN = en;

	err = APDS9930_Write_En(pCpnt, EN.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.WEN = en;

	ret:
	return err;
}


FctERR NONNULL__ APDS9930_Set_SAI(APDS9930_t * const pCpnt, const bool en)
{
	uAPDS9930_REG__ENABLE	EN;
	FctERR					err;

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &EN.Byte, APDS9930__ENABLE, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	EN.Bits.SAI = en;

	err = APDS9930_Write_En(pCpnt, EN.Byte);

	ret:
	return err;
}


FctERR NONNULL__ APDS9930_Set_ALS_Gain(APDS9930_t * const pCpnt, const APDS9930_als_gain_ex gain)
{
	uAPDS9930_REG__CONTROL	CTL;
	uAPDS9930_REG__CONFIG	CFG;
	FctERR					err = ERROR_OK;

	if (gain > APDS9930__ALS_GAIN_8ON6X)	{ err = ERROR_VALUE; }	// Unknown gain
	if (err != ERROR_OK)					{ goto ret; }

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &CFG.Byte, APDS9930__CONFIG, 1U);
	if (err != ERROR_OK)	{ goto ret; }
	err = APDS9930_Read(pCpnt->cfg.slave_inst, &CTL.Byte, APDS9930__CONTROL, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	CTL.Bits.AGAIN = gain % APDS9930__ALS_GAIN_1ON6X;
	CFG.Bits.AGL = (gain >= APDS9930__ALS_GAIN_1ON6X) ? true : false;

	err = APDS9930_Write_Cfg(pCpnt, CFG.Byte);
	if (err != ERROR_OK)	{ goto ret; }
	err = APDS9930_Write_Ctrl(pCpnt, CTL.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.ALS_Gain = gain;
	APDS9930_Set_LPC(pCpnt);

	ret:
	return err;
}


FctERR NONNULL__ APDS9930_Set_Prox_Gain(APDS9930_t * const pCpnt, const APDS9930_prox_gain gain)
{
	uAPDS9930_REG__CONTROL	CTL;
	FctERR					err = ERROR_OK;

	if (gain > APDS9930__PROX_8X_GAIN)	{ err = ERROR_VALUE; }	// Unknown gain
	if (err != ERROR_OK)				{ goto ret; }

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &CTL.Byte, APDS9930__CONTROL, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	CTL.Bits.PGAIN = gain;

	err = APDS9930_Write_Ctrl(pCpnt, CTL.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.Prox_Gain = gain;

	ret:
	return err;
}


FctERR NONNULL__ APDS9930_Set_Prox_Drive_Strength(APDS9930_t * const pCpnt, const APDS9930_drive_strength_ex pdrive)
{
	uAPDS9930_REG__CONTROL	CTL;
	uAPDS9930_REG__CONFIG	CFG;
	FctERR					err = ERROR_OK;

	if (pdrive > APDS9930__STRENGTH_1_4MA)	{ err = ERROR_VALUE; }	// Unknown drive strength
	if (err != ERROR_OK)					{ goto ret; }

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &CFG.Byte, APDS9930__CONFIG, 1U);
	if (err != ERROR_OK)	{ goto ret; }
	err = APDS9930_Read(pCpnt->cfg.slave_inst, &CTL.Byte, APDS9930__CONTROL, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	CTL.Bits.PDRIVE = pdrive % APDS9930__STRENGTH_11_1MA;
	CFG.Bits.PDL = (pdrive >= APDS9930__STRENGTH_11_1MA) ? true : false;

	err = APDS9930_Write_Cfg(pCpnt, CFG.Byte);
	if (err != ERROR_OK)	{ goto ret; }
	err = APDS9930_Write_Ctrl(pCpnt, CTL.Byte);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.Prox_Strength = pdrive;

	ret:
	return err;
}


FctERR NONNULL__ APDS9930_Set_ALS_Pesistence(APDS9930_t * const pCpnt, const APDS9930_als_it_persist persist)
{
	uAPDS9930_REG__PERSIST	PERS;
	FctERR					err = ERROR_OK;

	if (persist > APDS9930__PERSIST_60_ALS_PERIODS)	{ err = ERROR_VALUE; }	// Unknown persistence
	if (err != ERROR_OK)							{ goto ret; }

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &PERS.Byte, APDS9930__PERS, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	PERS.Bits.APERS = persist;

	err = APDS9930_Write(pCpnt->cfg.slave_inst, &PERS.Byte, APDS9930__PERS, 1U);

	ret:
	return err;
}


FctERR NONNULL__ APDS9930_Set_Prox_Pesistence(APDS9930_t * const pCpnt, const APDS9930_prox_it_persist persist)
{
	uAPDS9930_REG__PERSIST	PERS;
	FctERR					err = ERROR_OK;

	if (persist > APDS9930__PERSIST_15_PROX_PERIODS)	{ err = ERROR_VALUE; }	// Unknown persistence
	if (err != ERROR_OK)								{ goto ret; }

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &PERS.Byte, APDS9930__PERS, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	PERS.Bits.PPERS = persist;

	err = APDS9930_Write(pCpnt->cfg.slave_inst, &PERS.Byte, APDS9930__PERS, 1U);

	ret:
	return err;
}


FctERR NONNULL__ APDS9930_Set_ALS_Integration_Time(APDS9930_t * const pCpnt, const uint16_t integ)
{
	uint8_t	ATIME;
	FctERR	err = ERROR_OK;

	if ((integ < 3U) || (integ > 699U))	{ err = ERROR_RANGE; }	// Integration time out of range
	if (err != ERROR_OK)				{ goto ret; }

	// 2.73ms (0xFF) to 699ms (0x00)
	//ATIME = (uint8_t) ((integ - 2.73f) * (0x00 - 0xFF) / (699.0f - 2.73f) + 0xFF);
	ATIME = (uint8_t) (256U - (uintCPU_t) ((float) integ / 2.73f));

	err = APDS9930_Write(pCpnt->cfg.slave_inst, &ATIME, APDS9930__ATIME, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.ALS_Integ = integ;
	pCpnt->cfg.ALS_Integ_reg = ATIME;
	APDS9930_Set_LPC(pCpnt);

	ret:
	return err;
}


FctERR NONNULL__ APDS9930_Set_Prox_Integration_Time(APDS9930_t * const pCpnt, const uint16_t integ)
{
	uint8_t	ATIME;
	FctERR	err = ERROR_OK;

	if ((integ < 3U) || (integ > 699U))	{ err = ERROR_RANGE; }	// Integration time out of range
	if (err != ERROR_OK)				{ goto ret; }

	// 2.73ms (0xFF) to 699ms (0x00)
	//ATIME = (uint8_t) ((integ - 2.73f) * (0x00 - 0xFF) / (699.0f - 2.73f) + 0xFF);
	ATIME = (uint8_t) (256U - (uintCPU_t) ((float) integ / 2.73f));

	err = APDS9930_Write(pCpnt->cfg.slave_inst, &ATIME, APDS9930__PTIME, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.Prox_Integ = integ;

	ret:
	return err;
}


FctERR NONNULL__ APDS9930_Set_Wait_Time(APDS9930_t * const pCpnt, const uint16_t wait)
{
	uAPDS9930_REG__CONFIG	CFG;
	uint8_t					WAIT;
	FctERR					err = ERROR_OK;

	if ((wait < 3U) || (wait > 8400U))	{ err = ERROR_RANGE; }	// Wait time out of range
	if (err != ERROR_OK)				{ goto ret; }

	err = APDS9930_Read(pCpnt->cfg.slave_inst, &CFG.Byte, APDS9930__CONFIG, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	if (wait <= 699U)
	{
		// 2.73ms (0xFF) to 699ms (0x00)
		//WAIT = (uint8_t) ((wait - 2.73f) * (0x00 - 0xFF) / (699.0f - 2.73f) + 0xFF);
		WAIT = (uint8_t) (256U - (uintCPU_t) ((float) wait / 2.73f));
		CFG.Bits.WLONG = 0;
	}
	else
	{
		// 33ms (0xFF) to 8.4s (0x00)
		//WAIT = (uint8_t) ((wait - 33.0f) * (0x00 - 0xFF) / (8400.0f - 33.0f) + 0xFF);
		WAIT = (uint8_t) (256U - (uintCPU_t) (wait / 33.0f));
		CFG.Bits.WLONG = 1U;
	}

	err = APDS9930_Write_Cfg(pCpnt, CFG.Byte);
	if (err != ERROR_OK)	{ goto ret; }
	err = APDS9930_Write(pCpnt->cfg.slave_inst, &WAIT, APDS9930__WTIME, 1U);
	if (err != ERROR_OK)	{ goto ret; }

	pCpnt->cfg.Wait = wait;

	ret:
	return err;
}


/****************************************************************/


__WEAK void NONNULL__ APDS9930_INT_GPIO_Init(APDS9930_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&pCpnt->cfg.INT_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK bool NONNULL__ APDS9930_INT_GPIO_Get(const APDS9930_t * const pCpnt) {
	return I2C_peripheral_GPIO_get(&pCpnt->cfg.INT_GPIO); }


/****************************************************************/
#endif
#endif
/****************************************************************/
