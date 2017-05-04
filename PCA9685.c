/*!\file PCA9685.c
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief PCA9685 Driver code (16 channels 16b PWM driver)
**/
/****************************************************************/
#include "PCA9685.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9685)
/****************************************************************/
#if defined(I2C_PCA9624)
#warning "PCA9685 -> Multiple PCA96xx types: beware if using CALL addresses!!!"
#endif
/****************************************************************/


static I2C_slave	PCA9685 = { { pNull, PCA9685_BASE_ADDR, I2C_MEMADD_SIZE_8BIT, I2C_slave_timeout }, 0, HAL_OK, false };


/*!\brief Initialization of the PCA9685 peripheral
**			- Set slave PCA address
**			- Send values OFF + ON_delay (fixed) to ALL_LED
**			- Send sleep & respond to ALL_CALL
**			- Send prescaler (only available when in sleep mode)
**			- Send wake
**/
FctERR PCA9685_Init(void)
{
	FctERR err = ERR_OK;
	uint8_t Data[5];

	I2C_slave_init(&PCA9685, I2C_PCA9685, PCA9685_BASE_ADDR, I2C_MEMADD_SIZE_8BIT, I2C_slave_timeout);

	// Set Delay time & all led OFF
	Data[0] = DefValDelayON;	// ALL_LED_ON_L (1% delay to write once)
	Data[1] = 0x00U;			// ALL_LED_ON_H
	Data[2] = 0x00U;			// ALL_LED_OFF_L
	Data[3] = DefBitFullOnOff;	// ALL_LED_OFF_H (b4 pour LED full OFF)
	err = PCA9685_Write(Data, PCA9685__ALL_LED_ON_L, 4);
	if (err)	{ return err; }

	// MODE1: SLEEP + Respond to ALLCALL
	Data[0] = 0x11U;
	err = PCA9685_Write(Data, PCA9685__MODE1, 1);
	if (err)	{ return err; }

	// Send prescaler to obtain desired frequency (only in SLEEP)
	Data[0] = PCA9685_Get_PWM_Prescaler(PCA9685_FREQ);
	err = PCA9685_Write(Data, PCA9685__PRE_SCALE, 1);
	if (err)	{ return err; }

	// MODE1: Restart Enabled + Auto Increment + Respond to ALLCALL
	Data[0] = 0xA1U;
	err = PCA9685_Write(Data, PCA9685__MODE1, 1);
	return err;
}


/*!\brief I2C Write function for PCA9685
**
** \param[in] Buffer - pointer to write from
** \param[in] Addr - Address to write to
** \param[in] NbChar - Number of bytes to write
** \return FctERR - error code
**/
FctERR PCA9685_Write(uint8_t * Buffer, uint16_t Addr, uint16_t nb)
{
	if (Addr > PCA9685__TestMode)			{ return ERR_RANGE; }		// Unknown register
	if ((Addr + nb) > PCA9685__TestMode)	{ return ERR_OVERFLOW; }	// More bytes than registers

	PCA9685.status = HAL_I2C_Mem_Write(PCA9685.cfg.inst, PCA9685.cfg.addr, Addr, PCA9685.cfg.mem_size, Buffer, nb, PCA9685.cfg.timeout);
	return HALERRtoFCTERR(PCA9685.status);
}


/*!\brief I2C Read function for PCA9685
**
** \param[in] Buffer - pointer to read to
** \param[in] Addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR PCA9685_Read(uint8_t * Buffer, uint16_t Addr, uint16_t nb)
{
	if (Addr > PCA9685__TestMode)			{ return ERR_RANGE; }		// Unknown register
	if ((Addr + nb) > PCA9685__TestMode)	{ return ERR_OVERFLOW; }	// More bytes than registers

	PCA9685.status = HAL_I2C_Mem_Read(PCA9685.cfg.inst, PCA9685.cfg.addr, Addr, PCA9685.cfg.mem_size, Buffer, nb, PCA9685.cfg.timeout);
	return HALERRtoFCTERR(PCA9685.status);
}


/****************************************************************/
#else
#warning "You have to define I2C_PCA9685 in globals.h with an I2C instance for this to work!"
#endif
#endif
/****************************************************************/
