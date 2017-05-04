/*!\file PCA9624.c
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief PCA9624 Driver code (8 channels 8b PWM driver)
**/
/****************************************************************/
#include "PCA9624.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9624)
/****************************************************************/
#if defined(I2C_PCA9685)
#warning "PCA9624 -> Multiple PCA96xx types: beware if using CALL addresses!!!"
#endif
/****************************************************************/


static I2C_slave	PCA9624 = { { pNull, PCA9624_BASE_ADDR, I2C_MEMADD_SIZE_8BIT, I2C_slave_timeout }, 0, HAL_OK, false };


/*!\brief Initialization of the PCA9624 peripheral
**/
FctERR PCA9624_Init(void)
{
	FctERR err = ERR_OK;
	uint8_t Data[2];

	I2C_slave_init(&PCA9624, I2C_PCA9624, PCA9624_BASE_ADDR, I2C_MEMADD_SIZE_8BIT, I2C_slave_timeout);

	// MODE1: Restart Enabled + Auto Increment + Respond to ALLCALL
	// MODE2: Dimming group control, Latch on STOP
	Data[0] = 0x81U;
	Data[1] = 0x05U;
	err = PCA9624_Write(Data, PCA9624__MODE1, sizeof(Data));
	return err;
}


/*!\brief I2C Write function for PCA9624
**
** \param[in] Buffer - pointer to write from
** \param[in] Addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR PCA9624_Write(uint8_t * Buffer, uint16_t Addr, uint16_t nb)
{
	if (Addr > PCA9624__ALLCALLADR)			{ return ERR_RANGE; }		// Unknown register
	if ((Addr + nb) > PCA9624__ALLCALLADR)	{ return ERR_OVERFLOW; }	// More bytes than registers

	PCA9624.status = HAL_I2C_Mem_Write(PCA9624.cfg.inst, PCA9624.cfg.addr, Addr, PCA9624.cfg.mem_size, Buffer, nb, PCA9624.cfg.timeout);
	return HALERRtoFCTERR(PCA9624.status);
}


/*!\brief I2C Read function for PCA9624
**
** \param[in] Buffer - pointer to read to
** \param[in] Addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR PCA9624_Read(uint8_t * Buffer, uint16_t Addr, uint16_t nb)
{
	if (Addr > PCA9624__ALLCALLADR)			{ return ERR_RANGE; }		// Unknown register
	if ((Addr + nb) > PCA9624__ALLCALLADR)	{ return ERR_OVERFLOW; }	// More bytes than registers

	PCA9624.status = HAL_I2C_Mem_Read(PCA9624.cfg.inst, PCA9624.cfg.addr, Addr, PCA9624.cfg.mem_size, Buffer, nb, PCA9624.cfg.timeout);
	return HALERRtoFCTERR(PCA9624.status);
}


/****************************************************************/
#else
#warning "You have to define I2C_PCA9624 in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/
