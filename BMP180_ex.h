/*!\file BMP180_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief BMP180 Driver extensions
** \details BMP180: Digital pressure sensor
**/
/****************************************************************/
#ifndef BMP180_EX_H__
	#define BMP180_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "BMP180.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define	BMP180_RESET_VAL	0xB6U	//!< Reset value for "soft reset" register


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

/*!\brief Start BMP180 Temperature/Pressure conversion
** \param[in] pCpnt - Pointer to BMP180 component
** \param[in] meas - Temperature / Pressure
** \return FctERR - error code
**/
FctERR NONNULL__ BMP180_Start_Conversion(BMP180_t * const pCpnt, const BMP180_meas meas);


/*!\brief Get BMP180 chip ID
** \param[in] pCpnt - Pointer to BMP180 component
** \param[in,out] id - pointer to chip ID result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ BMP180_Get_ChipID(BMP180_t * const pCpnt, uint8_t * id) {
	return BMP180_Read(pCpnt->cfg.slave_inst, id, BMP180__ID, 1U); }

/*!\brief Reset BMP180 chip
** \param[in] pCpnt - Pointer to BMP180 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ BMP180_Reset(BMP180_t * const pCpnt) {
	const uint8_t rst = BMP180_RESET_VAL;
	return BMP180_Write(pCpnt->cfg.slave_inst, &rst, BMP180__SOFT_RESET, 1U); }


/*!\brief Get Raw Temperature
** \param[in] pCpnt - Pointer to BMP180 component
** \param[in,out] tp - pointer to raw temperature result
** \return FctERR - error code
**/
FctERR NONNULL__ BMP180_Get_Temperature_Raw(BMP180_t * const pCpnt, int32_t * tp);

/*!\brief Get Raw Pressure
** \param[in] pCpnt - Pointer to BMP180 component
** \param[in,out] pr - pointer to raw pressure result
** \return FctERR - error code
**/
FctERR NONNULL__ BMP180_Get_Pressure_Raw(BMP180_t * const pCpnt, int32_t * pr);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* BMP180_EX_H__ */
/****************************************************************/
