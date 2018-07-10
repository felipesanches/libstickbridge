HW_DESIGN_NAME=spi-slave
SW_CLIENT_NAME=spi_bridge

BLIF=./build/$(HW_DESIGN_NAME).blif
ASCII=./build/$(HW_DESIGN_NAME).txt
BITSTREAM=./build/$(HW_DESIGN_NAME).bin
VERILOG=$(HW_DESIGN_NAME).v
PCF=$(HW_DESIGN_NAME).pcf

PNR_OPTIONS=-d 1k
# PNR_OPTIONS=-d 8k -P ct256

all: software hardware

software:
	gcc $(SW_CLIENT_NAME).c -lstickbridge -lftdi -o $(SW_CLIENT_NAME)

hardware:
	mkdir -p ./build
	yosys -p 'synth_ice40 -top top -blif $(BLIF)' $(VERILOG) | grep -iE 'Warning|Error' || exit
	arachne-pnr   $(PNR_OPTIONS) -o $(ASCII) -p $(PCF) $(BLIF) 2> /dev/null || exit
	icepack $(ASCII) $(BITSTREAM)
	# icetime $(ASCII) -d hx8k

upload:
	iceprog $(BITSTREAM)

clean:
	rm -f $(SW_CLIENT_NAME)
	rm -rf ./build
