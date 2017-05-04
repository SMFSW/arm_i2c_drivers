/*!\file MTCH6102.c
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Microchip Capacitive Driver code
**/
/****************************************************************/
#include "MTCH6102.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_MTCH6102)
/****************************************************************/
// std libs
#include <string.h>
/****************************************************************/


static I2C_slave	MTCH6102 = { { pNull, MTCH6102_BASE_ADDR, I2C_MEMADD_SIZE_8BIT, I2C_slave_timeout }, 0, HAL_OK, false };


/*!\brief Initialization of the MTCH6102 peripheral
**/
FctERR MTCH6102_Init(void)
{
	uint8_t MTCH_CORE[7] = { 0, 0, 0, 0, 0, 0, 0 };

	I2C_slave_init(&MTCH6102, I2C_MTCH6102, MTCH6102_BASE_ADDR, I2C_MEMADD_SIZE_8BIT, I2C_slave_timeout);

	return MTCH6102_Read(&MTCH_CORE[0], MTCH__FW_MAJOR, sizeof(MTCH_CORE));
}


/*!\brief I2C Write function for MTCH6102
** \param[in] Buffer - pointer to write from
** \param[in] Addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR MTCH6102_Write(uint8_t * Buffer, uint16_t Addr, uint16_t nb)
{
	if (Addr > MTCH__RAW_ADC_31)		{ return ERR_RANGE; }		// Unknown register
	if ((Addr + nb) > MTCH__RAW_ADC_31)	{ return ERR_OVERFLOW; }	// More bytes than registers

	MTCH6102.status = HAL_I2C_Mem_Write(MTCH6102.cfg.inst, MTCH6102.cfg.addr, Addr, MTCH6102.cfg.mem_size, Buffer, nb, MTCH6102.cfg.timeout);
	return HALERRtoFCTERR(MTCH6102.status);
}


/*!\brief I2C Read function for MTCH6102
**
** \param[in] Buffer - pointer to read to
** \param[in] Addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR MTCH6102_Read(uint8_t * Buffer, uint16_t Addr, uint16_t nb)
{
	if (Addr > MTCH__RAW_ADC_31)		{ return ERR_RANGE; }		// Unknown register
	if ((Addr + nb) > MTCH__RAW_ADC_31)	{ return ERR_OVERFLOW; }	// More bytes than registers

	MTCH6102.status = HAL_I2C_Mem_Read(MTCH6102.cfg.inst, MTCH6102.cfg.addr, Addr, MTCH6102.cfg.mem_size, Buffer, nb, MTCH6102.cfg.timeout);
	return HALERRtoFCTERR(MTCH6102.status);
}


/****************************************************************/
#else
#warning "You have to define I2C_MTCH6102 in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/

