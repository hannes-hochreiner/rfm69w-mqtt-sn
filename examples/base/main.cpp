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
#include <iostream>
#include <exception>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include "baseException.h"
#include "../../src/rfm69w.h"

void setup();

int spiTransfer(unsigned char* const bytes, unsigned int length, void* const customData) {
  return wiringPiSPIDataRW(0, bytes, length);
}

void dataReceived() {
  std::cout << "data recevied\n";

  RFM_MODE ms = RFM_MODE_SLEEP;

  if (setMode(&ms, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting mode");
  }

  std::cout << "mode set to sleep\n";

  std::cout << "processing data\n";

  RFM_MODE mr = RFM_MODE_RX;

  if (setMode(&mr, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting mode");
  }

  std::cout << "mode set to receive\n";
}

int main() {
  try {
    setup();

    while (1) {
      wiringPiISR(13, INT_EDGE_RISING,  dataReceived);
    }

    return 0;
  } catch (std::exception e) {
    std::cerr << e.what() << "\n";

    return -1;
  }
}

void setup() {
  if (wiringPiSetupPhys() < 0) {
    throw new BaseException("error setting up GPIO interface");
  }

  if (wiringPiSPISetup(0, 500000) < 0) {
    throw new BaseException("error setting up SPI interface");
  }

  RFM_MODE ms = RFM_MODE_SLEEP;

  if (setMode(&ms, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting mode");
  }

  std::cout << "mode set to sleep\n";

  float cf = 868;

  if (setCarrierFrequency(&cf, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting carrier frequency");
  }

  std::cout << "carrier frequency set to " << cf << " MHz\n";

  RFM_PACKETFORMAT pf = RFM_PACKETFORMAT_VARIABLE;

  if (setPacketFormat(&pf, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting packet format");
  }

  std::cout << "packet format set to variable\n";

  RFM_MODE mr = RFM_MODE_RX;

  if (setMode(&mr, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting mode");
  }

  std::cout << "mode set to receive\n";
}
