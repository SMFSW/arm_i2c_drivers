/*!\file TMP1075_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
** \brief TMP1075 Driver extensions
** \details TMP1075: Temperature Sensor With I2C and SMBus Interface in Industry Standard LM75 Form Factor and Pinout
**/
/****************************************************************/
#ifndef __TMP1075_EX_H__
	#define __TMP1075_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "TMP1075.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

/*!\brief Set TMP1075 Configuration
** \param[in] pCpnt - Pointer to TMP1075 component
** \param[in,out] cfg - pointer to Configuration value to write from
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TMP1075_Write_Config(TMP1075_t * const pCpnt, uint8_t * cfg) {
	return TMP1075_Write_Byte(pCpnt->cfg.slave_inst, cfg, TMP1075__CFGR); }

/*!\brief Get TMP1075 Configuration
** \param[in] pCpnt - Pointer to TMP1075 component
** \param[in,out] cfg - pointer to Configuration value to read to
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TMP1075_Read_Config(TMP1075_t * const pCpnt, uint8_t * cfg) {
	return TMP1075_Read_Byte(pCpnt->cfg.slave_inst, cfg, TMP1075__CFGR); }

/*!\brief Get TMP1075 Raw temperature
** \param[in] pCpnt - Pointer to TMP1075 component
** \param[in,out] temp - pointer to Temperature value to read to
** \return FctERR - error code
**/
FctERR NONNULL__ TMP1075_Get_Temperature_Raw(TMP1075_t * const pCpnt, int16_t * temp);

/*!\brief Start TMP1075 single conversion
** \note Stops continuous conversions automatically
** \param[in] pCpnt - Pointer to TMP1075 component
** \return FctERR - error code
**/
FctERR NONNULL__ TMP1075_Start_SingleConversion(TMP1075_t * const pCpnt);

/*!\brief Set TMP1075 automatic conversion rate
** \note Starts continuous conversions automatically
** \param[in] pCpnt - Pointer to TMP1075 component
** \param[in] rate - Conversion rate
** \return FctERR - error code
**/
FctERR NONNULL__ TMP1075_Set_ConversionRate(TMP1075_t * const pCpnt, const TMP1075_conv_rate rate);

/*!\brief Set TMP1075 conversion mode (manual/auto)
** \param[in] pCpnt - Pointer to TMP1075 component
** \param[in] mode - Conversion mode (manual/auto)
** \return FctERR - error code
**/
FctERR NONNULL__ TMP1075_Set_ConversionMode(TMP1075_t * const pCpnt, const TMP1075_shutdown_mode mode);

/*!\brief Set TMP1075 alert mode (comparator/interrupt)
** \param[in] pCpnt - Pointer to TMP1075 component
** \param[in] mode - Alert mode (comparator/interrupt)
** \return FctERR - error code
**/
FctERR NONNULL__ TMP1075_Set_AlertMode(TMP1075_t * const pCpnt, const TMP1075_alert_mode mode);

/*!\brief Set TMP1075 alert polarity
** \param[in] pCpnt - Pointer to TMP1075 component
** \param[in] pol - Alert pin polarity
** \return FctERR - error code
**/
FctERR NONNULL__ TMP1075_Set_AlertPolarity(TMP1075_t * const pCpnt, const TMP1075_alert_pol pol);

/*!\brief Set TMP1075 alert number of faults trigger
** \param[in] pCpnt - Pointer to TMP1075 component
** \param[in] faults - Number of faults to trigger alert
** \return FctERR - error code
**/
FctERR NONNULL__ TMP1075_Set_AlertNbFaults(TMP1075_t * const pCpnt, const TMP1075_nb_faults faults);

/*!\brief Get TMP1075N chip ID
** \param[in] pCpnt - Pointer to TMP1075N component
** \param[in,out] id - pointer to chip ID result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ TMP1075N_Get_ChipID(TMP1075_t * const pCpnt, uint16_t * id) {
	return TMP1075_Read_Word(pCpnt->cfg.slave_inst, id, TMP1075__DIEID); }


/*******************/
/*** GPIO access ***/
/*******************/

/*!\brief Interrupt GPIO pin init for TMP1075
** \weak TMP1075 Interrupt GPIO pin init may be user implemented if needed
** \param[in] pCpnt - Pointer to TMP1075 component
** \param[in] GPIOx - INT port
** \param[in] GPIO_Pin - INT pin
** \param[in] GPIO_Active: INT pin active state
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ TMP1075_INT_GPIO_Init(TMP1075_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active);

/*!\brief Interrupt GPIO pin getter for ADS1115
** \weak TMP1075 Interrupt GPIO pin getter may be user implemented if needed
** \param[in] pCpnt - Pointer to TMP1075 component
** \param[in,out] pState - Pointer to INT pin state variable (0: inactive, 1: active)
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ TMP1075_INT_GPIO_Get(TMP1075_t * const pCpnt, bool * const pState);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* __TMP1075_EX_H__ */
/****************************************************************/
