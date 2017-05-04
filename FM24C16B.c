/*!\file FM24C16B.c
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief FM24C16B Driver code
**/
/****************************************************************/
#include "FM24C16B.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_FM24C16B)
/****************************************************************/
// std libs
#include <stdlib.h>
/****************************************************************/
// TODO: doxygen


static I2C_slave	FM24C16B = { { pNull, FM24C16B_BASE_ADDR, I2C_MEMADD_SIZE_8BIT, I2C_slave_timeout }, 0, HAL_OK, false };


/*!\brief Initialization of the FM24C16B peripheral
**/
FctERR FM24C16B_Init(void)
{
	I2C_slave_init(&FM24C16B, I2C_FM24C16B, FM24C16B_BASE_ADDR, I2C_MEMADD_SIZE_8BIT, I2C_slave_timeout);
	return ERR_OK;
}


/*!\brief Fonction d'écriture d'octets en FRAM
**
** \param[in] Buffer - pointer to write from
** \param[in] Addr - Address to write to
** \param[in] Bank - Bank dans laquelle écrire
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR FM24C16B_Write_Banked(uint8_t * Buffer, uint16_t Addr, uint8_t Bank, uint16_t nb)
{
	uint16_t i2c_addr = FM24C16B.cfg.addr + (Bank << 1);

	if (Bank >= (FM24C16B_SIZE / FM24C16B_BANK_SIZE))	{ return ERR_RANGE; }		// Unknown bank
	if (Addr > FM24C16B_BANK_SIZE)						{ return ERR_RANGE; }		// Unknown address
	if ((Addr + nb) > FM24C16B_BANK_SIZE)				{ return ERR_OVERFLOW; }	// Bank overflow

	FM24C16B.status = HAL_I2C_Mem_Write(FM24C16B.cfg.inst, i2c_addr, Addr, FM24C16B.cfg.mem_size, Buffer, nb, FM24C16B.cfg.timeout);
	return HALERRtoFCTERR(FM24C16B.status);
}


/*!\brief Fonction de lecture d'octets en FRAM
**
** \param[in] Buffer - pointer to read to
** \param[in] Addr - Address to read from
** \param[in] Bank - Bank dans laquelle lire
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR FM24C16B_Read_Banked(uint8_t * Buffer, uint16_t Addr, uint8_t Bank, uint16_t nb)
{
	uint16_t i2c_addr = FM24C16B.cfg.addr + (Bank << 1);

	if (Bank >= (FM24C16B_SIZE / FM24C16B_BANK_SIZE))	{ return ERR_RANGE; }		// Unknown bank
	if (Addr > FM24C16B_BANK_SIZE)						{ return ERR_RANGE; }		// Unknown address
	if ((Addr + nb) > FM24C16B_BANK_SIZE)				{ return ERR_OVERFLOW; }	// Bank overflow

	FM24C16B.status = HAL_I2C_Mem_Read(FM24C16B.cfg.inst, i2c_addr, Addr, FM24C16B.cfg.mem_size, Buffer, nb, FM24C16B.cfg.timeout);
	return HALERRtoFCTERR(FM24C16B.status);
}


FctERR FM24C16B_ReadWrite(uint8_t * Buffer, uint16_t Addr, uint16_t nb, bool wr)
{
	FctERR		err = ERR_OK;
	int			i;
	uint16_t	addr, bank, n;

	if (nb > FM24C16B_BANK_SIZE * 2)	{ return ERR_VALUE; }	// The function handle only one bank crossing

	div_t temp = div(Addr, FM24C16B_BANK_SIZE);	// Divide address by bank size
	int16_t nbBank2 = max(0, ((int16_t) temp.rem + (int16_t) nb - (int16_t) FM24C16B_BANK_SIZE));	// Number of bytes for bank+1 (if bank crossing)
	int16_t nbBank1 = nb - nbBank2;																	// Number of bytes for bank
	int nbloop = nbBank2 ? 2 : 1;																	// Number of for loop iterations

	for (i = 0 ; i < nbloop ; i++)
	{
		n = i ? nbBank2 : nbBank1;
		addr = i ? 0 : temp.rem;
		bank = temp.quot + i;

		if (wr)		{ err = FM24C16B_Write_Banked(&Buffer[nbBank1 * i], addr, bank, n); }	// Write
		if (err)	{ break; }																// Break if error occured

		err = FM24C16B_Read_Banked(&Buffer[nbBank1 * i], addr, bank, n);					// Read in all cases (simple read or write)
		if (err)	{ break; }																// Break if error occured
	}

	return err;
}


/****************************************************************/
#else
#warning "You have to define I2C_FM24C16B in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/
