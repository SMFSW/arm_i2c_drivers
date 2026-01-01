/*!\file FM24C.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief FM24C FRAM Driver (bank switching at I2C address level protocol)
** \details FM24C16B: 16-Kbit (2K * 8) Serial I2C F-RAM
**			FM24C04B: 4-Kbit (512 * 8) Serial I2C F-RAM
** \note Legacy component: consider switching to I2CMEM component for new designs
** \note Compatibility (tested):
**				- FM24C16B
**				- FM24C04B
**				- BR24T04FVM
**/
/****************************************************************/
#include "FM24C.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_FM24C)
/****************************************************************/


static const I2C_slave_t FM24C_defaults = { { pNull, 0, I2C_eep_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FMP }, 0, HAL_OK, true, false };

static I2C_slave_t FM24C_hal[I2C_FM24C_NB];		//!< FM24C Slave structure

FM24C_t FM24C[I2C_FM24C_NB] = { 0 };

#define MAX_RETRIES		0xFFFFFFFFU				//!< Maximum number retries


/****************************************************************/


FctERR NONNULL__ FM24C_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	assert_param(IS_I2C_PERIPHERAL(FM24C, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(FM24C, idx);

	I2C_slave_init(&FM24C_hal[idx], hi2c, devAddress, FM24C_hal[idx].cfg.timeout);

	return ERROR_OK;
}

FctERR FM24C_Init_Single(void) {
	return FM24C_Init(0, I2C_FM24C, FM24C_BASE_ADDR); }


/****************************************************************/


/*!\brief I2C Bank Write function for FM24C
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
	const uint16_t i2c_addr = pSlave->cfg.addr + (bank << 1U);

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, i2c_addr, addr, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


/*!\brief I2C Bank Read function for FM24C
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
	const uint16_t i2c_addr = pSlave->cfg.addr + (bank << 1U);

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

	if (!I2C_is_enabled(pCpnt->cfg.slave_inst))	{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > FM24C_SIZE)				{ err = ERROR_OVERFLOW; }	// More bytes than registers
	if (err != ERROR_OK)						{ goto ret; }

	size_t		data_len = nb;
	uint16_t	address = addr;
	uint8_t *	pData = data;

	while (data_len != 0)
	{
		size_t nb_rw = FM24C_BANK_SIZE - (address % FM24C_BANK_SIZE);	// Compute possible page/bank crossing access
		nb_rw = min(data_len, nb_rw);									// Choose between page/bank size data length max, or remaining data page/bank length

		if (wr)					{ err = FM24C_Write_Banked(pCpnt, pData, (uint8_t) address, RSHIFT(address, 8U), nb_rw); }	// Write
		else					{ err = FM24C_Read_Banked(pCpnt, pData, (uint8_t) address, RSHIFT(address, 8U), nb_rw); }	// Read

		if (err != ERROR_OK)	{ break; }

		data_len -= nb_rw;
		address += nb_rw;
		pData += nb_rw;
	}

	ret:
	return err;
}


FctERR NONNULL__ FM24C_Write(FM24C_t * const pCpnt, const uint8_t * const data, const uint16_t addr, const uint16_t nb)
{
	return FM24C_ReadWrite_Banked(pCpnt, (uint8_t * const) data, addr, nb, true);
}


FctERR NONNULL__ FM24C_Read(FM24C_t * const pCpnt, uint8_t * const data, const uint16_t addr, const uint16_t nb)
{
	return FM24C_ReadWrite_Banked(pCpnt, data, addr, nb, false);
}


/****************************************************************/
#endif
#endif
/****************************************************************/
