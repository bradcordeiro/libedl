#include "../include/edl.h"
#include "../include/event.h"
#include "../include/motioneffect.h"
#include "../include/timecode.h"
#include "gtest.h"
#include <fstream>
#include <string>

// EDL
TEST(EdlTest, FromCmx3600DotEdl) {
  std::ifstream inputFile;
  inputFile.open("test/cmx3600.edl");
  Edl e(inputFile);
  inputFile.close();
}

TEST(EdlTest, EdlSize) {
  const unsigned int testsize = 927;
  std::ifstream inputFile;
  inputFile.open("test/cmx3600.edl");
  Edl e(inputFile);
  inputFile.close();
  ASSERT_EQ(testsize, e.size());
}

TEST(EdlTest, EventSearch_FirstEvent) {
  const unsigned int searchevent = 1;
  std::ifstream inputFile;
  inputFile.open("test/cmx3600.edl");
  Edl e(inputFile);
  inputFile.close();
  Event *f = e.find_event(searchevent);
  ASSERT_EQ(f->eventNumber(), searchevent);
}

TEST(EdlTest, EventSearch_LastEvent) {
  const unsigned int searchevent = 904;
  std::ifstream inputFile;
  inputFile.open("test/cmx3600.edl");
  Edl e(inputFile);
  inputFile.close();
  Event *f = e.find_event(searchevent);
  ASSERT_EQ(f->eventNumber(), searchevent);
}

TEST(EdlTest, EventSearch_NoEvent) {
  const int searchevent = 999;
  std::ifstream inputFile;
  inputFile.open("test/cmx3600.edl");
  Edl e(inputFile);
  inputFile.close();
  Event *f = e.find_event(searchevent);
  ASSERT_EQ(f, nullptr);
}

TEST(EdlTest, Sort) {
  std::ifstream inputFile;
  inputFile.open("test/cmx3600.edl");
  Edl e(inputFile);
  inputFile.close();
  ASSERT_EQ(e[3].eventNumber(), 5);
  e.sort();
  ASSERT_EQ(e[3].eventNumber(), 4);
}

// Events
TEST(EventTest, Constructor_Default) {
  Event e;
  Timecode t;
  ASSERT_EQ(e.eventNumber(), 0);
  ASSERT_EQ(e.trackType(), 'V');
  ASSERT_EQ(e.trackNumber(), 1);
  ASSERT_EQ(e.sourceStart(), t);
  ASSERT_EQ(e.sourceEnd(), t);
  ASSERT_EQ(e.recordStart(), t);
  ASSERT_EQ(e.recordEnd(), t);
  ASSERT_EQ(e.reel(), "");
  ASSERT_EQ(e.sourceClipName(), "");
  ASSERT_EQ(e.sourceFileName(), "");
  ASSERT_EQ(e.comment(), "");
  ASSERT_FALSE(e.hasMotionEffect());
}

TEST(EventTest, Constructor_EDLString) {
  Timecode ss("00:05:43:12", 30.0, false);
  Timecode se("00:05:45:13", 30.0, false);
  Timecode rs("01:00:00:00", 30.0, false);
  Timecode re("01:00:02:00", 30.0, false);
  Event e(6, "EVL1_HEI", 'V', 1, Timecode("00:05:43:12", 30, false),
          Timecode("00:05:45:13", 30, false),
          Timecode("01:00:00:00", 30.0, false),
          Timecode("01:00:02:00", 30.0, false), 30.0, false);
  ASSERT_EQ(e.eventNumber(), 6);
  ASSERT_EQ(e.reel(), "EVL1_HEI");
  ASSERT_EQ(e.trackType(), 'V');
  ASSERT_EQ(e.sourceStart(), ss);
  ASSERT_EQ(e.sourceEnd(), se);
  ASSERT_EQ(e.recordStart(), rs);
  ASSERT_EQ(e.recordEnd(), re);
  ASSERT_FALSE(e.hasMotionEffect());
}

TEST(EventTest, Constructor_Copy) {
  Event e(6, "EVL1_HEI", 'V', 1, Timecode("00:05:43:12", 30.0, false),
          Timecode("00:05:45:13", 30.0, false),
          Timecode("01:00:00:00", 30.0, false),
          Timecode("01:00:02:00", 30.0, false), 30.0, false);
  MotionEffect m("EVL1_HEI", 31.3, e.sourceStart());
  e.motionEffect(m);
  Event f(e);
  ASSERT_EQ(e.eventNumber(), f.eventNumber());
  ASSERT_EQ(e.reel(), f.reel());
  ASSERT_EQ(e.trackType(), f.trackType());
  ASSERT_EQ(e.sourceStart(), f.sourceStart());
  ASSERT_EQ(e.sourceEnd(), f.sourceEnd());
  ASSERT_EQ(e.recordStart(), f.recordStart());
  ASSERT_EQ(e.recordEnd(), f.recordEnd());
  MotionEffect x = e.motionEffect();
  MotionEffect y = f.motionEffect();
  ASSERT_NE(&x, &y);
}

TEST(EventTest, Operator_Assignment) {
  Event e(6, "EVL1_HEI", 'V', 1, Timecode("00:05:43:12", 30.0, false),
          Timecode("00:05:45:13", 30.0, false),
          Timecode("01:00:00:00", 30.0, false),
          Timecode("01:00:02:00", 30.0, false), 30.0, false);
  MotionEffect m("EVL1_HEI", 31.3, e.sourceStart());
  e.motionEffect(m);
  Event f = e;
  ASSERT_EQ(e.eventNumber(), f.eventNumber());
  ASSERT_EQ(e.reel(), f.reel());
  ASSERT_EQ(e.trackType(), f.trackType());
  ASSERT_EQ(e.sourceStart(), f.sourceStart());
  ASSERT_EQ(e.sourceEnd(), f.sourceEnd());
  ASSERT_EQ(e.recordStart(), f.recordStart());
  ASSERT_EQ(e.recordEnd(), f.recordEnd());
  MotionEffect x = e.motionEffect();
  MotionEffect y = f.motionEffect();
  ASSERT_NE(&x, &y);
}

// Motion Effects
TEST(MotionEffectTest, hasMotionEffect) {
  float f = 30.0;
  bool d = false;
  Event e(6, "EVL1_HEI", 'V', 1, Timecode("00:05:43:12", f, d),
          Timecode("00:05:45:13", f, d), Timecode("01:00:00:00", f, d),
          Timecode("01:00:02:00", f, d), f, d);
  ASSERT_FALSE(e.hasMotionEffect());
  e.motionEffect("EVL1_HEI", 31.3, Timecode("00:05:43:12", f, d));
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
  ASSERT_THROW(Timecode x("01000000", 30.0, false), std::invalid_argument);
  ASSERT_THROW(Timecode x("01:000:000", 30.0, false), std::invalid_argument);
  ASSERT_THROW(Timecode x("57", 30.0, false), std::invalid_argument);
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
TEST(TimecodeTest, Function_totalFrames) {
  Timecode t("15:08:41;04", 29.97, true);
  ASSERT_EQ(1633998, t.totalFrames());
}

// Returns C-String with correct setSeparator
TEST(TimecodeTest, Function_to_string) {
  Timecode t("15:08:41;04", 29.97, false);
  Timecode u("15:08:41;04", 29.97, true);
  ASSERT_EQ("15:08:41:04", t.to_string());
  ASSERT_EQ("15:08:41;04", u.to_string());
}

TEST(TimecodeTest, Operator_Relational) {
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

TEST(TimecodeTest, Operator_Assignment) {
  Timecode t("15:08:41;04", 29.97, false);
  Timecode s;
  Timecode u = s = t;
  ASSERT_EQ(u, t);
  ASSERT_EQ(u, s);
  ASSERT_EQ(s, t);
}

TEST(TimecodeTest, Operator_Plus) {
  Timecode t("01:00:00:00", 29.97, true);
  Timecode u("00:04:59:28", 29.97, true);
  Timecode v("01:04:59:28", 29.97, true);
  Timecode w = t + u;
  ASSERT_EQ(v.totalFrames(), w.totalFrames());
  Timecode x("01:00:00:00", 29.97, true);
  Timecode y("01:00:00:15", 29.97, true);
  Timecode z = t + 15;
  ASSERT_EQ(z.totalFrames(), y.totalFrames());
  Timecode a("01:00:00:00", 29.97, true);
  Timecode b("00:01:00:00", 29.97, false);
  Timecode c = a + b;
  Timecode d("01:01:00:02", 29.97, true);
  ASSERT_EQ(c.totalFrames(), d.totalFrames());
  a = Timecode("23:59:59:29", 30.0, false);
  b = a + 1;
  c = Timecode("00:00:00:00", 30.0, false);
  ASSERT_EQ(b.totalFrames(), c.totalFrames());
}

TEST(TimecodeTest, Operator_Multiply) {
  Timecode t("01:00:00:00", 30, false);
  Timecode u("02:00:00:00", 30, false);
  Timecode v = t*2;
  ASSERT_EQ(u,v);
}

TEST(TimecodeTest, InvalidTimecodes_RollOver) {
  // ASSERT_THROW(Timecode(5184000, 59.94, false), std::invalid_argument);
  ASSERT_EQ(Timecode("24:00:00:00", 30.0, false),
            Timecode("00:00:00:00", 30.0, false));
  ASSERT_EQ(Timecode("01:60:00:00", 30.0, false),
            Timecode("02:00:00:00", 30.0, false));
  ASSERT_EQ(Timecode("01:00:60:00", 30.0, false),
            Timecode("01:01:00:00", 30.0, false));
  ASSERT_EQ(Timecode("01:00:00:31", 30.0, false),
            Timecode("01:00:01:01", 30.0, false));
  ASSERT_EQ(Timecode("01:05:00:00", 29.97, true),
            Timecode("01:05:00:02", 29.97, true));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
