/*!\file DRV2605L.c
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief TI Haptic Driver code
**/
/****************************************************************/
#include "DRV2605L.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_DRV2605L)
/****************************************************************/


static I2C_slave	DRV2605L = { { pNull, DRV2605L_BASE_ADDR, I2C_MEMADD_SIZE_8BIT, I2C_slave_timeout }, 0, HAL_OK, false };
//static uDRV_REG_MAP		DRV2605L_Mem;


/*!\brief Initialization of the PCA9624 peripheral
**/
FctERR DRV2605L_Init(void)
{
	I2C_slave_init(&DRV2605L, I2C_DRV2605L, DRV2605L_BASE_ADDR, I2C_MEMADD_SIZE_8BIT, I2C_slave_timeout);
	return ERR_OK;
}


/*!\brief I2C Write function for DRV2605L
** \param[in] Buffer - pointer to write from
** \param[in] Addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR DRV2605L_Write(uint8_t * Buffer, uint16_t Addr, uint16_t nb)
{
	if (Addr > DRV__LRA_RESONNANCE_PERIOD)			{ return ERR_RANGE; }		// Unknown register
	if ((Addr + nb) > DRV__LRA_RESONNANCE_PERIOD)	{ return ERR_OVERFLOW; }	// More bytes than registers

	DRV2605L.status = HAL_I2C_Mem_Write(DRV2605L.cfg.inst, DRV2605L.cfg.addr, Addr, DRV2605L.cfg.mem_size, Buffer, nb, DRV2605L.cfg.timeout);
	return HALERRtoFCTERR(DRV2605L.status);
}


/*!\brief I2C Read function for DRV2605L
**
** \param[in] Buffer - pointer to read to
** \param[in] Addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR DRV2605L_Read(uint8_t * Buffer, uint16_t Addr, uint16_t nb)
{
	if (Addr > DRV__LRA_RESONNANCE_PERIOD)			{ return ERR_RANGE; }		// Unknown register
	if ((Addr + nb) > DRV__LRA_RESONNANCE_PERIOD)	{ return ERR_OVERFLOW; }	// More bytes than registers

	DRV2605L.status = HAL_I2C_Mem_Read(DRV2605L.cfg.inst, DRV2605L.cfg.addr, Addr, DRV2605L.cfg.mem_size, Buffer, nb, DRV2605L.cfg.timeout);
	return HALERRtoFCTERR(DRV2605L.status);
}


/****************************************************************/
#else
#warning "You have to define I2C_DRV in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/
