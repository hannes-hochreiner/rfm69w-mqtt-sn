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
void waitForModeReady();
void waitForTxReady();
void checkMode(const RFM_MODE& m);

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

    RFM_FLAG f = RFM_FLAG_UNSET;

    while (1) {
      getPayloadReady(&f, spiTransfer, NULL);

      if (f == RFM_FLAG_SET) {
        std::cout << "payload ready\n";
      }
      // getPayloadReady(&f, spiTransfer, NULL);
      // wiringPiISR(13, INT_EDGE_RISING,  dataReceived);
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

  RFM_PACKETFORMAT pf = RFM_PACKETFORMAT_FIXED;

  if (setPacketFormat(&pf, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting packet format");
  }

  std::cout << "packet format set to fixed\n";

  unsigned int payloadLength = 30;

  if (setPayloadLength(&payloadLength, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting payload length");
  }

  std::cout << "payload length set to " << payloadLength << "\n";

  RFM_MODE mr = RFM_MODE_RX;
  RFM_MODE mt = RFM_MODE_TX;

  if (setMode(&mr, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting mode");
  }

  std::cout << "mode set to receive...";

  waitForModeReady();

  std::cout << "ready...";

  checkMode(RFM_MODE_RX);

  std::cout << "check passed\n";

  float cf = 868;

  if (setCarrierFrequency(&cf, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting carrier frequency");
  }

  std::cout << "carrier frequency set to " << cf << " MHz\n";

  if (setMode(&mt, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting mode");
  }

  std::cout << "mode set to transmission...";

  waitForModeReady();

  std::cout << "ready...";

  checkMode(RFM_MODE_TX);

  std::cout << "check passed\n";

  waitForTxReady();

  std::cout << "TX ready\n";

  // unsigned char data[] = {0x14, 0xA5, 0x5A, 0x1A, 0x1A};
  unsigned char data[] = "012345678901234567890123456789012345";

  if (setFifoData(data, 30, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting fifo data");
  }

  std::cout << "data written to fifo\n";

  RFM_FLAG ps = RFM_FLAG_UNSET;

  while (ps != RFM_FLAG_SET) {
    if (getPacketSent(&ps, spiTransfer, NULL) < 0) {
      throw new BaseException("error checking packet send");
    }
  }

  std::cout << "packet sent\n";
}

void waitForTxReady() {
  RFM_FLAG f = RFM_FLAG_UNSET;

  while(f != RFM_FLAG_SET) {
    if (getTxReady(&f, spiTransfer, NULL) < 0) {
      throw new BaseException("error getting mode ready");
    }
  }

  return;
}


void waitForModeReady() {
  RFM_FLAG f = RFM_FLAG_UNSET;

  while(f != RFM_FLAG_SET) {
    if (getModeReady(&f, spiTransfer, NULL) < 0) {
      throw new BaseException("error getting mode ready");
    }
  }

  return;
}

void checkMode(const RFM_MODE& m) {
  RFM_MODE mc;

  if (getMode(&mc, spiTransfer, NULL) <0) {
    throw new BaseException("error getting mode");
  }

  if (mc != m) {
    throw new BaseException("mode check failed");
  }

  return;
}
