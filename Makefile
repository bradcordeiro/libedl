edlthing: main.cpp bin/edl.o bin/event.o bin/timecode.o
	g++ main.cpp bin/edl.o bin/event.o bin/timecode.o -o bin/edlthing

bin/edl.o: src/edl.cpp include/edl.h
	g++ -c src/edl.cpp -o bin/edl.o

bin/event.o: src/event.cpp include/event.h
	g++ -c src/event.cpp -o bin/event.o

bin/timecode.o: src/timecode.cpp include/timecode.h
	g++ -c src/timecode.cpp -o bin/timecode.o

run: edlthing
	bin/edlthing

debug:
	g++ -g -O0 main.cpp src/edl.cpp src/event.cpp -o bin/edlthing_debug

clean:
	rm -rf bin/*
