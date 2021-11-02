/*!\file MB85RC256V_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief MB85RC256V Driver extensions
** \details MB85RC256V: 256-Kbit (32K * 8) I2C Memory FRAM
** \note	Compatibility (tested):
**				- MB85RC256V
**				- MB85RC256VL64B
** \note	Compatibility:
**				- other components using same i2c protocol may be compatible
**/
/****************************************************************/
#include "MB85RC256V.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MB85RC256V)
/****************************************************************/


#define VAL_CLR	0xFF	//!< Mass erase value for MB85RC256V


FctERR NONNULL__ MB85RC256V_Mass_Erase(MB85RC256V_t * const pCpnt)
{
	FctERR	err = ERROR_OK;
	uint8_t	bankData[32];
	memset(&bankData, VAL_CLR, sizeof(bankData));

	for (int i = 0 ; i < (MB85RC256V_SIZE / sizeof(bankData)) ; i++)
	{
		#if defined(HAL_IWDG_MODULE_ENABLED)
			HAL_IWDG_Refresh(&hiwdg);
		#endif
		err = MB85RC256V_Write(pCpnt, bankData, i * sizeof(bankData), sizeof(bankData));
		if (err) { break; }
	}

	return err;
}


/****************************************************************/


__WEAK FctERR NONNULL__ MB85RC256V_WP_GPIO_Init(MB85RC256V_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	return I2C_peripheral_GPIO_init(&pCpnt->cfg.WP_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK FctERR NONNULL__ MB85RC256V_WP_GPIO_Set(MB85RC256V_t * const pCpnt, const bool state) {
	return I2C_peripheral_GPIO_set(&pCpnt->cfg.WP_GPIO, state); }


/****************************************************************/
#endif
#endif
/****************************************************************/
