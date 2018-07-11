/*
 *  example spi bridge for the ICEStick FPGA board using libstickbridge
 *
 *  Copyright (C) 2015  Clifford Wolf <clifford@clifford.at>
 *  Copyright (C) 2018  julian1 <https://github.com/julian1/ice40-spi-usb-controller>
 *  Copyright (C) 2018  Felipe Correa da Silva Sanches <juca@members.fsf.org>
 *
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

//#define _GNU_SOURCE

#include <stickbridge.h>
#include <stdio.h>
#include <string.h>

void show_usage(char* argv0){
	fprintf(stderr, "%s (on|off)\n", argv0);
}

int main(int argc, char **argv)
{
	int led_value;
	uint8_t data[1]; 

	if (argc != 2){
		show_usage(argv[0]);
		return -1;
	}

	if (strcmp(argv[1], "on") == 0){
		led_value = 1;
	} else if (strcmp(argv[1], "off") == 0){
		led_value = 0;
        } else {
		show_usage(argv[0]);
		return -1;
	}

	fprintf(stderr, "writing data led_value %d\n", led_value);

	stickbridge_init();

	data[0] = led_value ? 0xcc : 0xdd;
	send_spi(data, 1);

	stickbridge_done();
	return 0;
}
