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
#include <wiringPiSPI.h>
#include <iostream>

int spiTransfer(unsigned char* const bytes, unsigned int length, void* const customData) {
  return wiringPiSPIDataRW(0, bytes, length);
}

int main() {
  int res = wiringPiSPISetup(0, 500000);

  if (res == -1) {
    std::cerr << "Error setting up SPI interface\n";
    return -1;
  }

  RFM_MODE m = RFM_MODE_SLEEP;

  res = setMode(&m, spiTransfer);

  if (res == -1) {
    std::cerr << "Error transferring data\n";
    return -1;
  }

  m = (RFM_MODE)0;

  res = getMode(&m, spiTransfer);

  if (res == -1) {
    std::cerr << "Error transferring data\n";
    return -1;
  }

  if (m == RFM_MODE_SLEEP) {
    std::cout << "...sleeping...\n";
  } else {
    std::cout << "...doing other stuff..\n";
  }

  return 0;
}
