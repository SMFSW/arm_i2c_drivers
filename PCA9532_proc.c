/*!\file PCA9532_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
** \brief PCA9532 Driver procedures
** \details PCA9532: 16-bit I2C-bus LED dimmer
**/
/****************************************************************/
#include "PCA9532.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9532)
/****************************************************************/


PCA9532_t PCA9532[I2C_PCA9532_NB] = { 0 };


/****************************************************************/


__WEAK FctERR NONNULL__ PCA9532_Init_Sequence(PCA9532_t * const pCpnt)
{
	uint8_t	Data[4];
	FctERR	err;

	err = PCA9532_DutyCycle_To_Byte(&Data[0], 66.0f);
	err |= PCA9532_Write(pCpnt->cfg.slave_inst, &Data[0], PCA9532__PWM0, sizeof(uint8_t));
	err |= PCA9532_DutyCycle_To_Byte(&Data[0], 33.0f);
	err |= PCA9532_Write(pCpnt->cfg.slave_inst, &Data[0], PCA9532__PWM1, sizeof(uint8_t));

	err |= PCA9532_Read(pCpnt->cfg.slave_inst, Data, PCA9532__PSC0, sizeof(Data));

	pCpnt->cfg.Freq0 = PCA9532_Byte_To_Freq(Data[0]);
	pCpnt->cfg.Duty0 = PCA9532_Byte_To_Duty(Data[1]);
	pCpnt->cfg.Freq1 = PCA9532_Byte_To_Freq(Data[2]);
	pCpnt->cfg.Duty1 = PCA9532_Byte_To_Duty(Data[3]);

	// Set all LEDs to Off
	err |= PCA9532_Set_Mode_LEDs(pCpnt, 0xFFFF, PCA95xx__LED_OFF);

	return err;
}


/****************************************************************/


/****************************************************************/
#endif
#endif
/****************************************************************/
