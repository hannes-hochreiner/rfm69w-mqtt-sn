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

int getMode(RFM_MODE* const m, int (*spiTransfer)(unsigned char*, unsigned int, void* customData), void* customData) {
  unsigned char data[] = {0x01, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res != 0) {
    return res;
  }

  *m = (RFM_MODE)((data[1] & 0b00011100) >> 2);

  return 0;
}

int setMode(const RFM_MODE* const m, int (*spiTransfer)(unsigned char*, unsigned int, void* customData), void* customData) {
  unsigned char data[] = {0x01, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res != 0) {
    return res;
  }

  data[0] = 0x01 | 1 << 7;
  data[1] = data[1] ^ (((*m << 2) ^ data[1]) & 0b00011100);

  return (*spiTransfer)(data, 2, customData);
}

int getDataMode(RFM_DATAMODE* const dm, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x02, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res != 0) {
    return res;
  }

  *dm = (RFM_DATAMODE)((data[1] & 0b01100000) >> 5);

  return 0;
}

int setDataMode(const RFM_DATAMODE* const dm, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x02, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res != 0) {
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

  if (res != 0) {
    return res;
  }

  *freq = 32.0 / 524288.0 * ((float)(data[1] << 16) + (float)(data[2] << 8) + (float)data[3]);

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
