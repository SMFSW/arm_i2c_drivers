/*!\file AMG88_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
** \brief AMG88 Driver extensions
** \details AMG88: Infrared Array Sensor (Grid-EYE)
**/
/****************************************************************/
#ifndef __AMG88_EX_H__
	#define __AMG88_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "AMG88.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\union uAMG88_REG_MAP
** \brief Union of AMG88 registry map
**/
typedef union uAMG88_REG_MAP {
	uint8_t Bytes[256];
	struct {
		uint8_t	Reg_1;
		uint8_t	Reg_2;
	} Reg;
} uAMG88_REG_MAP;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

/*!\brief Convert raw thermistor temperature to celsius degrees
** \param[in] therm - Raw thermistor value
** \return Thermistor temperature (in °C)
**/
float AMG88_Convert_Thermistor_Raw(const uint16_t therm);


/*!\brief Convert raw pixel temperature to celsius degrees
** \param[in] pixel - Raw pixel value
** \return Pixel temperature (in °C)
**/
float AMG88_Convert_Pixel_Raw(const uint16_t pixel);


/*!\brief Convert temperature to AMG88 temperature coded integer
** \param[in] temp - Temperature value
** \return AMG88 temperature coded integer
**/
uint16_t AMG88_Convert_Temp_To_Int(const float temp);


/*!\brief Get thermistor temperature
** \param[in] pCpnt - Pointer to AMG88 component
** \param[in,out] temp - Pointer to temperature result
** \return Error code
**/
FctERR NONNULL__ AMG88_Get_thermistor_temp(AMG88_t * pCpnt, float * temp);

/*!\brief Get pixel temperature
** \param[in] pCpnt - Pointer to AMG88 component
** \param[in,out] temp - Pointer to temperature result
** \param[in] pixel - Pixel index to retrieve data
** \return Error code
**/
FctERR NONNULL__ AMG88_Get_pixel_temp(AMG88_t * pCpnt, float * temp, const uint8_t pixel);


/*!\brief Get all pixels temperature
** \param[in] pCpnt - Pointer to AMG88 component
** \param[in,out] temp - Pointer to temperature result
** \return Error code
**/
FctERR NONNULL__ AMG88_Get_pixels_temp(AMG88_t * pCpnt, float temp[64]);


/*!\brief AMG88 reset
** \param[in] pCpnt - Pointer to AMG88 component
** \param[in] rst - Reset type
** \return Error code
**/
__INLINE FctERR NONNULL_INLINE__ AMG88_Reset(AMG88_t * pCpnt, const AMG88_reset rst) {
	return AMG88_Write(pCpnt->cfg.slave_inst, &rst, AMG88__RST, 1); }


/*!\brief AMG88 Set mode
** \param[in] pCpnt - Pointer to AMG88 component
** \param[in] mode - AMG88 mode
** \return Error code
**/
__INLINE FctERR NONNULL_INLINE__ AMG88_Set_Mode(AMG88_t * pCpnt, const AMG88_mode mode) {
	return AMG88_Write(pCpnt->cfg.slave_inst, &mode, AMG88__PCTL, 1); }

/*!\brief Set AMG88 Frame Per Seconds multiplier
** \param[in] pCpnt - Pointer to AMG88 component
** \param[in] ten - multiply by 10FPS when set to true
** \return Error code
**/
__INLINE FctERR NONNULL_INLINE__ AMG88_Set_FPS(AMG88_t * pCpnt, const bool ten) {
	uAMG88_REG__FRAME_RATE FPS = { .Bits.FPS = !ten };
	return AMG88_Write(pCpnt->cfg.slave_inst, &FPS.Byte, AMG88__FPSC, 1); }

/*!\brief Set AMG88 Moving Average
** \param[in] pCpnt - Pointer to AMG88 component
** \param[in] MA - Twice moving average when set to true
** \return Error code
**/
__INLINE FctERR NONNULL_INLINE__ AMG88_Set_MA(AMG88_t * pCpnt, const bool twice) {
	uAMG88_REG__AVERAGE MA = { .Bits.MAMOD = twice };
	return AMG88_Write(pCpnt->cfg.slave_inst, &MA.Byte, AMG88__AVE, 1); }


/*!\brief Set AMG88 Interrupt Level upper limit
** \param[in] pCpnt - Pointer to AMG88 component
** \param[in] temp - Temperature value
** \return Error code
**/
FctERR NONNULL__ AMG88_Set_Interrupt_LVLH(AMG88_t * pCpnt, const float temp);

/*!\brief Set AMG88 Interrupt Level lower limit
** \param[in] pCpnt - Pointer to AMG88 component
** \param[in] temp - Temperature value
** \return Error code
**/
FctERR NONNULL__ AMG88_Set_Interrupt_LVLL(AMG88_t * pCpnt, const float temp);

/*!\brief Set AMG88 Interrupt Hysteresis level
** \param[in] pCpnt - Pointer to AMG88 component
** \param[in] temp - Temperature value
** \return Error code
**/
FctERR NONNULL__ AMG88_Set_Interrupt_HYS(AMG88_t * pCpnt, const float temp);

/*!\brief Set AMG88 Interrupt levels
** \param[in] pCpnt - Pointer to AMG88 component
** \param[in] temp_LVLH - Temperature Interrupt Level upper limit value
** \param[in] temp_LVLL - Temperature Interrupt Level lower limit value
** \param[in] temp_HYS - Temperature Interrupt Hysteresis level value
** \return Error code
**/
FctERR NONNULL__ AMG88_Set_Interrupt_Levels(AMG88_t * pCpnt, const float temp_LVLH, const float temp_LVLL, const float temp_HYS);


/*!\brief Get interrupts registers
** \param[in] pCpnt - Pointer to AMG88 component
** \param[in,out] temp - Pointer to interrupts result
** \return Error code
**/
__INLINE FctERR NONNULL_INLINE__ AMG88_Get_interrupts(AMG88_t * pCpnt, uAMG88_REG__INT * interrupts) {
	return AMG88_Read(pCpnt->cfg.slave_inst, interrupts->Bytes, AMG88__INT0, 8); }

/*!\brief Get thermistor raw value
** \param[in] pCpnt - Pointer to AMG88 component
** \param[in,out] raw - Pointer to thermistor raw result
** \return Error code
**/
__INLINE FctERR NONNULL_INLINE__ AMG88_Get_Thermistor_Raw(AMG88_t * pCpnt, uint16_t * raw) {
	return AMG88_Read_Word(pCpnt->cfg.slave_inst, raw, AMG88__TTHL); }


/*!\brief Get pixel raw value
** \param[in] pCpnt - Pointer to AMG88 component
** \param[in,out] raw - Pointer to pixel raw result
** \param[in] pixel - Pixel index to retrieve data
** \return Error code
**/
__INLINE FctERR NONNULL_INLINE__ AMG88_Get_Pixel_Raw(AMG88_t * pCpnt, uint16_t * raw, const uint8_t pixel) {
	if ((!pixel) || (pixel > 64))	{ return ERROR_VALUE; }
	return AMG88_Read_Word(pCpnt->cfg.slave_inst, raw, AMG88__T01L + (2 * pixel)); }


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __AMG88_EX_H__ */
/****************************************************************/
