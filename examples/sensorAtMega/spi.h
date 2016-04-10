// The MIT License (MIT)
//
// Copyright (c) 2016 Hannes Hochreiner
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#ifndef __SPI__
#define __SPI__

#ifndef DDR_SPI
#error "Data direction register for SPI (DDR_SPI) needs to be defined."
#endif

#ifndef DD_MOSI
#error "Data direction bit for SPI MOSI (DD_MOSI) needs to be defined."
#endif

#ifndef DD_SCK
#error "Data direction bit for SPI clock (DD_SCK) needs to be defined."
#endif

#ifndef DD_SS
#error "Data direction bit for SPI slave select (DD_SS) needs to be defined."
#endif

#ifndef PORTR_SPI
#error "Port register for SPI (PORTR_SPI) needs to be defined."
#endif

#ifndef PORTR_SPI_SS
#error "Port register bit for SPI slave select pin (PORTR_SPI_SS) needs to be defined."
#endif

#include <avr/io.h>

int spiTransfer(unsigned char* const bytes, unsigned int length);
// uint8_t spiTransfer(uint8_t data);
void spiInitMaster();
#endif
