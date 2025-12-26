/*!\file NCA9595_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief NCA9595 Driver procedures
** \details NCA9595: Low-voltage 16-bit I²C and SMBus I/O expander
**/
/****************************************************************/
#include "NCA9595.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_NCA9595)
/****************************************************************/
// std libs
//#include <stdlib.h>
/****************************************************************/


NCA9595_t NCA9595[I2C_NCA9595_NB] = { 0 };


/****************************************************************/


__WEAK FctERR NONNULL__ NCA9595_Init_Sequence(NCA9595_t * const pCpnt)
{
	UNUSED(pCpnt);
	return ERROR_OK;
}


/****************************************************************/


__WEAK FctERR NONNULL__ NCA9595_handler(NCA9595_t * const pCpnt)
{
	FctERR err = NCA9595_Read_Register(pCpnt, &pCpnt->NCA9595_in.Word, NCA9595__InputPorts);
	if (err != ERROR_OK)	{ goto ret; }

	if (pCpnt->cfg.NCA9595_Cfg.Word != 0xFFFF)	// Not all GPIOs set to inputs
	{
		err = NCA9595_Write_Outputs(pCpnt, pCpnt->NCA9595_out.Word);
		if (err != ERROR_OK)	{ goto ret; }
	}

	#if defined(VERBOSE)
		const uint8_t idx = pCpnt - NCA9595;
		printf("NCA9595 id%d: Inputs %x", idx, pCpnt->NCA9595_in.Word);
	#endif

	ret:
	return err;
}


__WEAK FctERR NONNULL__ NCA9595_handler_it(NCA9595_t * const pCpnt)
{
	FctERR err = ERROR_OK;

	if (NCA9595_INT_GPIO_Get(pCpnt))	{ err = NCA9595_handler(pCpnt); }

	return err;
}


FctERR NCA9595_handler_all(void)
{
	FctERR err = ERROR_OK;

	for (NCA9595_t * pCpnt = NCA9595 ; pCpnt < &NCA9595[I2C_NCA9595_NB] ; pCpnt++)
	{
		err |= NCA9595_handler(pCpnt);
	}

	return err;
}

FctERR NCA9595_handler_it_all(void)
{
	FctERR err = ERROR_OK;

	for (NCA9595_t * pCpnt = NCA9595 ; pCpnt < &NCA9595[I2C_NCA9595_NB] ; pCpnt++)
	{
		err |= NCA9595_handler_it(pCpnt);
	}

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
