/*!\file ADS1115.h
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief ADS1115 Driver
** \details ADS1115: Ultra-Small, Low-Power, 16-Bit Analog-to-Digital Converter with Internal Reference
** \note	Compatibility with:
**				- ADS1113 (restrictions given in datasheet)
**				- ADS1114 (restrictions given in datasheet)
**				- ADS1115
** \note	Compatibility should be assured with:
**				- ADS1x1x (check datasheets for compatibility informations)
**/
/****************************************************************/
#ifndef ADS1115_H__
	#define ADS1115_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


#ifndef I2C_ADS1115_NB
//! \note Define I2C_ADS1115_NB to enable multiple peripherals of this type
#define I2C_ADS1115_NB	1U	//!< Number of ADS1115 peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define ADS1115_ADDR_GND	0x48U				//!< ADS1115 address (ADR tied to GND)
#define ADS1115_ADDR_VDD	0x49U				//!< ADS1115 address (ADR tied to VDD)
#define ADS1115_ADDR_SDA	0x4AU				//!< ADS1115 address (ADR tied to SDA)
#define ADS1115_ADDR_SCL	0x4BU				//!< ADS1115 address (ADR tied to SCL)

#ifndef ADS1115_BASE_ADDR
//! \note Define ADS1115_BASE_ADDR to change default device base address
#define ADS1115_BASE_ADDR	ADS1115_ADDR_GND	//!< ADS1115 Base address
#endif


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum ADS1115_reg
** \brief Register map enum of ADS1115
**/
typedef enum PACK__ {
	ADS1115__CONVERSION = 0U,	//!< Conversion register
	ADS1115__CONFIG,			//!< Config register
	ADS1115__LOW_THRESH,		//!< Lo_thresh register
	ADS1115__HI_THRESH,			//!< Hi_thresh register
} ADS1115_reg;


/*!\enum ADS1115_mux
** \brief MUX enum for ADS1115
**/
typedef enum PACK__ {
	ADS1115__MUX_pAIN0_nAIN1 = 0U,	//!< AINP = AIN0 and AINN = AIN1
	ADS1115__MUX_pAIN0_nAIN3,		//!< AINP = AIN0 and AINN = AIN3
	ADS1115__MUX_pAIN1_nAIN3,		//!< AINP = AIN1 and AINN = AIN3
	ADS1115__MUX_pAIN2_nAIN3,		//!< AINP = AIN2 and AINN = AIN3
	ADS1115__MUX_pAIN0_nGND,		//!< AINP = AIN0 and AINN = GND
	ADS1115__MUX_pAIN1_nGND,		//!< AINP = AIN1 and AINN = GND
	ADS1115__MUX_pAIN2_nGND,		//!< AINP = AIN2 and AINN = GND
	ADS1115__MUX_pAIN3_nGND,		//!< AINP = AIN3 and AINN = GND
} ADS1115_mux;


/*!\enum ADS1115_gain
** \brief GAIN enum for ADS1115
**/
typedef enum PACK__ {
	ADS1115__FS_6144mV = 0U,	//!< FS = ±6.144V (step = 187.5µV)
	ADS1115__FS_4096mV,			//!< FS = ±4.096V (step = 125µV)
	ADS1115__FS_2048mV,			//!< FS = ±2.048V (step = 62.5µV)
	ADS1115__FS_1024mV,			//!< FS = ±1.024V (step = 31.25µV)
	ADS1115__FS_512mV,			//!< FS = ±0.512V (step = 15.625µV)
	ADS1115__FS_256mV,			//!< FS = ±0.256V (step = 7.8125µV)
	ADS1115__FS_256mV_2,		//!< FS = ±0.256V (step = 7.8125µV)
	ADS1115__FS_256mV_3,		//!< FS = ±0.256V (step = 7.8125µV)
} ADS1115_gain;


/*!\enum ADS1115_mode
** \brief MODE enum for ADS1115
**/
typedef enum PACK__ {
	ADS1115__MODE_CONTINUOUS = 0U,	//!< Continuous conversion mode
	ADS1115__MODE_SINGLE_SHOT,		//!< Power-down single-shot mode
} ADS1115_mode;


/*!\enum ADS1115_rate
** \brief Data rate enum for ADS1115
**/
typedef enum PACK__ {
	ADS1115__SPS_8 = 0U,	//!< 8 SPS (Samples Per Second)
	ADS1115__SPS_16,		//!< 16 SPS
	ADS1115__SPS_32,		//!< 32 SPS
	ADS1115__SPS_64,		//!< 64 SPS
	ADS1115__SPS_128,		//!< 128 SPS
	ADS1115__SPS_250,		//!< 250 SPS
	ADS1115__SPS_475,		//!< 475 SPS
	ADS1115__SPS_860,		//!< 860 SPS
} ADS1115_rate;


/*!\enum ADS1115_comp
** \brief Comparator Mode enum for ADS1115
**/
typedef enum PACK__ {
	ADS1115__COMP_HYST = 0U,	//!< Traditional comparator with hysteresis
	ADS1115__COMP_WINDOW,		//!< Window comparator
} ADS1115_comp;


/*!\enum ADS1115_polarity
** \brief Comparator Polarity enum for ADS1115
**/
typedef enum PACK__ {
	ADS1115__POL_LOW = 0U,	//!< Active Low
	ADS1115__POL_HIGH,		//!< Active High
} ADS1115_polarity;


/*!\enum ADS1115_latch
** \brief Comparator Latching enum for ADS1115
**/
typedef enum PACK__ {
	ADS1115__LATCH_DISABLE = 0U,	//!< Non-latching comparator
	ADS1115__LATCH_ENABLE,			//!< Latching comparator
} ADS1115_latch;


/*!\enum ADS1115_queue
** \brief Comparator Queue and Disable enum for ADS1115
**/
typedef enum PACK__ {
	ADS1115__QUEUE_1CONV = 0U,	//!< Assert after one conversion
	ADS1115__QUEUE_2CONV,		//!< Assert after two conversions
	ADS1115__QUEUE_4CONV,		//!< Assert after four conversions
	ADS1115__QUEUE_DISABLE,		//!< Disable comparator
} ADS1115_queue;


/*!\union uADS1115_REG__CFG
** \brief Union for Config register of ADS1115
**/
typedef union {
	uint16_t	Word;
	uint8_t		Bytes[2];
	struct PACK__ {
		ADS1115_queue		COMP_QUE	:2;	//!< Comparator queue and disable (ADS1114 and ADS1115 only)
		ADS1115_latch		COMP_LAT	:1;	//!< Latching comparator (ADS1114 and ADS1115 only)
		ADS1115_polarity	COMP_POL	:1;	//!< Comparator polarity (ADS1114 and ADS1115 only)
		ADS1115_comp		COMP_MODE	:1;	//!< Comparator mode (ADS1114 and ADS1115 only)
		ADS1115_rate		DR			:3;	//!< Data rate
		ADS1115_mode		MODE		:1;	//!< Device operating mode
		ADS1115_gain		PGA			:3;	//!< Programmable gain amplifier configuration (ADS1114 and ADS1115 only)
		ADS1115_mux			MUX			:3;	//!< Input multiplexer configuration (ADS1115 only)
		uint16_t			OS			:1;	//!< Operational status/single-shot conversion start
	} Bits;
} uADS1115_REG__CFG;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for ADS1115 peripheral
** \param[in] idx - ADS1115 index
** \param[in] hi2c - pointer to ADS1115 I2C instance
** \param[in] devAddress - ADS1115 device address
** \return FctERR - error code
**/
FctERR NONNULL__ ADS1115_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress);

/*!\brief Initialization for ADS1115 peripheral
** \warning In case multiple devices (defined by I2C_ADS1115_NB > 1), you shall use ADS1115_Init instead
** \return FctERR - error code
**/
FctERR ADS1115_Init_Single(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for ADS1115
** \param[in] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \return FctERR - error code
**/
FctERR NONNULL__ ADS1115_Write(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr);


/*!\brief I2C Read function for ADS1115
** \param[in] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \return FctERR - error code
**/
FctERR NONNULL__ ADS1115_Read(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr);


/****************************************************************/
// cppcheck-suppress-begin misra-c2012-20.1 ; include directives after other declarations
#include "ADS1115_proc.h"	// Include procedures
#include "ADS1115_ex.h"		// Include extensions
// cppcheck-suppress-end misra-c2012-20.1

#endif

#ifdef __cplusplus
	}
#endif

#endif /* ADS1115_H__ */
/****************************************************************/
