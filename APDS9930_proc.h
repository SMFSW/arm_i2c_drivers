/*!\file APDS9930_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief APDS9930 Driver procedures
** \details APDS9930: Digital Proximity and Ambient Light Sensor
**/
/****************************************************************/
#ifndef __APDS9930_PROC_H__
	#define __APDS9930_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "APDS9930.h"

#include "shared_ALS.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/
// TODO: Add proximity results


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define APDS9930_CHIP_ID				0x39	//!< APDS9930 Chip ID to check against


#if !defined(APDS9930_OPEN_AIR_ATTENUATION)
#define APDS9930_OPEN_AIR_ATTENUATION	0.49f	//!< Open air attenuation factor
#endif
#if !defined(APDS9930_GLASS_ATTENUATION)
#define APDS9930_GLASS_ATTENUATION	APDS9930_OPEN_AIR_ATTENUATION
#endif


#define APDS9930_DEVICE_FACTOR			52		//!< APDS9930 Device factor

#define APDS9930_DEF_PROX_PULSES		8		//!< Recommended proximity pulses in a cycle


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum APDS9930_als_gain_ex
** \brief Gain values of APDS9930 ALS
** \warning Datasheet p9 states ok, but CONFIG register description p22 states not to use 16 or 120 gain when AGL asserted
**/
typedef enum PACK__ APDS9930_als_gain_ex {
	APDS9930__ALS_GAIN_1X = 0,		//!< ALS 1X Gain
	APDS9930__ALS_GAIN_8X,			//!< ALS 8X Gain
	APDS9930__ALS_GAIN_16X,			//!< ALS 16X Gain
	APDS9930__ALS_GAIN_120X,		//!< ALS 120X Gain
	APDS9930__ALS_GAIN_1ON6X,		//!< ALS 1/6X Gain
	APDS9930__ALS_GAIN_8ON6X,		//!< ALS 8/6X Gain
	//APDS9930__ALS_GAIN_16ON6X,	//!< ALS 16/6X Gain
	//APDS9930__ALS_GAIN_120ON6X	//!< ALS 120/6X Gain
} APDS9930_als_gain_ex;

/*!\enum APDS9930_drive_strength_ex
** \brief Proximity LED Drive strength values
**/
typedef enum PACK__ APDS9930_drive_strength_ex {
	APDS9930__STRENGTH_100MA = 0,	//!< PDL=0 : 100mA
	APDS9930__STRENGTH_50MA,		//!< PDL=0 : 50mA
	APDS9930__STRENGTH_25MA,		//!< PDL=0 : 25mA
	APDS9930__STRENGTH_12_5MA,		//!< PDL=0 : 12.5mA
	APDS9930__STRENGTH_11_1MA,		//!< PDL=1 : 11.1mA
	APDS9930__STRENGTH_5_6MA,		//!< PDL=1 : 5.6mA
	APDS9930__STRENGTH_2_8MA,		//!< PDL=1 : 2.8mA
	APDS9930__STRENGTH_1_4MA,		//!< PDL=1 : 1.4mA
} APDS9930_drive_strength_ex;


/*!\struct APDS9930_t
** \brief APDS9930 user interface struct
**/
typedef struct APDS9930_t {
	uint16_t					Full;				//!< Current Full spectrum raw value
	uint16_t					IR;					//!< Current IR raw value
	uint16_t					Prox;				//!< Current Proximity raw value
	uint32_t					Lux;				//!< Current Illuminance (lux)
	float						IRF;				//!< Current Current IR ratio
	bool						SaturationRipple;	//!< Current Ripple saturation status (75% of saturation value)
	bool						Saturation;			//!< Current Sensor saturation status
	struct {
	I2C_slave_t *				slave_inst;			//!< Slave structure
	float						GA;					//!< Glass attenuation factor
	float						LPC;				//!< Lux per count config
	APDS9930_als_gain_ex		ALS_Gain;			//!< ALS sensor gain config
	APDS9930_prox_gain			Prox_Gain;			//!< Proximity sensor gain config
	APDS9930_drive_strength_ex	Prox_Strength;		//!< Proximity LED drive strength
	uint8_t						Prox_Pulses;		//!< Proximity pulses sent in a cycle
	uint8_t						ALS_Integ_reg;		//!< ALS sensor integration time config (value written to register)
	uint16_t					ALS_Integ;			//!< ALS sensor integration time config
	uint16_t					Prox_Integ;			//!< Proximity sensor integration time config
	uint16_t					Wait;				//!< Sensor wait time config
	uint16_t					ALS_LowThreshold;	//!< ALS Low Threshold config
	uint16_t					ALS_HighThreshold;	//!< ALS High Threshold config
	uint16_t					Prox_LowThreshold;	//!< Proximity Low Threshold config
	uint16_t					Prox_HighThreshold;	//!< Proximity High Threshold config
	bool						AIEN;				//!< Sensor ALS (Ambient Light Sensing) interrupts enabled config
	bool						PIEN;				//!< Sensor Proximity interrupts enabled config
	bool						WEN;				//!< Wait between conversions enabled config
	uint8_t						Id;					//!< APDS9930 Chip ID
	} cfg;
} APDS9930_t;

extern APDS9930_t	APDS9930[I2C_APDS9930_NB];	//!< APDS9930 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for APDS9930 peripheral
** \weak APDS9930 Init sequence may be user implemented if custom initialization sequence needed
** \param[in] pCpnt - Pointer to APDS9930 component
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_Init_Sequence(APDS9930_t * pCpnt);


/*!\brief Set proper LPC value (Lux per Count)
 * \note Should be called after Configuration change of Integration time or Gain
** \param[in] pCpnt - Pointer to APDS9930 component
** \return FctERR - error code
**/
void NONNULL__ APDS9930_Set_LPC(APDS9930_t * pCpnt);

/*!\brief Get current Illuminance (in lux)
** \param[in] pCpnt - Pointer to APDS9930 component
** \return FctERR - error code
**/
__INLINE uint32_t NONNULL_INLINE__ APDS9930_Get_Lux(APDS9930_t * pCpnt) {
	return pCpnt->Lux; }

/*!\brief Handler for APDS9930 peripheral
** \weak APDS9930 handler may be user implemented to suit custom needs
** \note May be called periodically to handle APDS9930 tasks
** \note Alternately may be called when event occurs on APDS9930 pin
** \param[in] pCpnt - Pointer to APDS9930 component
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9930_handler(APDS9930_t * pCpnt);

/*!\brief Handler for all APDS9930 peripherals
** \note May be called periodically to handle all APDS9930 tasks
**/
__INLINE void INLINE__ APDS9930_handler_all(void) {
	for (APDS9930_t * pCpnt = APDS9930 ; pCpnt < &APDS9930[I2C_APDS9930_NB] ; pCpnt++) {
		APDS9930_handler(pCpnt); }
}


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif
#endif /* __APDS9930_PROC_H__ */
/****************************************************************/
