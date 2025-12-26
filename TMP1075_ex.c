/*!\file TMP1075_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief TMP1075 Driver extensions
** \details TMP1075: Temperature Sensor With I2C and SMBus Interface in Industry Standard LM75 Form Factor and Pinout
**/
/****************************************************************/
#include "TMP1075.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_TMP1075)
/****************************************************************/


FctERR NONNULL__ TMP1075_Get_Temperature_Raw(TMP1075_t * const pCpnt, int16_t * temp)
{
	uint16_t	TEMP;
	FctERR		err;

	err = TMP1075_Read_Word(pCpnt->cfg.slave_inst, &TEMP, TMP1075__TEMP);
	if (err != ERROR_OK)	{ return err; }

	pCpnt->Ongoing = false;

	*temp = (int16_t) TEMP;
	return err;
}


FctERR NONNULL__ TMP1075_Start_SingleConversion(TMP1075_t * const pCpnt)
{
	uTMP1075_REG__CFGR	CFGR;
	FctERR				err;

	err = TMP1075_Read_Config(pCpnt, &CFGR.Byte);
	if (err != ERROR_OK)	{ return err; }

	CFGR.Bits.OS = 1;
	CFGR.Bits.SD = TMP1075__MODE_SINGLE;
	err = TMP1075_Write_Config(pCpnt, &CFGR.Byte);
	if (err != ERROR_OK)	{ return err; }

	pCpnt->Ongoing = true;
	pCpnt->cfg.Continuous = false;
	pCpnt->hLast = HALTicks();
	return err;
}


FctERR NONNULL__ TMP1075_Set_ConversionRate(TMP1075_t * const pCpnt, const TMP1075_conv_rate rate)
{
	uTMP1075_REG__CFGR	CFGR;
	FctERR				err;

	err = TMP1075_Read_Config(pCpnt, &CFGR.Byte);
	if (err != ERROR_OK)	{ return err; }

	CFGR.Bits.R = rate;
	CFGR.Bits.SD = TMP1075__MODE_CONTINUOUS;
	err = TMP1075_Write_Config(pCpnt, &CFGR.Byte);
	if (err != ERROR_OK)	{ return err; }

	pCpnt->cfg.Continuous = true;
	pCpnt->cfg.Rate = rate;
	return err;
}


FctERR NONNULL__ TMP1075_Set_ConversionMode(TMP1075_t * const pCpnt, const TMP1075_shutdown_mode mode)
{
	uTMP1075_REG__CFGR	CFGR;
	FctERR				err;

	err = TMP1075_Read_Config(pCpnt, &CFGR.Byte);
	if (err != ERROR_OK)	{ return err; }

	CFGR.Bits.SD = mode;
	err = TMP1075_Write_Config(pCpnt, &CFGR.Byte);
	if (err != ERROR_OK)	{ return err; }

	pCpnt->cfg.Continuous = nbinEval(mode);
	return err;
}


FctERR NONNULL__ TMP1075_Set_AlertMode(TMP1075_t * const pCpnt, const TMP1075_alert_mode mode)
{
	uTMP1075_REG__CFGR	CFGR;
	FctERR				err;

	err = TMP1075_Read_Config(pCpnt, &CFGR.Byte);
	if (err != ERROR_OK)	{ return err; }

	CFGR.Bits.TM = mode;
	return TMP1075_Write_Config(pCpnt, &CFGR.Byte);
}


FctERR NONNULL__ TMP1075_Set_AlertPolarity(TMP1075_t * const pCpnt, const TMP1075_alert_pol pol)
{
	uTMP1075_REG__CFGR	CFGR;
	FctERR				err;

	err = TMP1075_Read_Config(pCpnt, &CFGR.Byte);
	if (err != ERROR_OK)	{ return err; }

	CFGR.Bits.POL = pol;
	return TMP1075_Write_Config(pCpnt, &CFGR.Byte);
}


FctERR NONNULL__ TMP1075_Set_AlertNbFaults(TMP1075_t * const pCpnt, const TMP1075_nb_faults faults)
{
	uTMP1075_REG__CFGR	CFGR;
	FctERR				err;

	err = TMP1075_Read_Config(pCpnt, &CFGR.Byte);
	if (err != ERROR_OK)	{ return err; }

	CFGR.Bits.F = faults;
	return TMP1075_Write_Config(pCpnt, &CFGR.Byte);
}


/****************************************************************/


__WEAK void NONNULL__ TMP1075_INT_GPIO_Init(TMP1075_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&pCpnt->cfg.INT_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK bool NONNULL__ TMP1075_INT_GPIO_Get(TMP1075_t * const pCpnt) {
	return I2C_peripheral_GPIO_get(&pCpnt->cfg.INT_GPIO); }


/****************************************************************/
#endif
#endif
/****************************************************************/
