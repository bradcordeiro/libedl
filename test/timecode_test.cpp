#include "cpptest.h"

class TimecodeTestSuite : public Test::Suite {
public:
    TimecodeTestSuite()
    {
        TEST_ADD(TimecodeTestSuite::first_test)
        TEST_ADD(TimecodeTestSuite::second_test)
    }

private:
    void first_test();
    void second_test();
};
