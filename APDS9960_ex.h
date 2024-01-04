/*!\file APDS9960_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
** \brief APDS9960 Driver extensions
** \details APDS9960: Digital Proximity, Ambient Light, RGB and Gesture Sensor
**/
/****************************************************************/
#ifndef __APDS9960_EX_H__
	#define __APDS9960_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "APDS9960.h"

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Constants
// *****************************************************************************


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum APDS9960_chan
** \brief Type channels of APDS9960
**/
typedef enum PACK__ APDS9960_chan {
	APDS9960__CHAN_CLEAR = 0,	//!< Clear light channel
	APDS9960__CHAN_RED,			//!< Red light channel
	APDS9960__CHAN_GREEN,		//!< Green light channel
	APDS9960__CHAN_BLUE,		//!< Blue light channel
	APDS9960__CHAN_PROXIMITY	//!< Proximity channel
} APDS9960_chan;


/*!\union uAPDS9960_REG_MAP
** \brief Union of APDS9960 registry map
**/
typedef union uAPDS9960_REG_MAP {
	uint8_t Bytes[176];
	struct PACK__ {
		uint8_t					RAM[128];	//!< APDS9960 RAM
		uAPDS9960_REG__ENABLE	ENABLE;		//!< Enables of states and interrupts
		uint8_t					ATIME;		//!< ALS ADC time
		uint8_t					:8;
		uint8_t					WTIME;		//!< Wait time
		uint8_t					AILTL;		//!< ALS interrupt low threshold low byte
		uint8_t					AILTH;		//!< ALS interrupt low threshold high byte
		uint8_t					AIHTL;		//!< ALS interrupt high threshold low byte
		uint8_t					AIHTH;		//!< ALS interrupt high threshold high byte
		uint8_t					:8;
		uint8_t					PILT;		//!< Proximity interrupt low threshold
		uint8_t					:8;
		uint8_t					PIHT;		//!< Proximity interrupt high threshold
		uAPDS9960_REG__PERSIST	PERS;		//!< Interrupt persistence filters
		uAPDS9960_REG__CONFIG1	CONFIG1;	//!< Configuration one
		uAPDS9960_REG__PULSE	PPULSE;		//!< Proximity pulse count and length
		uAPDS9960_REG__CONTROL	CONTROL;	//!< Gain control register
		uAPDS9960_REG__CONFIG2	CONFIG2;	//!< Configuration two
		uint8_t					:8;
		uint8_t					ID;			//!< Device ID
		uAPDS9960_REG__STATUS	STATUS;		//!< Device status
		uint8_t					CDATAL;		//!< Clear data low byte
		uint8_t					CDATAH;		//!< Clear data high byte
		uint8_t					RDATAL;		//!< Red data low byte
		uint8_t					RDATAH;		//!< Red data high byte
		uint8_t					GDATAL;		//!< Green data low byte
		uint8_t					GDATAH;		//!< Green data high byte
		uint8_t					BDATAL;		//!< Blue data low byte
		uint8_t					BDATAH;		//!< Blue data high byte
		uint8_t					PDATA;		//!< Proximity data
		int8_t					POFFSET_UR;	//!< Proximity offset for UP and RIGHT photodiodes
		int8_t					POFFSET_DL;	//!< Proximity offset for DOWN and LEFT photodiodes
		uAPDS9960_REG__CONFIG3	CONFIG3;	//!< Configuration three
		uint8_t					GPENTH;		//!< Gesture proximity enter threshold
		uint8_t					GEXTH;		//!< Gesture exit threshold
		uAPDS9960_REG__GCONFIG1	GCONFIG1;	//!< Gesture configuration one
		uAPDS9960_REG__GCONFIG2	GCONFIG2;	//!< Gesture configuration two
		int8_t					GOFFSET_U;	//!< Gesture UP offset register
		int8_t					GOFFSET_D;	//!< Gesture DOWN offset register
		uAPDS9960_REG__PULSE	GPULSE;		//!< Gesture pulse count and length
		int8_t					GOFFSET_L;	//!< Gesture LEFT offset register
		uint8_t					:8;
		int8_t					GOFFSET_R;	//!< Gesture RIGHT offset register
		uAPDS9960_REG__GCONFIG3	GCONFIG3;	//!< Gesture configuration three
		uAPDS9960_REG__GCONFIG4	GCONFIG4;	//!< Gesture configuration four
		uint8_t					:8;
		uint8_t					:8;
		uint8_t					GFLVL;		//!< Gesture FIFO level
		uAPDS9960_REG__GSTATUS	GSTATUS;	//!< Gesture status
		//!\note Latest registers not on register map
	} Reg;
} uAPDS9960_REG_MAP;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/****************************************/
/*** High level methods and functions ***/
/****************************************/

/*** Configuration ***/
/*!\brief Write APDS9960 Enable register
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] en - Enable register value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9960_Write_En(APDS9960_t * const pCpnt, const uint8_t en) {
	return APDS9960_Write(pCpnt->cfg.slave_inst, (uint8_t *) &en, APDS9960__ENABLE, 1); }


/*!\brief Write APDS9960 Config1
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] cfg1 - Configuration1 register value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9960_Write_Cfg1(APDS9960_t * const pCpnt, const uint8_t cfg1) {
	uAPDS9960_REG__CONFIG1	CFG1 = { .Byte = (cfg1 & 0x02) | 0x60 };	// Mask WLONG & Set WRITE1 bits to 1
	return APDS9960_Write(pCpnt->cfg.slave_inst, &CFG1.Byte, APDS9960__CONFIG1, 1); }

/*!\brief Write APDS9960 Config2
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] cfg2 - Configuration2 register value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9960_Write_Cfg2(APDS9960_t * const pCpnt, const uint8_t cfg2) {
	uAPDS9960_REG__CONFIG2	CFG2 = { .Byte = (cfg2 & 0xF0) | 0x01 };	// Mask used bits & Set WRITE1 bit to 1
	return APDS9960_Write(pCpnt->cfg.slave_inst, &CFG2.Byte, APDS9960__CONFIG2, 1); }


/*!\brief Write APDS9960 Control
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] ctrl - Control register value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9960_Write_Ctrl(APDS9960_t * const pCpnt, const uint8_t ctrl) {
	return APDS9960_Write(pCpnt->cfg.slave_inst, &ctrl, APDS9960__CONTROL, 1); }


/*!\brief Oscillator Enable / Disable
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Set_PON(APDS9960_t * const pCpnt, const bool en);

/*!\brief ALS module Enable / Disable
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Set_AEN(APDS9960_t * const pCpnt, const bool en);

/*!\brief Proximity module Enable / Disable
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Set_PEN(APDS9960_t * const pCpnt, const bool en);

/*!\brief ALS interrupt module Enable / Disable
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Set_AIEN(APDS9960_t * const pCpnt, const bool en);

/*!\brief Proximity interrupt module Enable / Disable
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Set_PIEN(APDS9960_t * const pCpnt, const bool en);

/*!\brief WAIT module Enable / Disable
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Set_WEN(APDS9960_t * const pCpnt, const bool en);

/*!\brief Gesture module Enable / Disable
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] en - 0 Disable, 1 Enable
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Set_GEN(APDS9960_t * const pCpnt, const bool en);


/*!\brief ALS interrupt low threshold configuration
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] thr - Low threshold value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9960_Set_AILT(APDS9960_t * const pCpnt, const uint16_t thr) {
	return APDS9960_Write_Word(pCpnt->cfg.slave_inst, (uint16_t *) &thr, APDS9960__AILTL); }

/*!\brief ALS interrupt high threshold configuration
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] thr - High threshold value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9960_Set_AIHT(APDS9960_t * const pCpnt, const uint16_t thr) {
	return APDS9960_Write_Word(pCpnt->cfg.slave_inst, (uint16_t *) &thr, APDS9960__AIHTL); }

/*!\brief ALS interrupt thresholds configuration
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] lthr - Low threshold value
** \param[in] hthr - High threshold value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9960_Set_AIT(APDS9960_t * const pCpnt, const uint16_t lthr, const uint16_t hthr) {
	uint8_t DAT[4] = { LOBYTE(hthr), HIBYTE(hthr), LOBYTE(lthr), HIBYTE(lthr) };
	return APDS9960_Write(pCpnt->cfg.slave_inst, DAT, APDS9960__AILTL, sizeof(DAT)); }


/*!\brief Proximity interrupt low threshold configuration
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] thr - Low threshold value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9960_Set_PILT(APDS9960_t * const pCpnt, const uint8_t thr) {
	return APDS9960_Write(pCpnt->cfg.slave_inst, (uint8_t *) &thr, APDS9960__PILT, 1); }

/*!\brief Proximity interrupt high threshold configuration
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] thr - High threshold value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9960_Set_PIHT(APDS9960_t * const pCpnt, const uint8_t thr) {
	return APDS9960_Write(pCpnt->cfg.slave_inst, (uint8_t *) &thr, APDS9960__PIHT, 1); }

/*!\brief Proximity interrupt thresholds configuration
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] lthr - Low threshold value
** \param[in] hthr - High threshold value
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9960_Set_PIT(APDS9960_t * const pCpnt, const uint8_t lthr, const uint8_t hthr) {
	FctERR	err;
	err = APDS9960_Set_PILT(pCpnt, lthr);
	err |= APDS9960_Set_PIHT(pCpnt, hthr);
	return err; }


/*!\brief ALS Persistence configuration
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] persist - Persistence value
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Set_ALS_Pesistence(APDS9960_t * const pCpnt, const APDS9960_als_it_persist persist);

/*!\brief Proximity Persistence configuration
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] persist - Persistence value
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Set_Prox_Pesistence(APDS9960_t * const pCpnt, const APDS9960_prox_it_persist persist);


/*!\brief Proximity Pulse Length configuration
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] length - Proximity pulse length
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Set_Prox_Pulse_Length(APDS9960_t * const pCpnt, const APDS9960_pulse_length length);

/*!\brief Proximity Pulse Count configuration
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] cnt - Proximity pulse count
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Set_Prox_Pulse_Count(APDS9960_t * const pCpnt, const uint8_t cnt);


/*!\brief ALS Gain configuration
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] gain - Gain value
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Set_ALS_Gain(APDS9960_t * const pCpnt, const APDS9960_als_gain gain);

/*!\brief Proximity Gain configuration
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] gain - Gain value
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Set_Prox_Gain(APDS9960_t * const pCpnt, const APDS9960_prox_gain gain);


/*!\brief Proximity Drive configuration
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] ldrive - Proximity LED Drive Strength value
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Set_Prox_Drive_Strength(APDS9960_t * const pCpnt, const APDS9960_drive_strength ldrive);


/*!\brief ADC Integration time configuration
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] integ - Integration time value (from 3ms to 712ms)
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Set_ADC_Integration_Time(APDS9960_t * const pCpnt, const uint16_t integ);


/*!\brief Wait time configuration
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] wait - Wait time value (from 3ms to 8540ms)
** \return FctERR - error code
**/
FctERR NONNULL__ APDS9960_Set_Wait_Time(APDS9960_t * const pCpnt, const uint16_t wait);


/*** Special Functions ***/
/*!\brief Force interruption
** \param[in] pCpnt - Pointer to APDS9960 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9960_SF_Force_IT(APDS9960_t * const pCpnt) {
	return APDS9960_Write_Special(pCpnt->cfg.slave_inst, APDS9960__SF_IFORCE); }

/*!\brief Clear pending Proximity interruption
** \warning if event pin enabled, shall be called after read to reset pin
** \param[in] pCpnt - Pointer to APDS9960 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9960_SF_Clear_PROX_IT(APDS9960_t * const pCpnt) {
	return APDS9960_Write_Special(pCpnt->cfg.slave_inst, APDS9960__SF_PICLEAR); }

/*!\brief Clear pending ALS interruption
** \warning if event pin enabled, shall be called after read to reset pin
** \param[in] pCpnt - Pointer to APDS9960 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9960_SF_Clear_ALS_IT(APDS9960_t * const pCpnt) {
	return APDS9960_Write_Special(pCpnt->cfg.slave_inst, APDS9960__SF_CICLEAR); }

/*!\brief Clear pending ALS & Proximity interruptions
** \warning if event pin enabled, shall be called after read to reset pin
** \param[in] pCpnt - Pointer to APDS9960 component
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9960_SF_Clear_ALS_PROX_IT(APDS9960_t * const pCpnt) {
	return APDS9960_Write_Special(pCpnt->cfg.slave_inst, APDS9960__SF_AICLEAR); }


/*** Operation ***/
/*!\brief Get APDS9960 chip ID
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in,out] id - pointer to chip ID result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9960_Get_ChipID(APDS9960_t * const pCpnt, uint8_t * id) {
	return APDS9960_Read(pCpnt->cfg.slave_inst, id, APDS9960__ID, 1); }


/*!\brief Get Clear conversion
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in,out] clear - pointer to Clear conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9960_Get_Clear(APDS9960_t * const pCpnt, uint16_t * clear) {
	return APDS9960_Read_Word(pCpnt->cfg.slave_inst, clear, APDS9960__CDATAL); }

/*!\brief Get Red conversion
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in,out] red - pointer to Red conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9960_Get_Red(APDS9960_t * const pCpnt, uint16_t * red) {
	return APDS9960_Read_Word(pCpnt->cfg.slave_inst, red, APDS9960__RDATAL); }

/*!\brief Get Green conversion
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in,out] green - pointer to Green conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9960_Get_Green(APDS9960_t * const pCpnt, uint16_t * green) {
	return APDS9960_Read_Word(pCpnt->cfg.slave_inst, green, APDS9960__GDATAL); }

/*!\brief Get Blue conversion
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in,out] blue - pointer to blue conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9960_Get_Blue(APDS9960_t * const pCpnt, uint16_t * blue) {
	return APDS9960_Read_Word(pCpnt->cfg.slave_inst, blue, APDS9960__BDATAL); }

/*!\brief Get Proximity conversion
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in,out] prox - pointer to Prox conversion result
** \return FctERR - error code
**/
__INLINE FctERR NONNULL_INLINE__ APDS9960_Get_Prox(APDS9960_t * const pCpnt, uint8_t * prox) {
	return APDS9960_Read(pCpnt->cfg.slave_inst, prox, APDS9960__PDATA, 1); }


/*******************/
/*** GPIO access ***/
/*******************/

/*!\brief Interrupt GPIO pin init for APDS9960
** \weak APDS9960 Interrupt GPIO pin init may be user implemented if needed
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in] GPIOx - INT port
** \param[in] GPIO_Pin - INT pin
** \param[in] GPIO_Active: INT pin active state
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ APDS9960_INT_GPIO_Init(APDS9960_t * const pCpnt, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState GPIO_Active);

/*!\brief Interrupt GPIO pin getter for APDS9960
** \weak APDS9960 Interrupt GPIO pin getter may be user implemented if needed
** \param[in] pCpnt - Pointer to APDS9960 component
** \param[in,out] pState - Pointer to INT pin state variable (0: inactive, 1: active)
** \return FctERR - ErrorCode
**/
FctERR NONNULL__ APDS9960_INT_GPIO_Get(APDS9960_t * const pCpnt, bool * const pState);


/****************************************************************/
#endif

#ifdef __cplusplus
	}
#endif

#endif /* __APDS9960_EX_H__ */
/****************************************************************/
