/*!\file I2C_component.h
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Base I2C component
** \note Needed symbols may be defined in globals.h or at project level. If globals.h is not used, symbol NO_GLOBALS_HEADER needs to be defined.
** \hideincludedbygraph
**/
/****************************************************************/
#ifndef I2C_COMPONENT_H__
	#define I2C_COMPONENT_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include <string.h>
#include "sarmfsw.h"

#ifndef NO_GLOBALS_HEADER
#include "globals.h"
#endif

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define	I2C_ADDR_General_Call	0			//!< I2C general call address

#define I2C_slave_timeout		50U			//!< Default timeout for transaction on I2C
#define I2C_eep_slave_timeout	500U		//!< Default timeout for eeprom type components transaction on I2C


#define I2C_ADDR(a)				((a) << 1U)	//!< Macro to shift \b a 1 bit left to define 7b I2C address


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum I2C_speed
** \brief I2C bus speed
**/
typedef enum PACK__ {
	I2C_STD = 100U,		//!< I2C Standard speed (100KHz)
	I2C_FM = 400U,		//!< I2C Fast Mode (400KHz)
	I2C_FMP = 1000U,	//!< I2C Fast Mode + (1MHz)
	I2C_HS = 3400U		//!< I2C High Speed (3.4MHz)
} I2C_speed;


/*!\enum I2C_reg_size
** \brief I2C slave internal address registers size
**/
typedef enum PACK__ {
	I2C_NO_REG = 0U,						//!< Internal address registers not applicable for slave
	I2C_8B_REG = I2C_MEMADD_SIZE_8BIT,		//!< Slave internal address registers space is 8bits wide
	I2C_16B_REG = I2C_MEMADD_SIZE_16BIT		//!< Slave internal address registers space is 16bits wide
} I2C_reg_size;


/*!\struct I2C_slave_t
** \brief i2c slave config and control parameters
**/
typedef struct {
	/*!\struct cfg
	** \brief I2C slave parameters
	**/
	struct {
		I2C_HandleTypeDef *	bus_inst;	//!< Slave I2C bus instance
		uint16_t			addr;		//!< Slave address
		uint32_t			timeout;	//!< Slave transaction timeout
		I2C_reg_size		mem_size;	//!< Slave internal registers size
		I2C_speed			max_speed;	//!< Maximal speed allowed by the component
	} cfg;
	uint16_t				addr;		//!< Current internal register address (not used yet)
	HAL_StatusTypeDef		status;		//!< Status of the last transaction
	bool					en;			//!< State of slave (disabled/enabled)
	bool					busy;		//!< TODO: implement to be checked if transaction pending (it?)
} I2C_slave_t;


#define IS_I2C_7B_ADDR(ADDR)	((ADDR) <= 0x7FU)	//!< Macro for use with assert_param to check I2C \b ADDR for slave


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief I2C Slave device initialization
** \hidecallergraph
** \param[in,out] slave - pointer to I2C slave instance to initialize
** \param[in] hi2c - pointer to HAL I2C instance
** \param[in] devAddress - Device Address
** \param[in] timeout - Transaction timeout
**/
void NONNULL__ I2C_slave_init(I2C_slave_t * const slave, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress, const uint32_t timeout);


/***************/
/*** SETTERS ***/
/***************/
/*!\brief I2C Slave device HAL instance change
** \param[in,out] slave - pointer to I2C slave instance
** \param[in] hi2c - pointer to HAL I2C instance
**/
void NONNULL__ I2C_set_slave_instance(I2C_slave_t * const slave, I2C_HandleTypeDef * const hi2c);

/*!\brief I2C Slave device address change
** \param[in,out] slave - pointer to I2C slave instance
** \param[in] devAddress - Device Address
**/
void NONNULL__ I2C_set_slave_address(I2C_slave_t * const slave, const uint16_t devAddress);

/*!\brief I2C Slave device transaction timeout change
** \param[in,out] slave - pointer to I2C slave instance
** \param[in] timeout - Transaction timeout
**/
__INLINE void NONNULL_INLINE__ I2C_set_slave_timeout(I2C_slave_t * const slave, const uint32_t timeout) {
	slave->cfg.timeout = timeout; }


/*!\brief Set I2C Slave device current internal memory address
** \param[in,out] slave - pointer to I2C slave instance
** \param[in] addr - address to store in slave instance
**/
__INLINE void NONNULL_INLINE__ I2C_set_current_mem_address(I2C_slave_t * const slave, const uint16_t addr) {
	slave->addr = addr; }

/*!\brief Set I2C Slave device disabled/enabled state
** \hidecallergraph
** \param[in,out] slave - pointer to I2C slave instance
** \param[in] en - I2C device state (disabled/enabled)
**/
__INLINE void NONNULL_INLINE__ I2C_set_enable(I2C_slave_t * const slave, const bool en) {
	slave->en = en; }


/*!\brief Set I2C Slave bus/device business
** \hidecallergraph
** \param[in,out] slave - pointer to I2C slave instance
** \param[in] busy - I2C bus/device state
**/
__INLINE void NONNULL_INLINE__ I2C_set_busy(I2C_slave_t * const slave, const bool busy) {
	slave->busy = busy; }


/***************/
/*** GETTERS ***/
/***************/
/*!\brief Get I2C Slave device current internal memory address
** \param[in,out] slave - pointer to I2C slave instance
** \return Current internal memory address
**/
__INLINE uint32_t NONNULL_INLINE__ I2C_get_current_mem_address(const I2C_slave_t * const slave) {
	return slave->addr; }

/*!\brief Get I2C Slave device enabled state
** \hidecallergraph
** \param[in,out] slave - pointer to I2C slave instance
** \return true if I2C slave is enabled
**/
__INLINE bool NONNULL_INLINE__ I2C_is_enabled(const I2C_slave_t * const slave) {
	return slave->en; }

/*!\brief Get I2C Slave device busy state
** \param[in,out] slave - pointer to I2C slave instance
** \return true if I2C bus/slave is busy
**/
__INLINE bool NONNULL_INLINE__ I2C_is_busy(const I2C_slave_t * const slave) {
	return slave->busy; }

/*!\brief Get I2C Slave device HAL I2C instance
** \param[in,out] slave - pointer to I2C slave instance
** \return I2C Slave device HAL I2C instance
**/
__INLINE I2C_HandleTypeDef * NONNULL_INLINE__ I2C_get_slave_instance(const I2C_slave_t * const slave) {
	return slave->cfg.bus_inst; }

/*!\brief Get I2C Slave device address
** \param[in,out] slave - pointer to I2C slave instance
** \return I2C Slave device address
**/
__INLINE uint16_t NONNULL_INLINE__ I2C_get_slave_address(const I2C_slave_t * const slave) {
	return slave->cfg.addr; }

/*!\brief Get I2C Slave device transaction timeout
** \param[in,out] slave - pointer to I2C slave instance
** \return I2C Slave device transaction timeout
**/
__INLINE uint32_t NONNULL_INLINE__ I2C_get_slave_timeout(const I2C_slave_t * const slave) {
	return slave->cfg.timeout; }

/*!\brief Get I2C Slave device internal memory size
** \param[in,out] slave - pointer to I2C slave instance
** \return I2C Slave device internal memory size
**/
__INLINE uint16_t NONNULL_INLINE__ I2C_get_slave_mem_size(const I2C_slave_t * const slave) {
	return slave->cfg.mem_size; }

/*!\brief Get I2C Slave device max speed
** \param[in,out] slave - pointer to I2C slave instance
** \return I2C Slave device maximum speed
**/
__INLINE uint16_t NONNULL_INLINE__ I2C_get_slave_max_speed(const I2C_slave_t * const slave) {
	return slave->cfg.max_speed; }


/*****************/
/*** CALLBACKS ***/
/*****************/
/*!\brief I2C Watchdog refresh callback
** \weak Function declared as weak, can be customly implemented in user code is specific actions needs to be taken (IWDG refreshed by default)
**/
void I2C_Watchdog_Refresh(void);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif	/* I2C_COMPONENT_H__ */
/****************************************************************/
