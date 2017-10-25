#include "../include/timecode.h"
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <string>

Timecode::Timecode() {}

Timecode::Timecode(const std::string &s, float f, bool b)
    : _frameRate(f), _dropFrame(b) {
  setTimecode(s);
}

Timecode::Timecode(const std::string &s, int i, bool b)
    : _frameRate(i), _dropFrame(b) {
  setTimecode(s);
}

Timecode::Timecode(int h, int m, int s, int fr, float f, bool b)
    : _hours(h), _minutes(m), _seconds(s), _frames(fr), _frameRate(f),
      _dropFrame(b) {}

Timecode::Timecode(int h, int m, int s, int fr, int f, bool b)
    : _hours(h), _minutes(m), _seconds(s), _frames(fr), _frameRate(f),
      _dropFrame(b) {}

// This algorithm isn't quite right
Timecode::Timecode(int frames, float frameRate, bool dropFrame) {
  int framesPerHour;
  int framesPer10Min;
  int framesPerMin;

  switch (static_cast<int>(frameRate)) {
  case 30:
  case 29:
    if (dropFrame) {
      framesPerHour = 60 * 60 * frameRate;
      framesPer10Min = 60 * 30 - 2;
      framesPerMin = 60 * 30;
    }
  }
}

std::string Timecode::to_string() const {
  char s[11];
  sprintf(s, "%2d:%2d:%2d:%2d", _hours, _minutes, _seconds, _frames);
  return std::string(s);
}

void Timecode::setTimecode(char* s) {
  sscanf(s, "%2d:%2d:%2d:%2d", &_hours, &_minutes, &_seconds, &_frames);
}

void Timecode::setTimecode(const std::string &s) {
  sscanf(s.c_str(), "%2d:%2d:%2d:%2d", &_hours, &_minutes, &_seconds, &_frames);
}

std::ostream &operator<<(std::ostream &out, const Timecode &t) {
  out << t.to_string();
  return out;
}

std::istream &operator>>(std::istream &in, Timecode &t) {
  std::string input;
  in >> input;
  t.setTimecode(input);
  return in;
}
