/*!\file MCP9808_proc.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief MCP9808 Driver procedures
** \details MCP9808: +/-0.5C Maximum Accuracy Digital Temperature Sensor
**/
/****************************************************************/
#ifndef MCP9808_PROC_H__
	#define MCP9808_PROC_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "MCP9808.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define MCP9808_MANUFACTURER_ID		0x0054		//!< MCP9808 Manufacturer ID to check against
#define MCP9808_CHIP_ID				0x0400		//!< MCP9808 Chip ID to check against


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum MCP9808_alert
** \brief Alerts enum of MCP9808
**/
typedef enum PACK__ MCP9808_alert {
	MCP9808__ALERT_HIGH = 0,	//!< High temperature alert
	MCP9808__ALERT_LOW,			//!< Low temperature alert
	MCP9808__ALERT_CRIT			//!< Critical temperature alert
} MCP9808_alert;


/*!\struct MCP9808_t
** \brief MCP9808 user interface struct
**/
typedef struct MCP9808_t {
	float				Temperature;		//!< Current temperature
	bool				TUpper;				//!< Upper alert reached
	bool				TLower;				//!< Lower alert reached
	bool				TCrit;				//!< Critical alert reached
	bool				NewData;			//!< New data available
	uint32_t			hLast;				//!< Tick from last data
	struct {
	I2C_slave_t *		slave_inst;			//!< Slave structure
	PeripheralGPIO_t	Alert_GPIO;			//!< Alert GPIO struct
	MCP9808_res			Resolution;			//!< Resolution config
	float				HighAlert;			//!< High alert config
	float				LowAlert;			//!< Low alert config
	float				CriticalAlert;		//!< Critical alert config
	uint16_t			Manufacturer_Id;	//!< Manufacturer ID
	uint16_t			Device_Id;			//!< Device ID
	} cfg;
} MCP9808_t;

extern MCP9808_t	MCP9808[I2C_MCP9808_NB];	//!< MCP9808 User structure


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Procedures ***/
/******************/

/*!\brief Initialization Sequence of the MCP9808 peripheral
** \weak MCP9808 handler may be user implemented to suit custom needs
** \param[in] pCpnt - Pointer to MCP9808 component
** \return FctERR - error code
**/
FctERR NONNULL__ MCP9808_Init_Sequence(MCP9808_t * const pCpnt);

/*!\brief Set the high/low/crit Alert temperature
** \param[in] pCpnt - Pointer to MCP9808 component
** \param[in,out] temp - temperature (in Celsius degrees)
** \param[in] alt - Alert type
** \return FctERR - error code
**/
FctERR NONNULL__ MCP9808_Set_AlertTemp(MCP9808_t * const pCpnt, const float temp, const MCP9808_alert alt);


/*!\brief Get the high/low/crit Alert temperature
** \param[in] pCpnt - Pointer to MCP9808 component
** \param[in,out] temp - pointer to temperature to read to (in Celsius degrees)
** \param[in] alt - Alert type
** \return FctERR - error code
**/
FctERR NONNULLX__(1) MCP9808_Get_AlertTemp(MCP9808_t * const pCpnt, float * temp, MCP9808_alert alt);

/*!\brief Get the temperature
** \param[in] pCpnt - Pointer to MCP9808 component
** \param[in,out] temp - pointer to temperature to read to (in Celsius degrees)
** \return FctERR - error code
**/
FctERR NONNULLX__(1) MCP9808_Get_Temperature(MCP9808_t * const pCpnt, float * temp);


/*!\brief Handler for MCP9808 peripheral
** \weak MCP9808 handler may be user implemented to suit custom needs
** \note Should be called periodically to handle MCP9808 tasks (or by calling \ref MCP9808_handler_it instead)
** \param[in] pCpnt - Pointer to MCP9808 component
** \return FctERR - error code
**/
FctERR NONNULL__ MCP9808_handler(MCP9808_t * const pCpnt);

/*!\brief Handler for MCP9808 peripheral GPIO interrupt
** \note \ref MCP9808_Alert_GPIO_Init has to be called at init before using interrupt handler function
** \weak MCP9808 GPIO interrupt handler may be user implemented to suit custom needs
** \note May be called periodically to handle MCP9808 tasks through interrupts
** \param[in] pCpnt - Pointer to MCP9808 component
** \return FctERR - error code
**/
FctERR NONNULL__ MCP9808_handler_it(MCP9808_t * const pCpnt);


/*!\brief Handler for all MCP9808 peripherals
** \note May be called periodically to handle all MCP9808 tasks
**/
__INLINE void INLINE__ MCP9808_handler_all(void) {
	for (MCP9808_t * pCpnt = MCP9808 ; pCpnt < &MCP9808[I2C_MCP9808_NB] ; pCpnt++) {
		MCP9808_handler(pCpnt); }
}

/*!\brief Handler for all MCP9808 peripherals GPIO interrupt
** \note \ref MCP9808_Alert_GPIO_Init has to be called at init before using interrupt handler function
** \note May be called periodically to handle all MCP9808 tasks
**/
__INLINE void INLINE__ MCP9808_handler_it_all(void) {
	for (MCP9808_t * pCpnt = MCP9808 ; pCpnt < &MCP9808[I2C_MCP9808_NB] ; pCpnt++) {
		MCP9808_handler_it(pCpnt); }
}


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* MCP9808_PROC_H__ */
/****************************************************************/
