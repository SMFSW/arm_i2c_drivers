/*!\file NCA9595.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief NCA9595 Driver
** \details NCA9595: Low-voltage 16-bit I²C and SMBus I/O expander
**/
/****************************************************************/
#ifndef NCA9595_H__
	#define NCA9595_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


#ifndef I2C_NCA9595_NB
//! \note Define I2C_NCA9595_NB to enable multiple peripherals of this type
#define I2C_NCA9595_NB	1	//!< Number of NCA9595 peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define NCA9595_ADDR			0x20			//!< NCA9595 address

#ifndef NCA9595_BASE_ADDR
//! \note Define NCA9595_BASE_ADDR to change default device base address
#define NCA9595_BASE_ADDR	NCA9595_ADDR		//!< NCA9595 Base address
#endif


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum NCA9595_reg_map
** \brief Register map enum of NCA9595
** \note Shouldn't be used directly (use \ref NCA9595_wreg_map instead)
**/
typedef enum PACK__ NCA9595_reg_map {
	NCA9595__InputPort0 = 0,	//!< Input port 0
	NCA9595__InputPort1,		//!< Input port 1
	NCA9595__OutputPort0,		//!< Output port 0
	NCA9595__OutputPort1,		//!< Output port 1
	NCA9595__PolarityPort0,		//!< Polarity inversion port 0
	NCA9595__PolarityPort1,		//!< Polarity inversion port 1
	NCA9595__ConfigPort0,		//!< Configuration port 0
	NCA9595__ConfigPort1,		//!< Configuration port 1
	NCA9595__PullUpConfigPort0,	//!< Configuration pull-up port 0
	NCA9595__PullUpConfigPort1,	//!< Configuration pull-up port 1
} NCA9595_reg;

/*!\enum NCA9595_wreg_map
** \brief Register map enum of NCA9595
**/
typedef enum PACK__ NCA9595_wreg_map {
	// WORDs map
#if defined(__BIG_ENDIAN__)
	NCA9595__InputPorts = NCA9595__InputPort1,				//!< Input ports
	NCA9595__OutputPorts = NCA9595__OutputPort1,			//!< Output ports
	NCA9595__PolarityPorts = NCA9595__PolarityPort1,		//!< Polarity inversion ports
	NCA9595__ConfigPorts = NCA9595__ConfigPort1,			//!< Configuration ports
	NCA9595__PullUpConfigPorts = NCA9595__PullUpConfigPort1	//!< Configuration pull-up port 0
#else
	NCA9595__InputPorts = NCA9595__InputPort0,				//!< Input ports
	NCA9595__OutputPorts = NCA9595__OutputPort0,			//!< Output ports
	NCA9595__PolarityPorts = NCA9595__PolarityPort0,		//!< Polarity inversion ports
	NCA9595__ConfigPorts = NCA9595__ConfigPort0,			//!< Configuration ports
	NCA9595__PullUpConfigPorts = NCA9595__PullUpConfigPort0	//!< Configuration pull-up port 0
#endif
} NCA9595_wreg;


/*!\enum NCA9595_Cfg
** \brief Configuration register values for NCA9595
**/
typedef enum PACK__ NCA9595_Cfg {
	NCA9595__Cfg_Output = 0,	//!< corresponding port pin is enabled as an output
	NCA9595__Cfg_Input,			//!< corresponding port pin is enabled as a high-impedance input
} NCA9595_Config;


/*!\enum NCA9595_Polarity
** \brief Polarity inversion register values for NCA9595
**/
typedef enum PACK__ NCA9595_Polarity {
	NCA9595__Pol_Direct = 0,	//!< corresponding port pin’s polarity is retained
	NCA9595__Pol_Invert,		//!< corresponding port pin’s polarity is inverted in the Input register
} NCA9595_Polarity;


/*!\enum NCA9595_PullUp
** \brief Pull-Up configuration register values for NCA9595
**/
typedef enum PACK__ NCA9595_PullUp {
	NCA9595__PUp_Disable = 0,	//!< corresponding port pull-up resistors are disabled
	NCA9595__PUp_Enable,		//!< corresponding port pull-up resistors are enabled
} NCA9595_PullUp;


/*!\union uNCA9595_REG__IN
** \brief Union for Input register pair of NCA9595
**/
typedef union uNCA9595_REG__IN {
	uint16_t Word;
	struct PACK__ {
		// Less significant bit first
		uint16_t I0_0	:1;		//!< Input bit for Port 0 Channel 0
		uint16_t I0_1	:1;		//!< Input bit for Port 0 Channel 1
		uint16_t I0_2	:1;		//!< Input bit for Port 0 Channel 2
		uint16_t I0_3	:1;		//!< Input bit for Port 0 Channel 3
		uint16_t I0_4	:1;		//!< Input bit for Port 0 Channel 4
		uint16_t I0_5	:1;		//!< Input bit for Port 0 Channel 5
		uint16_t I0_6	:1;		//!< Input bit for Port 0 Channel 6
		uint16_t I0_7	:1;		//!< Input bit for Port 0 Channel 7
		uint16_t I1_0	:1;		//!< Input bit for Port 1 Channel 0
		uint16_t I1_1	:1;		//!< Input bit for Port 1 Channel 1
		uint16_t I1_2	:1;		//!< Input bit for Port 1 Channel 2
		uint16_t I1_3	:1;		//!< Input bit for Port 1 Channel 3
		uint16_t I1_4	:1;		//!< Input bit for Port 1 Channel 4
		uint16_t I1_5	:1;		//!< Input bit for Port 1 Channel 5
		uint16_t I1_6	:1;		//!< Input bit for Port 1 Channel 6
		uint16_t I1_7	:1;		//!< Input bit for Port 1 Channel 7
	} Bits;
} uNCA9595_REG__IN;


/*!\union uNCA9595_REG__OUT
** \brief Union for Output register pair of NCA9595
**/
typedef union uNCA9595_REG__OUT {
	uint16_t Word;
	struct PACK__ {
		// Less significant bit first
		uint16_t O0_0	:1;		//!< Output bit for Port 0 Channel 0
		uint16_t O0_1	:1;		//!< Output bit for Port 0 Channel 1
		uint16_t O0_2	:1;		//!< Output bit for Port 0 Channel 2
		uint16_t O0_3	:1;		//!< Output bit for Port 0 Channel 3
		uint16_t O0_4	:1;		//!< Output bit for Port 0 Channel 4
		uint16_t O0_5	:1;		//!< Output bit for Port 0 Channel 5
		uint16_t O0_6	:1;		//!< Output bit for Port 0 Channel 6
		uint16_t O0_7	:1;		//!< Output bit for Port 0 Channel 7
		uint16_t O1_0	:1;		//!< Output bit for Port 1 Channel 0
		uint16_t O1_1	:1;		//!< Output bit for Port 1 Channel 1
		uint16_t O1_2	:1;		//!< Output bit for Port 1 Channel 2
		uint16_t O1_3	:1;		//!< Output bit for Port 1 Channel 3
		uint16_t O1_4	:1;		//!< Output bit for Port 1 Channel 4
		uint16_t O1_5	:1;		//!< Output bit for Port 1 Channel 5
		uint16_t O1_6	:1;		//!< Output bit for Port 1 Channel 6
		uint16_t O1_7	:1;		//!< Output bit for Port 1 Channel 7
	} Bits;
} uNCA9595_REG__OUT;


/*!\union uNCA9595_REG__CFG
** \brief Union for Config register pair of NCA9595
**/
typedef union uNCA9595_REG__CFG {
	uint16_t Word;
	struct PACK__ {
		// Less significant bit first
		NCA9595_Config C0_0	:1;		//!< Configuration bit for Port 0 Channel 0
		NCA9595_Config C0_1	:1;		//!< Configuration bit for Port 0 Channel 1
		NCA9595_Config C0_2	:1;		//!< Configuration bit for Port 0 Channel 2
		NCA9595_Config C0_3	:1;		//!< Configuration bit for Port 0 Channel 3
		NCA9595_Config C0_4	:1;		//!< Configuration bit for Port 0 Channel 4
		NCA9595_Config C0_5	:1;		//!< Configuration bit for Port 0 Channel 5
		NCA9595_Config C0_6	:1;		//!< Configuration bit for Port 0 Channel 6
		NCA9595_Config C0_7	:1;		//!< Configuration bit for Port 0 Channel 7
		NCA9595_Config C1_0	:1;		//!< Configuration bit for Port 1 Channel 0
		NCA9595_Config C1_1	:1;		//!< Configuration bit for Port 1 Channel 1
		NCA9595_Config C1_2	:1;		//!< Configuration bit for Port 1 Channel 2
		NCA9595_Config C1_3	:1;		//!< Configuration bit for Port 1 Channel 3
		NCA9595_Config C1_4	:1;		//!< Configuration bit for Port 1 Channel 4
		NCA9595_Config C1_5	:1;		//!< Configuration bit for Port 1 Channel 5
		NCA9595_Config C1_6	:1;		//!< Configuration bit for Port 1 Channel 6
		NCA9595_Config C1_7	:1;		//!< Configuration bit for Port 1 Channel 7
	} Bits;
} uNCA9595_REG__CFG;


/*!\union uNCA9595_REG__POL
** \brief Union for Polarity inversion register pair of NCA9595
**/
typedef union uNCA9595_REG__POL {
	uint16_t Word;
	struct PACK__ {
		// Less significant bit first
		NCA9595_Polarity N0_0	:1;		//!< Polarity inversion bit for Port 0 Channel 0
		NCA9595_Polarity N0_1	:1;		//!< Polarity inversion bit for Port 0 Channel 1
		NCA9595_Polarity N0_2	:1;		//!< Polarity inversion bit for Port 0 Channel 2
		NCA9595_Polarity N0_3	:1;		//!< Polarity inversion bit for Port 0 Channel 3
		NCA9595_Polarity N0_4	:1;		//!< Polarity inversion bit for Port 0 Channel 4
		NCA9595_Polarity N0_5	:1;		//!< Polarity inversion bit for Port 0 Channel 5
		NCA9595_Polarity N0_6	:1;		//!< Polarity inversion bit for Port 0 Channel 6
		NCA9595_Polarity N0_7	:1;		//!< Polarity inversion bit for Port 0 Channel 7
		NCA9595_Polarity N1_0	:1;		//!< Polarity inversion bit for Port 1 Channel 0
		NCA9595_Polarity N1_1	:1;		//!< Polarity inversion bit for Port 1 Channel 1
		NCA9595_Polarity N1_2	:1;		//!< Polarity inversion bit for Port 1 Channel 2
		NCA9595_Polarity N1_3	:1;		//!< Polarity inversion bit for Port 1 Channel 3
		NCA9595_Polarity N1_4	:1;		//!< Polarity inversion bit for Port 1 Channel 4
		NCA9595_Polarity N1_5	:1;		//!< Polarity inversion bit for Port 1 Channel 5
		NCA9595_Polarity N1_6	:1;		//!< Polarity inversion bit for Port 1 Channel 6
		NCA9595_Polarity N1_7	:1;		//!< Polarity inversion bit for Port 1 Channel 7
	} Bits;
} uNCA9595_REG__POL;


/*!\union uNCA9595_REG__PUP
** \brief Union for PullUp register pair of NCA9595
**/
typedef union uNCA9595_REG__PUP {
	uint16_t Word;
	struct PACK__ {
		// Less significant bit first
		NCA9595_PullUp R0_0	:1;		//!< PullUp bit for Port 0 Channel 0
		NCA9595_PullUp R0_1	:1;		//!< PullUp bit for Port 0 Channel 1
		NCA9595_PullUp R0_2	:1;		//!< PullUp bit for Port 0 Channel 2
		NCA9595_PullUp R0_3	:1;		//!< PullUp bit for Port 0 Channel 3
		NCA9595_PullUp R0_4	:1;		//!< PullUp bit for Port 0 Channel 4
		NCA9595_PullUp R0_5	:1;		//!< PullUp bit for Port 0 Channel 5
		NCA9595_PullUp R0_6	:1;		//!< PullUp bit for Port 0 Channel 6
		NCA9595_PullUp R0_7	:1;		//!< PullUp bit for Port 0 Channel 7
		NCA9595_PullUp R1_0	:1;		//!< PullUp bit for Port 1 Channel 0
		NCA9595_PullUp R1_1	:1;		//!< PullUp bit for Port 1 Channel 1
		NCA9595_PullUp R1_2	:1;		//!< PullUp bit for Port 1 Channel 2
		NCA9595_PullUp R1_3	:1;		//!< PullUp bit for Port 1 Channel 3
		NCA9595_PullUp R1_4	:1;		//!< PullUp bit for Port 1 Channel 4
		NCA9595_PullUp R1_5	:1;		//!< PullUp bit for Port 1 Channel 5
		NCA9595_PullUp R1_6	:1;		//!< PullUp bit for Port 1 Channel 6
		NCA9595_PullUp R1_7	:1;		//!< PullUp bit for Port 1 Channel 7
	} Bits;
} uNCA9595_REG__PUP;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for NCA9595 peripheral
** \param[in] idx - NCA9595 index
** \param[in] hi2c - pointer to NCA9595 I2C instance
** \param[in] devAddress - NCA9595 device address
** \return FctERR - error code
**/
FctERR NONNULL__ NCA9595_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress);

/*!\brief Initialization for NCA9595 peripheral
** \warning In case multiple devices (defined by I2C_NCA9595_NB > 1), you shall use NCA9595_Init instead
** \return FctERR - error code
**/
FctERR NCA9595_Init_Single(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for NCA9595
** \note Writes 8bits register pair (address shall be even for little endian / odd for big endian)
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \return FctERR - error code
**/
FctERR NONNULL__ NCA9595_Write_Word(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr);


/*!\brief I2C Read function for NCA9595
** \note Reads 8bits register pair (address shall be even for little endian / odd for big endian)
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \return FctERR - error code
**/
FctERR NONNULL__ NCA9595_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr);


/****************************************************************/
// cppcheck-suppress-begin misra-c2012-20.1 ; include directives after other declarations
#include "NCA9595_proc.h"	// Include procedures
#include "NCA9595_ex.h"		// Include extensions
// cppcheck-suppress-end misra-c2012-20.1

#endif

#ifdef __cplusplus
	}
#endif

#endif /* NCA9595_H__ */
/****************************************************************/
