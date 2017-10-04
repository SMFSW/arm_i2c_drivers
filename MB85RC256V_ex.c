/*!\file MB85RC256V_ex.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief MB85RC256V Driver extensions code
** \details MB85RC256V: 256-Kbit (32K * 8) I2C Memory FRAM
**/
/****************************************************************/
#include "MB85RC256V.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MB85RC256V)
/****************************************************************/
// std libs
#include <string.h>
/****************************************************************/


#define VAL_CLR	0xFF	//!< Mass erase value for MB85RC256V


/*!\brief Mass erase of MB85RC256V
** \return FctERR - error code
**/
FctERR MB85RC256V_Mass_Erase(void)
{
	FctERR	err = ERR_OK;
	uint8_t	bankData[32];
	memset(&bankData, VAL_CLR, sizeof(bankData));

	for (int i = 0 ; i < (MB85RC256V_SIZE / sizeof(bankData)) ; i++)
	{
		#if defined(HAL_IWDG_MODULE_ENABLED)
			HAL_IWDG_Refresh(&hiwdg);
		#endif
		err = MB85RC256V_Write(bankData, i * sizeof(bankData), sizeof(bankData));
		if (err) { break; }
	}

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
