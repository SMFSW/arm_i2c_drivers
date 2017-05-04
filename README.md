# arm_i2c_drivers
ARM I2C Drivers for various components

## Requires:
- sarmfsw
  - https://github.com/SMFSW/sarmfsw

- HARMcksL
  - https://github.com/SMFSW/HARMcksL


## Usage:
- define I2C_$CPNT$ in a file called globals.h with the instance I2C instance you want it to be tied to
- use $CPNT$_Init() once
- start using $CPNT$_funcs()

