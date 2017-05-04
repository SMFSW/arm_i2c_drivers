/*!\file PCA9624_ex.c
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief PCA9624 Driver extensions code (8 channels 8b PWM driver)
**/
/****************************************************************/
#include "PCA9624.h"
#include "globals.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_PCA9624)
/****************************************************************/
// std libs
#include <stdlib.h>
/****************************************************************/
// TODO: doxygen


extern I2C_slave PCA9624;


FctERR PCA9624_Set_Latch(PCA96xx_latch latch)
{
	FctERR				err;
	uPCA9624_REG__MODE2	mode2;

	if (latch > PCA96xx__LATCH_ON_ACK)	{ return ERR_VALUE; }	// Unknown latch mode

	err = PCA9624_Read((uint8_t *) &mode2, PCA9624__MODE2, 1);
	if (err)	{ return err; }

	mode2.Bits.OCH = latch;
	return PCA9624_Write((uint8_t *) &mode2, PCA9624__MODE2, 1);
}

FctERR PCA9624_Set_Mode_LED(PCA96xx_chan Chan, PCA9624_ledout mode)
{
	uint8_t					idx = (Chan > PCA96xx__PWM4) ? 1 : 0;
	FctERR					err;
	uPCA9624_REG__LEDOUT0	led;

	if ((Chan < PCA96xx__PWM1) && (Chan > PCA96xx__PWM8))	{ return ERR_RANGE; }	// Unknown channel
	if (mode > PCA9624__GROUP_BRIGHT)						{ return ERR_VALUE; }	// Unknown control mode

	err = PCA9624_Read((uint8_t *) &led, PCA9624__LEDOUT0 + idx, 1);
	if (err)	{ return err; }

	switch (Chan)
	{
		case PCA96xx__PWM1:
		case PCA96xx__PWM5:
			led.Bits.LDR0 = mode;
			break;

		case PCA96xx__PWM2:
		case PCA96xx__PWM6:
			led.Bits.LDR1 = mode;
			break;

		case PCA96xx__PWM3:
		case PCA96xx__PWM7:
			led.Bits.LDR2 = mode;
			break;

		case PCA96xx__PWM4:
		case PCA96xx__PWM8:
			led.Bits.LDR3 = mode;
			break;

		default:
			break;
	}

	return PCA9624_Write((uint8_t *) &led, PCA9624__LEDOUT0 + idx, 1);
}

FctERR PCA9624_Set_Mode_LEDs(uint8_t Chans, PCA9624_ledout mode)
{
	FctERR					err;
	uPCA9624_REG__LEDOUT0	led[2];

	if (!Chans)							{ return ERR_OK; }		// Nothing to do
	if (mode > PCA9624__GROUP_BRIGHT)	{ return ERR_VALUE; }	// Unknown control mode

	err = PCA9624_Read((uint8_t *) led, PCA9624__LEDOUT0, 2);
	if (err)	{ return err; }

	for (int i = PCA96xx__PWM1, j = 1 ; i <= PCA96xx__PWM8 ; i++, j <<= 1)
	{
		if (Chans & j)
		{
			div_t tmp = div(i, 4);

			if (tmp.rem == 0)		{ led[tmp.quot].Bits.LDR0 = mode; }
			else if (tmp.rem == 1)	{ led[tmp.quot].Bits.LDR1 = mode; }
			else if (tmp.rem == 2)	{ led[tmp.quot].Bits.LDR2 = mode; }
			else if (tmp.rem == 3)	{ led[tmp.quot].Bits.LDR3 = mode; }
		}
	}

	return PCA9624_Write((uint8_t *) led, PCA9624__LEDOUT0, 2);
}


/*!\brief Reset of the PCA9624 peripheral
** \param [in] all - Reset all devices
**/
FctERR PCA9624_Reset(bool all)
{
	uint8_t Data = 0x06;
	PCA9624.status = HAL_I2C_Master_Transmit(PCA9624.cfg.inst, all ? PCA96xx_GENERAL_CALL_ADDR : PCA9624.cfg.addr, &Data, 1, PCA9624.cfg.timeout);
	return HALERRtoFCTERR(PCA9624.status);
}



/*!\brief Reads I2C register from PCA9624
**
** \param [in] Reg - Register address to read from
** \param [in,out] Value - Pointer to the data for receive
** \return FctERR - ErrorCode
** \retval ERR_OK - OK
** \retval ERR_RANGE - Wrong Chan number
** \retval Some others possible from called func
**/
FctERR PCA9624_ReadRegister(PCA9624_reg Reg, uint8_t * Value)
{
	*Value = 0;

	if (Reg > PCA9624__ALLCALLADR)		{ return ERR_RANGE; }		// Unknown register

	return PCA9624_Read(Value, Reg, 1);
}


/*!\brief Reads I2C lighting values from a LED (4 bytes) and Computes the corresponding duty cycle value (uint8_t)
**
** \param [in] Chan - channel number
** \param [in,out] DutyCycle - Pointer to the data for receive coded on a uint8_t
** \return FctERR - ErrorCode
** \retval ERR_OK - OK
** \retval ERR_RANGE - Wrong channel number
** \retval Some others possible from called func
**/
FctERR PCA9624_ReadVal(PCA96xx_chan Chan, uint8_t * DutyCycle)
{
	*DutyCycle = 0;

	if ((Chan < PCA96xx__PWM1) && (Chan > PCA96xx__PWM8))	{ return ERR_RANGE; }	// Unknown channel

	return PCA9624_Read(DutyCycle, PCA9624__PWM0 + Chan - 1, 1);
}


/*!\brief Computes and send I2C lighting values to apply to a particular or all channels of the PCA9624
**
** \param [in] Chan - channel number
** \param [in] DutyCycle - Duty cycle coded on a uint8_t
** \return FctERR - ErrorCode
** \retval ERR_OK - OK
** \retval ERR_RANGE - Wrong channel number
**/
FctERR PCA9624_PutVal(PCA96xx_chan Chan, uint8_t DutyCycle)
{
	if ((Chan < PCA96xx__PWM1) && (Chan > PCA96xx__PWM8))	{ return ERR_RANGE; }	// Unknown channel

	return PCA9624_Write(&DutyCycle, PCA9624__PWM0 + Chan - 1, 1);
}

/*!\brief Sends I2C lighting ON values to apply to a particular or all channels of the PCA9624
**
** \param [in] Chan - channel number
** \return FctERR - ErrorCode
** \retval ERR_OK - OK
** \retval ERR_RANGE - Wrong channel number
**/
FctERR PCA9624_SetVal(PCA96xx_chan Chan)
{
	const uint8_t Val = 0xFF;

	if ((Chan < PCA96xx__PWM1) && (Chan > PCA96xx__PWM8))	{ return ERR_RANGE; }	// Unknown channel

	return PCA9624_Write((uint8_t *) &Val, PCA9624__PWM0 + Chan - 1, 1);
}


/*!\brief Sends I2C PWm OFF values to apply to a particular or all channels of the PCA9624
**
** \param [in] Chan - channel number
** \return FctERR - ErrorCode
** \retval ERR_OK - OK
** \retval ERR_RANGE - Wrong channel number
**/
FctERR PCA9624_ClrVal(PCA96xx_chan Chan)
{
	const uint8_t Val = 0;

	if ((Chan < PCA96xx__PWM1) && (Chan > PCA96xx__PWM8))	{ return ERR_RANGE; }	// Unknown channel

	return PCA9624_Write((uint8_t *) &Val, PCA9624__PWM0 + Chan - 1, 1);
}


/****************************************************************/
#endif
#endif
/****************************************************************/
