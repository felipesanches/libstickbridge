## libstickbridge

This is a C library to establish an SPI channel with hardware instantiated in an iCE40 ICEStick FPGA over an USB bridge via the FTDI FT2232H chip.

Original code written by julian1 <https://github.com/julian1/ice40-spi-usb-controller> based on Clifford Wolf's iceprog program from <https://github.com/cliffordwolf/icestorm.git>.

The code was subsequently refactored into this static library (Lib-Stick-Bridge) by Felipe Correa da Silva Sanches <juca@members.fsf.org>.

Happy Hacking!

### Using Icestick fpga USB channel B for SPI communication

http://blog.julian1.io/2017/01/29/icestick-ftdi-spi.html

----

### refs 
basics,
  http://www.ftdichip.com/Support/Documents/AppNotes/AN_135_MPSSE_Basics.pdf

op codes,
  http://www.ftdichip.com/Support/Documents/AppNotes/AN_108_Command_Processor_for_MPSSE_and_MCU_Host_Bus_Emulation_Modes.pdf

spi example,
  http://www.ftdichip.com/Support/Documents/AppNotes/AN_114_FTDI_Hi_Speed_USB_To_SPI_Example.pdf

data sheet,
  http://www.ftdichip.com/Support/Documents/DataSheets/ICs/DS_FT2232H.pdf

api
  https://www.intra2net.com/en/developer/libftdi/documentation/group__libftdi.html

iceprog
  https://github.com/cliffordwolf/icestorm/blob/master/iceprog/iceprog.c

