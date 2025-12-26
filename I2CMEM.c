/*!\file I2CMEM.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief FRAM / EEPROM Driver
** \note Fully compatible between EEPROM / FRAM type components
** \note When EEPROM compatibility is not needed, buf_size at init can be set to \ref I2CMEM_WBUF_NONE for more efficiency
**/
/****************************************************************/
#include "I2CMEM.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_I2CMEM)
/****************************************************************/


static const I2C_slave_t I2CMEM_defaults = { { pNull, 0, I2C_eep_slave_timeout, 0, I2C_FMP }, 0, HAL_OK, true, false };

static I2C_slave_t I2CMEM_hal[I2C_I2CMEM_NB];		//!< I2CMEM Slave structure

I2CMEM_t I2CMEM[I2C_I2CMEM_NB] = { 0 };


/****************************************************************/


FctERR NONNULL__ I2CMEM_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress, const size_t size, const size_t buf_size)
{
	assert_param(IS_I2C_PERIPHERAL(I2CMEM, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(I2CMEM, idx);

	I2CMEM_hal[idx].cfg.mem_size = (size > 0x800) ? I2C_16B_REG : I2C_8B_REG;	// 8b register size for up to 2048 bytes, 16b otherwise

	I2CMEM[idx].cfg.chip_size = size;						// Chip size
	I2CMEM[idx].cfg.buf_size = buf_size ? buf_size : size;	// Write buffer size (typically 16 to 64 for EEPROM, no restriction for FRAM)

	I2C_slave_init(&I2CMEM_hal[idx], hi2c, devAddress, I2CMEM_hal[idx].cfg.timeout);

	return ERROR_OK;
}

FctERR I2CMEM_Init_Single(const size_t size, const size_t buf_size) {
	return I2CMEM_Init(0, I2C_I2CMEM, I2CMEM_BASE_ADDR, size, buf_size); }


/****************************************************************/

/*!\brief I2C Test if device is ready for I2CMEM
** \note Useful for EEPROM devices which have non-transparent write time to device (tests for 10ms max if device is ready)
**
** \param[in,out] hi2c - Pointer to I2C_HandleTypeDef structure
** \param[in] slave_addr - I2C slave address to check
** \return HAL_StatusTypeDef - HAL status
**/
static HAL_StatusTypeDef I2CMEM_IsDeviceReady(I2C_HandleTypeDef * hi2c, const uint8_t slave_addr)
{
	HAL_StatusTypeDef status;
	const uint32_t tickstart = HALTicks();

	do
	{
		status = HAL_I2C_IsDeviceReady(hi2c, slave_addr, 0xFFFFFFFFU, 2);	// Max retries and 2ms timeout (in case device is not connected)

		if (status == HAL_OK)
		{
			break;
		}
	}
	while (TPSINF_MS(tickstart, 10));	// 10ms timeout (test for write completion of eeprom area, around 5ms)

	return status;
}


/*!\brief I2C Bank Write function for I2CMEM
**
** \param[in] pCpnt - Pointer to I2CMEM component
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
static FctERR NONNULL__ I2CMEM_Write_Page(I2CMEM_t * const pCpnt, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	I2C_slave_t * const pSlave = pCpnt->cfg.slave_inst;

	uint16_t i2c_addr = pSlave->cfg.addr;
	uint16_t mem_addr = addr;

	if (pSlave->cfg.mem_size == I2C_8B_REG)
	{
		i2c_addr += (RSHIFT(addr, 7) & 0x0E);
		mem_addr &= 0xFF;
	}

	I2C_set_busy(pSlave, true);

	pSlave->status = I2CMEM_IsDeviceReady(pSlave->cfg.bus_inst, i2c_addr);
	if (pSlave->status == HAL_OK)
	{
		pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, i2c_addr, mem_addr, pSlave->cfg.mem_size, (uint8_t *) data, nb, pSlave->cfg.timeout);
	}

	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


/*!\brief I2C Bank Read function for I2CMEM
**
** \param[in] pCpnt - Pointer to I2CMEM component
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
static FctERR NONNULL__ I2CMEM_Read_Page(I2CMEM_t * const pCpnt, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	I2C_slave_t * const pSlave = pCpnt->cfg.slave_inst;

	uint16_t i2c_addr = pSlave->cfg.addr;
	uint16_t mem_addr = addr;

	if (pSlave->cfg.mem_size == I2C_8B_REG)
	{
		i2c_addr += (RSHIFT(addr, 7) & 0x0E);
		mem_addr &= 0xFF;
	}

	I2C_set_busy(pSlave, true);

	pSlave->status = I2CMEM_IsDeviceReady(pSlave->cfg.bus_inst, i2c_addr);
	if (pSlave->status == HAL_OK)
	{
		pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, i2c_addr, mem_addr, pSlave->cfg.mem_size, data, nb, pSlave->cfg.timeout);
	}

	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


/*!\brief I2C Read/Write pages function for I2CMEM
**
** \param[in] pCpnt - Pointer to I2CMEM component
** \param[in,out] data - pointer to read/write to/from
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \param[in] wr - 0: Read / 1: Write
** \return FctERR - error code
**/
static FctERR NONNULL__ I2CMEM_ReadWrite_Pages(I2CMEM_t * const pCpnt, uint8_t * const data, const uint16_t addr, const uint16_t nb, const bool wr)
{
	if (!I2C_is_enabled(pCpnt->cfg.slave_inst))	{ return ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > pCpnt->cfg.chip_size)		{ return ERROR_OVERFLOW; }	// More bytes than registers

	FctERR		err = ERROR_OK;
	size_t		data_len = nb;
	uint16_t	address = addr;
	uint8_t *	pData = data;
	size_t		page_size;

	if (wr)															{ page_size = pCpnt->cfg.buf_size; }
	else if (pCpnt->cfg.slave_inst->cfg.mem_size == I2C_16B_REG)	{ page_size = pCpnt->cfg.chip_size; }
	else															{ page_size = I2CMEM_BANK_SIZE; }

	while (data_len)
	{
		size_t nb_rw = page_size - (address % page_size);	// Compute possible page crossing access
		nb_rw = min(data_len, nb_rw);						// Choose between remaining data length or fitting page boundaries length

		if (wr)		{ err = I2CMEM_Write_Page(pCpnt, pData, address, nb_rw); }	// Write
		else		{ err = I2CMEM_Read_Page(pCpnt, pData, address, nb_rw); }	// Read
		if (err != ERROR_OK)	{ break; }

		data_len -= nb_rw;
		address += nb_rw;
		pData += nb_rw;
	}

	return err;
}


FctERR NONNULL__ I2CMEM_Write(I2CMEM_t * const pCpnt, const uint8_t * const data, const uint16_t addr, const uint16_t nb)
{
	return I2CMEM_ReadWrite_Pages(pCpnt, (uint8_t * const) data, addr, nb, true);
}


FctERR NONNULL__ I2CMEM_Read(I2CMEM_t * const pCpnt, uint8_t * const data, const uint16_t addr, const uint16_t nb)
{
	return I2CMEM_ReadWrite_Pages(pCpnt, data, addr, nb, false);
}


/****************************************************************/
#endif
#endif
/****************************************************************/
