/*!\file MB85RC256V_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief MB85RC256V FRAM Driver extensions
** \details MB85RC256V: 256-Kbit (32K * 8) I2C Memory FRAM
** \note	Compatibility (tested):
**				- MB85RC256V
**				- MB85RC256VL64B
**/
/****************************************************************/
#include "MB85RC256V.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MB85RC256V)
/****************************************************************/


FctERR NONNULL__ MB85RC256V_Mass_Erase(MB85RC256V_t * const pCpnt)
{
	FctERR	err = ERROR_OK;
	uint8_t	bankData[256];
	const size_t sz_bank = sizeof(bankData);

	UNUSED_RET memset(&bankData, MB85RC256V_CLR_VAL, sz_bank);

	for (uintCPU_t i = 0 ; i < (MB85RC256V_SIZE / sz_bank) ; i++)
	{
		I2C_Watchdog_Refresh();
		err = MB85RC256V_Write(pCpnt, bankData, i * sz_bank, sz_bank);
		if (err != ERROR_OK) { break; }
	}

	return err;
}


/****************************************************************/


__WEAK void NONNULL__ MB85RC256V_WP_GPIO_Init(MB85RC256V_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&pCpnt->cfg.WP_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK void NONNULL__ MB85RC256V_WP_GPIO_Set(MB85RC256V_t * const pCpnt, const bool state) {
	I2C_peripheral_GPIO_set(&pCpnt->cfg.WP_GPIO, state); }


/****************************************************************/
#endif
#endif
/****************************************************************/
