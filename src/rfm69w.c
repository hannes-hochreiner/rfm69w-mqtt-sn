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
#include "rfm69w.h"

int getMode(enum RFM_MODE* const m, int (*spiTransfer)(unsigned char*, unsigned int, void* customData), void* customData) {
  unsigned char data[] = {0x01, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  *m = (enum RFM_MODE)((data[1] & 0b00011100) >> 2);

  return 0;
}

int setMode(const enum RFM_MODE* const m, int (*spiTransfer)(unsigned char*, unsigned int, void* customData), void* customData) {
  unsigned char data[] = {0x01, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  data[0] = 0x01 | 1 << 7;
  data[1] = data[1] ^ (((*m << 2) ^ data[1]) & 0b00011100);

  return (*spiTransfer)(data, 2, customData);
}

int getDataMode(enum RFM_DATAMODE* const dm, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x02, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  *dm = (enum RFM_DATAMODE)((data[1] & 0b01100000) >> 5);

  return 0;
}

int setDataMode(const enum RFM_DATAMODE* const dm, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x02, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  data[0] = 0x02 | 1 << 7;
  data[1] = data[1] ^ (((*dm << 5) ^ data[1]) & 0b01100000);

  return (*spiTransfer)(data, 2, customData);
}

// Freq = 32 MHz / 2^19 * Frf(23, 0) 0x07, 0x08, 0x09; default 0xE4 0xC0 0x00 => 915 MHz
// Important: set LSB (0x09) last!
int getCarrierFrequency(float* const freq, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x07, 0x00, 0x00, 0x00};
  int res = (*spiTransfer)(data, 4, customData);

  if (res < 0) {
    return res;
  }

  *freq = 32.0 / 524288.0 * (((unsigned long)data[1] << 16) + ((unsigned long)data[2] << 8) + (unsigned long)data[3]);

  return 0;
}

int setCarrierFrequency(const float* const freq, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[4];
  long steps = (long)(*freq / 32.0 * 524288.0);

  data[0] = 0x07 | 1 << 7;
  data[1] = (steps & 0xFF0000) >> 16;
  data[2] = (steps & 0xFF00) >> 8;
  data[3] = steps & 0xFF;

  return (*spiTransfer)(data, 4, customData);
}

int getPacketFormat(enum RFM_PACKETFORMAT* const pf, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x37, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  *pf = (enum RFM_PACKETFORMAT)(data[1] >> 7);

  return 0;
}

int setPacketFormat(const enum RFM_PACKETFORMAT* const pf, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x37, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  data[0] = 0x37 | 1 << 7;
  data[1] = data[1] ^ (((*pf << 7) ^ data[1]) & 0b10000000);

  return (*spiTransfer)(data, 2, customData);
}

int getFifoData(unsigned char* const data, unsigned int length, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned int spiLength = length + 1;
  unsigned char spiData[spiLength];

  for (int cntr = 0; cntr < spiLength; cntr++) {
    spiData[cntr] = 0x00;
  }

  int res = (*spiTransfer)(spiData, spiLength, customData);

  if (res < 0) {
    return res;
  }

  for (unsigned int cntr = 1; cntr < spiLength; cntr++) {
    data[cntr - 1] = spiData[cntr];
  }

  return 0;
}

int setFifoData(const unsigned char* const data, unsigned int length, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned int spiLength = length + 1;
  unsigned char spiData[spiLength];

  spiData[0] = 0x80;

  for (unsigned int cntr = 0; cntr < length; cntr++) {
    spiData[cntr + 1] = data[cntr];
  }

  return (*spiTransfer)(spiData, spiLength, customData);
}

int getPacketSent(enum RFM_FLAG* const f, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x28, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  *f = (enum RFM_FLAG)(data[1] >> 3 & 0x01);

  return 0;
}

int getPayloadReady(enum RFM_FLAG* const f, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x28, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  *f = (enum RFM_FLAG)(data[1] >> 2 & 0x01);

  return 0;
}
