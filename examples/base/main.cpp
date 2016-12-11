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
#include <unistd.h>
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
  // std::cout << "data recevied\n";

  RFM_MODE ms = RFM_MODE_SLEEP;

  if (setModeChecked(&ms, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting mode");
  }

  // std::cout << "mode set to sleep\n";

  // double rssi;
  //
  // if (getRssi(&rssi, spiTransfer, NULL) < 0) {
  //   throw new BaseException("error getting rssi");
  // }
  //
  // std::cout << "RSSI: " << rssi << " dBm\n";

  std::string out = "";

  RFM_FLAG notEmptyFlag = RFM_FLAG_SET;

  while (notEmptyFlag == RFM_FLAG_SET) {
    unsigned char buf;

    if (getFifoData(&buf, 1, spiTransfer, NULL) < 0) {
      throw new BaseException("error getting data");
    }

    out += buf;

    if (getFifoNotEmpty(&notEmptyFlag, spiTransfer, NULL) < 0) {
      throw new BaseException("error getting not empty flag");
    }
  }

  std::cout << "data: " << std::hex << out << "\n";

  RFM_MODE mr = RFM_MODE_RX;

  if (setModeChecked(&mr, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting mode");
  }

  // std::cout << "mode set to receive\n";
}

int main() {
  try {
    setup();

    RFM_FLAG f = RFM_FLAG_UNSET;

    // wiringPiISR(13, INT_EDGE_RISING,  dataReceived);

    while (1) {
      // double rssi;
      //
      // if (getRssi(&rssi, spiTransfer, NULL) < 0) {
      //   throw new BaseException("error reading rssi");
      // }
      //
      // std::cout << rssi << "\n";
      usleep(100000);
      getPayloadReady(&f, spiTransfer, NULL);
      // getSyncAddressMatch(&f, spiTransfer, NULL);

      if (f == RFM_FLAG_SET) {
        // std::cout << "sync match\n";
        // std::cout << "payload ready\n";
        dataReceived();
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

  RFM_DATAMODE dm = RFM_DATAMODE_PACKET;

  if (setDataModeChecked(&dm, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting data mode");
  }

  std::cout << "data mode set to packet\n";

  RFM_MODULATIONTYPE mt = RFM_MODULATIONTYPE_FSK;

  if (setModulationTypeChecked(&mt, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting modulation type");
  }

  std::cout << "modulation type set to FSK\n";

  RFM_MODULATIONSHAPING ms = RFM_MODULATIONSHAPING_FSK_GAUSSBT03;

  if (setModulationShapingChecked(&ms, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting modulation shaping");
  }

  std::cout << "set modulation shaping to gaussian bt 1.0\n";

  RFM_PACKETFORMAT pf = RFM_PACKETFORMAT_FIXED;

  if (setPacketFormatChecked(&pf, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting packet format");
  }

  std::cout << "packet format set to fixed\n";

  unsigned int payloadLength = 18;

  if (setPayloadLengthChecked(&payloadLength, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting payload length");
  }

  std::cout << "payload length set to " << payloadLength << "\n";

  float cfd = 0.0625;

  if (setCarrierFrequencyDeviationChecked(&cfd, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting carrier frequency deviation");
  }

  std::cout << "carrier frequency deviation set to " << cfd << " MHz\n";

  float cf = 868;

  if (setCarrierFrequencyChecked(&cf, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting carrier frequency");
  }

  std::cout << "carrier frequency set to " << cf << " MHz\n";

  unsigned char bw[] = {0x99, 0b01001001};

  if (spiTransfer(bw, 2, NULL) < 0) {
    throw new BaseException("error setting bandwidth");
  }

  std::cout << "bandwidth set to 200 kHz\n";

  unsigned char test[] = {0xB7, 0b0001100};

  if (spiTransfer(test, 2, NULL) < 0) {
    throw new BaseException("error setting test");
  }

  std::cout << "test set\n";

  unsigned char rssiThresh[] = {0xA9, 0xB4};

  if (spiTransfer(rssiThresh, 2, NULL) < 0) {
    throw new BaseException("error setting RSSI threshold");
  }

  std::cout << "RSSI threshold set\n";

  unsigned char sync[] = {0xA5, 0x7B};

  if (setSync(sync, 2, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting sync");
  }

  std::cout << "sync set\n";

  float bitRate = 25000;

  if (setBitRateChecked(&bitRate, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting bit rate");
  }

  std::cout << "bit rate set to " << bitRate << "\n";

  RFM_MODE mr = RFM_MODE_RX;

  if (setModeChecked(&mr, spiTransfer, NULL) < 0) {
    throw new BaseException("error setting mode");
  }

  std::cout << "mode set to receive\n";
}
