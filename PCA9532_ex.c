/*!\file PCA9532_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief PCA9532 Driver extensions
** \details PCA9532: 16-bit I2C-bus LED dimmer
**/
/****************************************************************/
#include "PCA9532.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9532)
/****************************************************************/


FctERR NONNULL__ PCA9532_ReadRegister(PCA9532_t * const pCpnt, const PCA9532_reg reg, uint8_t * const val)
{
	if (reg > PCA9532__LS3)	{ return ERROR_RANGE; }		// Unknown register

	return PCA9532_Read(pCpnt->cfg.slave_inst, val, reg, sizeof(uint8_t));
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
	if (chan > PCA9xxx__PWM16)		{ return ERROR_RANGE; }	// Unknown channel
	if (mode > PCA95xx__LED_PWM1)	{ return ERROR_VALUE; }	// Unknown control mode

	const uintCPU_t offset = chan / 4;
	const uintCPU_t shift = chan * 2;

	const uint32_t mask = LSHIFT(0x3, shift);
	const uint32_t val = LSHIFT(mode, shift);

	SET_BITS_VAL(pCpnt->LS.DWord, mask, val);

	const uint8_t LS = RSHIFT(pCpnt->LS.DWord, offset * 8);
	return PCA9532_Write(pCpnt->cfg.slave_inst, &LS, PCA9532__LS0 + offset, sizeof(LS));
}


FctERR NONNULL__ PCA9532_Set_Mode_LEDs(PCA9532_t * const pCpnt, const uint16_t chans, const PCA95xx_ledsel mode)
{
	if (!chans)						{ return ERROR_OK; }	// No channel selected
	if (mode > PCA95xx__LED_PWM1)	{ return ERROR_VALUE; }	// Unknown control mode

	uint32_t	mask = 0, val = 0;
	uint8_t		LS[4];

	for (PCA9xxx_chan chan = PCA9xxx__PWM1 ; chan <= PCA9xxx__PWM16 ; chan++)
	{
		if (LSHIFT(1, chan) & chans)
		{
			const uintCPU_t shift = chan * 2;
			mask |= LSHIFT(0x3, shift);
			val |= LSHIFT(mode, shift);
		}
	}

	SET_BITS_VAL(pCpnt->LS.DWord, mask, val);

	for (uintCPU_t i = 0 ; i < sizeof(LS) ; i++)	{ LS[i] = RSHIFT(pCpnt->LS.DWord, i * 8); }

	return PCA9532_Write(pCpnt->cfg.slave_inst, LS, PCA9532__LS0, sizeof(LS));
}


FctERR NONNULL__ PCA9532_SetFrequency(PCA9532_t * const pCpnt, const float freq, const bool index)
{
	uint8_t freq_byte;
	FctERR	err = PCA9532_Freq_To_Byte(&freq_byte, freq);

	if (!err)
	{
		err = PCA9532_Write(pCpnt->cfg.slave_inst, &freq_byte, PCA9532__PSC0 + (index * 2), sizeof(uint8_t));

		float * const pFreq = index ? &pCpnt->cfg.Freq1 : &pCpnt->cfg.Freq0;
		*pFreq = PCA9532_Byte_To_Freq(freq_byte);
	}

	return err;
}


FctERR NONNULL__ PCA9532_SetDuty(PCA9532_t * const pCpnt, const float duty, const bool index)
{
	uint8_t duty_byte;
	FctERR	err = PCA9532_DutyCycle_To_Byte(&duty_byte, duty);

	if (!err)
	{
		err = PCA9532_Write(pCpnt->cfg.slave_inst, &duty_byte, PCA9532__PWM0 + (index * 2), sizeof(uint8_t));

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
