gcc_flags_common = -g -O3 -ffreestanding -fno-exceptions #-std=c99 
gcc_flags_sensorMegaSp = -DDDR_SP=DDRD -DDD_SP_OK=DDD2 -DDD_SP_ERROR=DDD3 -DPORTR_SP=PORTD -DPORT_SP_OK=PORTD2 -DPORT_SP_ERROR=PORTD3
gcc_flags_sensorMegaSpi = -DDDR_SPI=DDRB -DDD_MOSI=DDB3 -DDD_SCK=DDB5 -DDD_SS=DDB2 -DPORTR_SPI=PORTB -DPORTR_SPI_SS=PORTB2
gcc_flags_sensorMega = -mmcu=atmega328p -DF_CPU=1000000UL $(gcc_flags_sensorMegaSp) $(gcc_flags_sensorMegaSpi)
avrdude_flags_common = -c buspirate -P /dev/buspirate
avrdude_flags_sensorMega = -p m328p

RFM69WSRC = src/rfm69w.c src/rfm69w.h
common_files = examples/sensor/spi.h examples/sensor/spi.c
sensorMega_files = examples/sensorAtMega/main.c \
									 examples/sensorAtMega/signal.h examples/sensorAtMega/signal.c \
									 examples/sensorAtMega/spi.h examples/sensorAtMega/spi.c \
									 $(RFM69WSRC)
sensorMegaTest_files = examples/sensorAtMega/main.cpp \
											examples/sensorAtMega/signal.h examples/sensorAtMega/signal.c

test: bld_test/test_all
	bld_test/test_all

bld_test:
	mkdir bld_test

bld_test/test_all: Makefile bld_test src_test/test_all.cpp src_test/rfm69w_test.cpp $(RFM69WSRC)
	g++ -std=c++11 -lgtest -lgtest_main -o bld_test/test_all src_test/test_all.cpp src_test/rfm69w_test.cpp $(RFM69WSRC)

bld:
	mkdir bld

base: bld examples/base/main.cpp examples/base/baseException.h examples/base/baseException.cpp src/rfm69w.c src/rfm69w.h
	g++ -lwiringPi examples/base/main.cpp examples/base/baseException.cpp src/rfm69w.c -o bld/base

baseSend: bld examples/base/mainSend.cpp examples/base/baseException.h examples/base/baseException.cpp src/rfm69w.c src/rfm69w.h
	g++ -lwiringPi examples/base/mainSend.cpp examples/base/baseException.cpp src/rfm69w.c -o bld/baseSend

bld/sensorMega.o: bld Makefile $(sensorMega_files)
	avr-gcc $(gcc_flags_common) $(gcc_flags_sensorMega) -o $@ $(filter %.c, $(sensorMega_files))

bld/sensorMegaTest.o: bld Makefile $(sensorMegaTest_files)
	avr-gcc $(gcc_flags_common) $(gcc_flags_sensorMega) -o $@ $(filter %.cpp, $(sensorMegaTest_files)) $(filter %.c, $(sensorMegaTest_files))


%.hex: %.o
	avr-objcopy -O ihex $< $@

flashMega: bld/sensorMega.hex
	avrdude $(avrdude_flags_common) $(avrdude_flags_sensorMega) -U flash:w:$<

flashMegaTest: bld/sensorMegaTest.hex
	avrdude $(avrdude_flags_common) $(avrdude_flags_sensorMega) -U flash:w:$<
