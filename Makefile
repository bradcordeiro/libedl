main: bin/timecode.o bin/edl.o bin/event.o
	g++ main.cpp bin/timecode.o bin/edl.o bin/event.o -o bin/edlthing

bin/timecode.o:
	g++ -c src/timecode.cpp -o bin/timecode.o

bin/edl.o:
	g++ -c src/edl.cpp -o bin/edl.o

bin/event.o:
	g++ -c src/event.cpp -o bin/event.o

libtimecode:
	g++ -c src/timecode.cpp

tests: test/gtest-all.o
	g++ -isystem ${GTEST_DIR}/include -pthread test/gtest-all.o test/timecode_test.cpp src/timecode.cpp -o test/timecode_test
	test/timecode_test

test/gtest-all.o:
	g++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -o test/gtest-all.o

debug:
	g++ -g -O0 main.cpp src/edl.cpp src/event.cpp -o bin/edlthing_debug

clean:
	rm bin/*
