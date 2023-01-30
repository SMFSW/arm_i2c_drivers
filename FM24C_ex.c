/*!\file FM24C_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief FRAM / EEPROM Driver (bank switching at I2C address level protocol) extensions
** \note The driver handles writing specificities for EEPROM type components
** \note The driver is fully compatible with FRAM type components
** \note When EEPROM / FRAM compatibility is not needed, FM24C_WRITE_SIZE can be set to FM24C_BANK_SIZE for more efficiency
** \details FM24C16B: 16-Kbit (2K * 8) Serial I2C F-RAM
**			FM24C04B: 4-Kbit (512 * 8) Serial I2C F-RAM
** \note	Compatibility (tested):
**				- FM24C16B
**				- FM24C04B
**				- BR24T04FVM
**				...
**/
/****************************************************************/
#include "FM24C.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_FM24C)
/****************************************************************/


#define VAL_CLR	0xFF	//!< Mass erase value for FM24C


FctERR NONNULL__ FM24C_Mass_Erase(FM24C_t * const pCpnt)
{
	FctERR	err = ERROR_OK;
	uint8_t	array[FM24C_BANK_SIZE];
	memset(array, VAL_CLR, sizeof(array));

	for (int i = 0 ; i < (FM24C_SIZE / sizeof(array)) ; i++)
	{
		#if defined(HAL_IWDG_MODULE_ENABLED)
			HAL_IWDG_Refresh(&hiwdg);
		#endif
		err = FM24C_Write(pCpnt, array, i * sizeof(array), sizeof(array));
		if (err) { break; }
	}

	return err;
}


/****************************************************************/


__WEAK FctERR NONNULL__ FM24C_WP_GPIO_Init(FM24C_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	return I2C_peripheral_GPIO_init(&pCpnt->cfg.WP_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK FctERR NONNULL__ FM24C_WP_GPIO_Set(FM24C_t * const pCpnt, const bool state) {
	return I2C_peripheral_GPIO_set(&pCpnt->cfg.WP_GPIO, state); }


/****************************************************************/
#endif
#endif
/****************************************************************/
