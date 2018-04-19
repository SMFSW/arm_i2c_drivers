# ARM I2C drivers

ARM I2C Drivers for various components

## Requires

- sarmfsw
  - [sarmfsw](https://github.com/SMFSW/sarmfsw)

- HARMcksL
  - [HARMcksL](https://github.com/SMFSW/HARMcksL)

## Usage

- define I2C_$CPNT$ in a file called globals.h with the I2C instance you want it to be tied to
- use $CPNT$_Init() once
- start using $CPNT$_funcs()

- if you find warning messages annoying, you could define NO_WARN_I2C_DRIVERS (underscores between words) in compiler options to hide warning messages

## Remarks

- define VERBOSE in compiler options to printf infos from component handlers

## Next

- The library lacks the possibility to implement multiple identical devices:
  - This will be fixed in a later version through I2C_component and the slave struct

## Supported peripherals

- **AT42QT1244**: 24-key QMatrix FMEA IEC/EN/UL60730 Touch Sensor

  -> _NOT TESTED YET_

- **BMP180**: Digital pressure sensor

  -> _WORKING_

- **DRV2605L**: 2 to 5.2V Haptic Driver for LRA and ERM With Effect Library and Smart-Loop Architecture

  -> _WORKING, TESTS PENDING_

- **DS-GPM.S**: 99 Channel Positioning System (GPS + GLONASS) Shield

  -> _WORKING_

- **FM24C**: 4/16-Kbit (512/2K * 8) Serial I2C F-RAM

  -> _WORKING_

- **L3GD20H**: MEMS motion sensor, three-axis digital output gyroscope

  -> _CODING IN PROGRESS / NO TESTS YET_

- **LSM303DLHC**: Ultra compact high performance e-compass 3D accelerometer and 3D magnetometer module

  -> _CODING IN PROGRESS / NO TESTS YET_

- **MB85RC256V**: 256-Kbit (32K * 8) I2C Memory FRAM

  -> _WORKING_

- **MCP4725**: 12-Bit Digital-to-Analog Converter with EEPROM Memory

  -> _WORKING_

- **MCP9808**: +/-0.5C Maximum Accuracy Digital Temperature Sensor

  -> _WORKING_

- **MTCH6102**: Low-Power Projected Capacitive Touch Controller

  -> _WORKING_

- **PCA9624**: 8-bit Fm+ I2C-bus 100mA 40V LED driver

  -> _NOT TESTED YET_

- **PCA9685**: 16-channel, 12-bit PWM Fm+ I2C-bus LED controller

  -> _WORKING (TODO: handle more extensions)_

- **PCF8523**: Real-Time Clock (RTC) and calendar

  -> _WORKING (TODO: handle clkout and alarms)_

- **S11059**: I2C interface-compatible color sensor

  -> _TESTS PENDING

- **TCS3400**: Color light-to-digital converter

  -> _TESTS PENDING (communication OK, auto-integration, persistence & conversions in the works)_

- **TCS3472**: Color light-to-digital converter with IR filter

  -> _TESTS PENDING (communication OK, auto-integration, persistence & conversions in the works)_

- **TSL2591**: Very-high sensitivity light-to-digital converter

  -> _TESTS PENDING (communication OK, auto-integration, persistence & conversions in the works)_


## FRAM / EEPROM

- Use FM24C for I2C address banked components (with 8b internal address):
  - Tested on:
    - FM24C16B
    - FM24C04B
    - BR24T04FVM

- Use MB85RC256V for 16b internal address components:
  - Tested on:
    - MB85RC256V
    - FM24CL64B

## Next peripherals

- EEPROM/FRAM (few others if needed?)
- Capacitive (MPR121)
- OLED
- Clock Generator (SI5351 with help of [etherkit](https://github.com/etherkit/Si5351Arduino))
- Accelerometer/Gyro (GY521 MPU6050 with help of [jrowberg](https://github.com/jrowberg/i2cdevlib/))
- ...

You may also:

- contribute to devices additions using the template in subfolder
- raise issue on github to ask for particular devices

## TODO

- Copatibility handling multiple identical components
- FreeRTOS compatibility using R/W functions using interrupts with callbacks

## Misc

Feel free to share your thoughts @ xgarmanboziax@gmail.com about:

- issues encountered (might also raise issue on github directly)
- optimizations
- improvements & new functionalities

## Documentation

Doxygen documentation can be generated using "Doxyfile"

## Release Notes

See [release notes](https://github.com/SMFSW/arm_i2c_drivers/ReleaseNotes.md)

## License

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
