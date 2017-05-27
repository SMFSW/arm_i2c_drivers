/*!\file DRV2605L.h
** \author SMFSW
** \version v0.2
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief DRV2605L Driver procedures declarations
** \details DRV2605L: 2 to 5.2V Haptic Driver for LRA and ERM With Effect Library and Smart-Loop Architecture
**/
/****************************************************************/
#ifndef __DRV2605L_PROC_H__
	#define __DRV2605L_PROC_H__

#include "DRV2605L.h"

#include "sarmfsw.h"
#include CMSIS_INC
#include CMSIS_CFG

#if defined(HAL_I2C_MODULE_ENABLED)
#include "i2c.h"
/****************************************************************/
// TODO: doxygen


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define DRV2605_CHIP_ID			0x60	//!< DRV2605 Chip ID to check against
#define DRV2604_CHIP_ID			0x80	//!< DRV2604 Chip ID to check against
#define DRV2604L_CHIP_ID		0xA0	//!< DRV2604L Chip ID to check against
#define DRV2605L_CHIP_ID		0xE0	//!< DRV2605L Chip ID to check against


// *****************************************************************************
// Section: Types
// *****************************************************************************
typedef struct DRV2605L_proc {
	struct {
	DRV2605L_mode		Mode;
	DRV2605L_act		ERM_LRA;
	DRV2605L_loop		Open_Loop;
	DRV2605L_input		Input_Mode;
	DRV2605L_rtp_format	RTP_Format;
	uint8_t				Id;
	} cfg;
} DRV2605L_proc;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for DRV2605L peripheral
** \return FctERR - error code
**/
FctERR DRV2605L_Init_Sequence(void);


/*!\brief Auto-calibration procedure for DRV2605L peripheral
** \return FctERR - error code
**/
FctERR DRV2605L_Auto_Calib(void);


/****************************************************************/
#endif
#endif /* __DRV2605L_PROC_H__ */
/****************************************************************/
