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
#ifndef __SIGNAL_H__
#define __SIGNAL_H__
#include <stdbool.h>
#include <avr/io.h>

#ifndef DDR_SP
#error "Data direction register for signal pins (DDR_SP) needs to be defined."
#endif

#ifndef DD_SP_OK
#error "Data direction bit for signal pin 'ok' (DD_SP_OK) needs to be defined."
#endif

#ifndef DD_SP_ERROR
#error "Data direction bit for signal pin 'error' (DD_SP_ERROR) needs to be defined."
#endif

#ifndef PORTR_SP
#error "Port register for signal pins (PORTR_SP) needs to be defined."
#endif

#ifndef PORT_SP_OK
#error "Port register bit for signal pin 'ok' (PORT_SP_OK) needs to be defined."
#endif

#ifndef PORT_SP_ERROR
#error "Port register bit for signal pin 'error' (PORT_SP_ERROR) needs to be defined."
#endif

void signalInit();
void signalOk(bool permanent, void (*delay)(double));
void signalError(bool permanent, void (*delay)(double));
#endif
