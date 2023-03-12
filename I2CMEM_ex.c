/*!\file I2CMEM_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief FRAM / EEPROM Driver extensions
** \note Fully compatible between EEPROM / FRAM type components
** \note When EEPROM compatibility is not needed, buf_size at init can be set to \ref I2CMEM_WBUF_NONE for more efficiency
**/
/****************************************************************/
#include "I2CMEM.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_I2CMEM)
/****************************************************************/


FctERR NONNULL__ I2CMEM_Mass_Erase(I2CMEM_t * const pCpnt)
{
	FctERR	err = ERROR_OK;
	uint8_t	array[I2CMEM_BANK_SIZE];
	memset(array, I2CMEM_CLR_VAL, sizeof(array));

	for (int i = 0 ; i < (pCpnt->cfg.chip_size / sizeof(array)) ; i++)
	{
		#if defined(HAL_IWDG_MODULE_ENABLED)
			HAL_IWDG_Refresh(&hiwdg);
		#endif
		err = I2CMEM_Write(pCpnt, array, i * sizeof(array), sizeof(array));
		if (err) { break; }
	}

	return err;
}


/****************************************************************/


__WEAK FctERR NONNULL__ I2CMEM_WP_GPIO_Init(I2CMEM_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	return I2C_peripheral_GPIO_init(&pCpnt->cfg.WP_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK FctERR NONNULL__ I2CMEM_WP_GPIO_Set(I2CMEM_t * const pCpnt, const bool state) {
	return I2C_peripheral_GPIO_set(&pCpnt->cfg.WP_GPIO, state); }


/****************************************************************/
#endif
#endif
/****************************************************************/
