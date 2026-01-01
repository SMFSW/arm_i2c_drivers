/*!\file PCA9532_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief PCA9532 Driver extensions
** \details PCA9532: 16-bit I2C-bus LED dimmer
**/
/****************************************************************/
#include "PCA9532.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9532)
/****************************************************************/


FctERR NONNULL__ PCA9532_Set_Auto_Increment(PCA9532_t * const pCpnt, const PCA95xx_reg_inc inc)
{
	FctERR err = ERROR_OK;

	if (RSHIFT(inc, 4U) > 1U)	{ err = ERROR_VALUE; }
	else						{ pCpnt->cfg.auto_inc = inc & PCA95xx__AUTO_INC_ALL; }	// Mask inc just in case

	return err;
}


FctERR NONNULL__ PCA9532_Freq_To_Byte(uint8_t * const byte, const float freq)
{
	FctERR err = ERROR_OK;

	if ((freq < 0.592f)	|| (freq > 152.0f))	{ err = ERROR_RANGE; }	// Told 0.591Hz min in datasheet, but would be an issue for cast after calculation
	else									{ *byte = (uint8_t) (152.0f / freq) - 1U; }

	return err;
}


FctERR NONNULL__ PCA9532_DutyCycle_To_Byte(uint8_t * const byte, const float duty)
{
	FctERR err = ERROR_OK;

	if ((duty < 0.0f) || (duty > 100.0f))	{ err = ERROR_RANGE; }
	else									{ *byte = (uint8_t) min(255U, (duty * 2.56f)); }

	return err;
}


FctERR NONNULL__ PCA9532_ReadRegister(PCA9532_t * const pCpnt, const PCA9532_reg reg, uint8_t * const val)
{
	FctERR err;

	if (reg > PCA9532__LS3)	{ err = ERROR_RANGE; }
	else					{ err = PCA9532_Read(pCpnt->cfg.slave_inst, val, reg, sizeof(uint8_t)); }

	return err;
}


FctERR NONNULL__ PCA9532_Read_INPUT0(PCA9532_t * const pCpnt, uPCA9532_REG__INPUT0 * const pINPUT0)
{
	return PCA9532_Read(pCpnt->cfg.slave_inst, &pINPUT0->Byte, PCA9532__INPUT0, sizeof(uint8_t));
}


FctERR NONNULL__ PCA9532_Read_INPUT1(PCA9532_t * const pCpnt, uPCA9532_REG__INPUT1 * const pINPUT1)
{
	return PCA9532_Read(pCpnt->cfg.slave_inst, &pINPUT1->Byte, PCA9532__INPUT1, sizeof(uint8_t));
}


FctERR NONNULL__ PCA9532_Set_Mode_LED(PCA9532_t * const pCpnt, const PCA9xxx_chan chan, const PCA95xx_ledsel mode)
{
	FctERR err = ERROR_OK;

	if (chan > PCA9xxx__PWM16)		{ err = ERROR_RANGE; }	// Unknown channel
	if (mode > PCA95xx__LED_PWM1)	{ err = ERROR_VALUE; }	// Unknown control mode
	if (err != ERROR_OK)			{ goto ret; }

	const uintCPU_t offset = chan / 4U;
	const uintCPU_t shift = chan * 2U;

	const uint32_t mask = LSHIFT(3U, shift);
	const uint32_t val = LSHIFT(mode, shift);

	SET_BITS_VAL(pCpnt->LS.DWord, mask, val);

	const uint8_t LS = RSHIFT(pCpnt->LS.DWord, offset * 8U);
	err = PCA9532_Write(pCpnt->cfg.slave_inst, &LS, PCA9532__LS0 + offset, sizeof(LS));

	ret:
	return err;
}


FctERR NONNULL__ PCA9532_Set_Mode_LEDs(PCA9532_t * const pCpnt, const uint16_t chans, const PCA95xx_ledsel mode)
{
	FctERR err = ERROR_OK;

	if (mode > PCA95xx__LED_PWM1)			{ err = ERROR_VALUE; }	// Unknown control mode
	if ((chans == 0) || (err != ERROR_OK))	{ goto ret; }

	uint32_t	mask = 0, val = 0;
	uint8_t		LS[4];

	for (PCA9xxx_chan chan = PCA9xxx__PWM1 ; chan <= PCA9xxx__PWM16 ; chan++)
	{
		if ((LSHIFT(1U, chan) & chans) != 0)
		{
			const uintCPU_t shift = chan * 2U;
			mask |= LSHIFT(3U, shift);
			val |= LSHIFT(mode, shift);
		}
	}

	SET_BITS_VAL(pCpnt->LS.DWord, mask, val);

	for (uintCPU_t i = 0 ; i < sizeof(LS) ; i++)	{ LS[i] = RSHIFT(pCpnt->LS.DWord, i * 8U); }

	err = PCA9532_Write(pCpnt->cfg.slave_inst, LS, PCA9532__LS0, sizeof(LS));

	ret:
	return err;
}


FctERR NONNULL__ PCA9532_SetFrequency(PCA9532_t * const pCpnt, const float freq, const bool index)
{
	uint8_t freq_byte;
	FctERR	err = PCA9532_Freq_To_Byte(&freq_byte, freq);

	if (err == ERROR_OK)
	{
		err = PCA9532_Write(pCpnt->cfg.slave_inst, &freq_byte, PCA9532__PSC0 + (index * 2U), sizeof(uint8_t));

		float * const pFreq = index ? &pCpnt->cfg.Freq1 : &pCpnt->cfg.Freq0;
		*pFreq = PCA9532_Byte_To_Freq(freq_byte);
	}

	return err;
}


FctERR NONNULL__ PCA9532_SetDuty(PCA9532_t * const pCpnt, const float duty, const bool index)
{
	uint8_t duty_byte;
	FctERR	err = PCA9532_DutyCycle_To_Byte(&duty_byte, duty);

	if (err == ERROR_OK)
	{
		err = PCA9532_Write(pCpnt->cfg.slave_inst, &duty_byte, PCA9532__PWM0 + (index * 2U), sizeof(uint8_t));

		float * const pDuty = index ? &pCpnt->cfg.Duty1 : &pCpnt->cfg.Duty0;
		*pDuty = PCA9532_Byte_To_Freq(duty_byte);
	}

	return err;
}


/****************************************************************/


__WEAK void NONNULL__ PCA9532_RST_GPIO_Init(PCA9532_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&pCpnt->cfg.RST_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK void NONNULL__ PCA9532_RST_GPIO_Set(PCA9532_t * const pCpnt, const bool state) {
	I2C_peripheral_GPIO_set(&pCpnt->cfg.RST_GPIO, state); }


/****************************************************************/
#endif
#endif
/****************************************************************/
