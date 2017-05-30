/*!\file FM24C_ex.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief FM24C Driver extensions code
** \details FM24C16B: 16-Kbit (2K * 8) Serial I2C F-RAM
**			FM24C04B: 4-Kbit (512 * 8) Serial I2C F-RAM
**/
/****************************************************************/
#include "FM24C.h"
#include "globals.h"

#if defined(HAL_IWDG_MODULE_ENABLED)
#include "iwdg.h"
#endif

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_FM24C)
/****************************************************************/
// std libs
#include <string.h>
/****************************************************************/


#define VAL_CLR	0xFF	//!< Mass erase value for FM24C


FctERR FM24C_Mass_Erase(void)
{
	FctERR	err;
	int		i;
	uint8_t	bankData[FM24C_BANK_SIZE];
	memset(&bankData, VAL_CLR, sizeof(bankData));

	for (i = 0 ; i < (FM24C_SIZE / FM24C_BANK_SIZE) ; i++)
	{
		#if defined(HAL_IWDG_MODULE_ENABLED)
			HAL_IWDG_Refresh(&hiwdg);
		#endif
		err = FM24C_Write_Banked(bankData, 0, i, FM24C_BANK_SIZE);
		if (err) { break; }
	}

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
