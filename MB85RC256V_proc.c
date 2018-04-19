/*!\file MB85RC256V_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief MB85RC256V Driver procedures
** \details MB85RC256V: 256-Kbit (32K * 8) I2C Memory FRAM
**/
/****************************************************************/
#include "MB85RC256V.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MB85RC256V)
/****************************************************************/


MB85RC256V_proc MB85RC256V = { { &MB85RC256V_hal, 0, 0, 0 } };


/****************************************************************/


__WEAK FctERR MB85RC256V_Init_Sequence(void)
{
	FctERR err = ERROR_OK;
	
	err = MB85RC256V_Get_ID();
	if (err)	{ return err; }
	
	if (	(MB85RC256V.cfg.Manufacture_ID != MB85RC256V_MANUFACTURE_ID)
		||	(MB85RC256V.cfg.Product_ID != MB85RC256V_PRODUCT_ID))
	{ return ERROR_COMMON; }	// Unknown device
	
	return err;
}


/****************************************************************/


FctERR MB85RC256V_Get_ID(void)
{
	uint8_t	ID[3];
	FctERR err;
	
	err = MB85RC256V_Read_ID(ID);
	if (err)	{ return err; }
	
	MB85RC256V.cfg.Manufacture_ID = LSHIFT(ID[0], 4) + RSHIFT(ID[1], 4);
	MB85RC256V.cfg.Density = ID[1] & 0x0F;
	MB85RC256V.cfg.Product_ID = ID[2];
	
	return ERROR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
