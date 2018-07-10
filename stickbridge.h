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

#include <stdint.h>

void check_rx();
uint8_t recv_byte();
void send_byte(uint8_t data);
void send_spi(uint8_t *data, int n);
void xfer_spi(uint8_t *data, int n);
void set_gpio(int a, int b, int c);
int get_cdone();
void stickbridge_init();
void stickbridge_done();
