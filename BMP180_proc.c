/*!\file BMP180_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief BMP180 Driver procedures
** \details BMP180: Digital pressure sensor
**/
/****************************************************************/
#include "BMP180.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_BMP180)
/****************************************************************/

//#define BMP180_TST	//!< Defined to check calculations with datasheet

BMP180_t BMP180[I2C_BMP180_NB] = { 0 };


/****************************************************************/


#if defined(BMP180_TST)
static void BMP180_Test_Result(const char * str, const bool res)
{
	UNUSED_RET printf("BMP180 %s result: TEST %s\r\n", str, res ? "PASS" : "FAIL");
}
#endif


__WEAK void NONNULL__ BMP180_Set_SeaLevel_Pressure(BMP180_t * const pCpnt)
{
	pCpnt->cfg.SeaLevelPressure = Get_SeaLevel_Pressure();
}


__WEAK FctERR NONNULL__ BMP180_Init_Sequence(BMP180_t * const pCpnt)
{
	FctERR err = BMP180_Get_ChipID(pCpnt, &pCpnt->cfg.ChipID);
	if (err != ERROR_OK)						{ goto ret; }
	if (pCpnt->cfg.ChipID != BMP180_CHIP_ID)	{ err = ERROR_COMMON; goto ret; }	// Unknown device

	BMP180_Set_SeaLevel_Pressure(pCpnt);

	pCpnt->cfg.OSS = BMP180__OSS_8_TIME;

	#if !defined(BMP180_TST)
		err = BMP180_Get_Calibration(pCpnt, &pCpnt->cfg.Calib);
	#else
		pCpnt->cfg.OSS = BMP180__OSS_1_TIME;
		pCpnt->cfg.Calib.ac1 = 408;
		pCpnt->cfg.Calib.ac2 = -72;
		pCpnt->cfg.Calib.ac3 = -14383;
		pCpnt->cfg.Calib.ac4 = 32741U;
		pCpnt->cfg.Calib.ac5 = 32757U;
		pCpnt->cfg.Calib.ac6 = 23153U;
		pCpnt->cfg.Calib.b1 = 6190;
		pCpnt->cfg.Calib.b2 = 4;
		pCpnt->cfg.Calib.mb = -32768;
		pCpnt->cfg.Calib.mc = -8711;
		pCpnt->cfg.Calib.md = 2868;
	#endif

	ret:
	return err;
}


/****************************************************************/


FctERR NONNULL__ BMP180_Set_Oversampling(BMP180_t * const pCpnt, const BMP180_oversampling oss)
{
	FctERR err = ERROR_OK;

	if (oss > BMP180__OSS_8_TIME)	{ err = ERROR_VALUE; }	// Unknown Oversampling
	else							{ pCpnt->cfg.OSS = oss; }

	return err;
}


FctERR NONNULL__ BMP180_Get_Calibration(BMP180_t * const pCpnt, BMP180_calib * pCalib)
{
	uint16_t *	addr = (uint16_t *) pCalib;
	FctERR		err;

	for (uintCPU_t i = 0 ; i < SZ_OBJ(BMP180_calib, int16_t) ; i++)
	{
		err = BMP180_Read_Word(pCpnt->cfg.slave_inst, addr++, BMP180__CALIB_AC1_MSB + (i * 2U));
		if (err != ERROR_OK)	{ break; }
	}

	return err;
}


/**!\brief Determine B5 compensated value used in temperature & pressure calculations.
** \param[in] pCalib - Pointer to BMP180 calibration structure
** \param[in] UT - UT value (temperature data)
** \return B5 value (compensated temperature value)
**/
static int32_t NONNULL__ UT_To_b5(const BMP180_calib * const pCalib, const int32_t UT)
{
	const int32_t X1 = (UT - pCalib->ac6) * pCalib->ac5 / 32768;
	const int32_t X2 = pCalib->mc * 2048 / (X1 + pCalib->md);
	return X1 + X2;
}


/**!\brief Convert B5 compensated value to Celsius degrees
** \param[in] B5 - B5 value (compensated temperature value)
** \return Celsius degrees value
**/
static float b5_To_Celcius(const int32_t b5)
{
	const int32_t t = (b5 + 8) / 16;
	return ((float) t / 10.0f);		// temperature given in 0.1°C (thus divide by 10 to get °C)
}


FctERR NONNULLX__(1) BMP180_Get_Pressure(BMP180_t * const pCpnt, float * pres)
{
	int32_t						UT = 0, UP = 0, compp = 0;
	int32_t						x1, x2, b6, x3, b3, p;
	uint32_t					b4, b7;
	FctERR						err = ERROR_OK;
	const BMP180_calib * const	pCalib = &pCpnt->cfg.Calib;

	/* Get the raw pressure and temperature values */
	#if !defined(BMP180_TST)
		err = BMP180_Get_Temperature_Raw(pCpnt, &UT);
		if (err != ERROR_OK)	{ goto ret; }
		err = BMP180_Get_Pressure_Raw(pCpnt, &UP);
		if (err != ERROR_OK)	{ goto ret; }
	#else
		UT = 27898;		//!< For test purposes
		UP = 23843;		//!< For test purposes
	#endif

	/* Temperature compensation */
	const int32_t b5 = UT_To_b5(pCalib, UT);

	#if defined(BMP180_TST)
	BMP180_Test_Result("B5", binEval(b5 == 2399));
	#endif

	/* Pressure compensation */
	b6 = b5 - 4000;
	x1 = (pCalib->b2 * ((b6 * b6) / 4096)) / 2048;
	x2 = pCalib->ac2 * b6 / 2048;
	x3 = x1 + x2;
	b3 = ((((pCalib->ac1 * 4) + x3) << pCpnt->cfg.OSS) + 2) / 4;
	x1 = (pCalib->ac3 * b6) / 8192;
	x2 = (pCalib->b1 * ((b6 * b6) / 4096)) / 65536;
	x3 = ((x1 + x2) + 2) / 4;
	b4 = (pCalib->ac4 * (uint32_t) (x3 + 32768)) / 32768U;
	b7 = (uint32_t) (UP - b3) * (50000U >> pCpnt->cfg.OSS);

	if (b7 < 0x80000000U)	{ p = (b7 * 2U) / b4; }
	else					{ p = (b7 / b4) * 2U; }

	x1 = p / 256;
	x1 *= x1;
	x1 = (x1 * 3038) / 65536;
	x2 = (-7357 * p) / 65536;
	compp = p + ((x1 + x2 + 3791) / 16);

	/* Set results */
	pCpnt->Pressure = compp / 100.0f;	// From Pa to hPa (mbar)
	pCpnt->Temperature = b5_To_Celcius(b5);
	pCpnt->Altitude = Atmospheric_Pressure_To_Altitude(pCpnt->Pressure, pCpnt->cfg.SeaLevelPressure);

	#if defined(BMP180_TST)
	BMP180_Test_Result("Pressure", binEval(compp == 69994));
	BMP180_Test_Result("Temperature", binEval(pCpnt->Temperature == 15));
	#endif

	if (pres != NULL)	{ *pres = pCpnt->Pressure; }

	#if !defined(BMP180_TST)
	ret:
	#endif
	return err;
}


FctERR NONNULLX__(1) BMP180_Get_Temperature(BMP180_t * const pCpnt, float * temp)
{
	int32_t	UT = 0;
	FctERR	err = ERROR_OK;

	#if !defined(BMP180_TST)
		err = BMP180_Get_Temperature_Raw(pCpnt, &UT);
		if (err != ERROR_OK)	{ goto ret; }
	#else
		UT = 27898;		//!< For test purposes
	#endif

	const int32_t b5 = UT_To_b5(&pCpnt->cfg.Calib, UT);

	#if defined(BMP180_TST)
	BMP180_Test_Result("B5", binEval(b5 == 2399));
	#endif

	/* Set results */
	pCpnt->Temperature = b5_To_Celcius(b5);

	#if defined(BMP180_TST)
	BMP180_Test_Result("Temperature", binEval(pCpnt->Temperature == 15));
	#endif

	if (temp != NULL)	{ *temp = pCpnt->Temperature; }

	#if !defined(BMP180_TST)
	ret:
	#endif
	return err;
}


__WEAK FctERR NONNULL__ BMP180_handler(BMP180_t * const pCpnt)
{
	FctERR err = BMP180_Get_Pressure(pCpnt, NULL);	// Get pressure also gets temperature
	if (err != ERROR_OK)	{ goto ret; }

	#if defined(VERBOSE)
		const uint8_t idx = pCpnt - BMP180;
		UNUSED_RET printf("BMP180 id%d: Pressure %ldhPa, Temperature %d.%02ld°C, Alt %ldm, Pressure at sea level %ldhPa (for reference)\r\n",
							idx, (int32_t) pCpnt->Pressure, (int16_t) pCpnt->Temperature, get_fp_dec(pCpnt->Temperature, 2),
							(int32_t) pCpnt->Altitude, (int32_t) pCpnt->cfg.SeaLevelPressure);
	#endif

	ret:
	return err;
}


FctERR BMP180_handler_all(void)
{
	FctERR err = ERROR_OK;

	for (BMP180_t * pCpnt = BMP180 ; pCpnt < &BMP180[I2C_BMP180_NB] ; pCpnt++)
	{
		err |= BMP180_handler(pCpnt);
	}

	return err;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
