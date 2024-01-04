/*!\file I2C_peripheral.h
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
** \brief I2C peripheral common
**/
/****************************************************************/
#ifndef __I2C_PERIPHERAL_H__
	#define __I2C_PERIPHERAL_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include <string.h>
#include "sarmfsw.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


#define IS_I2C_PERIPHERAL(name, idx)		((idx) < I2C_##name##_NB)							//!< Macro for use with assert_param to check I2C \b idx is existing for \b name peripheral

#define IS_I2C_PERIPHERAL_ADDR(name, addr)	((I2C_PERIPHERAL_IDX(addr, name) >= 0) && \
											(I2C_PERIPHERAL_IDX(addr, name) < I2C_##name##_NB))	//!< Macro for use with assert_param to check I2C peripheral \b addr is valid for \b name peripheral


#define I2C_PERIPHERAL_IDX(name, addr)		((int32_t) (((name##_t *) addr) - name))			//!< Macro to get I2C peripheral index given \b addr for \b name peripheral


/***********************************/
/*** Peripheral defaults setters ***/
/***********************************/

#define I2C_PERIPHERAL_SET_DEFAULTS(name, idx)							\
	memcpy(&name##_hal[idx], &name##_defaults, sizeof(I2C_slave_t));	\
	name[idx].cfg.slave_inst = &name##_hal[idx];	//!< Macro to set working defaults for peripheral \b name on index \b idx


#define I2C_PERIPHERAL_SET_DEFAULTS_SINGLETON(name)				\
	memcpy(&name##_hal, &name##_defaults, sizeof(I2C_slave_t));	\
	name.cfg.slave_inst = &name##_hal;				//!< Macro to set working defaults for singleton peripheral \b name


/******************************/
/*** Peripheral GPIO access ***/
/******************************/

/*! \struct PeripheralGPIO_t
**  \brief i2c peripheral GPIO pin parameters
**/
typedef struct PeripheralGPIO_t{
	GPIO_TypeDef *	GPIOx;			//!< HAL GPIO instance
	uint16_t		GPIO_Pin;		//!< HAL GPIO pin
	GPIO_PinState	GPIO_Active;	//!< HAL GPIO pin active state
} PeripheralGPIO_t;


/*!\brief I2C peripheral GPIO pin init
** \param[in,out] pGPIO - Pointer to GPIO structure instance
** \param[in] GPIOx - GPIO port
** \param[in] GPIO_Pin - GPIO pin
** \param[in] GPIO_Active: GPIO pin active state
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ I2C_peripheral_GPIO_init(PeripheralGPIO_t * const pGPIO, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active);

/*!\brief I2C peripheral GPIO pin getter
** \param[in] pGPIO - Pointer to GPIO structure instance
** \param[in,out] pState - Pointer to GPIO state variable (0: inactive, 1: active)
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ I2C_peripheral_GPIO_get(const PeripheralGPIO_t * const pGPIO, bool * const pState);

/*!\brief I2C peripheral GPIO pin setter
** \param[in] pGPIO - Pointer to GPIO structure instance
** \param[in] state - state to write on GPIO (0: inactive, 1: active)
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ I2C_peripheral_GPIO_set(const PeripheralGPIO_t * const pGPIO, const bool state);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif	/* __I2C_PERIPHERAL_H__ */
/****************************************************************/
