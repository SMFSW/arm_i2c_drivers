/*!\file AMG88_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief AMG88 Driver procedures
** \details AMG88: Infrared Array Sensor (Grid-EYE)
**/
/****************************************************************/
#include "AMG88.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_AMG88)
/****************************************************************/


AMG88_t AMG88[I2C_AMG88_NB] = { 0 };


/****************************************************************/


__WEAK FctERR NONNULL__ AMG88_Init_Sequence(AMG88_t * const pCpnt)
{
	FctERR					err;
	uAMG88_REG__INT_CONTROL	INT_CTRL = { 0 };

	err = AMG88_Set_Mode(pCpnt, AMG88__NORMAL);
	if (err)	{ return err; }

	// Reset
	err = AMG88_Reset(pCpnt, AMG88__INITIAL_RESET);
	if (err)	{ return err; }

	err = AMG88_Set_FPS(pCpnt, true);	// Set to 10FPS
	if (err)	{ return err; }

	// Set thresholds

	// Set interrupts
	INT_CTRL.Bits.INTEN = true;
	INT_CTRL.Bits.INTMOD = false;
	err = AMG88_Write(pCpnt->cfg.slave_inst, &INT_CTRL.Byte, AMG88__INTC, 1);
	if (err)	{ return err; }

	return ERROR_OK;
}


/****************************************************************/


__WEAK FctERR NONNULL__ AMG88_handler(AMG88_t * const pCpnt)
{
	FctERR						err;
	uAMG88_REG__STATUS_CLEAR	CLEAR = { 0x0E };
	float						thermistor, temp[64];

	err = AMG88_Read_Word(pCpnt->cfg.slave_inst, &pCpnt->Thermistor, AMG88__TTHL);
	if (err)	{ return err; }

	err = AMG88_Read(pCpnt->cfg.slave_inst, (uint8_t *) pCpnt->Pixels, AMG88__T01L, sizeof(pCpnt->Pixels));
	if (err)	{ return err; }

	thermistor = AMG88_Convert_Thermistor_Raw(pCpnt->Thermistor);
	for (uintCPU_t i = 0 ; i < SZ_ARRAY(temp) ; i++)
	{
		temp[i] = AMG88_Convert_Pixel_Raw(pCpnt->Pixels[i / 8][i % 8]);
	}

	#if defined(VERBOSE)
		const uint8_t idx = pCpnt - AMG88;
		printf("AMG88 id%d: Thermistor %d.%02ld°C\r\n", idx, (int16_t) thermistor, get_fp_dec(thermistor, 2));
		printf("AMG88 id%d: Pixels Temperature (°C)", idx);
		for (uintCPU_t i = 0 ; i < SZ_ARRAY(temp) ; i++)
		{
			if (i % 8 == 0)	{ printf("\r\n"); }
			printf("%03d.%02ld ", (int16_t) temp[i], get_fp_dec(temp[i], 2));
		}
		printf("\r\n");
	#endif

	return AMG88_Write(pCpnt->cfg.slave_inst, &CLEAR.Byte, AMG88__SCLR, sizeof(pCpnt->Pixels));
}


__WEAK FctERR NONNULL__ AMG88_handler_it(AMG88_t * const pCpnt)
{
	FctERR	err = ERROR_OK;
	bool	interrupt;

	AMG88_INT_GPIO_Get(pCpnt, &interrupt);
	if (interrupt)	{ err = AMG88_handler(pCpnt); }

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
