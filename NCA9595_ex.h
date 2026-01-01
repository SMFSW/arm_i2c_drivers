/*!\file NCA9595_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief NCA9595 Driver extensions
** \details NCA9595: Low-voltage 16-bit I²C and SMBus I/O expander
**/
/****************************************************************/
#ifndef NCA9595_EX_H__
	#define NCA9595_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "NCA9595.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

/*!\brief Read Register of NCA9595 peripheral
** \param[in,out] pCpnt - Pointer to NCA9595 component
** \param[in,out] data - Pointer to read register value
** \param[in] reg - Register address
** \return Error code
**/
__INLINE FctERR NONNULL_INLINE__ NCA9595_Read_Register(NCA9595_t * const pCpnt, uint16_t * const data, const NCA9595_wreg reg) {
	return NCA9595_Read_Word(pCpnt->cfg.slave_inst, data, reg); }


/*!\brief Read Inputs for NCA9595 peripheral
** \param[in,out] pCpnt - Pointer to NCA9595 component
** \param[in,out] in - Input values (MSB: Port 1 / LSB: Port 0)
** \return Error code
**/
FctERR NONNULL__ NCA9595_Read_Inputs(NCA9595_t * const pCpnt, uint16_t * const in);

/*!\brief Read Inputs for NCA9595 peripheral (using union)
** \param[in,out] pCpnt - Pointer to NCA9595 component
** \param[in,out] in - Input union values
** \return Error code
**/
__INLINE FctERR NONNULL_INLINE__ NCA9595_Read_Inputs_Union(NCA9595_t * const pCpnt, uNCA9595_REG__IN * const in) {
	return NCA9595_Read_Inputs(pCpnt, &in->Word); }


/*!\brief Write Outputs for NCA9595 peripheral
** \param[in,out] pCpnt - Pointer to NCA9595 component
** \param[in] out - Output values (MSB: Port 1 / LSB: Port 0)
** \return Error code
**/
FctERR NONNULL__ NCA9595_Write_Outputs(NCA9595_t * const pCpnt, const uint16_t out);

/*!\brief Write Outputs with mask for NCA9595 peripheral
** \param[in,out] pCpnt - Pointer to NCA9595 component
** \param[in] out - Output values (MSB: Port 1 / LSB: Port 0)
** \param[in] mask - Outputs mask (MSB: Port 1 / LSB: Port 0)
** \return Error code
**/
FctERR NONNULL__ NCA9595_Write_Outputs_Mask(NCA9595_t * const pCpnt, const uint16_t out, const uint16_t mask);

/*!\brief Write Outputs for NCA9595 peripheral (using union)
** \param[in,out] pCpnt - Pointer to NCA9595 component
** \param[in] out - Output union values
** \return Error code
**/
__INLINE FctERR NONNULL_INLINE__ NCA9595_Write_Outputs_Union(NCA9595_t * const pCpnt, const uNCA9595_REG__OUT out) {
	return NCA9595_Write_Outputs(pCpnt, out.Word); }


/*!\brief Set GPIOs configuration for NCA9595 peripheral
** \param[in,out] pCpnt - Pointer to NCA9595 component
** \param[in] cfg - Configuration values (MSB: Port 1 / LSB: Port 0)
** \return Error code
**/
FctERR NONNULL__ NCA9595_Set_Config(NCA9595_t * const pCpnt, const uint16_t cfg);

/*!\brief Set GPIOs configuration with mask for NCA9595 peripheral
** \param[in,out] pCpnt - Pointer to NCA9595 component
** \param[in] cfg - Configuration values (MSB: Port 1 / LSB: Port 0)
** \param[in] mask - GPIOs mask (MSB: Port 1 / LSB: Port 0)
** \return Error code
**/
FctERR NONNULL__ NCA9595_Set_Config_Mask(NCA9595_t * const pCpnt, const uint16_t cfg, const uint16_t mask);

/*!\brief Set GPIOs configuration for NCA9595 peripheral (using union)
** \param[in,out] pCpnt - Pointer to NCA9595 component
** \param[in] cfg - Configuration union values
** \return Error code
**/
__INLINE FctERR NONNULL_INLINE__ NCA9595_Set_Config_Union(NCA9595_t * const pCpnt, const uNCA9595_REG__CFG cfg) {
	return NCA9595_Set_Config(pCpnt, cfg.Word); }


/*!\brief Set Inputs polarity for NCA9595 peripheral
** \param[in] pCpnt - Pointer to NCA9595 component
** \param[in] pol - Polarity values (MSB: Port 1 / LSB: Port 0)
** \return Error code
**/
FctERR NONNULL__ NCA9595_Set_Polarity(NCA9595_t * const pCpnt, const uint16_t pol);

/*!\brief Set Inputs polarity with mask for NCA9595 peripheral
** \param[in] pCpnt - Pointer to NCA9595 component
** \param[in] pol - Polarity values (MSB: Port 1 / LSB: Port 0)
** \param[in] mask - Inputs mask (MSB: Port 1 / LSB: Port 0)
** \return Error code
**/
FctERR NONNULL__ NCA9595_Set_Polarity_Mask(NCA9595_t * const pCpnt, const uint16_t pol, const uint16_t mask);

/*!\brief Set Inputs polarity for NCA9595 peripheral (using union)
** \param[in] pCpnt - Pointer to NCA9595 component
** \param[in] pol - Polarity union values
** \return Error code
**/
__INLINE FctERR NONNULL_INLINE__ NCA9595_Set_Polarity_Union(NCA9595_t * const pCpnt, const uNCA9595_REG__POL pol) {
	return NCA9595_Set_Polarity(pCpnt, pol.Word); }


/*!\brief Set Outputs pull-up for NCA9595 peripheral
** \param[in] pCpnt - Pointer to NCA9595 component
** \param[in] pullup - Pull-up values (MSB: Port 1 / LSB: Port 0)
** \return Error code
**/
FctERR NONNULL__ NCA9595_Set_PullUp(NCA9595_t * const pCpnt, const uint16_t pullup);

/*!\brief Set Outputs pull-up with mask for NCA9595 peripheral
** \param[in] pCpnt - Pointer to NCA9595 component
** \param[in] pullup - Pull-up values (MSB: Port 1 / LSB: Port 0)
** \param[in] mask - Outputs mask (MSB: Port 1 / LSB: Port 0)
** \return Error code
**/
FctERR NONNULL__ NCA9595_Set_PullUp_Mask(NCA9595_t * const pCpnt, const uint16_t pullup, const uint16_t mask);

/*!\brief Set Outputs pull-up for NCA9595 peripheral (using union)
** \param[in] pCpnt - Pointer to NCA9595 component
** \param[in] pullup - Pull-up union values
** \return Error code
**/
__INLINE FctERR NONNULL_INLINE__ NCA9595_Set_PullUp_Union(NCA9595_t * const pCpnt, const uNCA9595_REG__PUP pullup) {
	return NCA9595_Set_PullUp(pCpnt, pullup.Word); }


/*******************/
/*** GPIO access ***/
/*******************/

/*!\brief Interrupt GPIO pin init for NCA9595
** \weak NCA9595 Interrupt GPIO pin init may be user implemented if needed
** \param[in] pCpnt - Pointer to NCA9595 component
** \param[in] GPIOx - INT port
** \param[in] GPIO_Pin - INT pin
** \param[in] GPIO_Active: INT pin active state
**/
void NONNULL__ NCA9595_INT_GPIO_Init(NCA9595_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active);

/*!\brief Interrupt GPIO pin getter for ADS1115
** \weak NCA9595 Interrupt GPIO pin getter may be user implemented if needed
** \param[in] pCpnt - Pointer to NCA9595 component
** \return INT pin state value (0: inactive, 1: active)
**/
bool NONNULL__ NCA9595_INT_GPIO_Get(const NCA9595_t * const pCpnt);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* NCA9595_EX_H__ */
/****************************************************************/
