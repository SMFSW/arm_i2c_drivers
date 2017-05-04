/*!\file AT42QT1244.c
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Atmel Capacitive Driver code
**/
/****************************************************************/
#include "AT42QT1244.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_AT42QT1244)
/****************************************************************/
// std libs
#include <stdlib.h>
#include <string.h>
/****************************************************************/


static I2C_slave	AT42QT1244 = { { pNull, AT42QT1244_BASE_ADDR, I2C_MEMADD_SIZE_8BIT, I2C_slave_timeout }, 0, HAL_OK, false };


/*!\brief 16bits CRC calculation for AT42QT1244
** \param[in] crc - current crc value
** \param[in] data - new data for crc
** \return New CRC value
** \details 16bits crc calculation. Initial crc entry must be 0.
** The message is not augmented with 'zero' bits. polynomial = X16 + X15 + X2 + 1
** Repeat this function for each data block byte, folding the result back into the call parameter crc
**/
static uint16_t crc16(uint16_t crc, uint8_t data)
{
	crc ^= (uint16_t) (data) * 0x100;
	for (int i = 7 ; i > 0 ; i--)
	{
		if (crc & 0x8000)	{ crc = (crc << 1) ^ 0x1021; }
		else				{ crc <<= 1; }
	}

	return crc;
}


/*!\brief Initialization of the AT42QT1244 peripheral
**/
FctERR AT42QT1244_Init(void)
{
	I2C_slave_init(&AT42QT1244, I2C_AT42QT1244, AT42QT1244_BASE_ADDR, I2C_MEMADD_SIZE_8BIT, I2C_slave_timeout);
	return ERR_OK;
}


/*!\brief I2C Write function for AT42QT1244
** \param[in] Buffer - pointer to write from
** \param[in] Addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR AT42QT1244_Write(uint8_t * Buffer, uint16_t Addr, uint16_t nb)
{
	if (Addr > AT42QT__SETUP_HOST_CRC_MSB)			{ return ERR_RANGE; }		// Unknown register
	if ((Addr + nb) > AT42QT__SETUP_HOST_CRC_MSB)	{ return ERR_OVERFLOW; }	// More bytes than registers

	// TODO: check if trick works
	// MemAddress then 0x00 has to be sent, trick is to tell MEMADD size is 16 bit and send Addr as the MSB
	AT42QT1244.status = HAL_I2C_Mem_Write(AT42QT1244.cfg.inst, AT42QT1244.cfg.addr, Addr * 0x100, I2C_MEMADD_SIZE_16BIT, Buffer, nb, AT42QT1244.cfg.timeout);
	return HALERRtoFCTERR(AT42QT1244.status);
}


/*!\brief I2C Read function for AT42QT1244
** \param[in] Buffer - pointer to read to
** \param[in] Addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR AT42QT1244_Read(uint8_t * Buffer, uint16_t Addr, uint16_t nb)
{
	uint16_t	crc = 0;
	uint8_t		preamble[2] = { (uint8_t) Addr, (uint8_t) nb };
	uint8_t *	tmp_read = malloc(nb + 2);

	if (!tmp_read)									{ return ERR_MEMORY; }		// Not enough RAM
	if (Addr > AT42QT__SETUP_HOST_CRC_MSB)			{ return ERR_RANGE; }		// Unknown register
	if ((Addr + nb) > AT42QT__SETUP_HOST_CRC_MSB)	{ return ERR_OVERFLOW; }	// More bytes than registers

	AT42QT1244.status = HAL_I2C_Master_Transmit(AT42QT1244.cfg.inst, AT42QT1244.cfg.addr, preamble, nb, AT42QT1244.cfg.timeout);

	// TODO: WAIT 150us to add??

	if (!AT42QT1244.status) {
		AT42QT1244.status = HAL_I2C_Master_Receive(AT42QT1244.cfg.inst, AT42QT1244.cfg.addr, tmp_read, nb + 2, AT42QT1244.cfg.timeout);
	}

	if (!AT42QT1244.status) {
		// Checksum calculation
		for (int i = 0 ; i < nb ; i++)	{ crc = crc16(crc, tmp_read[i]); }
		// Copy to destination if crc is ok
		if (crc == MAKEWORD(tmp_read[nb], tmp_read[nb + 1]))	{ memcpy(Buffer, tmp_read, nb); }
		else													{ free(tmp_read); return ERR_CRC; }
	}

	free(tmp_read);
	return HALERRtoFCTERR(AT42QT1244.status);
}


/****************************************************************/
#else
#warning "You have to define I2C_AT42QT1244 in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/
