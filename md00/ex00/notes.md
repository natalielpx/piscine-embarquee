# ex00
Write code → Compile → Convert → Flash to chip

## Compiler: `avr-gcc`
`avr-gcc` is a version of the GCC compiler specifically targeting AVR microcontrollers
```
avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os main.c -o main.bin
```
- `-mmcu=atmega328p`: target AVR chip (in lowercase)
- `-DF_CPU=16000000UL`: `D`efine `F_CPU` -- defines CPU clock speed as 16 MHz
- `-Os`: optimise for size (important on chips with limited memory)
- `main.c`: source file
- `-o main.bin`: output file

MacOS: https://github.com/osx-cross/homebrew-avr

## Converter: `avr-objcopy`
https://linux.die.net/man/1/avr-objcopy

`avr-objcopy` strips out the extra metadata and converts it to a raw .hex file (Intel HEX format)
```
avr-objcopy -O ihex -R .eeprom main.bin main.hex
```
- `-O ihex`: output format is Intel HEX
- `-R .eeprom`: exclude EEPROM section (handled separately if needed)
- `main.bin`: input file
- `main.hex`: output file

## Flasher: `avrdude`
`avrdude` physically uploads the .hex file onto the microcontroller over a programmer interface (Arduino)
```
avrdude -c arduino -p m328p -P /dev/ttyUSB0 -b 115200 -U flash:w:main.hex:i
```
- `-c arduino`: programmer type is arduino
- `-p m328p`: target chip is ATmega328P
- `-p /dev/ttyUSB0`: serial port is /dev/ttyUSB0
- `-b 115200`: baud rate is 115200
- `-U flash:w:main.hex:i`: memory operation -- `w`rite the file `main.hex` (in Intel HEX format `i`) to `flash` memory

## Useful Commands
- `lsusb` to check if arduino is detected  
MacOS: `brew install usbutils`
- `ls /dev/tty*` to list possible serial ports arduino is on
- `modprobe ch341` to load ch341 module manually