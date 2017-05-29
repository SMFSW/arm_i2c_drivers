/*!\file FM24C.c
** \author SMFSW
** \version v0.3
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief FM24C Driver code
** \details FM24C16B: 16-Kbit (2K * 8) Serial I2C F-RAM
**			FM24C04B: 4-Kbit (512 * 8) Serial I2C F-RAM
**/
/****************************************************************/
#include "FM24C.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_FM24C)
/****************************************************************/
#if defined(I2C_MB85RC256V) && !defined(NO_WARN_I2C_DRIVERS)
#warning "FM24CxxB -> Defined along with MB85RC256V: use with caution, might have same I2C addresses if on same I2C bus!!!"
#endif
/****************************************************************/
// std libs
#include <stdlib.h>
/****************************************************************/


I2C_slave FM24C_hal = { { pNull, I2C_ADDR(FM24C_BASE_ADDR), I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, false };


/****************************************************************/


FctERR FM24C_Init(void)
{
	I2C_slave_init(&FM24C_hal, I2C_FM24C, FM24C_BASE_ADDR, I2C_slave_timeout);
	return ERR_OK;
}


/****************************************************************/


FctERR FM24C_Write_Banked(uint8_t * data, uint16_t addr, uint8_t bank, uint16_t nb)
{
	uint16_t i2c_addr = FM24C_hal.cfg.addr + (bank << 1);

	if (!data)										{ return ERR_MEMORY; }		// Null pointer
	if (bank >= (FM24C_SIZE / FM24C_BANK_SIZE))		{ return ERR_RANGE; }		// Unknown bank
	if (addr >= FM24C_BANK_SIZE)					{ return ERR_RANGE; }		// Unknown address
	if ((addr + nb) > FM24C_BANK_SIZE)				{ return ERR_OVERFLOW; }	// Bank overflow

	I2C_set_busy(&FM24C_hal, true);

	FM24C_hal.status = HAL_I2C_Mem_Write(FM24C_hal.cfg.inst, i2c_addr, addr, FM24C_hal.cfg.mem_size, data, nb, FM24C_hal.cfg.timeout);

	I2C_set_busy(&FM24C_hal, false);
	return HALERRtoFCTERR(FM24C_hal.status);
}


FctERR FM24C_Read_Banked(uint8_t * data, uint16_t addr, uint8_t bank, uint16_t nb)
{
	uint16_t i2c_addr = FM24C_hal.cfg.addr + (bank << 1);

	if (!data)										{ return ERR_MEMORY; }		// Null pointer
	if (bank >= (FM24C_SIZE / FM24C_BANK_SIZE))		{ return ERR_RANGE; }		// Unknown bank
	if (addr >= FM24C_BANK_SIZE)					{ return ERR_RANGE; }		// Unknown address
	if ((addr + nb) > FM24C_BANK_SIZE)				{ return ERR_OVERFLOW; }	// Bank overflow

	I2C_set_busy(&FM24C_hal, true);

	FM24C_hal.status = HAL_I2C_Mem_Read(FM24C_hal.cfg.inst, i2c_addr, addr, FM24C_hal.cfg.mem_size, data, nb, FM24C_hal.cfg.timeout);

	I2C_set_busy(&FM24C_hal, false);
	return HALERRtoFCTERR(FM24C_hal.status);
}


FctERR FM24C_ReadWrite(uint8_t * data, uint16_t addr, uint16_t nb, bool wr)
{
	FctERR		err = ERR_OK;
	int			i;
	uint16_t	subaddr, bank, n;

	if (nb > FM24C_BANK_SIZE * 2)	{ return ERR_VALUE; }	// The function handle only one bank crossing

	div_t temp = div(addr, FM24C_BANK_SIZE);	// Divide address by bank size
	int16_t nbBank2 = max(0, ((int16_t) temp.rem + (int16_t) nb - (int16_t) FM24C_BANK_SIZE));	// Number of bytes for bank+1 (if bank crossing)
	int16_t nbBank1 = nb - nbBank2;																// Number of bytes for bank
	int nbloop = nbBank2 ? 2 : 1;																// Number of for loop iterations

	for (i = 0 ; i < nbloop ; i++)
	{
		n = i ? nbBank2 : nbBank1;
		subaddr = i ? 0 : temp.rem;
		bank = temp.quot + i;

		if (wr)		{ err = FM24C_Write_Banked(&data[nbBank1 * i], subaddr, bank, n); }	// Write
		if (err)	{ break; }																// Break if error occurred

		err = FM24C_Read_Banked(&data[nbBank1 * i], subaddr, bank, n);					// Read in all cases (simple read or write)
		if (err)	{ break; }																// Break if error occurred
	}

	return err;
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_FM24C in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/
