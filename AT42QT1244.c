/*!\file AT42QT1244.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief AT42QT1244 Driver code
** \details AT42QT1244: 24-key QMatrix FMEA IEC/EN/UL60730 Touch Sensor
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


I2C_slave AT42QT1244_hal = { { pNull, I2C_ADDR(AT42QT1244_BASE_ADDR), I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };



/*!\brief 16bits CRC calculation for AT42QT1244
** \details 16bits crc calculation. Initial crc entry must be 0.
** The message is not augmented with 'zero' bits. polynomial = X16 + X15 + X2 + 1
** Repeat this function for each data block byte, folding the result back into the call parameter crc
** \param[in] crc - current crc value
** \param[in] data - new data for crc
** \return New CRC value
**/
static uint16_t crc16(uint16_t crc, uint8_t data)
{
	crc ^= (uint16_t) data * 0x100;
	for (int i = 7 ; i >= 0 ; i--)
	{
		if (crc & 0x8000)	{ crc = (crc << 1) ^ 0x1021; }
		else				{ crc <<= 1; }
	}

	return crc;
}


/****************************************************************/


FctERR AT42QT1244_Init(void)
{
	I2C_slave_init(&AT42QT1244_hal, I2C_AT42QT1244, AT42QT1244_BASE_ADDR, I2C_slave_timeout);

	FctERR err = AT42QT1244_Init_Sequence();
	if (err)	{ I2C_set_enable(&AT42QT1244_hal, false); }

	return err;
}


/****************************************************************/


FctERR AT42QT1244_Write(uint8_t * data, uint16_t addr, uint16_t nb)
{
	if (!I2C_is_enabled(&AT42QT1244_hal))				{ return ERR_DISABLED; }	// Peripheral disabled
	if (!data)											{ return ERR_MEMORY; }		// Null pointer
	if (addr > AT42QT__SETUP_HOST_CRC_MSB)				{ return ERR_RANGE; }		// Unknown register
	if ((addr + nb) > AT42QT__SETUP_HOST_CRC_MSB + 1)	{ return ERR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&AT42QT1244_hal, true);

	// TODO: check if trick works
	// MemAddress then 0x00 has to be sent, trick is to tell MEMADD size is 16 bit and send addr as the MSB
	AT42QT1244_hal.status = HAL_I2C_Mem_Write(AT42QT1244_hal.cfg.inst, AT42QT1244_hal.cfg.addr, addr * 0x100, I2C_MEMADD_SIZE_16BIT, data, nb, AT42QT1244_hal.cfg.timeout);

	I2C_set_busy(&AT42QT1244_hal, false);
	return HALERRtoFCTERR(AT42QT1244_hal.status);
}


FctERR AT42QT1244_Read(uint8_t * data, uint16_t addr, uint16_t nb)
{
	FctERR		err = ERR_OK;
	uint16_t	crc = 0;
	uint8_t		preamble[2] = { (uint8_t) addr, (uint8_t) nb };
	uint8_t *	tmp_read = malloc(nb + 2);

	if (!I2C_is_enabled(&AT42QT1244_hal))				{ return ERR_DISABLED; }	// Peripheral disabled
	if ((!data) || (!tmp_read))							{ return ERR_MEMORY; }		// Null pointer or not malloc failed
	if (addr > AT42QT__SETUP_HOST_CRC_MSB)				{ return ERR_RANGE; }		// Unknown register
	if ((addr + nb) > AT42QT__SETUP_HOST_CRC_MSB + 1)	{ return ERR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(&AT42QT1244_hal, true);

	AT42QT1244_hal.status = HAL_I2C_Master_Transmit(AT42QT1244_hal.cfg.inst, AT42QT1244_hal.cfg.addr, preamble, nb, AT42QT1244_hal.cfg.timeout);
	err = HALERRtoFCTERR(AT42QT1244_hal.status);

	// TODO: WAIT 150us to add??

	if (AT42QT1244_hal.status == HAL_OK) {
		AT42QT1244_hal.status = HAL_I2C_Master_Receive(AT42QT1244_hal.cfg.inst, AT42QT1244_hal.cfg.addr, tmp_read, nb + 2, AT42QT1244_hal.cfg.timeout);
		err = HALERRtoFCTERR(AT42QT1244_hal.status);
	}

	if (AT42QT1244_hal.status == HAL_OK) {
		// Checksum calculation
		for (int i = 0 ; i < nb ; i++)	{ crc = crc16(crc, tmp_read[i]); }
		// Copy to destination if crc is ok
		if (crc == MAKEWORD(tmp_read[nb], tmp_read[nb + 1]))	{ memcpy(data, tmp_read, nb); }
		else													{ err = ERR_CRC; }
	}

	free(tmp_read);
	I2C_set_busy(&AT42QT1244_hal, false);
	return err;
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_AT42QT1244 in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/
