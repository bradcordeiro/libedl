default: build/event.o build/edl.o build/timecode.o build/motioneffect.o

clean:
	rm -rf build

build/edl.o: include/edl.h src/edl.cpp
		mkdir -p build
		g++ -c --std=c++11 src/edl.cpp -o build/edl.o

build/event.o: include/event.h src/event.cpp
	mkdir -p build
	g++ -c --std=c++11 src/event.cpp -o build/event.o

build/motioneffect.o: include/motioneffect.h src/motioneffect.cpp
	mkdir -p build
	g++ -c --std=c++11 src/motioneffect.cpp -o build/motioneffect.o

build/timecode.o: include/timecode.h src/timecode.cpp
	mkdir -p build
	g++ -c --std=c++11 src/timecode.cpp -o build/timecode.o

roundtrip: build/edl.o build/event.o build/timecode.o examples/roundtrip.cpp
	g++ --std=c++11 build/edl.o build/event.o build/timecode.o build/motioneffect.o examples/roundtrip.cpp -o roundtrip

tests: test/gtest-all.o test/timecode_test.cpp src/event.cpp src/motioneffect.cpp src/timecode.cpp
	g++ -isystem $(GTEST_DIR)/include -pthread test/gtest-all.o test/timecode_test.cpp src/event.cpp src/motioneffect.cpp src/timecode.cpp -o test/timecode_test
	test/timecode_test
	rm test/timecode_test

test/gtest-all.o:
	g++ -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -pthread -c $(GTEST_DIR)/src/gtest-all.cc -o test/gtest-all.o
