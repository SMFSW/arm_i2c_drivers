# ARM I2C drivers

[![author](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/SMFSW/a9a2b2a02fda1b33461d53ddfe69d649/raw/auth_SMFSW.json)](https://github.com/SMFSW)
![badge](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/SMFSW/a9a2b2a02fda1b33461d53ddfe69d649/raw/i2c_drivers_status_badge.json)
[![license](https://img.shields.io/badge/License-MIT-darkred.svg)](LICENSE)
[![CodeFactor](https://www.codefactor.io/repository/github/smfsw/arm_i2c_drivers/badge)](https://www.codefactor.io/repository/github/smfsw/arm_i2c_drivers)

![platform](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/SMFSW/a9a2b2a02fda1b33461d53ddfe69d649/raw/platform_STM32.json)

[![doxy](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/SMFSW/a9a2b2a02fda1b33461d53ddfe69d649/raw/tool_DOXY.json)](https://smfsw.github.io/arm_i2c_drivers)
[![re_note](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/SMFSW/a9a2b2a02fda1b33461d53ddfe69d649/raw/tool_RN.json)](ReleaseNotes.md)

![badge](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/SMFSW/a9a2b2a02fda1b33461d53ddfe69d649/raw/i2c_drivers_custom_repo_badge.json)

ARM I2C Drivers for various components

## Requires

- sarmfsw
  - [sarmfsw](https://github.com/SMFSW/sarmfsw)

- HARMcksL (following used components(s))
  - [HARMcksL](https://github.com/SMFSW/HARMcksL)

## Usage

- define `I2C_$CPNT$` in a file called globals.h with the I2C instance you want it to be tied to (no neeed to declare instance in case of multiple devices)
- define `I2C_$CPNT$_NB` with number of devices to enable multiple devices
- define `$CPNT$_BASE_ADDR` with base device address if in the need to override default address defined in header
- use `$CPNT$_Init(idx, hi2c, devAddress)` for every device with its index, proper instance and I2C address
  - I2CMEM type is generic and needs 2 more parameters: storage size and write buffer size (can be set to 0 for FRAM)
- use `$CPNT$_Init_Single()` once in case of single device using defaults
  - I2CMEM type is generic and needs 2 parameters: storage size and write buffer size (can be set to 0 for FRAM)
- start using `$CPNT$_funcs()`

## Remarks

- define `VERBOSE` in compiler options to printf infos from component handlers
- define `NO_WARN_I2C_DRIVERS` at project level to disable possible warning messages when multiple devices declared which may have same address
- Components with a xxx_proc file can access `I2C_slave` instance through `CPNT->cfg.slave_inst`.
Please keep in mind some components are somewhat custom and needs to be accesses through `CPNT->cfg.slave_inst_xxx` instead if needed.

## Supported devices

### EEPROM / FRAM:

- **I2CMEM**: Any EEPROM / FRAM
  - **STATUS**: DONE
  - Use I2CMEM when compatibility between EEPROM/FRAM is needed
  - Check [I2CMEM_ex.h](I2CMEM_ex.h) for a few components init aliases
  - In case FRAM is only useful, you may use legacy FM24C or MB85RC256V components, or set buf_size to I2CMEM_WBUF_NONE at init
  - Transaction timeout set to 500ms:
    - There shouldn't be any timeout reached for these components, otherwise refer to MB85RC256V comment

- **FM24C**: 4/16-Kbit (512/2K * 8) Serial I2C F-RAM
  - **STATUS**: DONE
  - I2C_Drivers legacy FRAM component, consider switching to I2CMEM for new designs
  - Use FM24C for I2C addresses banked components (with 8b internal address):
    - Tested on:
      - FM24C16B
      - FM24C04B
      - BR24T04FVM
  - Transaction timeout set to 500ms:
    - There shouldn't be any timeout reached for these components, otherwise refer to MB85RC256V comment

- **MB85RC256V**: 256-Kbit (32K * 8) I2C Memory FRAM
  - **STATUS**: DONE
  - I2C_Drivers legacy FRAM component, consider switching to I2CMEM for new designs
  - Use MB85RC256V for 16b internal addresses components:
    - Tested on:
      - MB85RC256V
      - FM24CL64B
  - Transaction timeout set to 500ms:
    - For bigger size components, in case timeout is reached, read/write transactions may be split in multiple calls (refreshing watchdog if in use).
    - Set a longer timeout by calling I2C_set_slave_timeout in init sequence is another way to address such a need

### OTHER:

- **ADS1115**: Ultra-Small, Low-Power, 16-Bit Analog-to-Digital Converter with Internal Reference
  - **STATUS**: DONE

- **AMG88**: Infrared Array Sensor (Grid-EYE)
  - **STATUS**: DONE (TODO: recognition)

- **APDS9930**: Digital Proximity and Ambient Light Sensor
  - **STATUS**: DONE

- **APDS9960**: Digital Proximity, Ambient Light, RGB and Gesture Sensor
  - **STATUS**: DONE (TODO: GESTURES TO TEST/SWITCH, DISABLED YET)

- **AT42QT1244**: 24-key QMatrix FMEA IEC/EN/UL60730 Touch Sensor
  - **STATUS**: DONE

- **BMP180**: Digital pressure sensor
  - **STATUS**: DONE

- **DRV2605L**: 2 to 5.2V Haptic Driver for LRA and ERM With Effect Library and Smart-Loop Architecture
  - **STATUS**: DONE

- **DS-GPM.S**: 99 Channel Positioning System (GPS + GLONASS) Shield
  - **STATUS**: DONE

- **MCP4725**: 12-Bit Digital-to-Analog Converter with EEPROM Memory
  - **STATUS**: DONE

- **MCP9808**: +/-0.5C Maximum Accuracy Digital Temperature Sensor
  - **STATUS**: DONE

- **MTCH6102**: Low-Power Projected Capacitive Touch Controller
  - **STATUS**: DONE

- **NCA9595**: Low-voltage 16-bit I²C and SMBus I/O expander
  - **STATUS**: DONE

- **PCA9532**: 16-bit I2C-bus LED dimmer
  - **STATUS**: DONE

- **PCA9624**: 8-bit Fm+ I2C-bus 100mA 40V LED driver
  - **STATUS**: DONE

- **PCA9685**: 16-channel, 12-bit PWM Fm+ I2C-bus LED controller
  - **STATUS**: DONE

- **PCA9952**: 16-channel Fm+ I2C-bus 57 mA constant current LED driver (compatible with PCA9955)
  - **STATUS**: DONE

- **PCA9956**: 24-channel Fm+ I2C-bus 57 mA/20 V constant current LED driver
  - **STATUS**: DONE

- **PCF8523**: Real-Time Clock (RTC) and calendar
  - **STATUS**: DONE (TODO: handle clkout and alarms)

- **S11059**: I2C interface-compatible color sensor
  - **STATUS**: DONE

- **TCS3400**: Color light-to-digital converter
  - **STATUS**: DONE (TODO: auto-integration & persistence in the works)

- **TCS3472**: Color light-to-digital converter with IR filter
  - **STATUS**: DONE (TODO: auto-integration & persistence in the works)

- **TMP1075**: Temperature Sensor With I2C and SMBus Interface in Industry Standard LM75 Form Factor and Pinout
  - **STATUS**: In progress

- **TSL2591**: Very-high sensitivity light-to-digital converter
  - **STATUS**: DONE (TODO: auto-integration & persistence in the works)

## Multiple component type on single project (singleton components excluded)

- If multiple component type are used on the same MCU device:
  - A for loop can be used passing I2C_$CPNT$ if all components are tied to the same i2C instance (physical bus)
  - In case same multiple same component type are used on different physical busses, please pass each instance to $CPNT$_Init
    - note: formerly, I2C_$CPNT$ was used to determine device type enabling (and its instance); as long as enabled with former I2C_$CPNT$, init function can be called using other params

## Following peripherals (?)

- Capacitive (MPR121)
- OLED
- Clock Generator (SI5351 with help of [etherkit](https://github.com/etherkit/Si5351Arduino))
- Accelerometer/Gyro (GY521 MPU6050 with help of [jrowberg](https://github.com/jrowberg/i2cdevlib/))
- ...

You may also:

- contribute with more devices using templates in subfolder
  - if in doubt, `multiple` template implementation is recommended
- raise issue on github to ask for particular devices

## Limitations

- Compatibility with interrupt driven R/W transactions (when possible & usefull, yet already tested manually on some components)

## TODO

- Any RTOS compatibility (blocking transactions doesn't match with RTOS task timings)
- (FreeRTOS) compatibility using R/W functions using interrupts with callbacks (when possible)

## Misc

Feel free to share your thoughts @ xgarmanboziax@gmail.com about:

- issues encountered (might also raise issue on github directly)
- optimizations
- improvements & new functionalities
