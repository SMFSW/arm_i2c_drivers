/*!\file FM24C.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief FM24C Driver
** \details FM24C16B: 16-Kbit (2K * 8) Serial I2C F-RAM
**			FM24C04B: 4-Kbit (512 * 8) Serial I2C F-RAM
**/
/****************************************************************/
#include "FM24C.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_FM24C)
/****************************************************************/
#if defined(I2C_MB85RC256V)
#warning "FM24CxxB -> Defined along with MB85RC256V: use with caution, might have same I2C addresses if on same I2C bus!!!"
#endif
/****************************************************************/
// std libs
#include <stdlib.h>
/****************************************************************/


static const I2C_slave_t FM24C_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FMP }, 0, HAL_OK, true, false };

I2C_slave_t FM24C_hal[I2C_FM24C_NB];
FM24C_t FM24C[I2C_FM24C_NB];


/****************************************************************/


FctERR NONNULL__ FM24C_Init(const uint8_t idx, const I2C_HandleTypeDef * hi2c, const uint16_t devAddress)
{
	FctERR err;

	assert_param(IS_I2C_PERIPHERAL(FM24C, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(FM24C, idx, devAddress);

	err = I2C_slave_init(&FM24C_hal[idx], hi2c, devAddress, I2C_slave_timeout);

	if (err)	{ I2C_set_enable(&FM24C_hal[idx], false); }

	return ERROR_OK;
}

FctERR FM24C_Init_Single(void) {
	return FM24C_Init(0, I2C_FM24C, FM24C_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ FM24C_Write_Banked(FM24C_t * pCpnt, const uint8_t * data, const uint16_t addr, const uint8_t bank, const uint16_t nb)
{
	I2C_slave_t * pSlave = pCpnt->cfg.slave_inst;

	if (!I2C_is_enabled(pSlave))				{ return ERROR_DISABLED; }	// Peripheral disabled
	if (bank >= (FM24C_SIZE / FM24C_BANK_SIZE))	{ return ERROR_RANGE; }		// Unknown bank
	if (addr >= FM24C_BANK_SIZE)				{ return ERROR_RANGE; }		// Unknown address
	if ((addr + nb) > FM24C_BANK_SIZE)			{ return ERROR_OVERFLOW; }	// Bank overflow

	uint16_t i2c_addr = pSlave->cfg.addr + (bank << 1);

	I2C_set_busy(pSlave, true);

	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, i2c_addr, addr, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);

	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ FM24C_Read_Banked(FM24C_t * pCpnt, uint8_t * data, const uint16_t addr, const uint8_t bank, const uint16_t nb)
{
	I2C_slave_t * pSlave = pCpnt->cfg.slave_inst;

	if (!I2C_is_enabled(pSlave))				{ return ERROR_DISABLED; }	// Peripheral disabled
	if (bank >= (FM24C_SIZE / FM24C_BANK_SIZE))	{ return ERROR_RANGE; }		// Unknown bank
	if (addr >= FM24C_BANK_SIZE)				{ return ERROR_RANGE; }		// Unknown address
	if ((addr + nb) > FM24C_BANK_SIZE)			{ return ERROR_OVERFLOW; }	// Bank overflow

	uint16_t i2c_addr = pSlave->cfg.addr + (bank << 1);

	I2C_set_busy(pSlave, true);

	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, i2c_addr, addr, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);

	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ FM24C_ReadWrite(FM24C_t * pCpnt, uint8_t * data, const uint16_t addr, const uint16_t nb, const bool wr)
{
	FctERR		err = ERROR_OK;
	uint16_t	subaddr, bank, n;

	if (nb > FM24C_BANK_SIZE * 2)	{ return ERROR_VALUE; }	// The function handle only one bank crossing

	div_t temp = div(addr, FM24C_BANK_SIZE);	// Divide address by bank size
	uint16_t nbBank2 = max(0, (temp.rem + nb - FM24C_BANK_SIZE));	// Number of bytes for bank+1 (if bank crossing)
	uint16_t nbBank1 = nb - nbBank2;								// Number of bytes for bank
	int nbloop = nbBank2 ? 2 : 1;									// Number of for loop iterations

	for (int i = 0 ; i < nbloop ; i++)
	{
		n = i ? nbBank2 : nbBank1;
		subaddr = i ? 0 : temp.rem;
		bank = temp.quot + i;

		if (wr)		{ err = FM24C_Write_Banked(pCpnt, &data[nbBank1 * i], subaddr, bank, n); }	// Write
		if (err)	{ break; }																	// Break if error occurred

		err = FM24C_Read_Banked(pCpnt, &data[nbBank1 * i], subaddr, bank, n);					// Read in all cases (simple read or write)
		if (err)	{ break; }																	// Break if error occurred
	}

	return err;
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_FM24C in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/
