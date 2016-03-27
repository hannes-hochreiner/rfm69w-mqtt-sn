gcc_flags_common = -g -O3 -std=c99
gcc_flags_sensor = -mmcu=attiny84 -DF_CPU=1000000UL
avrdude_flags_common = -c buspirate -P /dev/buspirate
avrdude_flags_sensor = -p t84

RFM69WSRC = src/rfm69w.c src/rfm69w.h
common_files = examples/sensor/spi.h examples/sensor/spi.c
sensor_files = examples/sensor/main.c \
							 $(RFM69WSRC) \
               $(common_files)

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

bld/sensor.o: bld Makefile $(sensor_files)
	avr-gcc $(gcc_flags_common) $(gcc_flags_sensor) -o $@ $(filter %.c, $(sensor_files)) $(filter %.S, $(sensor_files))

%.hex: %.o
	avr-objcopy -O ihex $< $@

flash: bld/sensor.hex
	avrdude $(avrdude_flags_common) $(avrdude_flags_sensor) -U flash:w:$<

fuse_default:
	avrdude $(avrdude_flags_common) $(avrdude_flags_sensor) -U lfuse:w:0x62:m

fuse_custom:
	avrdude $(avrdude_flags_common) $(avrdude_flags_sensor) -U lfuse:w:0xE8:m

read:
	avrdude $(avrdude_flags_common) $(avrdude_flags_sensor) -v
