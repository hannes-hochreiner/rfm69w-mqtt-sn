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
#include "spi.h"

void spiInitMaster()
{
  /* PB5 (SCK)
  PB4 (MISO)
  PB3 (MOSI)
  PB2 (SS) */
  /* Set MOSI, SCK, and SS output, all others input */
  DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS);
  /* Enable SPI, Master, set clock rate fck/64 */
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);//|(1<<SPR0);
  // Set slave select to high (inactive)
  PORTR_SPI |= (1<<PORTR_SPI_SS);
}

uint8_t _spiTransfer(uint8_t data)
{
  /* Start transmission */
  SPDR = data;
  /* Wait for transmission complete */
  while(!(SPSR & (1<<SPIF))) {}

  return SPDR;
}

int spiTransfer(unsigned char* const bytes, unsigned int length) {
  PORTR_SPI &= ~(1<<PORTR_SPI_SS);

  for (unsigned int cntr = 0; cntr < length; cntr++) {
    bytes[cntr] = _spiTransfer(bytes[cntr]);
  }

  PORTR_SPI |= (1<<PORTR_SPI_SS);

  return 0;
}
