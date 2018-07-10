/*
 *  libstickbridge
 *  A library to communicate with the ICEStick FPGA board
 *  using SPI protocol over a USB bridge.
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

#define _GNU_SOURCE

#include <ftdi.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

static struct ftdi_context ftdic;
static bool ftdic_open = false;

void check_rx()
{
	while (1) {
		uint8_t data;
		int rc = ftdi_read_data(&ftdic, &data, 1);
		if (rc <= 0) break;
		fprintf(stderr, "unexpected rx byte: %02X\n", data);
	}
}


static void stickbridge_error()
{
	check_rx();
	fprintf(stderr, "ABORT.\n");
	if (ftdic_open)
		ftdi_usb_close(&ftdic);
	ftdi_deinit(&ftdic);
	exit(1);
}


uint8_t recv_byte()
{
	uint8_t data;
	while (1) {
		int rc = ftdi_read_data(&ftdic, &data, 1);
		if (rc < 0) {
			fprintf(stderr, "Read error.\n");
			stickbridge_error();
		}
		if (rc == 1)
			break;
		usleep(100);
	}
	return data;
}


void send_byte(uint8_t data)
{
	int rc = ftdi_write_data(&ftdic, &data, 1);
	if (rc != 1) {
		fprintf(stderr, "Write error (single byte, rc=%d, expected %d).\n", rc, 1);
		stickbridge_error();
	}
}


void send_spi(uint8_t *data, int n)
{
	if (n < 1)
		return;

	// max size check...
	// 3.3.2 Clock Data Bytes Out on - ve clo ck edge MSB first  (no read) Use if CLK starts at '0

	send_byte(0x11);
	send_byte(n-1);
	send_byte((n-1) >> 8);

	int rc = ftdi_write_data(&ftdic, data, n);
	if (rc != n) {
		fprintf(stderr, "Write error (chunk, rc=%d, expected %d).\n", rc, n);
		stickbridge_error();
	}
}


void xfer_spi(uint8_t *data, int n)
{
	if (n < 1)
		return;

	send_byte(0x31);
	send_byte(n-1);
	send_byte((n-1) >> 8);

	int rc = ftdi_write_data(&ftdic, data, n);
	if (rc != n) {
		fprintf(stderr, "Write error (chunk, rc=%d, expected %d).\n", rc, n);
		stickbridge_error();
	}

	for (int i = 0; i < n; i++)
		data[i] = recv_byte();
}


//This is a very bad library function signature! --fsanches
void set_gpio(int a, int b, int c)
{
	// sets two bytes - intial led_values and i/o direction

	// what does the one indicate?
	// one is the clock ... maybe set high by default?
	uint8_t gpio = 1;

	if (a) {
		// (GPIOL0)
		gpio |= 0x10;    // 1 << 4
	}
	if (b) {
		// (GPIOL1)
		gpio |= 0x20;    // 1 << 5
	}
	if (c) {
		// (GPIOL2)
		gpio |= 0x40;    // 1 << 6
	}
	// GPIOL3 not connected to fpga.

	/*
	At this point, the MPSSE is ready for commands .  Each command consists of an
	op - code followed by any necessary parameters or data.

	The following commands are used to set the initial direction and logic state of
	the pins when first entering MPSSE mode. They are also use to set or re a d
	GPIO pins.

	0x80 is a op-code to set initial led_values and direction.
	*/

	send_byte(0x80);       // must be a command,,,
	send_byte(gpio);

	// 0xfb.toString(2);
	// 0xFB = "11111011"  should be correct enables miso as only input
	send_byte(0xFB);        // this is the direction a bit mask?
}

int get_cdone()
{
	uint8_t data;
	send_byte(0x81);
	data = recv_byte();
	// ADBUS6 (GPIOL2)
	return (data & 0x40) != 0;
}

void stickbridge_init()
{
	const char *devstr = NULL;
	enum ftdi_interface ifnum = INTERFACE_B;

	// ---------------------------------------------------------
	// iNITIalize USB connection to FT2232H
	// ---------------------------------------------------------

	ftdi_init(&ftdic);
	ftdi_set_interface(&ftdic, ifnum);

	if (devstr != NULL) {
		if (ftdi_usb_open_string(&ftdic, devstr)) {
			fprintf(stderr, "Can't find iCE FTDI USB device (device string %s).\n", devstr);
			stickbridge_error();
		}
	} else {
		if (ftdi_usb_open(&ftdic, 0x0403, 0x6010)) {
			fprintf(stderr, "Can't find iCE FTDI USB device (vedor_id 0x0403, device_id 0x6010).\n");
			stickbridge_error();
		}
	}

	ftdic_open = true;

	if (ftdi_usb_reset(&ftdic)) {
		fprintf(stderr, "Failed to reset iCE FTDI USB device.\n");
		stickbridge_error();
	}

	if (ftdi_usb_purge_buffers(&ftdic)) {
		fprintf(stderr, "Failed to purge buffers on iCE FTDI USB device.\n");
		stickbridge_error();
	}

	if (ftdi_set_bitmode(&ftdic, 0xff, BITMODE_MPSSE) < 0) {
		fprintf(stderr, "Failed set BITMODE_MPSSE on iCE FTDI USB device.\n");
		stickbridge_error();
	}

	// enable clock divide by 5
	send_byte(0x8b);

	// set 6 MHz clock
	send_byte(0x86);
	send_byte(0x02);   // 1 or 2 MHz ?
	send_byte(0x00);

	get_cdone();	//fprintf(stderr, "cdone: %s\n", get_cdone() ? "high" : "low");

	// initialize data out
	set_gpio(1, 1, 1);

	get_cdone();	//fprintf(stderr, "cdone: %s\n", get_cdone() ? "high" : "low");
}

void stickbridge_done(){
	ftdi_disable_bitbang(&ftdic);
	ftdi_usb_close(&ftdic);
	ftdi_deinit(&ftdic);
}
