/*!\file BMP180.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief BMP180 Driver
** \details BMP180: Digital pressure sensor
**/
/****************************************************************/
#ifndef BMP180_H__
	#define BMP180_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/

#ifndef I2C_BMP180_NB
//! \note Define I2C_BMP180_NB to enable multiple peripherals of this type
#define I2C_BMP180_NB	1U	//!< Number of BMP180 peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
#ifndef BMP180_BASE_ADDR
//! \note Define BMP180_BASE_ADDR to change default device base address
#define BMP180_BASE_ADDR	0x77U		//!< BMP180 Base address
#endif


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum BMP180_reg_map
** \brief Register map enum of BMP180
**/
typedef enum PACK__ BMP180_reg_map {
	BMP180__CALIB_AC1_MSB = 0xAAU,	//!< AC1 MSB
	BMP180__CALIB_AC1_LSB,			//!< AC1 LSB
	BMP180__CALIB_AC2_MSB,			//!< AC2 MSB
	BMP180__CALIB_AC2_LSB,			//!< AC2 LSB
	BMP180__CALIB_AC3_MSB,			//!< AC3 MSB
	BMP180__CALIB_AC3_LSB,			//!< AC3 LSB
	BMP180__CALIB_AC4_MSB,			//!< AC4 MSB
	BMP180__CALIB_AC4_LSB,			//!< AC4 LSB
	BMP180__CALIB_AC5_MSB,			//!< AC5 MSB
	BMP180__CALIB_AC5_LSB,			//!< AC5 LSB
	BMP180__CALIB_AC6_MSB,			//!< AC6 MSB
	BMP180__CALIB_AC6_LSB,			//!< AC6 LSB
	BMP180__CALIB_B1_MSB,			//!< B1 MSB
	BMP180__CALIB_B1_LSB,			//!< B1 LSB
	BMP180__CALIB_B2_MSB,			//!< B2 MSB
	BMP180__CALIB_B2_LSB,			//!< B2 LSB
	BMP180__CALIB_MB_MSB,			//!< MB MSB
	BMP180__CALIB_MB_LSB,			//!< MB LSB
	BMP180__CALIB_MC_MSB,			//!< MC MSB
	BMP180__CALIB_MC_LSB,			//!< MC LSB
	BMP180__CALIB_MD_MSB,			//!< MD MSB
	BMP180__CALIB_MD_LSB,			//!< MD LSB
	BMP180__ID = 0xD0U,				//!< Identifier
	BMP180__VERSION,				//!< Version
	BMP180__SOFT_RESET = 0xE0U,		//!< Software reset
	BMP180__CTRL_MEAS = 0xF4U,		//!< Measurements Control
	BMP180__OUT_MSB = 0xF6U,		//!< Output MSB
	BMP180__OUT_LSB,				//!< Output LSB
	BMP180__OUT_XLSB,				//!< Output XLSB
} BMP180_reg;


/*!\enum BMP180_oversampling
** \brief Oversampling enum of BMP180
**/
typedef enum PACK__ BMP180_oversampling {
	BMP180__OSS_1_TIME = 0U,	//!< Oversampling 1 time
	BMP180__OSS_2_TIME,			//!< Oversampling 1 time
	BMP180__OSS_4_TIME,			//!< Oversampling 1 time
	BMP180__OSS_8_TIME,			//!< Oversampling 1 time
} BMP180_oversampling;


/*!\enum BMP180_measure
** \brief BMP180 measure types
**/
typedef enum PACK__ BMP180_measure {
	BMP180__MEAS_TEMPERATURE = 0x0EU,	//!< Temperature
	BMP180__MEAS_PRESSURE = 0x14U		//!< Pressure
} BMP180_meas;


/*!\union uBMP180_REG__OUT_MSB
** \brief Union for out_msb of BMP180
**/
typedef union uBMP180_REG__OUT_MSB {
	uint8_t Byte;
	struct PACK__ {
		uint8_t ADC_OUT_MSB		:8;		//!< ADC out msb
	} Bits;
} uBMP180_REG__OUT_MSB;

/*!\union uBMP180_REG__OUT_LSB
** \brief Union for out_lsb of BMP180
**/
typedef union uBMP180_REG__OUT_LSB {
	uint8_t Byte;
	struct PACK__ {
		uint8_t ADC_OUT_LSB		:8;		//!< ADC out lsb
	} Bits;
} uBMP180_REG__OUT_LSB;

/*!\union uBMP180_REG__OUT_XLSB
** \brief Union for out_xlsb of BMP180
**/
typedef union uBMP180_REG__OUT_XLSB {
	uint8_t Byte;
	struct PACK__ {
		uint8_t 				:3;
		uint8_t ADC_OUT_XLSB	:5;		//!< ADC out xlsb
	} Bits;
} uBMP180_REG__OUT_XLSB;

/*!\union uBMP180_REG__MEAS_CTRL
** \brief Union for meas_ctrl of BMP180
**/
typedef union uBMP180_REG__MEAS_CTRL {
	uint8_t Byte;
	struct PACK__ {
		BMP180_meas			MEAS_CTRL	:5;		//!< Command temperature or pressure
		uint8_t				SCO			:1;		//!< Start of conversion. The value of this bit stays 1 during conversion and is reset to 0 after conversion is complete (data registers are filled)
		BMP180_oversampling	OSS			:2;		//!< Controls the oversampling ratio for pressure measurement (00b: single, 01b: 2 times, 10b: 4 times, 11b: 8 times)
	} Bits;
} uBMP180_REG__MEAS_CTRL;

// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for BMP180 peripheral
** \param[in] idx - BMP180 index
** \param[in] hi2c - pointer to BMP180 I2C instance
** \param[in] devAddress - BMP180 device address
** \return FctERR - error code
**/
FctERR NONNULL__ BMP180_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress);

/*!\brief Initialization for BMP180 peripheral
** \warning In case multiple devices (defined by I2C_BMP180_NB > 1), you shall use BMP180_Init instead
** \return FctERR - error code
**/
FctERR BMP180_Init_Single(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for BMP180
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR NONNULL__ BMP180_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for BMP180
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ BMP180_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Word Read function for BMP180
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \return FctERR - error code
**/
FctERR NONNULL__ BMP180_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr);


/****************************************************************/
// cppcheck-suppress-begin misra-c2012-20.1 ; include directives after other declarations
#include "BMP180_proc.h"	// Include procedures
#include "BMP180_ex.h"		// Include extensions
// cppcheck-suppress-end misra-c2012-20.1

#endif

#ifdef __cplusplus
	}
#endif

#endif /* BMP180_H__ */
/****************************************************************/
