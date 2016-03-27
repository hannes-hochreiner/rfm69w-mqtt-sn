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
#include <util/delay.h>
#include <stdio.h>

#include "spi.h"
#include "../../src/rfm69w.h"

int transfer(unsigned char* const bytes, unsigned int length, void* const customData) {
  // cable select low => active
  PORTA &= ~(1<<PORTA2);

  unsigned int cntr;

  for (cntr = 0; cntr < length; cntr++) {
    bytes[cntr] = spiTransfer(bytes[cntr]);
  }

  // cable select high => inactive
  PORTA |= (1<<PORTA2);
}

int main()
{
  // Safety delay.
  _delay_ms(1000);

  // Enable clock pre-scaler change.
  CLKPR = (1<<CLKPCE);
  // CLKPS3:0 Set pre-scaler value; 0001 = 2; 0101 = 32; 0110 = 64; 1000 = 256
  // Max frequency for 1.8 V = 4 MHz => using the internal oscillator @ 8 MHz => min pre-scaler factor = 2.
  CLKPR = (1<<CLKPS2)|(1<<CLKPS0);

  DDRA |= (1<<DDA2); // cable select.
  PORTA |= (1<<PORTA2);

  spiInitMaster();

  enum RFM_MODE ms = RFM_MODE_SLEEP;
  setMode(&ms, transfer, NULL);

  float cf = 868;
  setCarrierFrequency(&cf, transfer, NULL);

  enum RFM_PACKETFORMAT pf = RFM_PACKETFORMAT_VARIABLE;
  setPacketFormat(&pf, transfer, NULL);

  while(1)
  {
    _delay_ms(1000);

    unsigned char data[3] = {0x02, 0xAA, 0x5E};

    setFifoData(data, 3, transfer, NULL);

    enum RFM_MODE mr = RFM_MODE_TX;
    setMode(&mr, transfer, NULL);

  }

  return 0;
}
