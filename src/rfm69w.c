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

int setModeChecked(const enum RFM_MODE* const m, int (*spiTransfer)(unsigned char*, unsigned int, void* customData), void* customData) {
  int res = setMode(m, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  enum RFM_FLAG f = RFM_FLAG_UNSET;

  while (f != RFM_FLAG_SET) {
    res = getModeReady(&f, spiTransfer, customData);

    if (res < 0) {
      return res;
    }
  }

  enum RFM_MODE modeNew;

  res = getMode(&modeNew, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  if (modeNew != *m) {
    return -1;
  }

  if (*m == RFM_MODE_TX || *m == RFM_MODE_RX) {
    f = RFM_FLAG_UNSET;

    while (f != RFM_FLAG_SET) {
      if (*m == RFM_MODE_TX) {
        res = getTxReady(&f, spiTransfer, customData);
      } else if (*m == RFM_MODE_RX) {
        res = getRxReady(&f, spiTransfer, customData);
      } else {
        return -1;
      }

      if (res < 0) {
        return res;
      }
    }
  }

  return 0;
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

int setDataModeChecked(const enum RFM_DATAMODE* const dm, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  int res = setDataMode(dm, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  enum RFM_DATAMODE dmNew;

  res = getDataMode(&dmNew, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  if (dmNew != *dm) {
    return -1;
  }

  return 0;
}

int getModulationType(enum RFM_MODULATIONTYPE* const mt, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x02, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  *mt = (enum RFM_MODULATIONTYPE)((data[1] & 0b00011000) >> 3);

  return 0;
}

int setModulationType(enum RFM_MODULATIONTYPE* const mt, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x02, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  data[0] = 0x02 | 1 << 7;
  data[1] = data[1] ^ (((*mt << 3) ^ data[1]) & 0b00011000);

  return (*spiTransfer)(data, 2, customData);
}

int setModulationTypeChecked(enum RFM_MODULATIONTYPE* const mt, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  int res = setModulationType(mt, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  enum RFM_MODULATIONTYPE mtNew;

  res = getModulationType(&mtNew, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  if (mtNew != *mt) {
    return -1;
  }

  return 0;
}

int getModulationShaping(enum RFM_MODULATIONSHAPING* const ms, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x02, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  *ms = (enum RFM_MODULATIONSHAPING)((data[1] & 0b00000011));

  return 0;
}

int setModulationShaping(enum RFM_MODULATIONSHAPING* const ms, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x02, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  data[0] = 0x02 | 1 << 7;
  data[1] = data[1] ^ ((*ms ^ data[1]) & 0b00000011);

  return (*spiTransfer)(data, 2, customData);
}

int setModulationShapingChecked(enum RFM_MODULATIONSHAPING* const ms, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  int res = setModulationShaping(ms, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  enum RFM_MODULATIONSHAPING msNew;

  res = getModulationShaping(&msNew, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  if (msNew != *ms) {
    return -1;
  }

  return 0;
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

int setCarrierFrequencyChecked(const float* const freq, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  enum RFM_MODE modeCurrent;

  int res = getMode(&modeCurrent, spiTransfer, customData);

  if (res < 0) {
    return -1;
  }

  enum RFM_MODE m = RFM_MODE_RX;

  res = setModeChecked(&m, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  res = setCarrierFrequency(freq, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  m = RFM_MODE_FS;

  res = setModeChecked(&m, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  res = setModeChecked(&modeCurrent, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  float freqNew;

  res = getCarrierFrequency(&freqNew, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  if (freqNew != *freq) {
    return -1;
  }

  return 0;
}

// Freq = 32 MHz / 2^19 * Frf(23, 0) 0x05, 0x06; default 0x00 0x52 => 5 kHz
int getCarrierFrequencyDeviation(float* const freq, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x05, 0x00, 0x00};
  int res = (*spiTransfer)(data, 3, customData);

  if (res < 0) {
    return res;
  }

  *freq = 32.0 / 524288.0 * (((unsigned long)data[1] << 8) + (unsigned long)data[2]);

  return 0;
}

int setCarrierFrequencyDeviation(const float* const freq, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[3];
  long steps = (long)(*freq / 32.0 * 524288.0); //16384

  data[0] = 0x05 | 1 << 7;
  data[1] = (steps & 0xFF00) >> 8;
  data[2] = steps & 0xFF;

  return (*spiTransfer)(data, 3, customData);
}

int setCarrierFrequencyDeviationChecked(const float* const freq, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  int res = setCarrierFrequencyDeviation(freq, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  float freqNew;

  res = getCarrierFrequencyDeviation(&freqNew, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  if (freqNew != *freq) {
    return -1;
  }

  return 0;
}

int getBitRate(float* const bitRate, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x03, 0x00, 0x00};
  int res = (*spiTransfer)(data, 3, customData);

  if (res < 0) {
    return res;
  }

  *bitRate = 32000000 / (((unsigned long)data[1] << 8) + (unsigned long)data[2]);

  return 0;
}

int setBitRate(const float* const bitRate, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[3];
  long br = (long)(32000000 / *bitRate);

  data[0] = 0x03 | 1 << 7;
  data[1] = (br & 0xFF00) >> 8;
  data[2] = br & 0xFF;

  return (*spiTransfer)(data, 3, customData);
}

int setBitRateChecked(const float* const bitRate, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  int res = setBitRate(bitRate, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  float bitRateNew;

  res = getBitRate(&bitRateNew, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  if (bitRateNew != *bitRate) {
    return -1;
  }

  return 0;
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

int setPacketFormatChecked(const enum RFM_PACKETFORMAT* const pf, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  int res = setPacketFormat(pf, spiTransfer, customData);

  enum RFM_PACKETFORMAT pfNew;

  res = getPacketFormat(&pfNew, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  if (pfNew != *pf) {
    return -1;
  }

  return 0;
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

int getModeReady(enum RFM_FLAG* const f, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x27, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  *f = (enum RFM_FLAG)(data[1] >> 7 & 0x01);

  return 0;
}

int getSyncAddressMatch(enum RFM_FLAG* const f, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x27, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  *f = (enum RFM_FLAG)(data[1] & 0x01);

  return 0;
}

int getTxReady(enum RFM_FLAG* const f, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x27, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  *f = (enum RFM_FLAG)(data[1] >> 5 & 0x01);

  return 0;
}

int getRxReady(enum RFM_FLAG* const f, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x27, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  *f = (enum RFM_FLAG)(data[1] >> 6 & 0x01);

  return 0;
}

int getPayloadLength(unsigned int* const length, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x38, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  *length = (unsigned int)data[1];

  return 0;
}

int setPayloadLength(const unsigned int* const length, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  if (*length > 255) {
    return -1;
  }

  unsigned char data[] = {0x38 | 1 << 7, (unsigned char)*length};

  return (*spiTransfer)(data, 2, customData);
}

int setPayloadLengthChecked(const unsigned int* const length, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  int res = setPayloadLength(length, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  unsigned int lengthNew;

  res = getPayloadLength(&lengthNew, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  if (lengthNew != *length) {
    return -1;
  }

  return 0;
}

int getPreambleLength(unsigned int* const length, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x2C, 0x00, 0x00};
  int res = (*spiTransfer)(data, 3, customData);

  if (res < 0) {
    return res;
  }

  *length = ((unsigned int)data[1] << 8) + data[2];

  return 0;
}

int setPreambleLength(const unsigned int* const length, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x2C | 1 << 7, (unsigned char)(*length >> 8), (unsigned char)*length};

  return (*spiTransfer)(data, 3, customData);
}

int setPreambleLengthChecked(const unsigned int* const length, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  int res = setPreambleLength(length, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  unsigned int lengthNew;

  res = getPreambleLength(&lengthNew, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  if (lengthNew != *length) {
    return -1;
  }

  return 0;
}

int getOutputPower(unsigned int* const power, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x11, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  *power = (unsigned int)(data[1]&0b00011111);

  return 0;
}

int setOutputPower(const unsigned int* const power, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  if (*power > 31) {
    return -1;
  }

  unsigned char data[] = {0x11, 0x00};
  int res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  data[0] = 0x11 | 1 << 7;
  data[1] = data[1] ^ ((*power ^ data[1]) & 0b00011111);

  return (*spiTransfer)(data, 2, customData);
}

int setOutputPowerChecked(const unsigned int* const power, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  int res = setOutputPower(power, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  unsigned int outputPowerNew;

  res = getOutputPower(&outputPowerNew, spiTransfer, customData);

  if (res < 0) {
    return res;
  }

  if (outputPowerNew != *power) {
    return -1;
  }

  return 0;
}

int getRssi(double* const power, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0xA3, 0x01};

  int res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  data[0] = 0x23;
  data[1] = 0x00;

  while (data[1] & 0x02 != 0) {
    res = (*spiTransfer)(data, 2, customData);

    if (res < 0) {
      return res;
    }
  }

  data[0] = 0x24;
  data[1] = 0x00;
  res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  *power = -data[1] / 2.0;

  return 0;
}

int getFifoNotEmpty(enum RFM_FLAG* const f, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  unsigned char data[] = {0x28, 0x00};

  int res = (*spiTransfer)(data, 2, customData);

  if (res < 0) {
    return res;
  }

  *f = (enum RFM_FLAG)(data[1] >> 6 & 0x01);

  return 0;
}

int setSync(const unsigned char* const data, unsigned int length, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData) {
  if (length > 8) {
    return -1;
  }

  unsigned char readData[] = {0x2E, 0x00};

  int res = (*spiTransfer)(readData, 2, customData);

  if (res < 0) {
    return res;
  }

  unsigned char writeData[length + 2];

  writeData[0] = 0x2E | 1 << 7;
  writeData[1] = readData[1] ^ (((length - 1) ^ readData[1]) & 0b00111000);

  for (int cntr = 0; cntr < length; cntr++) {
    writeData[cntr + 2] = data[cntr];
  }

  return (*spiTransfer)(writeData, length + 2, customData);
}
