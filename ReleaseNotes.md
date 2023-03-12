# ARM I2C drivers (release notes)

The MIT License (MIT)

Copyright (c) 2017-2023 SMFSW (Sebastien Bizien)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## v1.1

* Check for address alignment in r/w word functions
* No more check of r/w memory address (implicitly included in overflow check)
* I2CMEM: EEPROM/FRAM Memories component added (compatible between EEPROM/FRAM, any protocol)
* MB85RC256V & FM24C: Few refactoring & reset to legacy use (only FRAM)
* FM24C: R/W refactoring to allow operations on the whole chip, bank switching handled internally
* Every components _hal structures are now hidden to user
* Singleton components initialized as other components (const copy)
* I2C address only written one (calling to slave init function), not set by I2C_PERIPHERAL_SET_DEFAULTS anymore
* DRV2605L: few refactoring of DRV2605L_Init_Sequence
* Doxygen comments fixes for the whole library
* Fix case in include names
* I2C_slave_timeout set to 50ms instead of 20ms (in case of components with lots to read at low speed)
* I2C_eep_slave_timeout added, set to 500ms (for EEPROM/FRAM components type) to keep some margin for really long transactions
  * For bigger size components, in case timeout is reached, read/write transactions may be split in multiple calls (refreshing watchdog if in use).
  * Set a longer timeout by calling I2C_set_slave_timeout in init sequence is another way to address such a need
* const pointer address qualifier added for pSlave & pCpnt parameters
* CPNT_handler_it functions added when relevant for component
* Peripheral structures declared as intialized data
* I2C_peripheral: GPIO functions added + components update
* ADS1115 & AT42QT1244: call to init_Delay_us added (following changes in HARMcksL library)
* ADS1115: waiting for current conversion done by handler (not in ADS1115_Start_NextConversion) -> enables proper handling of interrupt pin instead of delay
* PCA9956: added
* PCA9532, PCA9624, PCA9956: contents not written directly in PCA9xxx_Set_Mode_LED & PCA9xxx_Set_Mode_LEDs (to work no matter endian)
* PCA9624, PCA9956: functions added to write/read array of duty cycle values (instead of only one channel at a time)
* PCA9xxx: few changes for consistency between functions behaviors across all PCA9xxx peripherals
* PCA9xxx & PCA9624 & PCA9685: typedefs changed and created for consistency across peripherals

## v1.0

* Fix for cpp preprocessor end bracket misplacement in regard of other endif directives in headers
* rationalization of includes (globals.h included at less possible places)
* comments and warnings/errors refactoring
* strict aliasing types in printf statements
* Some informations added in components doxygen documentation for compatibility with other devices
* implementation of multiple peripheral of the same type (for devices that may most likely not being alone in a project)
  * needed a few refactoring (inlines becoming functions / some typedefs moved)
  * except: DS_GPMS, PCF8523 & DRV2605L
* template update (splitted into singleton/multiple device(s))
* more NONNULL__ checks
* possibility to set different device base address than default set in header (at project level or in globals.h)
* init function for peripherals use value in structure for timeout init (in case using a custom timeout)
* all register unions have their struct definition packed
* VERBOSE mode padding and refactoring following components (also displaying component index when applicable)
* ADS1115: added
* AMG88: added
* APDS9930: added (+ warning if APDS9960 also compiled, OFFSET union removed as coded as signed 8bits integer)
* APDS9960: added with fixes and default handler
* APDS99xx: use of config bits (instead of Status register ones) to trigger proper interrupt(s) clear function
* AT42QT1244: renaming of bitfield bit DEBUG in uAT42QT_REG__SETUP_238 (potential compilation issue, DEBUG symbol being defined in some latest STM32 HAL)
* AT42QT1244: AT42QT1244_GET_KEYS_ONLY option added to read only key status in handler (otherwise reads status registers too)
* AT42QT1244: Added union for HOST_CRC
* AT42QT1244: Fix uAT42QT_REG__KEY_DATA bitfield order
* AT42QT1244: crc calculation not static (to be able to use it to generate crc for setup block)
* AT42QT1244: AT42QT1244_Send_Setup to write whole setup block at once with updated CRC, also returned in given function parameter
* AT42QT1244: all functions writing setup register updated to have new CRC as returned parameter
* AT42QT1244: AT42QT1244_CHECK_CRC added to enable/disable CRC checks after read transactions
* AT42QT1244: former AT42QT1244_Setup_Key becomes AT42QT1244_Setup_Keys (allowing writing multiple keys at once with a mask)
* AT42QT1244: delay fix
* AT42QT1244: Few delay added between each communication for calibration pending in FHM setup procedure
* AT42QT1244: weak handler & init implementations
* BMP180: fixed temperature and pressure calculation (no use of unsigned LSHIFT or RSHIFT for signed calculations)
* BMP180: changes to allow (custom) intialization of sea level pressure for the component (relying on shared_APS module)
* DRV2605: fixed init following timeout configuration using defaults from structure
* FM24C: added possibility to customize FM24C_SIZE macro if needed
* MCP4725: Fixed general call function (transmit command, not receive), and taking I2C_HandleTypeDef pointer as parameter
* MTCH6102: 1ms delay between each transaction end has been added to ensure proper communication flow
* MTCH6102: As MTCH6102 seems somewhat erratic on first try when it comes to configure it, I often get default RX/TX number values after config performed with custom projects:
  * added a while loop to test if parameters are taken into account (it works well the second time if not on the first time)...
  * I already encountered a lot of issues with this chip, related to timing between transactions, power on, etc... mostly when it comes to configure chip or store to NV storage
  * As the Datasheet is very light about these subjects (and I got no answers from Microchip), here are some workarounds that seems to be working all the time (so far)
* MTCH6102: splitted init function into several functions (to also be able to store settings to Non-volatile storage)
* MTCH6102: period calculation doesn't use floats anymore to save some flash space
* MTCH6102: simplified MTCH6102_Get_MFG_Results
* MTCH6102: added automatic handling of referential rotation (and referential initialisation)
* MTCH6102: removal of rotation function (now part of sarmfsw in arm_inlines_rotation.h)
* PCAxxxx: refactoring including renaming of PCA96xx.h to PCA9xxx.h and symbols refactoring inside
* PCA9532: added
* PCA96xx: splitted reset function (one for single device, other as general call)
* PCA96xx: channels enum with more channels (starting at 0)
* PCA96xx: optimized code with channel enum values, other refactoring and more functions
* PCA9624: setter added for register auto incrementation options (set to auto-increment over all registers as default for consistency with most of components included in the library / common handling behavior)
* PCA9624: set all channels to dimmable mode in default init sequence
* PCA9624: added setters for MODE1 & MODE2 registers as inlines (consistancy especially for MODE2 which have some reserved bits values for proper operation)
* PCA9685: possible handling of external clock and computation of prescaler and frequency modified
* PCA9685: functions to compute frequency/prescaler for outputs frequency rationalization with PCA9532 functions
* PCA9685: Refactoring and changes to handle PCA9685 outputs shifting (delay on)
* PCA9685: Dedicated functions to compute PWM register values (useful to manually fill an array with registers values to send multiple channels at once in one I2C transaction)
* README.md: updated (and added limitations section)
* shared_APS: added files for common code regarding ambient pressure sensing
* shared_ALS: added files for common code regarding ambient light sensing
* shared_CLS: added files for common code regarding color light sensing (RGB to chromacity CIE1931 xy conversion and CCT calculation)
* shared_CLS: double precision now single precision
* shared_Temperature: added files for common code regarding temperature conversion (Ceelsius & Farenheit)
* TCS3400, TCS3472: added matrix in peripheral configuration struct (to be able to use custom matrix for RGB to XYZ conversion)
* TCS34xx, TSL2591: set x(I)EN flags with same value in weak init sequence
* TCS34xx, TSL2591: fix calculation of maximum count (using stored ALS Integration register value)
* TCS34xx, TSL2591: get Status register in handler for results validity
* TCS34xx, TSL2591: removed variable set to 0 after reading from register (to keep other register params intact)
* TCS34xx, TSL2591: setting both thresholds at once (now fixed, thresholds were inverted); added persistence configuration function in ex file
* TCS34xx: Status register bitfield declaration fix
* TSL2591: fix uTSL2591_REG__STATUS AVALID bit
* TSL2591: changed lux calculations to fit with the ones used for APDS930

## v0.7

* Added cpp preprocessor directives in headers
* Doxyfile update
* Adding support for doxygen documentation generation with Travis CI
* Using of NONNULL__ attribute instead of checking pointer set to value in code
* template: misc fixes
* use of const qualifier in function parameters
* rationalization of includes
* init sequence returns with error value if something goes wrong during initialization
* implementation of slave instance (slave_inst) in xxx_proc files (easier debug access to component structure except special cases)
* I2C_component: instance inst becomes bus_inst (new slave_inst implementation in xxx_proc files to avoid name confusions)
* FM24C & MB85RC256V: added macros to create simple inlines to read/write values from addresses (to use in some header)
* MTCH6102: added standalone function for commands & inlines for configuration, restore to defaults & tests commands
* MTCH6102: added filtering type configuration function & inlines for configuration
* MTCH6102: added average compensation calculation for channels following datasheet & manufacturing test execution with results
* MTCH6102: after testing, added delay of 1ms in MTCH6102_Command while loop waiting for operation to complete
* TCSxxxx, TSL2591: err affected when clearing it flag in handler
* TCSxxxx: corrected McCammy's CCT formula to get coherent results
* S11059: Hamamatsu Color Sensor added

## v0.6

* overhaul update due to updates from sarmfsw
* removed peripheral includes (already done by sarmfsw library)
* warnings about sharing same address on bus re-enabled
* changed CPNT_Init, CPNT_Init_Sequence & CPNT_handler as weak functions
* removed inclusion of FctERR.h now handled by sarmfsw
* ERR_xx refactored to ERROR_xx
* BMP180: reworked pressure and temperature calculations
* BMP180: added datasheet example for testing purposes
* MCP4725: fixed ReadDAC function & WriteCommand for Fast Mode
* MTCH6102: added example init sequence to ensure MTCH6102 takes changed parameters into account
* PCF8523: notes & changes for 12/24 hours formats handling
* TCSxxxx, TSL2591: Fixed WriteWord functions (low byte first)

## v0.5

* added en variable in slave struct and getters/setters (to disable peripheral if init goes wrong)
* added test if slave is enabled before sending/getting messages on I2C bus
* disable peripheral if Init_Sequence goes wrong (except for EEPROM/FRAM peripherals which are somewhat generic)
* fixed some casts in library (from gcc strict warnings)
* added external declaration of CPNT in CPNT_proc.h (removed CPNT_ex.c files)
* FM24C: fixed max speed to 1MHz
* TCSxxxx, TSLxxxx: added missing carriage returns in verbose parts
* TCSxxxx: fixed McCamy's formula to calculate cct
* DRV2605L: fixed missing inline returns in base header
* DRV2605L: fixed registers definitions for: Status, FeedbackControl
* TCSxxxx, TSLxxxx: added missing Doxygen comments
* DRV2605L: added missing Doxygen comments
* BMP180: added missing Doxygen comments
* MCP9808: added missing Doxygen comments
* MCP4725: added missing Doxygen comments
* MTCH6102: added single RX/TX pin resolution define
* MTCH6102: added min/max x/y depending on RX/TX number
* MTCH6102: added Centered Coords to proc struct
* MTCH6102: added coordinate rotation function (most valuable when coordinates are set to pad center)
* Removed CMSIS & HAL includes from files (already included by sarmfsw library)

## v0.4

* minor change in component base to calculate max address in pre-comp
* removed version in files, everything will be in release notes

## v0.3

* few fixes & added precomp define to hide warnings from component files

## v0.2

* fixes & lots of peripheral added

## v0.1

* initial commit
