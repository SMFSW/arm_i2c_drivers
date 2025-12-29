/*!\file NCA9595.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief NCA9595 Driver
** \details NCA9595: Low-voltage 16-bit I²C and SMBus I/O expander
**/
/****************************************************************/
#include "NCA9595.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_NCA9595)
/****************************************************************/


static const I2C_slave_t NCA9595_defaults = { { pNull, 0, I2C_slave_timeout, I2C_MEMADD_SIZE_8BIT, I2C_FM }, 0, HAL_OK, true, false };

static I2C_slave_t NCA9595_hal[I2C_NCA9595_NB];	//!< NCA9595 Slave structure


/****************************************************************/


FctERR NONNULL__ NCA9595_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress)
{
	FctERR	err;

	assert_param(IS_I2C_PERIPHERAL(NCA9595, idx));

	I2C_PERIPHERAL_SET_DEFAULTS(NCA9595, idx);

	I2C_slave_init(&NCA9595_hal[idx], hi2c, devAddress, NCA9595_hal[idx].cfg.timeout);

	NCA9595[idx].cfg.NCA9595_Cfg.Word = 0xFFFFU;	// Set default as inputs
	err = NCA9595_Init_Sequence(&NCA9595[idx]);

	if (err != ERROR_OK)	{ I2C_set_enable(&NCA9595_hal[idx], false); }

	return err;
}

FctERR NCA9595_Init_Single(void) {
	return NCA9595_Init(0, I2C_NCA9595, NCA9595_BASE_ADDR); }

/****************************************************************/


FctERR NONNULL__ NCA9595_Write_Word(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr)
{
	FctERR err = ERROR_OK;

	if (!I2C_is_enabled(pSlave))			{ err = ERROR_DISABLED; }	// Peripheral disabled
#if defined(__BIG_ENDIAN__)
	if (isEven(addr))						{ err = ERROR_FRAMING; }	// Misaligned access for current endianness
	if (addr > NCA9595__PullUpConfigPort1)	{ err = ERROR_OVERFLOW; }	// More bytes than registers
#else
	if (isOdd(addr))						{ err = ERROR_FRAMING; }	// Misaligned access for current endianness
	if (addr > NCA9595__PullUpConfigPort0)	{ err = ERROR_OVERFLOW; }	// Outside registers area
#endif
	if (err != ERROR_OK)					{ goto ret; }

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Write(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, (uint8_t *) data, sizeof(uint16_t), pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


FctERR NONNULL__ NCA9595_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr)
{
	FctERR err = ERROR_OK;

	if (!I2C_is_enabled(pSlave))			{ err = ERROR_DISABLED; }	// Peripheral disabled
#if defined(__BIG_ENDIAN__)
	if (isEven(addr))						{ err = ERROR_FRAMING; }	// Misaligned access for current endianness
	if (addr > NCA9595__PullUpConfigPort1)	{ err = ERROR_OVERFLOW; }	// More bytes than registers
#else
	if (isOdd(addr))						{ err = ERROR_FRAMING; }	// Misaligned access for current endianness
	if (addr > NCA9595__PullUpConfigPort0)	{ err = ERROR_OVERFLOW; }	// Outside registers area
#endif
	if (err != ERROR_OK)					{ goto ret; }

	I2C_set_busy(pSlave, true);
	pSlave->status = HAL_I2C_Mem_Read(pSlave->cfg.bus_inst, pSlave->cfg.addr, addr, pSlave->cfg.mem_size, (uint8_t *) data, sizeof(uint16_t), pSlave->cfg.timeout);
	err = HALERRtoFCTERR(pSlave->status);
	I2C_set_busy(pSlave, false);

	ret:
	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
