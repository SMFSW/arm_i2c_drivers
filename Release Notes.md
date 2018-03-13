# ARM I2C drivers (release notes)

The MIT License (MIT)

Copyright (c) 2017-2018 SMFSW (Sebastien Bizien)

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

## v0.7

* use of const qualifier in function parameters
* init sequence returns with error value if something goes wrong during initialization
* FM24C & MB85RC256V: added macros to create simple inlines to read/write values from addresses (to use in some header)
* MTCH6102: added standalone function for commands & inlines for configuration, restore to defaults & tests commands
* MTCH6102: added filtering type configuration function & inlines for configuration
* MTCH6102: added average compensation calculation for channels following datasheet & manufacturing test execution with results

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
