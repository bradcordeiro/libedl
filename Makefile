tests: test/gtest-all.o
	g++ -isystem $(GTEST_DIR)/include -pthread test/gtest-all.o test/timecode_test.cpp src/timecode.cpp -o test/timecode_test
	test/timecode_test
	rm test/timecode_test

test/gtest-all.o:
	g++ -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -pthread -c $(GTEST_DIR)/src/gtest-all.cc -o test/gtest-all.o
