# FreeRTOS Ported to Raspberry Pi

This provides a very basic port of FreeRTOS to Raspberry Pi. This is based on James Walmsleys port 
and is updated to FreeRTOS 10.4.4. The good stuff can be accredited to him and any mistakes are mine.

This has been somewhat tested with Raspberry Pi Zero W and is not for any of the newer ones out there.

## Howto Build

Type make! -- If you get an error then:

    $ cd .dbuild/pretty
    $ chmod +x *.py

Currently the makefile expect an arm-none-eabi- toolchain in the path. Either export the path to yours or
modify the TOOLCHAIN variable in dbuild.config.mk file.

You may also need to modify the library locations in the Makefile:

    kernel.elf: LDFLAGS += -L"c:/yagarto/lib/gcc/arm-none-eabi/4.7.1/" -lgcc
    kernel.elf: LDFLAGS += -L"c:/yagarto/arm-none-eabi/lib/" -lc

The build system also expects find your python interpreter by using /usr/bin/env python,
if this doesn't work you will get problems.

To resolve this, modify the #! lines in the .dbuild/pretty/*.py files.

Hope this helps.
