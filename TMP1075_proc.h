/*!\file TMP1075_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief TMP1075 Driver procedures
** \details TMP1075: Temperature Sensor With I2C and SMBus Interface in Industry Standard LM75 Form Factor and Pinout
**/
/****************************************************************/
#ifndef TMP1075_PROC_H__
	#define TMP1075_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "TMP1075.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define TMP1075_CFGR_7_0	0xFF		//!< TMP1075 Reserved bits set to FFh
#define TMP1075N_CHIP_ID	0x7500		//!< TMP1075 Chip ID to check against



// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum TMP1075_limit
** \brief Limits enum of TMP1075
**/
typedef enum PACK__ TMP1075_limit {
	TMP1075__LIMIT_LOW = 0,		//!< Low temperature limit
	TMP1075__LIMIT_HIGH			//!< High temperature limit
} TMP1075_limit;


/*!\struct TMP1075_t
** \brief TMP1075 user interface struct
**/
typedef struct TMP1075_t {
	float				Temperature;		//!< Current temperature
	bool				NewData;			//!< New data available
	bool				Ongoing;			//!< Single conversion launched
	uint32_t			hLast;				//!< Tick from last data
	struct {
	I2C_slave_t *		slave_inst;			//!< Slave structure
	PeripheralGPIO_t	INT_GPIO;			//!< Interrupt GPIO struct
	TMP1075_conv_rate	Rate;				//!< Conversion rate config
	bool				Continuous;			//!< Continuous conversion mode
	bool				TMP1075N;			//!< Set to true if TMP1075N
	float				LowLimit;			//!< Low limit config
	float				HighLimit;			//!< High limit config
	uint16_t			Id;					//!< Device ID
	} cfg;
} TMP1075_t;

extern TMP1075_t TMP1075[I2C_TMP1075_NB];		//!< TMP1075 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence for TMP1075 peripheral
** \weak TMP1075 Init sequence may be user implemented if custom initialization sequence needed
** \param[in] pCpnt - Pointer to TMP1075 component
** \return FctERR - error code
**/
FctERR NONNULL__ TMP1075_Init_Sequence(TMP1075_t * const pCpnt);


/*!\brief Set the low/high limit temperature
** \param[in] pCpnt - Pointer to TMP1075 component
** \param[in,out] temp - temperature (in Celsius degrees)
** \param[in] lim - Limit type
** \return FctERR - error code
**/
FctERR NONNULL__ TMP1075_Set_LimitTemp(TMP1075_t * const pCpnt, const float temp, const TMP1075_limit lim);


/*!\brief Get the low/high limit temperature
** \param[in] pCpnt - Pointer to TMP1075 component
** \param[in,out] temp - pointer to temperature to read to (in Celsius degrees)
** \param[in] lim - Limit type
** \return FctERR - error code
**/
FctERR NONNULLX__(1) TMP1075_Get_LimitTemp(TMP1075_t * const pCpnt, float * temp, TMP1075_limit lim);


/*!\brief Get the temperature
** \param[in] pCpnt - Pointer to TMP1075 component
** \param[in,out] temp - pointer to temperature to read to (in Celsius degrees)
** \return FctERR - error code
**/
FctERR NONNULLX__(1) TMP1075_Get_Temperature(TMP1075_t * const pCpnt, float * temp);


/*!\brief Handler for TMP1075 peripheral
** \weak TMP1075 handler may be user implemented to suit custom needs
** \note May be called periodically to handle TMP1075 tasks
** \note Alternately may be called when event occurs on TMP1075 pin (or by calling \ref TMP1075_handler_it instead)
** \param[in] pCpnt - Pointer to TMP1075 component
** \return FctERR - error code
**/
FctERR NONNULL__ TMP1075_handler(TMP1075_t * const pCpnt);

/*!\brief Handler for TMP1075 peripheral GPIO interrupt
** \note \ref TMP1075_INT_GPIO_Init has to be called at init before using interrupt handler function
** \weak TMP1075 GPIO interrupt handler may be user implemented to suit custom needs
** \note May be called periodically to handle TMP1075 tasks through interrupts
** \param[in] pCpnt - Pointer to TMP1075 component
** \return FctERR - error code
**/
FctERR NONNULL__ TMP1075_handler_it(TMP1075_t * const pCpnt);

/*!\brief Handler for all TMP1075 peripherals
** \note May be called periodically to handle all TMP1075 tasks
** \return FctERR - error code
**/
FctERR TMP1075_handler_all(void);

/*!\brief Handler for all TMP1075 peripherals GPIO interrupt
** \note \ref TMP1075_INT_GPIO_Init has to be called at init before using interrupt handler function
** \note May be called periodically to handle all TMP1075 tasks
** \return FctERR - error code
**/
FctERR TMP1075_handler_it_all(void);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* TMP1075_PROC_H__ */
/****************************************************************/
