RFM69WSRC = src/rfm69w.c src/rfm69w.h

test: bld_test/test_all
	bld_test/test_all

bld_test:
	mkdir bld_test

bld_test/test_all: Makefile bld_test src_test/test_all.cpp src_test/rfm69w_test.cpp $(RFM69WSRC)
	g++ -std=c++11 -lgtest -lgtest_main -o bld_test/test_all src_test/test_all.cpp src_test/rfm69w_test.cpp $(RFM69WSRC)

bld:
	mkdir bld

base: bld examples/base/main.cpp src/rfm69w.c src/rfm69w.h
	g++ -lwiringPi examples/base/main.cpp src/rfm69w.c -o bld/main
