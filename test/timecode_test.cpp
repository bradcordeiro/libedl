#include "../include/event.h"
#include "../include/motioneffect.h"
#include "../include/timecode.h"
#include "gtest.h"

// Motion Effects
TEST(MotionEffect, hasMotionEffect) {
  Event e = Event("006  EVL1_HEI V     C        00:05:43:12 00:05:45:13 "
                  "01:00:00:00 01:00:02:00");
  ASSERT_FALSE(e.hasMotionEffect());
  e.motionEffect("M2   EVL1_HEI       031.3                00:05:43:12 ");
  ASSERT_TRUE(e.hasMotionEffect());
  e.removeMotionEffect();
  ASSERT_FALSE(e.hasMotionEffect());
}

// Timecode
TEST(TimecodeTest, Constructor_Default) {
  Timecode t;
  Timecode s(0, 0, 0, 0, 30, false);
  ASSERT_EQ(t, s);
}

TEST(TimecodeTest, Constructor_String) {
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
  Timecode v("15.08.41.04", 29.97, false);
  ASSERT_EQ(15, v.hours());
  ASSERT_EQ(8, v.minutes());
  ASSERT_EQ(41, v.seconds());
  ASSERT_EQ(4, v.frames());
  Timecode w("15-08-41-04", 29.97, false);
  ASSERT_EQ(15, v.hours());
  ASSERT_EQ(8, v.minutes());
  ASSERT_EQ(41, v.seconds());
  ASSERT_EQ(4, v.frames());
  ASSERT_THROW(Timecode x("01000000"), std::invalid_argument);
  ASSERT_THROW(Timecode x("01:000:000"), std::invalid_argument);
  ASSERT_THROW(Timecode x("57"), std::invalid_argument);
}

// Converts frames to TC in constructor
TEST(TimecodeTest, Constructor_Int) {
  Timecode t(1633998, 29.97, true);
  ASSERT_EQ(15, t.hours());
  ASSERT_EQ(8, t.minutes());
  ASSERT_EQ(41, t.seconds());
  ASSERT_EQ(4, t.frames());
}

TEST(TimecodeTest, Constructor_IntFields) {
  Timecode t(15, 8, 41, 4, 29.97, true);
  ASSERT_EQ(15, t.hours());
  ASSERT_EQ(8, t.minutes());
  ASSERT_EQ(41, t.seconds());
  ASSERT_EQ(4, t.frames());
  ASSERT_EQ(t.totalFrames(), 1633998);
}

// Converts TC to frames
TEST(TimecodeTest, totalFrames_2997_DropFrame) {
  Timecode t("15:08:41;04", 29.97, true);
  ASSERT_EQ(1633998, t.totalFrames());
}

TEST(TimecodeTest, maxFrames) {
  Timecode t("23:59:59:59", 59.94, false);
  Timecode u("23:59:59:59", 59.94, true);
  Timecode v("23:59:59:29", 29.97, false);
  Timecode w("23:59:59:29", 29.97, true);
  ASSERT_EQ(t.totalFrames(), t.maxFrames());
  ASSERT_EQ(u.totalFrames(), u.maxFrames());
  ASSERT_EQ(v.totalFrames(), v.maxFrames());
  ASSERT_EQ(w.totalFrames(), w.maxFrames());
}


// Returns C-String with correct setSeparator
TEST(TimecodeTest, c_str) {
  Timecode t("15:08:41;04", 29.97, false);
  Timecode u("15:08:41;04", 29.97, true);
  ASSERT_STREQ("15:08:41:04", t.c_str());
  ASSERT_STREQ("15;08;41;04", u.c_str());
}

TEST(TimecodeTest, Typecasting) {
  Timecode t("15:08:41;04", 29.97, false);
  int u = t;
  ASSERT_EQ(u, t.totalFrames());
}

TEST(TimecodeTest, Relational_Operators) {
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

TEST(TimecodeTest, Assignment_Operator) {
  Timecode t("15:08:41;04", 29.97, false);
  Timecode s;
  Timecode u = s = t;
  ASSERT_EQ(u, t);
  ASSERT_EQ(u, s);
  ASSERT_EQ(s, t);
}

TEST(TimecodeTest, Can_Add_Two_Timecodes) {
  Timecode t("01:00:00:00", 29.97, true);
  Timecode u("00:04:59:28", 29.97, true);
  Timecode v("01:04:59:28", 29.97, true);
  Timecode x = t + u;
  ASSERT_EQ(v.totalFrames(), x.totalFrames());
}

TEST(TimecodeTest, Can_Add_Int_And_Timecode) {
  Timecode t("01:00:00:00", 29.97, true);
  Timecode u("01:00:00:15", 29.97, true);
  Timecode v = t + 15;
  ASSERT_EQ(v.totalFrames(), u.totalFrames());
}

TEST(TimecodeTest, Can_Add_And_Drop_Frames) {
  Timecode t("01:00:00:00", 29.97, true);
  Timecode u("00:01:00:00", 29.97, false);
  Timecode v = t + u;
  Timecode w("01:01:00:02", 29.97, true);
  ASSERT_EQ(v.totalFrames(), w.totalFrames());
  t = Timecode("23:59:59:29");
  u = t + 1;
  v = Timecode("00:00:00:00");
  ASSERT_EQ(u.totalFrames(), v.totalFrames());
}

TEST(TimecodeTest, Invalid_Timecodes_Throw_Exceptions) {
  ASSERT_THROW(Timecode(5184000, 59.94, false), std::invalid_argument);
  ASSERT_THROW(Timecode("24:00:00:00"), std::invalid_argument);
  ASSERT_THROW(Timecode("01:60:00:00"), std::invalid_argument);
  ASSERT_THROW(Timecode("01:00:60:00"), std::invalid_argument);
  ASSERT_THROW(Timecode("01:00:00:31"), std::invalid_argument);
  ASSERT_THROW(Timecode("01:05:00:00", 29.97, true), std::invalid_argument);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
