all: software hardware

software:
	gcc example_spi_bridge.c -lstickbridge -lftdi -o ./example_spi_bridge

hardware:
	mkdir ./build

	echo ------- yosys
	yosys -p 'synth_ice40 -top top -blif ./build/example.blif' example.v | grep -iE 'Warning|Error' || exit

	echo ------- arachne-pnr
	# arachne-pnr -d 8k -P ct256 -o ./build/example.asc -p example.pcf ./build/example.blif 2> /dev/null || exit
	arachne-pnr -d 1k -o ./build/example.asc -p example.pcf ./build/example.blif 2> /dev/null || exit

	echo ------- icepack
	icepack ./build/example.asc ./build/example.bin

	# echo ------- icetime
	# icetime ./build/example.asc -d hx8k

upload: hardware
	echo ------- iceprog
	iceprog build/example.bin

clean:
	rm -f example_spi_bridge
	rm -rf ./build
