/*!\file ADS1115_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief ADS1115 Driver extensions
** \details ADS1115: Ultra-Small, Low-Power, 16-Bit Analog-to-Digital Converter with Internal Reference
** \note	Compatibility with:
**				- ADS1113 (restrictions given in datasheet)
**				- ADS1114 (restrictions given in datasheet)
**				- ADS1115
** \note	Compatibility should be assured with:
**				- ADS1x1x (check datasheets for compatibility informations)
**/
/****************************************************************/
#ifndef ADS1115_EX_H__
	#define ADS1115_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "ADS1115.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\union uADS1115_REG_MAP
** \brief Union of ADS1115 registry map
** \warning Using the particular union depends on the target endian (Big endian needed)
**/
typedef union uADS1115_REG_MAP {
	uint16_t Words[4];
	struct PACK__ {
		int16_t				Conversion;	// Conversion register
		uADS1115_REG__CFG	Config;		// Config register
		int16_t				Lo_Thresh;	// Lo_thresh register
		int16_t				Hi_Thresh;	// Hi_thresh register
	} Reg;
} uADS1115_REG_MAP;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/
/*!\brief Get conversion time for ADS1115 component (µs)
** \param[in] pCpnt - Pointer to ADS1115 component
** \return Conversion time in µs
**/
uint16_t NONNULL__ ADS1115_Get_conv_us(ADS1115_t * const pCpnt);

/*!\brief Get conversion time for ADS1115 component (ms)
** \param[in] pCpnt - Pointer to ADS1115 component
** \return Conversion time in ms
**/
uint16_t NONNULL__ ADS1115_Get_conv_ms(ADS1115_t * const pCpnt);


/*!\brief Get ADS1115 conversion
** \param[in] pCpnt - Pointer to ADS1115 component
** \return FctERR - error code
**/
FctERR NONNULL__ ADS1115_Get_Conversion(ADS1115_t * const pCpnt);

/*!\brief Set ADS1115 functional mode
** \param[in] pCpnt - Pointer to ADS1115 component
** \param[in] func - ADS1115 function
** \param[in] mode - ADS1115 mode
** \param[in] nb - Number of channels used
** \return FctERR - error code
**/
FctERR NONNULL__ ADS1115_Set_Function(ADS1115_t * const pCpnt, const ADS1115_func func, const ADS1115_mode mode, const uint8_t nb);

/*!\brief Set ADS1115 mode
** \param[in] pCpnt - Pointer to ADS1115 component
** \param[in] mode - Mode
** \return FctERR - error code
**/
FctERR NONNULL__ ADS1115_Set_Mode(ADS1115_t * const pCpnt, const ADS1115_mode mode);

/*!\brief Set ADS1115 gain
** \param[in] pCpnt - Pointer to ADS1115 component
** \param[in] gain - Gain
** \return FctERR - error code
**/
FctERR NONNULL__ ADS1115_Set_Gain(ADS1115_t * const pCpnt, const ADS1115_gain gain);

/*!\brief Set ADS1115 rate
** \param[in] pCpnt - Pointer to ADS1115 component
** \param[in] rate - Data rate
** \return FctERR - error code
**/
FctERR NONNULL__ ADS1115_Set_Rate(ADS1115_t * const pCpnt, const ADS1115_rate rate);

/*!\brief Set ADS1115 Comparator mode
** \param[in] pCpnt - Pointer to ADS1115 component
** \param[in] comp - Comparator Mode
** \return FctERR - error code
**/
FctERR NONNULL__ ADS1115_Set_CompMode(ADS1115_t * const pCpnt, const ADS1115_comp comp);

/*!\brief Set ADS1115 Comparator polarity
** \param[in] pCpnt - Pointer to ADS1115 component
** \param[in] pol - Comparator Polarity
** \return FctERR - error code
**/
FctERR NONNULL__ ADS1115_Set_CompPolarity(ADS1115_t * const pCpnt, const ADS1115_polarity pol);

/*!\brief Set ADS1115 Comparator latch
** \param[in] pCpnt - Pointer to ADS1115 component
** \param[in] latch - Comparator latch
** \return FctERR - error code
**/
FctERR NONNULL__ ADS1115_Set_CompLatch(ADS1115_t * const pCpnt, const ADS1115_latch latch);

/*!\brief Set ADS1115 Comparator queue
** \param[in] pCpnt - Pointer to ADS1115 component
** \param[in] queue - Comparator queue
** \return FctERR - error code
**/
FctERR NONNULL__ ADS1115_Set_CompQueue(ADS1115_t * const pCpnt, const ADS1115_queue queue);


/*!\brief Set ADS1115 low threshold
** \param[in] pCpnt - Pointer to ADS1115 component
** \param[in] thrh - Low threshold value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ ADS1115_Set_LowThreshold(ADS1115_t * const pCpnt, const int16_t thrh) {
	pCpnt->cfg.Lo_Thresh = thrh;
	return ADS1115_Write(pCpnt->cfg.slave_inst, (uint16_t *) &thrh, ADS1115__LOW_THRESH); }

/*!\brief Set ADS1115 high threshold
** \param[in] pCpnt - Pointer to ADS1115 component
** \param[in] thrh - High threshold value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ ADS1115_Set_HighThreshold(ADS1115_t * const pCpnt, const int16_t thrh) {
	pCpnt->cfg.Hi_Thresh = thrh;
	return ADS1115_Write(pCpnt->cfg.slave_inst, (uint16_t *) &thrh, ADS1115__HI_THRESH); }


/*!\brief Get ADS1115 low threshold
** \param[in] pCpnt - Pointer to ADS1115 component
** \return Low threshold value
**/
__INLINE int16_t NONNULL_INLINE__ ADS1115_Get_LowThreshold(ADS1115_t * const pCpnt) {
	return pCpnt->cfg.Lo_Thresh; }

/*!\brief Get ADS1115 high threshold
** \param[in] pCpnt - Pointer to ADS1115 component
** \return High threshold value
**/
__INLINE int16_t NONNULL_INLINE__ ADS1115_Get_HighThreshold(ADS1115_t * const pCpnt) {
	return pCpnt->cfg.Hi_Thresh; }


/*!\brief Start ADS1115 conversion
** \param[in] pCpnt - Pointer to ADS1115 component
** \param[in] chan - Channel
** \return FctERR - error code
**/
FctERR NONNULL__ ADS1115_Start_Conversion(ADS1115_t * const pCpnt, const ADS1115_mux chan);

/*!\brief Start ADS1115 Next conversion
** \note Using ADS1115__MODE_CONTINUOUS with ADS1115__FUNC_SINGLE_DIFF or single channel used, function ADS1115_Get_Conversion may be used instead
** \param[in] pCpnt - Pointer to ADS1115 component
** \return FctERR - error code
**/
FctERR NONNULL__ ADS1115_Start_NextConversion(ADS1115_t * const pCpnt);


/*!\brief Get ADS1115 raw channel value
** \param[in] pCpnt - Pointer to ADS1115 component
** \param[in] chan - ADS1115 channel
** \return Raw channel value
**/
__INLINE int16_t NONNULL_INLINE__ ADS1115_Get_raw_value(ADS1115_t * const pCpnt, const uint8_t chan) {
	return (chan >= 4) ? 0 : pCpnt->AIN[chan]; }


/*!\brief Get ADS1115 raw channel value in µV
** \param[in] pCpnt - Pointer to ADS1115 component
** \param[in] chan - ADS1115 channel
** \return Converted channel value (µV)
**/
float NONNULL__ ADS1115_Get_converted_value_uV(ADS1115_t * const pCpnt, const uint8_t chan);

/*!\brief Get ADS1115 raw channel value in mV
** \param[in] pCpnt - Pointer to ADS1115 component
** \param[in] chan - ADS1115 channel
** \return Converted channel value (mV)
**/
float NONNULL__ ADS1115_Get_converted_value_mV(ADS1115_t * const pCpnt, const uint8_t chan);

/*!\brief Get ADS1115 raw channel value in V
** \param[in] pCpnt - Pointer to ADS1115 component
** \param[in] chan - ADS1115 channel
** \return Converted channel value (V)
**/
float NONNULL__ ADS1115_Get_converted_value_V(ADS1115_t * const pCpnt, const uint8_t chan);


/*******************/
/*** GPIO access ***/
/*******************/

/*!\brief Ready GPIO pin init for ADS1115
** \weak ADS1115 Ready GPIO pin init may be user implemented if needed
** \param[in] pCpnt - Pointer to ADS1115 component
** \param[in] GPIOx - RDY port
** \param[in] GPIO_Pin - RDY pin
** \param[in] GPIO_Active: RDY pin active state
**/
void NONNULL__ ADS1115_RDY_GPIO_Init(ADS1115_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active);

/*!\brief Ready GPIO pin getter for ADS1115
** \weak ADS1115 Ready GPIO pin getter may be user implemented if needed
** \param[in] pCpnt - Pointer to ADS1115 component
** \param[in,out] pState - Pointer to RDY pin state variable (0: inactive, 1: active)
**/
void NONNULL__ ADS1115_RDY_GPIO_Get(ADS1115_t * const pCpnt, bool * const pState);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* ADS1115_EX_H__ */
/****************************************************************/
