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


FctERR NONNULL__ PCA9532_Set_Mode_LED(PCA9532_t * const pCpnt, const PCA95xx_chan chan, const PCA95xx_ledsel mode)
{
	if (chan > PCA95xx__PWM16)		{ return ERROR_RANGE; }	// Unknown channel
	if (mode > PCA95xx__LED_PWM1)	{ return ERROR_VALUE; }	// Unknown control mode

	if (chan == PCA95xx__PWM1)			{ pCpnt->LS.Bits.LED0 = mode; }
	else if (chan == PCA95xx__PWM2)		{ pCpnt->LS.Bits.LED1 = mode; }
	else if (chan == PCA95xx__PWM3)		{ pCpnt->LS.Bits.LED2 = mode; }
	else if (chan == PCA95xx__PWM4)		{ pCpnt->LS.Bits.LED3 = mode; }
	else if (chan == PCA95xx__PWM5)		{ pCpnt->LS.Bits.LED4 = mode; }
	else if (chan == PCA95xx__PWM6)		{ pCpnt->LS.Bits.LED5 = mode; }
	else if (chan == PCA95xx__PWM7)		{ pCpnt->LS.Bits.LED6 = mode; }
	else if (chan == PCA95xx__PWM8)		{ pCpnt->LS.Bits.LED7 = mode; }
	else if (chan == PCA95xx__PWM9)		{ pCpnt->LS.Bits.LED8 = mode; }
	else if (chan == PCA95xx__PWM10)	{ pCpnt->LS.Bits.LED9 = mode; }
	else if (chan == PCA95xx__PWM11)	{ pCpnt->LS.Bits.LED10 = mode; }
	else if (chan == PCA95xx__PWM12)	{ pCpnt->LS.Bits.LED11 = mode; }
	else if (chan == PCA95xx__PWM13)	{ pCpnt->LS.Bits.LED12 = mode; }
	else if (chan == PCA95xx__PWM14)	{ pCpnt->LS.Bits.LED13 = mode; }
	else if (chan == PCA95xx__PWM15)	{ pCpnt->LS.Bits.LED14 = mode; }
	else if (chan == PCA95xx__PWM16)	{ pCpnt->LS.Bits.LED15 = mode; }

	return PCA9532_Write(pCpnt->cfg.slave_inst, (uint8_t *) &pCpnt->LS, PCA9532__LS0 + (chan / 4), sizeof(pCpnt->LS));
}


FctERR NONNULL__ PCA9532_Set_Mode_LEDs(PCA9532_t * const pCpnt, const uint16_t chans, const PCA95xx_ledsel mode)
{
	if (!chans)						{ return ERROR_OK; }	// Nothing to do
	if (mode > PCA95xx__LED_PWM1)	{ return ERROR_VALUE; }	// Unknown control mode

	if (chans & LSHIFT(1, PCA95xx__PWM1))	{ pCpnt->LS.Bits.LED0 = mode; }
	if (chans & LSHIFT(1, PCA95xx__PWM2))	{ pCpnt->LS.Bits.LED1 = mode; }
	if (chans & LSHIFT(1, PCA95xx__PWM3))	{ pCpnt->LS.Bits.LED2 = mode; }
	if (chans & LSHIFT(1, PCA95xx__PWM4))	{ pCpnt->LS.Bits.LED3 = mode; }
	if (chans & LSHIFT(1, PCA95xx__PWM5))	{ pCpnt->LS.Bits.LED4 = mode; }
	if (chans & LSHIFT(1, PCA95xx__PWM6))	{ pCpnt->LS.Bits.LED5 = mode; }
	if (chans & LSHIFT(1, PCA95xx__PWM7))	{ pCpnt->LS.Bits.LED6 = mode; }
	if (chans & LSHIFT(1, PCA95xx__PWM8))	{ pCpnt->LS.Bits.LED7 = mode; }
	if (chans & LSHIFT(1, PCA95xx__PWM9))	{ pCpnt->LS.Bits.LED8 = mode; }
	if (chans & LSHIFT(1, PCA95xx__PWM10))	{ pCpnt->LS.Bits.LED9 = mode; }
	if (chans & LSHIFT(1, PCA95xx__PWM11))	{ pCpnt->LS.Bits.LED10 = mode; }
	if (chans & LSHIFT(1, PCA95xx__PWM12))	{ pCpnt->LS.Bits.LED11 = mode; }
	if (chans & LSHIFT(1, PCA95xx__PWM13))	{ pCpnt->LS.Bits.LED12 = mode; }
	if (chans & LSHIFT(1, PCA95xx__PWM14))	{ pCpnt->LS.Bits.LED13 = mode; }
	if (chans & LSHIFT(1, PCA95xx__PWM15))	{ pCpnt->LS.Bits.LED14 = mode; }
	if (chans & LSHIFT(1, PCA95xx__PWM16))	{ pCpnt->LS.Bits.LED15 = mode; }

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
