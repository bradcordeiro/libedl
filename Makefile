default: build/event.o build/edl.o build/timecode.o build/motioneffect.o

clean:
	rm -rf build

build/edl.o:
		mkdir -p build
		g++ -c --std=c++11 src/edl.cpp -o build/edl.o

build/event.o: src/event.cpp include/event.h
	mkdir -p build
	g++ -c --std=c++11 src/event.cpp -o build/event.o

build/motioneffect.o: src/motioneffect.cpp include/motioneffect.h
	mkdir -p build
	g++ -c --std=c++11 src/motioneffect.cpp -o build/motioneffect.o

build/timecode.o:
	mkdir -p build
	g++ -c --std=c++11 src/timecode.cpp -o build/timecode.o

roundtrip: build/edl.o build/event.o build/timecode.o examples/roundtrip.cpp
	g++ --std=c++11 build/edl.o build/event.o build/timecode.o build/motioneffect.o examples/roundtrip.cpp -o roundtrip

tests: test/gtest-all.o
	g++ -isystem $(GTEST_DIR)/include -pthread test/gtest-all.o test/timecode_test.cpp src/timecode.cpp -o test/timecode_test
	test/timecode_test
	rm test/timecode_test

test/gtest-all.o:
	g++ -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -pthread -c $(GTEST_DIR)/src/gtest-all.cc -o test/gtest-all.o
