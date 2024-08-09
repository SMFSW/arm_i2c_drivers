/*!\file NCA9595_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
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
	FctERR	err;

	err = NCA9595_Read_Register(pCpnt, &pCpnt->NCA9595_in.Word, NCA9595__InputPorts);
	if (err)	{ return err; }

	if (pCpnt->cfg.NCA9595_Cfg.Word != 0xFFFF)	// Not all GPIOs set to inputs
	{
		err = NCA9595_Write_Outputs(pCpnt, pCpnt->NCA9595_out.Word);
		if (err)	{ return err; }
	}

	#if defined(VERBOSE)
		const uint8_t idx = pCpnt - NCA9595;
		printf("NCA9595 id%d: Inputs %x", idx, pCpnt->NCA9595_in.Word);
	#endif

	return ERROR_OK;
}


__WEAK FctERR NONNULL__ NCA9595_handler_it(NCA9595_t * const pCpnt)
{
	FctERR	err;
	bool	interrupt;

	err = NCA9595_INT_GPIO_Get(pCpnt, &interrupt);
	if ((!err) && interrupt)	{ err = NCA9595_handler(pCpnt); }

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
