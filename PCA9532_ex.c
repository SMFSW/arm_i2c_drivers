/*!\file PCA9532_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
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
	*val = 0;

	if (reg > PCA9532__LS3)	{ return ERROR_RANGE; }		// Unknown register

	return PCA9532_Read(pCpnt->cfg.slave_inst, val, reg, 1);
}


FctERR NONNULL__ PCA9532_Read_INPUT0(PCA9532_t * const pCpnt, uPCA9532_REG__INPUT0 * const pINPUT0)
{
	pINPUT0->Byte = 0;
	return PCA9532_Read(pCpnt->cfg.slave_inst, &pINPUT0->Byte, PCA9532__INPUT0, 1);
}


FctERR NONNULL__ PCA9532_Read_INPUT1(PCA9532_t * const pCpnt, uPCA9532_REG__INPUT1 * const pINPUT1)
{
	pINPUT1->Byte = 0;
	return PCA9532_Read(pCpnt->cfg.slave_inst, &pINPUT1->Byte, PCA9532__INPUT1, 1);
}


FctERR NONNULL__ PCA9532_Set_Mode_LED(PCA9532_t * const pCpnt, const PCA9xxx_chan chan, const PCA95xx_ledsel mode)
{
	if (chan > PCA9xxx__PWM16)		{ return ERROR_RANGE; }	// Unknown channel
	if (mode > PCA95xx__LED_PWM1)	{ return ERROR_VALUE; }	// Unknown control mode

	const unsigned int offset = chan / 4;
	const unsigned int shift = chan * 2;

	const uint32_t mask = LSHIFT(0x3, shift), val = LSHIFT(mode, shift);

	pCpnt->LS.DWord = (pCpnt->LS.DWord & ~mask) | val;

	return PCA9532_Write(pCpnt->cfg.slave_inst, (uint8_t *) &pCpnt->LS + offset, PCA9532__LS0 + offset, 1);
}


FctERR NONNULL__ PCA9532_Set_Mode_LEDs(PCA9532_t * const pCpnt, const uint16_t chans, const PCA95xx_ledsel mode)
{
	if (!chans)						{ return ERROR_OK; }	// No channel selected
	if (mode > PCA95xx__LED_PWM1)	{ return ERROR_VALUE; }	// Unknown control mode

	uint32_t mask = 0, val = 0;

	for (PCA9xxx_chan chan = PCA9xxx__PWM1 ; chan <= PCA9xxx__PWM16 ; chan++)
	{
		if (LSHIFT(1, chan) & chans)
		{
			const unsigned int shift = chan * 2;
			mask |= LSHIFT(0x3, shift);
			val |= LSHIFT(mode, shift);
		}
	}

	pCpnt->LS.DWord = (pCpnt->LS.DWord & ~mask) | val;

	return PCA9532_Write(pCpnt->cfg.slave_inst, (uint8_t *) &pCpnt->LS, PCA9532__LS0, sizeof(pCpnt->LS));
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
#endif
#endif
/****************************************************************/
