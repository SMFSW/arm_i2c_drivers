/*!\file I2C_peripheral.h
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
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


#define I2C_PERIPHERAL_SET_DEFAULTS(name, idx, address)					\
	memcpy(&name##_hal[idx], &name##_defaults, sizeof(I2C_slave_t));	\
	name##_hal[idx].cfg.addr = I2C_ADDR(address);						\
	name[idx].cfg.slave_inst = &name##_hal[idx];	//!< Macro to set working defaults for peripheral \b name with I2C address \b address on index \b idx


#define IS_I2C_PERIPHERAL(name, idx)	((idx) < I2C_##name##_NB)	//!< Macro for use with assert_param to check I2C \b idx is existing for \b name peripheral


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif	/* __I2C_PERIPHERAL_H__ */
/****************************************************************/
