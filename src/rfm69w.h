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
#ifndef __rfm69w_h__
#define __rfm69w_h__

enum RFM_MODE {
  RFM_MODE_SLEEP = 0,
  RFM_MODE_STDBY = 1,
  RFM_MODE_FS = 2,
  RFM_MODE_TX = 3,
  RFM_MODE_RX = 4
};

int getMode(enum RFM_MODE* const m, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);
int setMode(const enum RFM_MODE* const m, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);
int setModeChecked(const enum RFM_MODE* const m, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);

enum RFM_DATAMODE {
  RFM_DATAMODE_PACKET = 0,
  RFM_DATAMODE_CONTWITHSYNC = 2,
  RFM_DATAMODE_CONTWITHOUTSYNC = 3
};

int getDataMode(enum RFM_DATAMODE* const dm, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);
int setDataMode(const enum RFM_DATAMODE* const dm, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);

int getCarrierFrequency(float* const freq, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);
int setCarrierFrequency(const float* const freq, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);
int setCarrierFrequencyChecked(const float* const freq, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);

enum RFM_PACKETFORMAT {
  RFM_PACKETFORMAT_FIXED = 0,
  RFM_PACKETFORMAT_VARIABLE = 1
};

int getPacketFormat(enum RFM_PACKETFORMAT* const pf, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);
int setPacketFormat(const enum RFM_PACKETFORMAT* const pf, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);
int setPacketFormatChecked(const enum RFM_PACKETFORMAT* const pf, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);

int getFifoData(unsigned char* const data, unsigned int length, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);
int setFifoData(const unsigned char* const data, unsigned int length, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);

enum RFM_FLAG {
  RFM_FLAG_UNSET = 0,
  RFM_FLAG_SET = 1
};

int getPacketSent(enum RFM_FLAG* const f, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);
int getPayloadReady(enum RFM_FLAG* const f, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);
int getModeReady(enum RFM_FLAG* const f, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);
int getSyncAddressMatch(enum RFM_FLAG* const f, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);
int getTxReady(enum RFM_FLAG* const f, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);

int getPayloadLength(unsigned int* const length, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);
int setPayloadLength(const unsigned int* const length, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);
int setPayloadLengthChecked(const unsigned int* const length, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);
int getPreambleLength(unsigned int* const length, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);
int setPreambleLength(const unsigned int* const length, int (*spiTransfer)(unsigned char* const, unsigned int, void* const customData), void* const customData);
#endif
