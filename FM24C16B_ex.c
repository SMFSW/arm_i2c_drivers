/*!\file FM24C16B_ex.c
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief FM24C16B Driver extensions code
**/
/****************************************************************/
#include "FM24C16B.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_FM24C16B)
/****************************************************************/
// std libs
#include <string.h>
/****************************************************************/


#define VAL_CLR	0xFF	//!< Mass erase value for FM24C16B

/*!\brief Mass erase of FM24C16B
** \return FctERR - error code
**/
FctERR FM24C16B_Mass_Erase(void)
{
	FctERR	err;
	int		i;
	uint8_t	bankData[FM24C16B_BANK_SIZE];
	memset(&bankData, VAL_CLR, sizeof(bankData));

	for (i = 0 ; i < (FM24C16B_SIZE / FM24C16B_BANK_SIZE) ; i++)
	{
		err = FM24C16B_Write_Banked(bankData, 0, i, FM24C16B_BANK_SIZE);
		if (err) { break; }
	}

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
