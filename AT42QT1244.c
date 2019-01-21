/*!\file AT42QT1244.c
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
** \brief AT42QT1244 Driver
** \details AT42QT1244: 24-key QMatrix FMEA IEC/EN/UL60730 Touch Sensor
**/
/****************************************************************/
#include "AT42QT1244.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_AT42QT1244)
/****************************************************************/
// std libs
#include <stdlib.h>
/****************************************************************/


static const I2C_slave_t AT42QT1244_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };

I2C_slave_t AT42QT1244_hal[I2C_AT42QT1244_NB];


/****************************************************************/


/*!\brief 16bits CRC calculation for AT42QT1244
** \details 16bits crc calculation. Initial crc entry must be 0.
** The message is not augmented with 'zero' bits. polynomial = X16 + X15 + X2 + 1
** Repeat this function for each data block byte, folding the result back into the call parameter crc
** \param[in] crc - current crc value
** \param[in] data - new data for crc
** \return New CRC value
**/
static uint16_t crc16(uint16_t crc, const uint8_t data)
{
	crc ^= (uint16_t) LSHIFT(data, 8);
	for (int i = 7 ; i >= 0 ; i--)
	{
		if (crc & 0x8000)	{ crc = (crc << 1) ^ 0x1021; }
		else				{ crc <<= 1; }
	}

	return crc;
}


/****************************************************************/


FctERR NONNULL__ AT42QT1244_Init(const uint8_t idx, const I2C_HandleTypeDef * hi2c, const uint16_t devAddress)
{
	FctERR err;

	assert_param(IS_I2C_PERIPHERAL(AT42QT1244, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(AT42QT1244, idx, devAddress);

	err = I2C_slave_init(&AT42QT1244_hal[idx], hi2c, devAddress, I2C_slave_timeout);
	if (!err)	{ err = AT42QT1244_Init_Sequence(&AT42QT1244[idx]); }

	if (err)	{ I2C_set_enable(&AT42QT1244_hal[idx], false); }

	return err;
}

FctERR AT42QT1244_Init_Single(void) {
	return AT42QT1244_Init(0, I2C_AT42QT1244, AT42QT1244_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ AT42QT1244_Write(I2C_slave_t * pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(pSlave))						{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > AT42QT__SETUP_HOST_CRC_MSB)				{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > AT42QT__SETUP_HOST_CRC_MSB + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	I2C_set_busy(pSlave, true);
	// TODO: check if trick works
	// MemAddress then 0x00 has to be sent, trick is to tell MEMADD size is 16 bit and send addr as the MSB
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, LSHIFT(addr, 8), I2C_MEMADD_SIZE_16BIT, (uint8_t *) data, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	return HALERRtoFCTERR(pSlave->status);
}


FctERR NONNULL__ AT42QT1244_Read(I2C_slave_t * pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	if (!I2C_is_enabled(pSlave))						{ return ERROR_DISABLED; }	// Peripheral disabled
	if (addr > AT42QT__SETUP_HOST_CRC_MSB)				{ return ERROR_RANGE; }		// Unknown register
	if ((addr + nb) > AT42QT__SETUP_HOST_CRC_MSB + 1)	{ return ERROR_OVERFLOW; }	// More bytes than registers

	FctERR		err;
	uint16_t	crc = 0;
	uint8_t		preamble[2] = { (uint8_t) addr, (uint8_t) nb };
	uint8_t *	tmp_read = malloc(nb + 2);

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Master_Transmit(pSlave->cfg.bus_inst, pSlave->cfg.addr, preamble, nb, pSlave->cfg.timeout);
	I2C_set_busy(pSlave, false);
	err = HALERRtoFCTERR(pSlave->status);

	// TODO: WAIT 150us to add??

	if (pSlave->status == HAL_OK)
	{
		pSlave->status = HAL_I2C_Master_Receive(pSlave->cfg.bus_inst, pSlave->cfg.addr, tmp_read, nb + 2, pSlave->cfg.timeout);
		err = HALERRtoFCTERR(pSlave->status);
	}

	if (pSlave->status == HAL_OK)
	{
		// Checksum calculation
		for (int i = 0 ; i < nb ; i++)	{ crc = crc16(crc, tmp_read[i]); }
		// Copy to destination if crc is ok
		if (crc == MAKEWORD(tmp_read[nb], tmp_read[nb + 1]))	{ memcpy(data, tmp_read, nb); }
		else													{ err = ERROR_CRC; }
	}

	free(tmp_read);
	return err;
}


/****************************************************************/
#elif !defined(NO_WARN_I2C_DRIVERS)
#warning "You have to define I2C_AT42QT1244 in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/
