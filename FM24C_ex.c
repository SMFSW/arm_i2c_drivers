/*!\file FM24C_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief FM24C Driver extensions
** \details FM24C16B: 16-Kbit (2K * 8) Serial I2C F-RAM
**			FM24C04B: 4-Kbit (512 * 8) Serial I2C F-RAM
** \note	Compatibility (tested):
**				- FM24C16B
**				- FM24C04B
**				- BR24T04FVM
** \note	Compatibility:
**				- other components using same i2c protocol may be compatible
**/
/****************************************************************/
#include "FM24C.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_FM24C)
/****************************************************************/


#define VAL_CLR	0xFF	//!< Mass erase value for FM24C


FctERR NONNULL__ FM24C_Mass_Erase(FM24C_t * pCpnt)
{
	FctERR	err;
	uint8_t	bankData[FM24C_BANK_SIZE];
	memset(&bankData, VAL_CLR, sizeof(bankData));

	for (int i = 0 ; i < (FM24C_SIZE / FM24C_BANK_SIZE) ; i++)
	{
		#if defined(HAL_IWDG_MODULE_ENABLED)
			HAL_IWDG_Refresh(&hiwdg);
		#endif
		err = FM24C_Write_Banked(pCpnt, bankData, 0, i, FM24C_BANK_SIZE);
		if (err) { break; }
	}

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
