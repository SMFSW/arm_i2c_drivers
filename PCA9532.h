/*!\file PCA9532.h
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief PCA9532 Driver
** \details PCA9532: 16-bit I2C-bus LED dimmer
**/
/****************************************************************/
#ifndef __PCA9532_H__
	#define __PCA9532_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "globals.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#include "PCA9xxx.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/

#ifndef I2C_PCA9532_NB
//! \note Define I2C_PCA9532_NB in globals.h or at project to enable multiple peripherals of this type
#define I2C_PCA9532_NB	1	//!< Number of PCA9532 peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
#ifndef PCA9532_BASE_ADDR
//! \note Define PCA9532_BASE_ADDR in globals.h or at project to change default device base address
#define	PCA9532_BASE_ADDR			0x60		//!< PCA9532 Base Address
#endif


// *****************************************************************************
// Section: Datas
// *****************************************************************************
extern I2C_slave_t PCA9532_hal[I2C_PCA9532_NB];	//!< PCA9532 Slave structure


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum PCA9532_reg_map
** \brief Register map enum of PCA9532
**/
typedef enum PACK__ PCA9532_reg_map {
	PCA9532__INPUT0 = 0,	//!< register INPUT0
	PCA9532__INPUT1,		//!< register INPUT1
	PCA9532__PSC0,			//!< register Frequency Prescaler 0
	PCA9532__PWM0,			//!< register PWM0
	PCA9532__PSC1,			//!< register Frequency Prescaler 1
	PCA9532__PWM1,			//!< register PWM1
	PCA9532__LS0,			//!< register LED0 to LED3 selector
	PCA9532__LS1,			//!< register LED4 to LED7 selector
	PCA9532__LS2,			//!< register LED8 to LED11 selector
	PCA9532__LS3,			//!< register LED12 to LED15 selector
} PCA9532_reg;


/*!\union uPCA9532_REG__INPUT0
** \brief Union for INPUT0 register of PCA9532
**/
typedef union PACK__ uPCA9532_REG__INPUT0 {
	uint8_t Byte;
	struct {
		uint8_t LED0		:1;		//!< LED0 input pin state
		uint8_t LED1		:1;		//!< LED1 input pin state
		uint8_t LED2		:1;		//!< LED2 input pin state
		uint8_t LED3		:1;		//!< LED3 input pin state
		uint8_t LED4		:1;		//!< LED4 input pin state
		uint8_t LED5		:1;		//!< LED5 input pin state
		uint8_t LED6		:1;		//!< LED6 input pin state
		uint8_t LED7		:1;		//!< LED7 input pin state
	} Bits;
} uPCA9532_REG__INPUT0;

/*!\union uPCA9532_REG__INPUT1
** \brief Union for INPUT1 register of PCA9532
**/
typedef union PACK__ uPCA9532_REG__INPUT1 {
	uint8_t Byte;
	struct {
		uint8_t LED8		:1;		//!< LED8 input pin state
		uint8_t LED9		:1;		//!< LED9 input pin state
		uint8_t LED10		:1;		//!< LED10 input pin state
		uint8_t LED11		:1;		//!< LED11 input pin state
		uint8_t LED12		:1;		//!< LED12 input pin state
		uint8_t LED13		:1;		//!< LED13 input pin state
		uint8_t LED14		:1;		//!< LED14 input pin state
		uint8_t LED15		:1;		//!< LED15 input pin state
	} Bits;
} uPCA9532_REG__INPUT1;

/*!\union uPCA9532_REG__INPUT
** \brief Union for INPUT registers of PCA9532
**/
typedef union PACK__ uPCA9532_REG__INPUT {
	uint16_t Word;
	struct {
		uint16_t LED0		:1;		//!< LED0 input pin state
		uint16_t LED1		:1;		//!< LED1 input pin state
		uint16_t LED2		:1;		//!< LED2 input pin state
		uint16_t LED3		:1;		//!< LED3 input pin state
		uint16_t LED4		:1;		//!< LED4 input pin state
		uint16_t LED5		:1;		//!< LED5 input pin state
		uint16_t LED6		:1;		//!< LED6 input pin state
		uint16_t LED7		:1;		//!< LED7 input pin state
		uint16_t LED8		:1;		//!< LED8 input pin state
		uint16_t LED9		:1;		//!< LED9 input pin state
		uint16_t LED10		:1;		//!< LED10 input pin state
		uint16_t LED11		:1;		//!< LED11 input pin state
		uint16_t LED12		:1;		//!< LED12 input pin state
		uint16_t LED13		:1;		//!< LED13 input pin state
		uint16_t LED14		:1;		//!< LED14 input pin state
		uint16_t LED15		:1;		//!< LED15 input pin state
	} Bits;
} uPCA9532_REG__INPUT;


/*!\union uPCA9532_REG__LS0
** \brief Union for LS0 register of PCA9532
**/
typedef union PACK__ uPCA9532_REG__LS0 {
	uint8_t Byte;
	struct {
		PCA95xx_ledsel LED0		:2;		//!< LED0 output state control
		PCA95xx_ledsel LED1		:2;		//!< LED1 output state control
		PCA95xx_ledsel LED2		:2;		//!< LED2 output state control
		PCA95xx_ledsel LED3		:2;		//!< LED3 output state control
	} Bits;
} uPCA9532_REG__LS0;

/*!\union uPCA9532_REG__LS1
** \brief Union for LS1 register of PCA9532
**/
typedef union PACK__ uPCA9532_REG__LS1 {
	uint8_t Byte;
	struct {
		PCA95xx_ledsel LED4		:2;		//!< LED4 output state control
		PCA95xx_ledsel LED5		:2;		//!< LED5 output state control
		PCA95xx_ledsel LED6		:2;		//!< LED6 output state control
		PCA95xx_ledsel LED7		:2;		//!< LED7 output state control
	} Bits;
} uPCA9532_REG__LS1;

/*!\union uPCA9532_REG__LS2
** \brief Union for LS2 register of PCA9532
**/
typedef union PACK__ uPCA9532_REG__LS2 {
	uint8_t Byte;
	struct {
		PCA95xx_ledsel LED8		:2;		//!< LED8 output state control
		PCA95xx_ledsel LED9		:2;		//!< LED9 output state control
		PCA95xx_ledsel LED10	:2;		//!< LED10 output state control
		PCA95xx_ledsel LED11	:2;		//!< LED11 output state control
	} Bits;
} uPCA9532_REG__LS2;

/*!\union uPCA9532_REG__LS3
** \brief Union for LS3 register of PCA9532
**/
typedef union PACK__ uPCA9532_REG__LS3 {
	uint8_t Byte;
	struct {
		PCA95xx_ledsel LED12	:2;		//!< LED12 output state control
		PCA95xx_ledsel LED13	:2;		//!< LED13 output state control
		PCA95xx_ledsel LED14	:2;		//!< LED14 output state control
		PCA95xx_ledsel LED15	:2;		//!< LED15 output state control
	} Bits;
} uPCA9532_REG__LS3;

/*!\union uPCA9532_REG__LS
** \brief Union for LS registers of PCA9532
**/
typedef union PACK__ uPCA9532_REG__LS {
	uint32_t DWord;
	struct {
		PCA95xx_ledsel LED0		:2;		//!< LED0 output state control
		PCA95xx_ledsel LED1		:2;		//!< LED1 output state control
		PCA95xx_ledsel LED2		:2;		//!< LED2 output state control
		PCA95xx_ledsel LED3		:2;		//!< LED3 output state control
		PCA95xx_ledsel LED4		:2;		//!< LED4 output state control
		PCA95xx_ledsel LED5		:2;		//!< LED5 output state control
		PCA95xx_ledsel LED6		:2;		//!< LED6 output state control
		PCA95xx_ledsel LED7		:2;		//!< LED7 output state control
		PCA95xx_ledsel LED8		:2;		//!< LED8 output state control
		PCA95xx_ledsel LED9		:2;		//!< LED9 output state control
		PCA95xx_ledsel LED10	:2;		//!< LED10 output state control
		PCA95xx_ledsel LED11	:2;		//!< LED11 output state control
		PCA95xx_ledsel LED12	:2;		//!< LED12 output state control
		PCA95xx_ledsel LED13	:2;		//!< LED13 output state control
		PCA95xx_ledsel LED14	:2;		//!< LED14 output state control
		PCA95xx_ledsel LED15	:2;		//!< LED15 output state control
	} Bits;
} uPCA9532_REG__LS;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for PCA9532 peripheral
** \param[in] idx - PCA9532 index
** \param[in] hi2c - pointer to PCA9532 I2C instance
** \param[in] devAddress - PCA9532 device address
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9532_Init(const uint8_t idx, const I2C_HandleTypeDef * hi2c, const uint16_t devAddress);

/*!\brief Initialization for PCA9532 peripheral
** \warning In case multiple devices (defined by I2C_PCA9532_NB > 1), you shall use PCA9532_Init instead
** \return FctERR - error code
**/
FctERR PCA9532_Init_Single(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for PCA9532
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9532_Write(I2C_slave_t * pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for PCA9532
** \warning When Auto-Increment flag is set (AI = 1) and a read sequence is initiated, the sequence must start by reading a register different from the INPUT0 register.
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ PCA9532_Read(I2C_slave_t * pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb);


/****************************************************************/
#include "PCA9532_proc.h"	// Include procedures
#include "PCA9532_ex.h"		// Include extensions

#ifdef __cplusplus
	}
#endif

#endif
#endif	/* __PCA9532_H__ */
/****************************************************************/
