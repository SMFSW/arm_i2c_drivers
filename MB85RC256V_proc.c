/*!\file MB85RC256V_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief MB85RC256V Driver procedures
** \details MB85RC256V: 256-Kbit (32K * 8) I2C Memory FRAM
** \note	Compatibility (tested):
**				- MB85RC256V
**				- FM24CL64B
** \note	Compatibility:
**				- other components using same i2c protocol may be compatible
**/
/****************************************************************/
#include "MB85RC256V.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MB85RC256V)
/****************************************************************/


/****************************************************************/


__WEAK FctERR NONNULL__ MB85RC256V_Init_Sequence(MB85RC256V_t * const pCpnt)
{
	FctERR err = ERROR_OK;

	err = MB85RC256V_Get_ID(pCpnt);
	if (err)	{ return err; }

	if (	(pCpnt->cfg.Manufacture_ID != MB85RC256V_MANUFACTURE_ID)
		||	(pCpnt->cfg.Product_ID != MB85RC256V_PRODUCT_ID))
	{ return ERROR_COMMON; }	// Unknown device

	return err;
}


/****************************************************************/


FctERR NONNULL__ MB85RC256V_Get_ID(MB85RC256V_t * const pCpnt)
{
	uint8_t	ID[3];
	FctERR	err;

	err = MB85RC256V_Read_ID(pCpnt, ID);
	if (err)	{ return err; }

	pCpnt->cfg.Manufacture_ID = LSHIFT(ID[0], 4) + RSHIFT(ID[1], 4);
	pCpnt->cfg.Density = ID[1] & 0x0F;
	pCpnt->cfg.Product_ID = ID[2];

	return ERROR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
