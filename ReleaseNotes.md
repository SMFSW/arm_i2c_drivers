# ARM I2C drivers (release notes)

The MIT License (MIT)

Copyright (c) 2017-2019 SMFSW (Sebastien Bizien)

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

## v1.0

* implementation of multiple peripheral of the same type (for devices that may most likely not being alone in a project)
	- needed a few refactoring (inlines becoming functions / some typedefs moved)
	- except: DS_GPMS, PCF8523 & DRV2605L
* template update (splitted into singleton/multiple device(s))
* more NONNULL__ checks
* AMG88: added
* APDS9930: added (+ warning if APDS9960 also compiled, OFFSET union removed as coded as signed 8bits integer)
* APDS: added
* BMP180: fixed temperature and pressure calculation (no use of unsigned LSHIFT or RSHIFT for signed calculations)
* FM24C: added possibility to customize FM24C_SIZE macro if needed
* MCP4725: Fixed general call function (transmit command, not receive), and taking I2C_HandleTypeDef pointer as parameter
* MTCH6102: 1ms delay between each transaction end has been added to ensure proper communication flow
* MTCH6102: As MTCH6102 seems somewhat erratic on first try when it comes to configure it, I often get default RX/TX number values after config performed with custom projects:
	- added a while loop to test if parameters are taken into account (it works well the second time if not on the first time)...
	- I already encountered a lot of issues with this chip, related to timing between transactions, power on, etc... mostly when it comes to configure chip or store to NV storage
	- As the Datasheet is very light about these subjects (and I got no answers from Microchip), here are some workarounds that seems to be working all the time (so far)
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
* PCA9685: fix all channels commands (writing values from ON_H, not ON_L)
* PCA9685: fix for PutVal commands (writing some wrong Byte), 8b one is now inlined
* PCA9685: optimizations and code refactoring
* shared_ALS: added files for common code regarding ambient light sensing
* shared_CLS: added files for common code regarding color light sensing (RGB to chromacity CIE1931 xy conversion and CCT calculation)
* shared_Temperature: added files for common code regarding temperature conversion (Ceelsius & Farenheit)
* TCS3400, TCS3472: added matrix in peripheral configuration struct (to be able to use custom matrix for RGB to XYZ conversion)
* TCS34xx, TSL2591: set x(I)EN flags with same value in weak init sequence
* TCS34xx, TSL2591: fix calculation of maximum count (using stored ALS Integration register value)
* TCS34xx, TSL2591: get Status register in handler for results validity
* TCS34xx, TSL2591: removed variable set to 0 after reading from register (to keep other register params intact)
* TCS34xx, TSL2591: setting both thresholds at once (now fixed, thresholds were inverted); added persistence configuration function in ex file
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
