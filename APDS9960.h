/*!\file APDS9960.h
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief APDS9960 Driver
** \details APDS9960: Digital Proximity, Ambient Light, RGB and Gesture Sensor
**/
/****************************************************************/
#ifndef APDS9960_H__
	#define APDS9960_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#include "I2C_component.h"
#include "I2C_peripheral.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


#ifndef I2C_APDS9960_NB
//! \note Define I2C_APDS9960_NB to enable multiple peripherals of this type
#define I2C_APDS9960_NB	1U	//!< Number of APDS9960 peripherals
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************
#define APDS9960_ADDR		0x39U				//!< APDS9960 address

#ifndef APDS9960_BASE_ADDR
//! \note Define APDS9960_BASE_ADDR to change default device base address
#define APDS9960_BASE_ADDR	APDS9960_ADDR		//!< APDS9960 Base address
#endif


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum APDS9960_reg
** \brief Register map enum of APDS9960
**/
typedef enum PACK__ {
	APDS9960__ENABLE = 0x80U,		//!< Enables states and interrupts
	APDS9960__ATIME,				//!< ADC integration time
	APDS9960__WTIME = 0x83U,		//!< Wait time (non-gesture)
	APDS9960__AILTL,				//!< ALS interrupt low threshold low byte
	APDS9960__AILTH,				//!< ALS interrupt low threshold high byte
	APDS9960__AIHTL,				//!< ALS interrupt high threshold low byte
	APDS9960__AIHTH,				//!< ALS interrupt high threshold high byte
	APDS9960__PILT = 0x89U,			//!< Proximity interrupt low threshold
	APDS9960__PIHT = 0x8BU,			//!< Proximity interrupt high threshold
	APDS9960__PERS,					//!< Interrupt persistence filters (non-gesture)
	APDS9960__CONFIG1,				//!< Configuration register one
	APDS9960__PPULSE,				//!< Proximity pulse count and length
	APDS9960__CONTROL,				//!< Gain control
	APDS9960__CONFIG2,				//!< Configuration register two
	APDS9960__ID = 0x92U,			//!< Device ID
	APDS9960__STATUS,				//!< Device status
	APDS9960__CDATAL,				//!< Low byte of clear channel data
	APDS9960__CDATAH,				//!< High byte of clear channel data
	APDS9960__RDATAL,				//!< Low byte of red channel data
	APDS9960__RDATAH,				//!< High byte of red channel data
	APDS9960__GDATAL,				//!< Low byte of green channel data
	APDS9960__GDATAH,				//!< High byte of green channel data
	APDS9960__BDATAL,				//!< Low byte of blue channel data
	APDS9960__BDATAH,				//!< High byte of blue channel data
	APDS9960__PDATA,				//!< Proximity data
	APDS9960__POFFSET_UR,			//!< Proximity offset for UP and RIGHT photodiodes
	APDS9960__POFFSET_DL,			//!< Proximity offset for DOWN and LEFT photodiodes
	APDS9960__CONFIG3,				//!< Configuration register three
	APDS9960__GPENTH,				//!< Gesture proximity enter threshold
	APDS9960__GEXTH,				//!< Gesture exit threshold
	APDS9960__GCONF1,				//!< Gesture configuration one
	APDS9960__GCONF2,				//!< Gesture configuration two
	APDS9960__GOFFSET_U,			//!< Gesture UP offset register
	APDS9960__GOFFSET_D,			//!< Gesture DOWN offset register
	APDS9960__GPULSE,				//!< Gesture pulse count and length
	APDS9960__GOFFSET_L,			//!< Gesture LEFT offset register
	APDS9960__GOFFSET_R = 0xA9U,	//!< Gesture RIGHT offset register
	APDS9960__GCONF3,				//!< Gesture configuration three
	APDS9960__GCONF4,				//!< Gesture configuration four
	APDS9960__GFLVL = 0xAEU,		//!< Gesture FIFO level
	APDS9960__GSTATUS,				//!< Gesture status
	APDS9960__IFORCE = 0xE4U,		//!< Force interrupt
	APDS9960__PICLEAR,				//!< Proximity interrupt clear
	APDS9960__CICLEAR,				//!< ALS clear channel interrupt clear
	APDS9960__AICLEAR,				//!< All non-gesture interrupts clear
	APDS9960__GFIFO_U = 0xFCU,		//!< Gesture FIFO UP value
	APDS9960__GFIFO_D,				//!< Gesture FIFO DOWN value
	APDS9960__GFIFO_L,				//!< Gesture FIFO LEFT value
	APDS9960__GFIFO_R,				//!< Gesture FIFO RIGHT value
} APDS9960_reg;

/*!\enum APDS9960_spec_func
** \brief Special functions of APDS9960
**/
typedef enum PACK__ {
	APDS9960__SF_IFORCE = 0U,	//!< Forces an interrupt
	APDS9960__SF_PICLEAR,		//!< Proximity interrupt clear
	APDS9960__SF_CICLEAR,		//!< ALS interrupt clear
	APDS9960__SF_AICLEAR		//!< Clears all non-gesture interrupts
} APDS9960_spec_func;


/*!\enum APDS9960_als_it_persist
** \brief Persistence control of APDS9960 ALS
**/
typedef enum PACK__ {
	APDS9960__PERSIST_EVERY_ALS_CYCLE = 0U,	//!< Every ALS cycle generates interrupt
	APDS9960__PERSIST_1_ALS_PERIODS,		//!< 1 consecutive Ch0 channel values out of range
	APDS9960__PERSIST_2_ALS_PERIODS,		//!< 2 consecutive Ch0 channel values out of range
	APDS9960__PERSIST_3_ALS_PERIODS,		//!< 3 consecutive Ch0 channel values out of range
	APDS9960__PERSIST_5_ALS_PERIODS,		//!< 5 consecutive Ch0 channel values out of range
	APDS9960__PERSIST_10_ALS_PERIODS,		//!< 10 consecutive Ch0 channel values out of range
	APDS9960__PERSIST_15_ALS_PERIODS,		//!< 15 consecutive Ch0 channel values out of range
	APDS9960__PERSIST_20_ALS_PERIODS,		//!< 20 consecutive Ch0 channel values out of range
	APDS9960__PERSIST_25_ALS_PERIODS,		//!< 25 consecutive Ch0 channel values out of range
	APDS9960__PERSIST_30_ALS_PERIODS,		//!< 30 consecutive Ch0 channel values out of range
	APDS9960__PERSIST_35_ALS_PERIODS,		//!< 35 consecutive Ch0 channel values out of range
	APDS9960__PERSIST_40_ALS_PERIODS,		//!< 40 consecutive Ch0 channel values out of range
	APDS9960__PERSIST_45_ALS_PERIODS,		//!< 45 consecutive Ch0 channel values out of range
	APDS9960__PERSIST_50_ALS_PERIODS,		//!< 50 consecutive Ch0 channel values out of range
	APDS9960__PERSIST_55_ALS_PERIODS,		//!< 55 consecutive Ch0 channel values out of range
	APDS9960__PERSIST_60_ALS_PERIODS,		//!< 60 consecutive Ch0 channel values out of range
} APDS9960_als_it_persist;

/*!\enum APDS9960_prox_it_persist
** \brief Persistence control of APDS9960 Proximity
**/
typedef enum PACK__ {
	APDS9960__PERSIST_EVERY_PROX_CYCLE = 0U,	//!< Every proximity cycle generates an interrupt
	APDS9960__PERSIST_1_PROX_PERIODS,			//!< 1 consecutive proximity values out of range
	APDS9960__PERSIST_2_PROX_PERIODS,			//!< 2 consecutive proximity values out of range
	APDS9960__PERSIST_3_PROX_PERIODS,			//!< 3 consecutive proximity values out of range
	APDS9960__PERSIST_4_PROX_PERIODS,			//!< 4 consecutive proximity values out of range
	APDS9960__PERSIST_5_PROX_PERIODS,			//!< 5 consecutive proximity values out of range
	APDS9960__PERSIST_6_PROX_PERIODS,			//!< 6 consecutive proximity values out of range
	APDS9960__PERSIST_7_PROX_PERIODS,			//!< 7 consecutive proximity values out of range
	APDS9960__PERSIST_8_PROX_PERIODS,			//!< 8 consecutive proximity values out of range
	APDS9960__PERSIST_9_PROX_PERIODS,			//!< 9 consecutive proximity values out of range
	APDS9960__PERSIST_10_PROX_PERIODS,			//!< 10 consecutive proximity values out of range
	APDS9960__PERSIST_11_PROX_PERIODS,			//!< 11 consecutive proximity values out of range
	APDS9960__PERSIST_12_PROX_PERIODS,			//!< 12 consecutive proximity values out of range
	APDS9960__PERSIST_13_PROX_PERIODS,			//!< 13 consecutive proximity values out of range
	APDS9960__PERSIST_14_PROX_PERIODS,			//!< 14 consecutive proximity values out of range
	APDS9960__PERSIST_15_PROX_PERIODS,			//!< 15 consecutive proximity values out of range
} APDS9960_prox_it_persist;

/*!\enum APDS9960_pulse_length
** \brief Proximity LED Pulse Length values
**/
typedef enum PACK__ {
	APDS9960__PL_4US = 0U,	//!< Pulse Length 4�s
	APDS9960__PL_8US,		//!< Pulse Length 8�s
	APDS9960__PL_16US,		//!< Pulse Length 16�s
	APDS9960__PL_32US,		//!< Pulse Length 32�s
} APDS9960_pulse_length;


/*!\enum APDS9960_als_gain
** \brief Gain values of APDS9960 ALS & Color
**/
typedef enum PACK__ {
	APDS9960__ALS_1X_GAIN = 0U,	//!< ALS 1X Gain
	APDS9960__ALS_4X_GAIN,		//!< ALS 4X Gain
	APDS9960__ALS_16X_GAIN,		//!< ALS 16X Gain
	APDS9960__ALS_64X_GAIN		//!< ALS 64X Gain
} APDS9960_als_gain;

/*!\enum APDS9960_prox_gain
** \brief Gain values of APDS9960 proximity
**/
typedef enum PACK__ {
	APDS9960__PROX_1X_GAIN = 0U,	//!< Proximity 1X Gain
	APDS9960__PROX_2X_GAIN,			//!< Proximity 2X Gain
	APDS9960__PROX_4X_GAIN,			//!< Proximity 4X Gain
	APDS9960__PROX_8X_GAIN			//!< Proximity 8X Gain
} APDS9960_prox_gain;


/*!\enum APDS9960_drive_strength
** \brief Proximity LED Drive strength values
**/
typedef enum PACK__ {
	APDS9960__DS_100MA = 0U,	//!< Proximity LED Drive Strength 100mA
	APDS9960__DS_50MA,			//!< Proximity LED Drive Strength 50mA
	APDS9960__DS_25MA,			//!< Proximity LED Drive Strength 25mA
	APDS9960__DS_12_5MA,		//!< Proximity LED Drive Strength 12.5mA
} APDS9960_drive_strength;

/*!\enum APDS9960_led_boost
** \brief Proximity LED boost values
**/
typedef enum PACK__ {
	APDS9960__BOOST_1X = 0U,	//!< LED Boost 100%. A LED_BOOST value of 0 results in 100% of the current as set by LDRIVE (no additional current)
	APDS9960__BOOST_1_5X,		//!< LED Boost 150%
	APDS9960__BOOST_2X,			//!< LED Boost 200%
	APDS9960__BOOST_3X,			//!< LED Boost 300%
} APDS9960_led_boost;

/*!\enum APDS9960_gesture_dim
** \brief Gesture dimension values
**/
typedef enum PACK__ {
	APDS9960__GD_UDLR = 0U,	//!< Both pairs are active. UP-DOWN and LEFT-RIGHT FIFO data is valid.
	APDS9960__GD_UD,		//!< Only the UP-DOWN pair is active. Ignore LEFT-RIGHT data in FIFO.
	APDS9960__GD_LR,		//!< Only the LEFT-RIGHT pair is active. Ignore UP-DOWN data in FIFO.
	APDS9960__GD_UDLR2,		//!< Both pairs are active. UP-DOWN and LEFT-RIGHT FIFO data is valid.
} APDS9960_gesture_dim;


/*!\union uAPDS9960_REG__ENABLE
** \brief Union for ENABLE register of APDS9960
**/
typedef union {
	uint8_t Byte;
	struct PACK__ {
		uint8_t PON		:1;	//!< Power ON. This field activates the internal oscillator to permit the timers and ADC channels to operate
		uint8_t AEN		:1;	//!< ALS Enable. This field activates ALS function. Writing a one activates the ALS. Writing a zero disables the ALS
		uint8_t PEN		:1;	//!< Proximity Detect Enable. This field activates the proximity detection. Writing a 1 activates the proximity. Writing a 0 disables the proximity.
		uint8_t WEN		:1;	//!< Wait Enable. This bit activates the wait feature. Writing a 1 activates the wait timer. Writing a 0 disables the wait timer.
		uint8_t AIEN	:1;	//!< ALS Interrupt Enable. When asserted, permits ALS interrupt to be generated, subject to the persistence filter settings.
		uint8_t PIEN	:1;	//!< Proximity Interrupt Enable. When asserted, permits proximity interrupts to be generated, subject to the persistence filter settings.
		uint8_t GEN		:1;	//!< Gesture Enable. When asserted, the gesture state machine can be activated. Activation is subject to the states of PEN and GMODE bits.
		uint8_t			:1;	//!< Reserved
	} Bits;
} uAPDS9960_REG__ENABLE;


/*!\union uAPDS9960_REG__PERSIST
** \brief Union for PERSIST register of APDS9960
**/
typedef union {
	uint8_t Byte;
	struct PACK__ {
		APDS9960_als_it_persist		APERS	:4;	//!< Interrupt persistence. Controls rate of interrupt to the host processor.
		APDS9960_prox_it_persist	PPERS	:4;	//!< Proximity interrupt persistence. Controls rate of proximity interrupt to the host processor.
	} Bits;
} uAPDS9960_REG__PERSIST;


/*!\union uAPDS9960_REG__CONFIG1
** \brief Union for CONFIG1 register of APDS9960
**/
typedef union {
	uint8_t Byte;
	struct PACK__ {
		uint8_t			:1;
		uint8_t	WLONG	:1;	//!< Wait Long. When asserted, the wait cycles are increased by a factor 12x from that programmed in the WTIME register.
		uint8_t			:3;	//!< ALS gain level. When asserted, the 1� and 8� ALS gain (AGAIN) modes are scaled by 0.16. Otherwise, AGAIN is scaled by 1. Do not use with AGAIN greater than 8�.
		uint8_t	_WRITE1	:2;	//!< Reserved. Write as 1
		uint8_t			:1;
	} Bits;
} uAPDS9960_REG__CONFIG1;

/*!\union uAPDS9960_REG__CONFIG2
** \brief Union for CONFIG2 register of APDS9960
**/
typedef union {
	uint8_t Byte;
	struct PACK__ {
		uint8_t				_WRITE1		:1;	//!< Reserved. Write as 1
		uint8_t							:3;
		APDS9960_led_boost	LED_BOOST	:2;	//!< Additional LDR current during proximity and gesture LED pulses. Current value, set by LDRIVE, is increased by the percentage of LED_BOOST.
		uint8_t				CPSIEN		:1;	//!< Clear Photodiode Saturation Interrupt Enable. 0 = ALS Saturation Interrupt disabled, 1 = ALS Saturation Interrupt enabled
		uint8_t				PSIEN		:1;	//!< Proximity Saturation Interrupt Enable. 0 = Proximity saturation interrupt disabled, 1 = Proximity saturation interrupt enabled
	} Bits;
} uAPDS9960_REG__CONFIG2;

/*!\union uAPDS9960_REG__CONFIG3
** \brief Union for CONFIG3 register of APDS9960
**/
typedef union {
	uint8_t Byte;
	struct PACK__ {
		uint8_t	PMASK_R		:1;	//!< Proximity Mask RIGHT Enable. Writing a 1 disables this photodiode.
		uint8_t	PMASK_L		:1;	//!< Proximity Mask LEFT Enable. Writing a 1 disables this photodiode.
		uint8_t	PMASK_D		:1;	//!< Proximity Mask DOWN Enable. Writing a 1 disables this photodiode.
		uint8_t	PMASK_U		:1;	//!< Proximity Mask UP Enable. Writing a 1 disables this photodiode.
		uint8_t	SAI			:1;	//!< Sleep After Interrupt. When enabled, the device will automatically enter low power mode when the INT pin is asserted and the state machine has progressed to the SAI decision block. Normal operation is resumed when INT pin is cleared over I2C.
		uint8_t	PCMP		:1;	//!< Proximity Gain Compensation Enable. This bit provides gain compensation when proximity photodiode signal is reduced as a result of sensor masking.
		uint8_t				:2;
	} Bits;
} uAPDS9960_REG__CONFIG3;

/*!\union uAPDS9960_REG__GCONFIG1
** \brief Union for GCONFIG1 register of APDS9960
**/
typedef union {
	uint8_t Byte;
	struct PACK__ {
		uint8_t	GEXPERS		:2;	//!< Gesture Exit Persistence. When a number of consecutive �gesture end� occurrences become equal or greater to the GEPERS value, the Gesture state machine is exited.
		uint8_t	GEXMSK_R	:1;	//!< R detector data will not be included in sum if set to 1.
		uint8_t	GEXMSK_L	:1;	//!< L detector data will not be included in sum if set to 1.
		uint8_t	GEXMSK_D	:1;	//!< D detector data will not be included in sum if set to 1.
		uint8_t	GEXMSK_U	:1;	//!< U detector data will not be included in sum if set to 1.
		uint8_t	GFIFOTH		:2;	//!< Gesture FIFO Threshold. This value is compared with the FIFO Level (i.e. the number of UDLR datasets) to generate an interrupt (if enabled).
	} Bits;
} uAPDS9960_REG__GCONFIG1;

/*!\union uAPDS9960_REG__GCONFIG2
** \brief Union for GCONFIG2 register of APDS9960
**/
typedef union {
	uint8_t Byte;
	struct PACK__ {
		uint8_t					GWTIME	:3;	//!< Gesture Wait Time. The GWTIME controls the amount of time in a low power mode between gesture detection cycles.
		APDS9960_drive_strength	GLDRIVE	:2;	//!< Gesture LED Drive Strength. Sets LED Drive Strength in gesture mode.
		APDS9960_prox_gain		GGAIN	:2;	//!< Gesture Gain Control. Sets the gain of the proximity receiver in gesture mode.
		uint8_t							:1;
	} Bits;
} uAPDS9960_REG__GCONFIG2;

/*!\union uAPDS9960_REG__GCONFIG3
** \brief Union for GCONFIG3 register of APDS9960
**/
typedef union {
	uint8_t Byte;
	struct PACK__ {
		APDS9960_gesture_dim	GDIMS	:2;	//!< Gesture Dimension Select. Selects which gesture photodiode pairs are enabled to gather results during gesture.
		uint8_t							:6;
	} Bits;
} uAPDS9960_REG__GCONFIG3;

/*!\union uAPDS9960_REG__GCONFIG4
** \brief Union for GCONFIG4 register of APDS9960
**/
typedef union {
	uint8_t Byte;
	struct PACK__ {
		uint8_t	GMODE		:1;	//!< Gesture Mode. Reading this bit reports if the gesture state machine is actively running, 1 = Gesture, 0= ALS, Proximity, Color.
		uint8_t	GIEN		:1;	//!< Gesture interrupt enable. Gesture Interrupt Enable. When asserted, all gesture related interrupts are unmasked.
		uint8_t	GFIFO_CLR	:1;	//!< Setting this bit to '1' clears GFIFO, GINT, GVALID, GFIFO_OV and GFIFO_LVL.
		uint8_t				:5;
	} Bits;
} uAPDS9960_REG__GCONFIG4;


/*!\union uAPDS9960_REG__PULSE
** \brief Union for PPULSE & GPULSE registers of APDS9960
**/
typedef union {
	uint8_t Byte;
	struct PACK__ {
		uint8_t					PULSE	:6;	//!< Pulse Count. Specifies the number of pulses to be generated on LDR. Number of pulses is set by xPULSE value plus 1.
		APDS9960_pulse_length	PLEN	:2;	//!< Pulse Length. Sets the LED-ON pulse width during a LDR pulse.
	} Bits;
} uAPDS9960_REG__PULSE;


/*!\union uAPDS9960_REG__CONTROL
** \brief Union for CONTROL register of APDS9960
**/
typedef union {
	uint8_t Byte;
	struct PACK__ {
		APDS9960_als_gain		AGAIN	:2;	//!< ALS and Color Gain Control.
		APDS9960_prox_gain		PGAIN	:2;	//!< Proximity Gain Control.
		uint8_t							:2;	//!< Proximity Diode Select.
		APDS9960_drive_strength	LDRIVE	:2;	//!< LED Drive Strength.
	} Bits;
} uAPDS9960_REG__CONTROL;


/*!\union uAPDS9960_REG__STATUS
** \brief Union for STATUS register of APDS9960
**/
typedef union {
	uint8_t Byte;
	struct PACK__ {
		uint8_t AVALID	:1;		//!< ALS Valid. Indicates that an ALS cycle has completed since AEN was asserted or since a read from any of the ALS/Color data registers.
		uint8_t PVALID	:1;		//!< Proximity Valid. Proximity Valid. Indicates that a proximity cycle has completed since PEN was asserted or since PDATA was last read. A read of PDATA automatically clears PVALID.
		uint8_t GINT	:1;		//!< Gesture Interrupt. GINT is asserted when GFVLV becomes greater than GFIFOTH or if GVALID has become asserted when GMODE transitioned to zero. The bit is reset when FIFO is completely emptied (read).
		uint8_t			:1;
		uint8_t AINT	:1;		//!< ALS Interrupt. This bit triggers an interrupt if AIEN in ENABLE is set.
		uint8_t PINT	:1;		//!< Proximity Interrupt. This bit triggers an interrupt if PIEN in ENABLE is set.
		uint8_t PGSAT	:1;		//!< Indicates that an analog saturation event occurred during a previous proximity or gesture cycle. Once set, this bit remains set until cleared by clear proximity interrupt special function command (0xE5 PICLEAR) or by disabling Prox (PEN=0). This bit triggers an interrupt if PSIEN is set.
		uint8_t CPSAT	:1;		//!< Clear Photodiode Saturation. When asserted, the analog sensor was at the upper end of its dynamic range. The bit can be de-asserted by sending a Clear channel interrupt command (0xE6 CICLEAR) or by disabling the ADC (AEN=0). This bit triggers an interrupt if CPSIEN is set.
	} Bits;
} uAPDS9960_REG__STATUS;

/*!\union uAPDS9960_REG__GSTATUS
** \brief Union for GSTATUS register of APDS9960
**/
typedef union {
	uint8_t Byte;
	struct PACK__ {
		uint8_t GVALID	:1;		//!< Gesture FIFO Data. GVALID bit is sent when GFLVL becomes greater than GFIFOTH (i.e. FIFO has enough data to set GINT). GFIFOD is reset when GMODE = 0 and the GFLVL=0 (i.e., All FIFO data has been read).
		uint8_t GFOV	:1;		//!< Gesture FIFO Overflow. A setting of 1 indicates that the FIFO has filled to capacity and that new gesture detector data has been lost.
		uint8_t			:6;
	} Bits;
} uAPDS9960_REG__GSTATUS;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/******************/
/*** Slave init ***/
/******************/

/*!\brief Initialization for APDS9960 peripheral
** \param[in] idx - APDS9960 index
** \param[in] hi2c - pointer to APDS9960 I2C instance
** \param[in] devAddress - APDS9960 device address
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Init(const uint8_t idx, I2C_HandleTypeDef * const hi2c, const uint16_t devAddress);

/*!\brief Initialization for APDS9960 peripheral
** \warning In case multiple devices (defined by I2C_APDS9960_NB > 1), you shall use APDS9960_Init instead
** \return FctERR - error code
**/
FctERR APDS9960_Init_Single(void);


/************************/
/*** Low level access ***/
/************************/

/*!\brief I2C Write function for APDS9960
** \param[in] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \param[in] nb - Number of bytes to write
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Write(I2C_slave_t * const pSlave, const uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Read function for APDS9960
** \param[in] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \param[in] nb - Number of bytes to read
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Read(I2C_slave_t * const pSlave, uint8_t * data, const uint16_t addr, const uint16_t nb);


/*!\brief I2C Word Write (little endian) function for APDS9960
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] data - pointer to write from
** \param[in] addr - Address to write to
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Write_Word(I2C_slave_t * const pSlave, const uint16_t * data, const uint16_t addr);


/*!\brief I2C Word Read (little endian) function for APDS9960
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in,out] data - pointer to read to
** \param[in] addr - Address to read from
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Read_Word(I2C_slave_t * const pSlave, uint16_t * data, const uint16_t addr);


/*!\brief I2C Special function Write function for APDS9960
** \param[in,out] pSlave - Pointer to I2C slave instance
** \param[in] func - special function to execute
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Write_Special(I2C_slave_t * const pSlave, const APDS9960_spec_func func);


/****************************************************************/
// cppcheck-suppress-begin misra-c2012-20.1 ; include directives after other declarations
#include "APDS9960_proc.h"	// Include procedures
#include "APDS9960_ex.h"	// Include extensions
// cppcheck-suppress-end misra-c2012-20.1

#endif

#ifdef __cplusplus
	}
#endif

#endif /* APDS9960_H__ */
/****************************************************************/
