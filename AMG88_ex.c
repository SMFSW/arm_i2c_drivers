/*!\file AMG88_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief AMG88 Driver extensions
** \details AMG88: Infrared Array Sensor (Grid-EYE)
**/
/****************************************************************/
#include "AMG88.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_AMG88)
/****************************************************************/
// std libs
#include <math.h>
/****************************************************************/


float AMG88_Convert_Thermistor_Raw(const uint16_t therm)
{
	uAMG88_REG__TEMP	TEMP;
	float				tmp;

	TEMP.Word = therm;
	tmp = TEMP.Bits.temp * 0.0625f;
	return TEMP.Bits.sign ? -tmp : tmp;
}

float AMG88_Convert_Pixel_Raw(const uint16_t pixel)
{
	// VAL is signed 12b, result done by shifting to 16b signed and multiplying by granularity (0.25�C) divided by 16
	return (float) ((int16_t) (pixel * 16) * 0.015625f);
}

uint16_t AMG88_Convert_Temp_To_Int(const float temp)
{
	uint16_t val = (uint16_t) (fabs(temp) / 0.25f);
	if (temp < 0.0f)	{ val |= 0x800; }
	return val;
}


FctERR NONNULL__ AMG88_Get_thermistor_temp(AMG88_t * const pCpnt, float * temp)
{
	FctERR		err;
	uint16_t	VAL;

	err = AMG88_Get_Thermistor_Raw(pCpnt, &VAL);
	if (err)	{ return err; }

	*temp = AMG88_Convert_Thermistor_Raw(VAL);

	return err;
}


FctERR NONNULL__ AMG88_Get_pixel_temp(AMG88_t * const pCpnt, float * temp, const uint8_t pixel)
{
	FctERR		err;
	uint16_t	VAL;

	err = AMG88_Get_Pixel_Raw(pCpnt, &VAL, pixel);
	if (err)	{ return err; }

	*temp = AMG88_Convert_Pixel_Raw(VAL);

	return err;
}

FctERR NONNULL__ AMG88_Get_pixels_temp(AMG88_t * const pCpnt, float temp[64])
{
	FctERR	err;
	uint8_t	raw[128];

	err = AMG88_Read(pCpnt->cfg.slave_inst, raw, AMG88__T01L, sizeof(raw));
	if (err)	{ return err; }

	for (uintCPU_t i = 0 ; i < sizeof(raw) / 2 ; i++)
	{
		uint16_t val = MAKEWORD(raw[i * 2], raw[(i * 2) + 1]);
		temp[i] = AMG88_Convert_Pixel_Raw(val);
	}

	return err;
}


FctERR NONNULL__ AMG88_Set_Interrupt_LVLH(AMG88_t * const pCpnt, const float temp)
{
	uint16_t	Temp;
	uint8_t		LEVEL[2];

	Temp = AMG88_Convert_Temp_To_Int(temp);

	LEVEL[0] = LOBYTE(Temp);
	LEVEL[1] = HIBYTE(Temp);

	return AMG88_Write(pCpnt->cfg.slave_inst, LEVEL, AMG88__INTHL, sizeof(LEVEL));
}

FctERR NONNULL__ AMG88_Set_Interrupt_LVLL(AMG88_t * const pCpnt, const float temp)
{
	uint16_t	Temp;
	uint8_t		LEVEL[2];

	Temp = AMG88_Convert_Temp_To_Int(temp);

	LEVEL[0] = LOBYTE(Temp);
	LEVEL[1] = HIBYTE(Temp);

	return AMG88_Write(pCpnt->cfg.slave_inst, LEVEL, AMG88__INTLL, sizeof(LEVEL));
}

FctERR NONNULL__ AMG88_Set_Interrupt_HYS(AMG88_t * const pCpnt, const float temp)
{
	uint16_t	Temp;
	uint8_t		LEVEL[2];

	Temp = AMG88_Convert_Temp_To_Int(temp);

	LEVEL[0] = LOBYTE(Temp);
	LEVEL[1] = HIBYTE(Temp);

	return AMG88_Write(pCpnt->cfg.slave_inst, LEVEL, AMG88__IHYSL, sizeof(LEVEL));
}

FctERR NONNULL__ AMG88_Set_Interrupt_Levels(AMG88_t * const pCpnt, const float temp_LVLH, const float temp_LVLL, const float temp_HYS)
{
	uint16_t	Temp;
	uint8_t		LEVEL[6];

	Temp = AMG88_Convert_Temp_To_Int(temp_LVLH);
	LEVEL[0] = LOBYTE(Temp);
	LEVEL[1] = HIBYTE(Temp);
	Temp = AMG88_Convert_Temp_To_Int(temp_LVLL);
	LEVEL[2] = LOBYTE(Temp);
	LEVEL[3] = HIBYTE(Temp);
	Temp = AMG88_Convert_Temp_To_Int(temp_HYS);
	LEVEL[4] = LOBYTE(Temp);
	LEVEL[5] = HIBYTE(Temp);

	return AMG88_Write(pCpnt->cfg.slave_inst, LEVEL, AMG88__INTHL, sizeof(LEVEL));
}


/****************************************************************/


__WEAK void NONNULL__ AMG88_INT_GPIO_Init(AMG88_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active) {
	I2C_peripheral_GPIO_init(&pCpnt->cfg.INT_GPIO, GPIOx, GPIO_Pin, GPIO_Active); }

__WEAK void NONNULL__ AMG88_INT_GPIO_Get(AMG88_t * const pCpnt, bool * const pState) {
	I2C_peripheral_GPIO_get(&pCpnt->cfg.INT_GPIO, pState); }


/****************************************************************/
#endif
#endif
/****************************************************************/
