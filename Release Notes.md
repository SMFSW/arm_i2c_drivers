# ARM I2C drivers (release notes)

The MIT License (MIT)

Copyright (c) 2017 SMFSW (Sebastien Bizien)

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


## v0.5

* added en variable in slave struct and getters/setters (to disable peripheral if init goes wrong)
* added test if slave is enabled before sending/getting messages on I2C bus
* disable peripheral if Init_Sequence goes wrong (except for EEPROM/FRAM peripherals which are somewhat generic)
* fixed some casts in library (from gcc strict warnings)
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

## v0.4

* minor change in component base to calculate max address in pre-comp
* removed version in files, everything will be in release notes


## v0.3

* few fixes & added precomp define to hide warnings from component files


## v0.2

* fixes & lots of peripheral added


## v0.1

* initial commit
