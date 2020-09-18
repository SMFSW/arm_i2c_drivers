/*!\file ADS1115_proc.c
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief ADS1115 Driver procedures
** \details ADS1115: Ultra-Small, Low-Power, 16-Bit Analog-to-Digital Converter with Internal Reference
** \note	Compatibility with:
**				- ADS1113 (restrictions given in datasheet)
**				- ADS1114 (restrictions given in datasheet)
**				- ADS1115
** \note	Compatibility should be assured with:
**				- ADS1x1x (check datasheets for compatibility informations)
**/
/****************************************************************/
#include "ADS1115.h"

#if defined(HAL_I2C_MODULE_ENABLED)
#if defined(I2C_ADS1115)
/****************************************************************/


ADS1115_t ADS1115[I2C_ADS1115_NB];


/****************************************************************/


__WEAK FctERR NONNULL__ ADS1115_Init_Sequence(ADS1115_t * pCpnt)
{
	pCpnt->cfg.Lo_Thresh = 0x8000;		// - max
	pCpnt->cfg.Hi_Thresh = 0x7FFF;		// + amx
	pCpnt->cfg.Config.Word = 0x8583;	// Default value

	pCpnt->cfg.Config.Bits.MUX = ADS1115__MUX_pAIN0_nGND;
	pCpnt->cfg.Config.Bits.MODE = ADS1115__MODE_CONTINUOUS;
	pCpnt->cfg.Config.Bits.PGA = ADS1115__FS_2048mV;

	return ADS1115_Write(pCpnt->cfg.slave_inst, &pCpnt->cfg.Config.Word, ADS1115__CONFIG);
}


/****************************************************************/


__WEAK FctERR NONNULL__ ADS1115_handler(ADS1115_t * pCpnt)
{
	FctERR	err;
	const uint8_t AIN_idx = pCpnt->cfg.Config.Bits.MUX % 4;

	err = ADS1115_Read(pCpnt->cfg.slave_inst, (uint16_t *) &pCpnt->AIN[AIN_idx], ADS1115__CONVERSION);
	if (err)	{ return err; }

	if (pCpnt->cfg.Config.Bits.MUX++ < ADS1115__MUX_pAIN0_nGND)	{ pCpnt->cfg.Config.Bits.MUX = ADS1115__MUX_pAIN0_nGND; }
	ADS1115_Write(pCpnt->cfg.slave_inst, &pCpnt->cfg.Config.Word, ADS1115__CONFIG);

	#if defined(VERBOSE)
		const uint8_t idx = pCpnt - ADS1115;
		printf("ADS1115 id%d: Val%d %d", idx, AIN_idx, pCpnt->AIN[AIN_idx]);
	#endif

	return ERROR_OK;
}


/****************************************************************/
#endif
#endif
/****************************************************************/
