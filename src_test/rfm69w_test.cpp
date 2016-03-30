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
#include <queue>
#include <tuple>
#include "gtest/gtest.h"
#include "../src/rfm69w.h"

int fakeSpiFun(unsigned char* bytes, unsigned int length, void* customData) {
  std::queue<std::tuple<unsigned char, unsigned char>>* cust = static_cast<std::queue<std::tuple<unsigned char, unsigned char>>*>(customData);

  for (int cntr = 0; cntr < length; cntr++) {
    std::tuple<unsigned char, unsigned char> curr = cust->front();

    cust->pop();

    EXPECT_EQ(std::get<0>(curr), bytes[cntr]);
    if (std::get<0>(curr) != bytes[cntr]) {
      return -1;
    }

    bytes[cntr] = std::get<1>(curr);
  }

  return 0;
}

TEST(RFM69W, getMode) {
  RFM_MODE m;
  std::queue<int> responses;
  std::queue<std::tuple<unsigned char, unsigned char>> resps;

  resps.push(std::make_tuple(0x01, 0x00));
  resps.push(std::make_tuple(0x00, 0b00000100));

  EXPECT_EQ(0, getMode(&m, fakeSpiFun, &resps));
  EXPECT_EQ(RFM_MODE_STDBY, m);
}

TEST(RFM69W, setMode) {
  RFM_MODE m = RFM_MODE_RX;
  std::queue<int> responses;
  std::queue<std::tuple<unsigned char, unsigned char>> resps;

  resps.push(std::make_tuple(0x01, 0x00));
  resps.push(std::make_tuple(0x00, 0b00000100));
  resps.push(std::make_tuple(0x81, 0x00));
  resps.push(std::make_tuple(0b00010000, 0x00));

  EXPECT_EQ(0, setMode(&m, fakeSpiFun, &resps));
}

TEST(RFM69W, getDataMode) {
  RFM_DATAMODE dm;
  std::queue<int> responses;
  std::queue<std::tuple<unsigned char, unsigned char>> resps;

  resps.push(std::make_tuple(0x02, 0x00));
  resps.push(std::make_tuple(0x00, 0b01000000));

  EXPECT_EQ(0, getDataMode(&dm, fakeSpiFun, &resps));
  EXPECT_EQ(RFM_DATAMODE_CONTWITHSYNC, dm);
}

TEST(RFM69W, setDataMode) {
  RFM_DATAMODE dm = RFM_DATAMODE_CONTWITHOUTSYNC;
  std::queue<int> responses;
  std::queue<std::tuple<unsigned char, unsigned char>> resps;

  resps.push(std::make_tuple(0x02, 0x00));
  resps.push(std::make_tuple(0x00, 0b11000001));
  resps.push(std::make_tuple(0x82, 0x00));
  resps.push(std::make_tuple(0b11100001, 0x00));

  EXPECT_EQ(0, setDataMode(&dm, fakeSpiFun, &resps));
}

TEST(RFM69W, getCarrierFrequency) {
  float cf;
  std::queue<int> responses;
  std::queue<std::tuple<unsigned char, unsigned char>> resps;

  resps.push(std::make_tuple(0x07, 0x00));
  resps.push(std::make_tuple(0x00, 0xE4));
  resps.push(std::make_tuple(0x00, 0xC0));
  resps.push(std::make_tuple(0x00, 0x00));

  EXPECT_EQ(0, getCarrierFrequency(&cf, fakeSpiFun, &resps));
  EXPECT_EQ(915, cf);
}

TEST(RFM69W, setCarrierFrequency) {
  float cf = 868;
  std::queue<int> responses;
  std::queue<std::tuple<unsigned char, unsigned char>> resps;

  resps.push(std::make_tuple(0x87, 0x00));
  resps.push(std::make_tuple(0xD9, 0x00));
  resps.push(std::make_tuple(0x00, 0x00));
  resps.push(std::make_tuple(0x00, 0x00));

  EXPECT_EQ(0, setCarrierFrequency(&cf, fakeSpiFun, &resps));
}

TEST(RFM69W, getPacketFormat) {
  RFM_PACKETFORMAT pf;
  std::queue<int> responses;
  std::queue<std::tuple<unsigned char, unsigned char>> resps;

  resps.push(std::make_tuple(0x37, 0x00));
  resps.push(std::make_tuple(0x00, 0b00010000));

  EXPECT_EQ(0, getPacketFormat(&pf, fakeSpiFun, &resps));
  EXPECT_EQ(RFM_PACKETFORMAT_FIXED, pf);
}

TEST(RFM69W, setPacketFormat) {
  RFM_PACKETFORMAT pf = RFM_PACKETFORMAT_VARIABLE;
  std::queue<int> responses;
  std::queue<std::tuple<unsigned char, unsigned char>> resps;

  resps.push(std::make_tuple(0x37, 0x00));
  resps.push(std::make_tuple(0x00, 0b00010000));
  resps.push(std::make_tuple(0xB7, 0x00));
  resps.push(std::make_tuple(0b10010000, 0x00));

  EXPECT_EQ(0, setPacketFormat(&pf, fakeSpiFun, &resps));
}

TEST(RFM69W, getFifoData) {
  unsigned char data[2];
  std::queue<int> responses;
  std::queue<std::tuple<unsigned char, unsigned char>> resps;

  resps.push(std::make_tuple(0x00, 0x00));
  resps.push(std::make_tuple(0x00, 0xAA));
  resps.push(std::make_tuple(0x00, 0x5B));

  EXPECT_EQ(0, getFifoData((unsigned char *)&data, 2, fakeSpiFun, &resps));
  EXPECT_EQ(data[0], 0xAA);
  EXPECT_EQ(data[1], 0x5B);
}

TEST(RFM69W, setFifoData) {
  unsigned char data[] = {0x9C, 0x37};
  std::queue<int> responses;
  std::queue<std::tuple<unsigned char, unsigned char>> resps;

  resps.push(std::make_tuple(0x80, 0x00));
  resps.push(std::make_tuple(0x9C, 0x00));
  resps.push(std::make_tuple(0x37, 0x00));

  EXPECT_EQ(0, setFifoData((unsigned char *)&data, 2, fakeSpiFun, &resps));
}

TEST(RFM69W, getPacketSent) {
  RFM_FLAG f;
  std::queue<int> responses;
  std::queue<std::tuple<unsigned char, unsigned char>> resps;

  resps.push(std::make_tuple(0x28, 0x00));
  resps.push(std::make_tuple(0x00, 0b00001000));

  EXPECT_EQ(0, getPacketSent(&f, fakeSpiFun, &resps));
  EXPECT_EQ(RFM_FLAG_SET, f);
}

TEST(RFM69W, getPayloadReady) {
  RFM_FLAG f;
  std::queue<int> responses;
  std::queue<std::tuple<unsigned char, unsigned char>> resps;

  resps.push(std::make_tuple(0x28, 0x00));
  resps.push(std::make_tuple(0x00, 0b00000100));

  EXPECT_EQ(0, getPayloadReady(&f, fakeSpiFun, &resps));
  EXPECT_EQ(RFM_FLAG_SET, f);
}
