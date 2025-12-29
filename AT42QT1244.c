/*!\file AT42QT1244.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief AT42QT1244 Driver
** \details AT42QT1244: 24-key QMatrix FMEA IEC/EN/UL60730 Touch Sensor
**/
/****************************************************************/
#include "AT42QT1244.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_AT42QT1244)

// std libs
#if AT42QT1244_CHECK_CRC
#include <stdlib.h>
#endif
// HARMcksL libs
#include "tick_utils.h"
/****************************************************************/


static const I2C_slave_t AT42QT1244_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };

static I2C_slave_t AT42QT1244_hal[I2C_AT42QT1244_NB];	//!< AT42QT1244 Slave structure


/****************************************************************/


FctERR NONNULL__ AT42QT1244_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	FctERR err;

	assert_param(IS_I2C_PERIPHERAL(AT42QT1244, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(AT42QT1244, idx);

	I2C_slave_init(&AT42QT1244_hal[idx], hi2c, devAddress, AT42QT1244_hal[idx].cfg.timeout);

	err = AT42QT1244_Init_Sequence(&AT42QT1244[idx]);

	if (err != ERROR_OK)	{ I2C_set_enable(&AT42QT1244_hal[idx], false); }
	else					{ err = init_Delay_Generator(); }

	return err;
}

FctERR AT42QT1244_Init_Single(void) {
	return AT42QT1244_Init(0, I2C_AT42QT1244, AT42QT1244_BASE_ADDR); }


/****************************************************************/


FctERR NONNULL__ AT42QT1244_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR err = ERROR_OK;

	if (!I2C_is_enabled(pSlave))						{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > AT42QT__SETUP_HOST_CRC_MSB + 1U)	{ err = ERROR_OVERFLOW; }	// More bytes than registers
	if (err != ERROR_OK)								{ goto ret; }

	I2C_set_busy(pSlave, true);
	// MemAddress then 0x00 has to be sent, trick is to tell MEMADD size is 16 bit and send addr as the MSB
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, LSHIFT(addr, 8U), I2C_MEMADD_SIZE_16BIT, (uint8_t *) data, nb, pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


FctERR NONNULL__ AT42QT1244_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb)
{
	FctERR err = ERROR_OK;

	if (!I2C_is_enabled(pSlave))						{ err = ERROR_DISABLED; }	// Peripheral disabled
	if ((addr + nb) > AT42QT__SETUP_HOST_CRC_MSB + 1U)	{ err = ERROR_OVERFLOW; }	// More bytes than registers
#if AT42QT1244_CHECK_CRC
	uint8_t * read = malloc(nb + 2U);
	if (read == NULL)									{ err = ERROR_MEMORY; }		// Memory allocation failed
#endif
	if (err != ERROR_OK)								{ goto ret; }

	uint8_t	preamble[2] = { addr, nb };

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Master_Transmit(pSlave->cfg.bus_inst, pSlave->cfg.addr, preamble, sizeof(preamble), pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	if (err == ERROR_OK)
	{
		Delay_us(150U);	// Have to wait for 150us
		I2C_set_busy(pSlave, true);
#if AT42QT1244_CHECK_CRC
		pSlave->status = HAL_I2C_Master_Receive(pSlave->cfg.bus_inst, pSlave->cfg.addr, read, nb + 2U, pSlave->cfg.timeout);
#else
		pSlave->status = HAL_I2C_Master_Receive(pSlave->cfg.bus_inst, pSlave->cfg.addr, data, nb, pSlave->cfg.timeout);
#endif
		err = HALERRtoFCTERR(pSlave->status);
		I2C_set_busy(pSlave, false);
	}

#if AT42QT1244_CHECK_CRC
	if (err == ERROR_OK)
	{
		// Checksum calculation
		uint16_t crc = AT42QT1244_crc16(0, RSHIFT(pSlave->cfg.addr, 1U));
		for (uintCPU_t i = 0 ; i < sizeof(preamble) ; i++)	{ crc = AT42QT1244_crc16(crc, preamble[i]); }
		for (uintCPU_t i = 0 ; i < nb ; i++)				{ crc = AT42QT1244_crc16(crc, read[i]); }

		// Copy to destination if crc is ok
		if (crc == MAKEWORD(read[nb], read[nb + 1U]))		{ UNUSED_RET memcpy(data, read, nb); }
		else												{ err = ERROR_CRC; }
	}

	free(read);
#endif

	ret:
	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
