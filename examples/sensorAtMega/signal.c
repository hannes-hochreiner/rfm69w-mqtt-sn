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
#include "signal.h"

void signalInit() {
  DDR_SP = (1<<DD_SP_OK)|(1<<DD_SP_ERROR);
}

void signal(bool permanent, void (*delay)(double), unsigned char port) {
  PORTR_SP |= (1<<port);
  (*delay)(10);

  if (permanent) {
    while (1) {
      PORTR_SP ^= (1<<port);
      (*delay)(10);
    }
  } else {
    PORTR_SP &= ~(1<<port);
  }
}

void signalOk(bool permanent, void (*delay)(double)) {
  signal(permanent, delay, PORT_SP_OK);
}

void signalError(bool permanent, void (*delay)(double)) {
  signal(permanent, delay, PORT_SP_ERROR);
}
