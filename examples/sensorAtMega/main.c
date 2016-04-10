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
#include <stdbool.h>
#include <util/delay.h>
#include <avr/io.h>
#include "signal.h"
#include "spi.h"
#include "../../src/rfm69w.h"

// void setup();
// void waitForModeReady();
// void waitForTxReady();
// void checkMode(const RFM_MODE& m);

int transfer(unsigned char* const bytes, unsigned int length, void* const customData) {
  return spiTransfer(bytes, length);
}

int main() {
  // Safety delay.
  _delay_ms(1000);

  signalInit();

  signalError(false, &_delay_ms);
  signalOk(false, &_delay_ms);

  spiInitMaster();

  enum RFM_PACKETFORMAT pf = RFM_PACKETFORMAT_FIXED;

  if (setPacketFormatChecked(&pf, transfer, 0) < 0) {
    signalError(true, &_delay_ms);
  }

  unsigned int payloadLength = 30;

  if (setPayloadLengthChecked(&payloadLength, transfer, 0) < 0) {
    signalError(true, &_delay_ms);
  }

  float cf = 868;

  if (setCarrierFrequencyChecked(&cf, transfer, 0) < 0) {
    signalError(true, &_delay_ms);
  }

  while (1) {
    enum RFM_MODE modeTx = RFM_MODE_TX;

    if (setModeChecked(&modeTx, transfer, 0) < 0) {
      signalError(true, &_delay_ms);
    }

    unsigned char data[] = "012345678901234567890123456789";

    if (setFifoData(data, 30, transfer, 0) < 0) {
      signalError(true, &_delay_ms);
    }

    enum RFM_FLAG f = RFM_FLAG_UNSET;

    while (f != RFM_FLAG_SET) {
      if (getPacketSent(&f, transfer, 0) < 0) {
        signalError(true, &_delay_ms);
      }
    }

    signalOk(false, &_delay_ms);

    enum RFM_MODE modeStdby = RFM_MODE_STDBY;

    if (setModeChecked(&modeStdby, transfer, 0) < 0) {
      signalError(true, &_delay_ms);
    }

    _delay_ms(1000);
  }
}
