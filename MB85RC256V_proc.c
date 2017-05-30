/*!\file MB85RC256V_proc.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief MB85RC256V Driver procedures code
** \details MB85RC256V: 256-Kbit (32K * 8) I2C Memory FRAM
**/
/****************************************************************/
#include "MB85RC256V.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MB85RC256V)
/****************************************************************/


MB85RC256V_proc MB85RC256V = { 0, 0, 0 };


/****************************************************************/


FctERR MB85RC256V_Init_Sequence(void)
{
	FctERR err = ERR_OK;
	
	err = MB85RC256V_Get_ID();
	if (err)	{ return err; }
	
	if (	(MB85RC256V.Manufacture_ID != MB85RC256V_MANUFACTURE_ID)
		||	(MB85RC256V.Product_ID != MB85RC256V_PRODUCT_ID))
	{ return ERR_COMMON; }	// Unknown device
	
	return err;
}


/****************************************************************/


FctERR MB85RC256V_Get_ID(void)
{
	uint8_t	ID[3];
	FctERR err;
	
	err = MB85RC256V_Read_ID(ID);
	if (err)	{ return err; }
	
	MB85RC256V.Manufacture_ID = (ID[0] * 0x10) + (ID[1] / 0x10);
	MB85RC256V.Density = ID[1] & 0x0F;
	MB85RC256V.Product_ID = ID[2];
	
	return ERR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
