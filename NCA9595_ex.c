/*!\file NCA9595_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief NCA9595 Driver extensions
** \details NCA9595: Low-voltage 16-bit I²C and SMBus I/O expander
**/
/****************************************************************/
#include "NCA9595.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_NCA9595)
/****************************************************************/


/*!\brief Write Register with mask for NCA9595 peripheral
** \param[in,out] pCpnt - Pointer to NCA9595 component
** \param[in] val - Value to write
** \param[in] mask - Mask for write
** \param[in] reg - Register address
** \return Error code
**/
static FctERR NONNULL__ NCA9595_Write_Reg_Mask(NCA9595_t * const pCpnt, const uint16_t val, const uint16_t mask, const NCA9595_wreg reg)
{
	uint16_t data;

	FctERR err = NCA9595_Read_Word(pCpnt->cfg.slave_inst, &data, reg);
	if (err != ERROR_OK)	{ goto ret; }

	const uint16_t final_mask = ~pCpnt->cfg.NCA9595_Cfg.Word & mask;
	SET_BITS_VAL(data, final_mask, val);

	err = NCA9595_Write_Word(pCpnt->cfg.slave_inst, &data, reg);

	ret:
	return err;
}


FctERR NONNULL__ NCA9595_Read_Inputs(NCA9595_t * const pCpnt, uint16_t * const in)
{
	const FctERR err = NCA9595_Read_Word(pCpnt->cfg.slave_inst, &pCpnt->NCA9595_in.Word, NCA9595__InputPorts);

	if (err == ERROR_OK)	{ *in = pCpnt->NCA9595_in.Word; }

	return err;
}


FctERR NONNULL__ NCA9595_Write_Outputs(NCA9595_t * const pCpnt, const uint16_t out)
{
	const uint16_t data = out & ~pCpnt->cfg.NCA9595_Cfg.Word;	// Keep outputs only
	const FctERR err = NCA9595_Write_Word(pCpnt->cfg.slave_inst, &data, NCA9595__OutputPorts);

	if (err == ERROR_OK)	{ pCpnt->NCA9595_out.Word = data; }

	return err;
}

FctERR NONNULL__ NCA9595_Write_Outputs_Mask(NCA9595_t * const pCpnt, const uint16_t out, const uint16_t mask)
{
	const uint16_t final_mask = ~pCpnt->cfg.NCA9595_Cfg.Word & mask;

	uint16_t data = pCpnt->NCA9595_out.Word;
	SET_BITS_VAL(data, final_mask, out);

	FctERR err = NCA9595_Write_Word(pCpnt->cfg.slave_inst, &data, NCA9595__OutputPorts);

	if (err == ERROR_OK)	{ pCpnt->NCA9595_out.Word = data; }

	return err;
}


FctERR NONNULL__ NCA9595_Set_Config(NCA9595_t * const pCpnt, const uint16_t cfg)
{
	const FctERR err = NCA9595_Write_Word(pCpnt->cfg.slave_inst, &cfg, NCA9595__ConfigPorts);

	if (err == ERROR_OK)	{ pCpnt->cfg.NCA9595_Cfg.Word = cfg; }

	return err;
}

FctERR NONNULL__ NCA9595_Set_Config_Mask(NCA9595_t * const pCpnt, const uint16_t cfg, const uint16_t mask)
{
	uint16_t data = pCpnt->cfg.NCA9595_Cfg.Word;	// Instead of getting values from chip
	SET_BITS_VAL(data, mask, cfg);

	const FctERR err = NCA9595_Write_Word(pCpnt->cfg.slave_inst, &data, NCA9595__ConfigPorts);

	if (err == ERROR_OK)	{ pCpnt->cfg.NCA9595_Cfg.Word = data; }

	return err;
}


FctERR NONNULL__ NCA9595_Set_Polarity(NCA9595_t * const pCpnt, const uint16_t pol)
{
	return NCA9595_Write_Word(pCpnt->cfg.slave_inst, &pol, NCA9595__PolarityPorts);
}

FctERR NONNULL__ NCA9595_Set_Polarity_Mask(NCA9595_t * const pCpnt, const uint16_t pol, const uint16_t mask)
{
	return NCA9595_Write_Reg_Mask(pCpnt, pol, mask, NCA9595__PolarityPorts);
}


FctERR NONNULL__ NCA9595_Set_PullUp(NCA9595_t * const pCpnt, const uint16_t pullup)
{
	return NCA9595_Write_Word(pCpnt->cfg.slave_inst, &pullup, NCA9595__PullUpConfigPorts);
}

FctERR NONNULL__ NCA9595_Set_PullUp_Mask(NCA9595_t * const pCpnt, const uint16_t pullup, const uint16_t mask)
{
	return NCA9595_Write_Reg_Mask(pCpnt, pullup, mask, NCA9595__PullUpConfigPorts);
}


/****************************************************************/


__WEAK void NONNULL__ NCA9595_INT_GPIO_Init(NCA9595_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&pCpnt->cfg.INT_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK bool NONNULL__ NCA9595_INT_GPIO_Get(const NCA9595_t * const pCpnt) {
	return I2C_peripheral_GPIO_get(&pCpnt->cfg.INT_GPIO); }


/****************************************************************/
#endif
#endif
/****************************************************************/
