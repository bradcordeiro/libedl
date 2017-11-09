#include "gtest.h"
#include "../include/timecode.h"


// Gets correct components from 00;00;00;00
TEST(Timecode, Constructor_Default) {
  Timecode t;
  Timecode s(0, 0, 0, 0, 30, false);
  ASSERT_EQ(t, s);
}

TEST(Timecode, Constructor_String) {
  Timecode t("15;08;41;04", 29.97, true);
  ASSERT_EQ(15, t.hours());
  ASSERT_EQ(8, t.minutes());
  ASSERT_EQ(41, t.seconds());
  ASSERT_EQ(4, t.frames());
  Timecode u("15:08:41:04", 29.97, false);
  ASSERT_EQ(15, u.hours());
  ASSERT_EQ(8, u.minutes());
  ASSERT_EQ(41, u.seconds());
  ASSERT_EQ(4, u.frames());
  Timecode v("15:08:41;04", 29.97, false);
  ASSERT_EQ(15, v.hours());
  ASSERT_EQ(8, v.minutes());
  ASSERT_EQ(41, v.seconds());
  ASSERT_EQ(4, v.frames());
}

// Converts frames to TC in constructor
TEST(Timecode, Constructor_Int) {
  Timecode t(1633998, 29.97, true);
  ASSERT_EQ(15, t.hours());
  ASSERT_EQ(8, t.minutes());
  ASSERT_EQ(41, t.seconds());
  ASSERT_EQ(4, t.frames());
}

TEST(Timecode, Constructor_IntFields) {
  Timecode t(15, 8, 41, 4, 29.97, true);
  ASSERT_EQ(15, t.hours());
  ASSERT_EQ(8, t.minutes());
  ASSERT_EQ(41, t.seconds());
  ASSERT_EQ(4, t.frames());
}

// Converts TC to frames
TEST(Timecode, totalFrames_2997_DropFrame) {
  Timecode t("15:08:41;04", 29.97, true);
  ASSERT_EQ(1633998, t.totalFrames());
}

// Returns C-String with correct setSeparator
TEST(Timecode, c_str) {
  Timecode t("15:08:41;04", 29.97, false);
  Timecode u("15:08:41;04", 29.97, true);
  ASSERT_STREQ("15:08:41:04",t.c_str());
  ASSERT_STREQ("15;08;41;04",u.c_str());
}

TEST(Timecode, Typecasting) {
  Timecode t("15:08:41;04", 29.97, false);
  int u = t;
  ASSERT_EQ(u, t.totalFrames());
}

TEST(Timecode, RelationalOperators) {
  Timecode t("15:08:41;04", 29.97, false);
  Timecode u("15:08:41;04", 29.97, false);
  ASSERT_EQ(u, t);
  ASSERT_GE(u, t);
  ASSERT_LE(u, t);
  u.minutes(9);
  ASSERT_NE(u, t);
  ASSERT_GT(u, t);
  ASSERT_LT(t, u);
}

TEST(Timecode, AssignmentOperator) {
  Timecode t("15:08:41;04", 29.97, false);
  Timecode s;
  Timecode u = s = t;
  ASSERT_EQ(u,t);
  ASSERT_EQ(u,s);
  ASSERT_EQ(s,t);
}

TEST(Timecode, CanAdd_TwoTimecodes) {
  Timecode t("01:00:00:00", 29.97, true);
  Timecode u("00:04:59:28", 29.97, true);
  Timecode v("01:04:59:28", 29.97, true);
  Timecode x = t + u;
  ASSERT_EQ(v.totalFrames(), x.totalFrames());
}

TEST(Timecode, InvalidTimecodeThrows) {
  ASSERT_THROW(Timecode(5184000, 59.94, false), std::invalid_argument);
  ASSERT_THROW(Timecode("25:00:00:00"), std::invalid_argument);
  ASSERT_THROW(Timecode("01:61:00:00"), std::invalid_argument);
  ASSERT_THROW(Timecode("01:00:61:00"), std::invalid_argument);
  ASSERT_THROW(Timecode("01:00:00:31"), std::invalid_argument);
  ASSERT_THROW(Timecode("01:05:00:00", 29.97, true), std::invalid_argument);
}
/*
TEST(Timecode, CanAdd_IntAndTimecode) {
  Timecode t("01:00:00:00", 29.97, true);
  Timecode u("01:00:00:15", 29.97, false);
  t = t + 15;
  ASSERT_EQ(t.totalFrames(), u.totalFrames());
}
*/

namespace {

// The fixture for testing class Foo.
class FooTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  FooTest() {
    // You can do set-up work for each test here.
  }

  virtual ~FooTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
