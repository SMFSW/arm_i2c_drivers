/*!\file FM24C.c
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief FM24C Driver
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
#if defined(I2C_MB85RC256V)
#warning "FM24CxxB -> Defined along with MB85RC256V: use with caution, might have same I2C addresses if on same I2C bus!!!"
#endif
/****************************************************************/
// std libs
#include <stdlib.h>
/****************************************************************/


static const I2C_slave_t FM24C_defaults = { { pNull, 0, I2C_eep_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FMP }, 0, HAL_OK, true, false };

static I2C_slave_t FM24C_hal[I2C_FM24C_NB];		//!< FM24C Slave structure

FM24C_t FM24C[I2C_FM24C_NB] = { 0 };


/****************************************************************/


FctERR NONNULL__ FM24C_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	FctERR err;

	assert_param(IS_I2C_PERIPHERAL(FM24C, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(FM24C, idx);

	err = I2C_slave_init(&FM24C_hal[idx], hi2c, devAddress, FM24C_hal[idx].cfg.timeout);

	if (err)	{ I2C_set_enable(&FM24C_hal[idx], false); }

	return ERROR_OK;
}

FctERR FM24C_Init_Single(void) {
	return FM24C_Init(0, I2C_FM24C, FM24C_BASE_ADDR); }


/****************************************************************/


/*!\brief I2C Write function for FM24C
**
** \param[in] pCpnt - Pointer to FM24C component
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] bank - Bank to write
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
static FctERR NONNULL__ FM24C_Write_Banked(FM24C_t * const pCpnt, const uint8_t * data, const uint16_t addr, const uint8_t bank, const uint16_t nb)
{
	I2C_slave_t * const pSlave = pCpnt->cfg.slave_inst;

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


/*!\brief I2C Read function for FM24C
**
** \param[in] pCpnt - Pointer to FM24C component
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] bank - Bank to read
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
static FctERR NONNULL__ FM24C_Read_Banked(FM24C_t * const pCpnt, uint8_t * data, const uint16_t addr, const uint8_t bank, const uint16_t nb)
{
	I2C_slave_t * const pSlave = pCpnt->cfg.slave_inst;

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


/*!\brief I2C Read/Write function for FM24C
**
** \param[in] pCpnt - Pointer to FM24C component
** \param[in,out] data - pointer to read/write to/from
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \param[in] wr - 0: Read / 1: Write
** \return FctERR - error code
**/
static FctERR NONNULL__ FM24C_ReadWrite_Banked(FM24C_t * const pCpnt, uint8_t * const data, const uint16_t addr, const uint16_t nb, const bool wr)
{
	FctERR err = ERROR_OK;

	const div_t temp = div(addr, FM24C_BANK_SIZE);									// Divide address by bank size

	if (wr)		{ err = FM24C_Write_Banked(pCpnt, data, temp.rem, temp.quot, nb); }	// Write
	if (!err)	{ err = FM24C_Read_Banked(pCpnt, data, temp.rem, temp.quot, nb); }	// Read in all cases (simple read or write)

	return err;
}


FctERR NONNULL__ FM24C_Write(FM24C_t * const pCpnt, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR		err = ERROR_OK;
	size_t		data_len = nb;
	uint16_t	address = addr;

	while (data_len)
	{
		size_t nb_write = address % FM24C_BANK_SIZE;			// Compute unaligned address
		nb_write = min(data_len, (FM24C_BANK_SIZE - nb_write));	// Bank size data length max, or remaining data length

		err = FM24C_ReadWrite_Banked(pCpnt, (uint8_t *) data, address, nb_write, true);
		if (err)	{ break; }

		data_len -= nb_write;
		address += nb_write;
		data += nb_write;
	}

	return err;
}


FctERR NONNULL__ FM24C_Read(FM24C_t * const pCpnt, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR		err = ERROR_OK;
	size_t		data_len = nb;
	uint16_t	address = addr;

	while (data_len)
	{
		size_t nb_read = address % FM24C_BANK_SIZE;				// Compute unaligned address
		nb_read = min(data_len, (FM24C_BANK_SIZE - nb_read));	// Bank size data length max, or remaining data length

		err = FM24C_ReadWrite_Banked(pCpnt, data, address, nb_read, false);
		if (err)	{ break; }

		data_len -= nb_read;
		address += nb_read;
		data += nb_read;
	}

	return err;
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_FM24C for component to be enabled!"
#endif
#endif
/****************************************************************/
