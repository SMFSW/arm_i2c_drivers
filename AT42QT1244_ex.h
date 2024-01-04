/*!\file AT42QT1244_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
** \brief AT42QT1244 Driver extensions
** \details AT42QT1244: 24-key QMatrix FMEA IEC/EN/UL60730 Touch Sensor
**/
/****************************************************************/
#ifndef __AT42QT1244_EX_H__
	#define __AT42QT1244_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "AT42QT1244.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\union uAT42QT_REG_MAP
** \brief Union of AT42QT1244 registry map
**/
typedef union uAT42QT_REG_MAP {
	uint8_t Bytes[251];
	struct PACK__ {
		uint8_t						Res_1;
		uint8_t						Res_2;
		uint8_t						Counter_100ms;
		uint8_t						Counter_Signal_Fail;
		uint8_t						Counter_Matrix_Scan;
		uAT42QT_REG__DEVICE_STATUS	Device_Status;
		uint8_t						Detect_Status_0_7;
		uint8_t						Detect_Status_8_15;
		uint8_t						Detect_Status_16_23;
		uint8_t						Res_3;
		uint8_t						Current_Frequency;
		uint8_t						Current_Pulse_Spacing;
		uAT42QT_REG__KEY_DATA		KeyData[AT42QT1244_MAX_KEYS];
		uint8_t						Res_4[31];
		uint8_t						Control_Command;
		uAT42QT_REG__SETUP_141_164	Setup_Key_Thr[AT42QT1244_MAX_KEYS];
		uAT42QT_REG__SETUP_165_188	Setup_Key_Mode[AT42QT1244_MAX_KEYS];
		uint8_t						Setup_Calib_Frequency_Offset_1[AT42QT1244_MAX_KEYS];
		uint8_t						Setup_Calib_Frequency_Offset_2[AT42QT1244_MAX_KEYS];
		uint8_t						Setup_Negative_Recall_Delay;
		uAT42QT_REG__SETUP_238		Setup_Debug_MSync;
		uint8_t						Setup_Awake_Timeout;
		uint8_t						Setup_Drift_Hold_Time;
		uAT42QT_REG__SETUP_241		Setup_Drift;
		uAT42QT_REG__SETUP_242		Setup_Lower_Signal_Limit_1;
		uAT42QT_REG__SETUP_243		Setup_Lower_Signal_Limit_2;
		uAT42QT_REG__SETUP_244		Setup_Freq_Hoping_Dwell;
		uint8_t						Setup_Frequency_0;
		uint8_t						Setup_Frequency_1;
		uint8_t						Setup_Frequency_2;
		uAT42QT_REG__SETUP_248		Setup_Noise;
		union {
		uint8_t						Setup_Host_CRC[2];		//!< Host CRC (LSB at addr 249, MSB at addr 250)
		uint16_t					Setup_Host_CRC_Word;	//!< Host CRC
		};
		uint8_t						Setup_Positive_Recalibration_Delay;
	} Reg;
} uAT42QT_REG_MAP;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

/*!\brief Send command to AT42QT1244 peripheral
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \param[in] cmd - Command to send
** \note \ref AT42QT__WRITE_SETUPS cannot be used in this function (does not make sense, as it's used to write whole setup in one transmission)
** \return FctERR - error code
**/
FctERR NONNULL__ AT42QT1244_Send_Command(AT42QT1244_t * const pCpnt, const AT42QT_cmd cmd);

/*!\brief Send setup parameters to AT42QT1244 peripheral
** \note No AT42QT1244 peripheral reset done after writing setups, should be customly implemented when needed
** \note Some issues encountered while writing setup block (reason unidentified), resetting the MCU and trying to write again seems to solve issue
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \param[in,out] hcrc - Pointer to resulting CRC value
** \param[in] setup - Parameters to send
** \param[in] addr - Setup address in AT42QT1244 peripheral
** \param[in] nb - Number of setup values to send
** \return FctERR - error code
**/
FctERR NONNULL__ AT42QT1244_Send_Setup(AT42QT1244_t * const pCpnt, uint16_t * const hcrc, const uint8_t * setup, const uint8_t addr, const uint8_t nb);

/*!\brief Key use setup for AT42QT1244 peripheral
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \param[in,out] hcrc - Pointer to resulting CRC value
** \param[in] mask_keys - mask for keys to configure
** \param[in] use - Key enabled/disabled
** \return FctERR - error code
**/
FctERR NONNULL__ AT42QT1244_Setup_Keys(AT42QT1244_t * const pCpnt, uint16_t * const hcrc, const uint32_t mask_keys, const bool use);

/*!\brief Frequency hopping mode setup for AT42QT1244 peripheral
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \param[in,out] hcrc - Pointer to resulting CRC value
** \param[in] FHM - Frequency hopping mode parameters
** \return FctERR - error code
**/
FctERR NONNULL__ AT42QT1244_Setup_FHM(AT42QT1244_t * const pCpnt, uint16_t * const hcrc, const AT42QT_FHM FHM);


/*!\brief Reset command for AT42QT1244 peripheral
** \warning After Reset command is sent, a blocking delay described in AT42QT1244_Delay_PowerOn is called to let component initialize properly
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \return FctERR - error code
**/
FctERR NONNULL__ AT42QT1244_Reset(AT42QT1244_t * const pCpnt);


/*!\brief Get keys detect status for AT42QT1244 peripheral
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \param[in] Keys - Pointer to keys result
** \return FctERR - error code
**/
FctERR NONNULL__ AT42QT1244_Get_Keys(AT42QT1244_t * const pCpnt, uint32_t * Keys);


/*!\brief Get key data for AT42QT1244 peripheral
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \param[in] Key_Data - Pointer to key data result
** \param[in] Key_Num - Key to get datas
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ AT42QT1244_Get_Key_Data(AT42QT1244_t * const pCpnt, uAT42QT_REG__KEY_DATA * Key_Data, const uint8_t Key_Num) {
	return AT42QT1244_Read(pCpnt->cfg.slave_inst, (uint8_t *) Key_Data, AT42QT__DATA_KEY_1 + (Key_Num * sizeof(uAT42QT_REG__KEY_DATA)), sizeof(uAT42QT_REG__KEY_DATA)); }


/*!\brief Get status of AT42QT1244 peripheral
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \param[in] st - Pointer to peripheral status result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ AT42QT1244_Get_Status(AT42QT1244_t * const pCpnt, uAT42QT_REG__DEVICE_STATUS * st) {
	return AT42QT1244_Read(pCpnt->cfg.slave_inst, (uint8_t *) st, AT42QT__DEVICE_STATUS, 1); }


/*!\brief Get calibration status for AT42QT1244 peripheral
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \return Calibration status
**/
intCPU_t NONNULL__ AT42QT1244_is_Calib_Pending(AT42QT1244_t * const pCpnt);


/*******************/
/*** GPIO access ***/
/*******************/

/*!\brief Change GPIO pin init for AT42QT1244
** \weak AT42QT1244 Change GPIO pin init may be user implemented if needed
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \param[in] GPIOx - CHANGE port
** \param[in] GPIO_Pin - CHANGE pin
** \param[in] GPIO_Active: CHANGE pin active state
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ AT42QT1244_CHANGE_GPIO_Init(AT42QT1244_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active);

/*!\brief Change GPIO pin getter for AT42QT1244
** \weak AT42QT1244 Change GPIO pin getter may be user implemented if needed
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \param[in,out] pState - Pointer to CHANGE pin state variable (0: inactive, 1: active)
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ AT42QT1244_CHANGE_GPIO_Get(AT42QT1244_t * const pCpnt, bool * const pState);


/*!\brief Reset GPIO pin init for AT42QT1244
** \weak AT42QT1244 Reset GPIO pin init may be user implemented if needed
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \param[in] GPIOx - RST port
** \param[in] GPIO_Pin - RST pin
** \param[in] GPIO_Active: RST pin active state
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ AT42QT1244_RST_GPIO_Init(AT42QT1244_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active);

/*!\brief Reset GPIO pin setter for AT42QT1244
** \weak AT42QT1244 Reset GPIO pin setter may be user implemented if needed
** \param[in] pCpnt - Pointer to AT42QT1244 component
** \param[in] state - state to write on RST pin (0: inactive, 1: active)
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ AT42QT1244_RST_GPIO_Set(AT42QT1244_t * const pCpnt, const bool state);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* __AT42QT1244_EX_H__ */
/****************************************************************/
