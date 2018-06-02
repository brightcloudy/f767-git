# f767-git

## things that need to be installed (i think)

### system packages
  - arm-none-eabi-gcc-cs
  - arm-none-eabi-gcc-cs-c++
  - arm-none-eabi-binutils-cs
  - arm-none-eabi-gdb
  - arm-none-eabi-newlib
  - mercurial
  - stlink
  - openocd

### python packages
  - mbed-cli

## setup
  - `mbed add $(cat mbed-os.lib)`
  - `pip install -r mbed-os/requirements.txt`
  - `mbed deploy`
  _hopefully at this point everything is ready_

## build
  - `mbed compile`
  The output will give you a breakdown of the compiled image:
  ```
  +-----------------------+--------+-------+-------+
  | Module                |  .text | .data |  .bss |
  +-----------------------+--------+-------+-------+
  | USBHOST/USBHost       |     75 |     0 |     0 |
  | USBHOST/USBHostSerial |     50 |     0 |     0 |
  | [fill]                |    296 |    11 |    46 |
  | [lib]/c.a             |  50485 |  2476 |    56 |
  | [lib]/gcc.a           |    768 |     0 |     0 |
  | [lib]/misc            |    268 |    12 |    28 |
  | main.o                |    910 |     4 |   700 |
  | mbed-os/drivers       |   2150 |     4 |   100 |
  | mbed-os/features      |  46053 |   164 | 46972 |
  | mbed-os/hal           |   1601 |     4 |    68 |
  | mbed-os/platform      |   4095 |   268 |   145 |
  | mbed-os/rtos          |  14537 |   168 |  6073 |
  | mbed-os/targets       |  15263 |     5 |  1660 |
  | Subtotals             | 136551 |  3116 | 55848 |
  +-----------------------+--------+-------+-------+
  Total Static RAM memory (data + bss): 58964 bytes
  Total Flash memory (text + data): 139667 bytes

  Image: ./BUILD/NUCLEO_F767ZI/GCC_ARM/f767-git.bin
  ```

  - The compiled images are in ./BUILD/NUCLEO_F767ZI/GCC_ARM unless you changed targets or toolchains.

## flash
  - Multiple options:
    * `st-flash write <file.bin> 0x8000000`
    * Mount MBED block device and copy .bin to root of drive
    * Load in gdb
## debug
  openocd or st-util work equally well, but there's some small differences i think
  ### openocd
    - Using the board script for the STM32F7 Discovery board seems to work fine.
    - `openocd -f /usr/share/openocd/scripts/board/stm32f7discovery.cfg -c init -c "reset init"`
    - GDB is at localhost:3333
    - Monitor commands are at localhost:4444
  ### st-util
    - `st-util`
    - GDB is at localhost:4242
    - `--semihosting` TODO: ???
  ### GDB
    - `arm-none-eabi-gdb`
    - Connect to the target with `target remote localhost:3333`
    - Set file to the **.elf** binary: `file ./BUILD/NUCLEO_F767ZI/GCC_ARM/f767-git.elf`
    - Load the program into the MCU: `load`
      * I don't believe this writes it to flash
