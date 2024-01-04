/*!\file I2C_peripheral.c
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
** \brief I2C peripheral common
**/
/****************************************************************/
#include "I2C_peripheral.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


FctERR NONNULL__ I2C_peripheral_GPIO_init(PeripheralGPIO_t * const pGPIO, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active)
{
	/* Check the parameters */
	assert_param(IS_GPIO_PIN(GPIO_Pin));
	assert_param(IS_GPIO_PIN_ACTION(GPIO_Active));

	pGPIO->GPIOx = GPIOx;
	pGPIO->GPIO_Pin = GPIO_Pin;
	pGPIO->GPIO_Active = GPIO_Active;

	return ERROR_OK;
}

FctERR NONNULL__ I2C_peripheral_GPIO_get(const PeripheralGPIO_t * const pGPIO, bool * const pState)
{
	if (!pGPIO->GPIOx)	{ return ERROR_INSTANCE; }

	const GPIO_PinState pin = HAL_GPIO_ReadPin(pGPIO->GPIOx, pGPIO->GPIO_Pin);
	*pState = binEval(pin == pGPIO->GPIO_Active);

	return ERROR_OK;
}

FctERR NONNULL__ I2C_peripheral_GPIO_set(const PeripheralGPIO_t * const pGPIO, const bool state)
{
	if (!pGPIO->GPIOx)	{ return ERROR_INSTANCE; }

	const GPIO_PinState val = state ^ (pGPIO->GPIO_Active ? 0 : 1);
	HAL_GPIO_WritePin(pGPIO->GPIOx, pGPIO->GPIO_Pin, val);

	return ERROR_OK;
}


/****************************************************************/
#endif
/****************************************************************/
