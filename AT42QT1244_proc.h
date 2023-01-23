/*!\file AT42QT1244_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief AT42QT1244 Driver procedures
** \details AT42QT1244: 24-key QMatrix FMEA IEC/EN/UL60730 Touch Sensor
**/
/****************************************************************/
#ifndef __AT42QT1244_PROC_H__
	#define __AT42QT1244_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "AT42QT1244.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/

#ifndef AT42QT1244_GET_KEYS_ONLY
//! \note Define AT42QT1244_GET_KEYS_ONLY to change read behavior
#define AT42QT1244_GET_KEYS_ONLY	0		//!< AT42QT1244 get keys status only
#endif


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct AT42QT1244_t
** \brief AT42QT1244 user interface struct
**/
typedef struct AT42QT1244_t {
	uint32_t					hPowerOn;			//!< Time of the last reset
#if	!AT42QT1244_GET_KEYS_ONLY
	uint8_t						cnt_100ms;			//!< 100ms counter
	uint8_t						cnt_SignalFail;		//!< Signal Fail counter
	uint8_t						cnt_MatrixScan;		//!< Matrix Scan counter
	uAT42QT_REG__DEVICE_STATUS	status;				//!< Device status
#endif
	uint32_t					keys;				//!< Keys results
	struct {
	I2C_slave_t *				slave_inst;			//!< Slave structure
	PeripheralGPIO_t			CHANGE_GPIO;		//!< Change GPIO struct
	PeripheralGPIO_t			RST_GPIO;			//!< Reset GPIO struct
	} cfg;
} AT42QT1244_t;

extern AT42QT1244_t AT42QT1244[I2C_AT42QT1244_NB];	//!< AT42QT1244 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for AT42QT1244 peripheral
** \weak AT42QT1244 Init sequence may be user implemented if custom initialization sequence needed
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \return FctERR - error code
**/
FctERR NONNULL__ AT42QT1244_Init_Sequence(AT42QT1244_t * const pCpnt);


/*!\brief Set last reset time for AT42QT1244 peripheral
**/
__INLINE void NONNULL_INLINE__ AT42QT1244_Set_Reset_Time(AT42QT1244_t * const pCpnt) {
	pCpnt->hPowerOn = HAL_GetTick(); }

/*!\brief 100ms delay generator for AT42QT1244 peripheral
** \warning Delay is in blocking mode (only interrupts will run) and can take up to 100ms
** \param[in] pCpnt - Pointer to AT42QT1244 component
**/
void NONNULL__ AT42QT1244_Delay_PowerOn(AT42QT1244_t * const pCpnt);


/*!\brief 16bits CRC calculation for AT42QT1244
** \details 16bits crc calculation. Initial crc entry must be 0.
** The message is not augmented with 'zero' bits. polynomial = X16 + X15 + X2 + 1
** Repeat this function for each data block byte, folding the result back into the call parameter crc
** \param[in] crc - current crc value
** \param[in] data - new data for crc
** \return New CRC value
**/
uint16_t AT42QT1244_crc16(uint16_t crc, const uint8_t data);


/*!\brief Frequencies calibration for AT42QT1244 peripheral
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \param[in,out] hcrc - Pointer to resulting CRC value
** \return FctERR - error code
**/
FctERR NONNULL__ AT42QT1244_Calibrate_Freq_Hopping(AT42QT1244_t * const pCpnt, uint16_t * const hcrc);


/*!\brief Low level calibration for AT42QT1244 peripheral
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \return FctERR - error code
**/
FctERR NONNULL__ AT42QT1244_Calibrate_Low_Level(AT42QT1244_t * const pCpnt);


/*!\brief All keys calibration for AT42QT1244 peripheral
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \return FctERR - error code
**/
FctERR NONNULL__ AT42QT1244_Calibrate_All_Keys(AT42QT1244_t * const pCpnt);


/*!\brief Key calibration for AT42QT1244 peripheral
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \param[in] Key - Key to calibrate
** \return FctERR - error code
**/
FctERR NONNULL__ AT42QT1244_Calibrate_Key(AT42QT1244_t * const pCpnt, uint8_t Key);


/*!\brief Handler for AT42QT1244 peripheral
** \weak AT42QT1244 handler may be user implemented to suit custom needs
** \note May be called periodically to handle AT42QT1244 tasks
** \note Alternately may be called when event occurs on AT42QT1244 pin (or by calling \ref AT42QT1244_handler_it instead)
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \return FctERR - error code
**/
FctERR NONNULL__ AT42QT1244_handler(AT42QT1244_t * const pCpnt);

/*!\brief Handler for AT42QT1244 peripheral GPIO interrupt
** \note \ref AT42QT1244_INT_GPIO_Init has to be called at init before using interrupt handler function
** \weak AT42QT1244 GPIO interrupt handler may be user implemented to suit custom needs
** \note May be called periodically to handle AT42QT1244 tasks through interrupts
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \return FctERR - error code
**/
FctERR NONNULL__ AT42QT1244_handler_it(AT42QT1244_t * const pCpnt);


/*!\brief Handler for all AT42QT1244 peripherals
** \note May be called periodically to handle all AT42QT1244 tasks
**/
__INLINE void INLINE__ AT42QT1244_handler_all(void) {
	for (AT42QT1244_t * pCpnt = AT42QT1244 ; pCpnt < &AT42QT1244[I2C_AT42QT1244_NB] ; pCpnt++) {
		AT42QT1244_handler(pCpnt); }
}

/*!\brief Handler for all AT42QT1244 peripherals GPIO interrupt
** \note \ref AT42QT1244_INT_GPIO_Init has to be called at init before using interrupt handler function
** \note May be called periodically to handle all AT42QT1244 tasks
**/
__INLINE void INLINE__ AT42QT1244_handler_it_all(void) {
	for (AT42QT1244_t * pCpnt = AT42QT1244 ; pCpnt < &AT42QT1244[I2C_AT42QT1244_NB] ; pCpnt++) {
		AT42QT1244_handler_it(pCpnt); }
}


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* __AT42QT1244_PROC_H__ */
/****************************************************************/
