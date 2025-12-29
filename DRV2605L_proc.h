/*!\file DRV2605L_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief DRV2605L Driver procedures
** \details DRV2605L: 2 to 5.2V Haptic Driver for LRA and ERM With Effect Library and Smart-Loop Architecture
**/
/****************************************************************/
#ifndef DRV2605L_PROC_H__
	#define DRV2605L_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "DRV2605L.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define DRV2604_CHIP_ID			0x80U	//!< DRV2604 Chip ID to check against
#define DRV2605_CHIP_ID			0x60U	//!< DRV2605 Chip ID to check against
#define DRV2604L_CHIP_ID		0xA0U	//!< DRV2604L Chip ID to check against
#define DRV2605L_CHIP_ID		0xE0U	//!< DRV2605L Chip ID to check against


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct _DRV2605L_t
** \brief DRV2605L user interface struct
**/
typedef struct _DRV2605L_t {
	struct {
	I2C_slave_t *		slave_inst;	//!< Slave structure
	PeripheralGPIO_t	EN_GPIO;	//!< Enable GPIO struct
	PeripheralGPIO_t	TRIG_GPIO;	//!< Trigger GPIO struct
	DRV2605L_mode		Mode;		//!< Current haptic driver mode
	DRV2605L_act		ERM_LRA;	//!< Current haptic actuator type
	DRV2605L_loop		Open_Loop;	//!< Current loop type (0 closed loop / auto resonance, 1 open)
	DRV2605L_input		Input_Mode;	//!< Current input mode(PWM / Analog)
	DRV2605L_rtp_format	RTP_Format;	//!< Current Real Time Playback format
	uint8_t				ChipID;		//!< Chip ID
	} cfg;
} DRV2605L_t;

extern DRV2605L_t	DRV2605L;		//!< DRV2605L User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for DRV2605L peripheral
** \weak DRV2605L Init sequence may be user implemented if custom initialization sequence needed
** \return FctERR - error code
**/
FctERR DRV2605L_Init_Sequence(void);


/*!\brief Auto-calibration procedure for DRV2605L peripheral
** \return FctERR - error code
**/
FctERR DRV2605L_Auto_Calib(void);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* DRV2605L_PROC_H__ */
/****************************************************************/
