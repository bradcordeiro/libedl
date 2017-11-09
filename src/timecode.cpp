#include "../include/timecode.h"
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>

// Constructors

Timecode::Timecode(int h, int m, int s, int f, float fr, bool b)
    : _hours(h), _minutes(m), _seconds(s), _frames(f), _frameRate(fr),
      _dropFrame(b) {
  setSeparator();
  validate();
}

Timecode::Timecode(const std::string &s, float fr, bool b)
    : _frameRate(fr), _dropFrame(b) {
  setSeparator();
  setTimecode(s);
  validate();
}

Timecode::Timecode(const char *s, float frameRate, bool b)
    : _frameRate(frameRate), _dropFrame(b) {
  setTimecode(s);
  setSeparator();
  validate();
}

Timecode::Timecode(int frameInput, float frameRate, bool b)
    : _frameRate(frameRate), _dropFrame(b) {
  setSeparator();
  setTimecode(frameInput);
}

// Private member functions
void Timecode::setTimecode(int frameInput) {
  uint_fast16_t nominal_fps = _frameRate;
  uint_fast16_t framesPerMin;
  uint_fast16_t framesPer10Min;
  uint_fast32_t framesPerHour;

  // nominal fps for 29.97 is 30, for 59.94 is 60, for 23.98 is 24
  if (nominal_fps == 29 || nominal_fps == 59 || nominal_fps == 23)
    nominal_fps++;

  uint_fast32_t frameLimit = nominal_fps * 60 * 60 * 24;
  if (frameInput >= frameLimit) {
    std::string m = "Invalid frame count, frames cannot be larger than " +
                    std::to_string(frameLimit) + " at " +
                    std::to_string(_frameRate) + ". (" +
                    std::to_string(frameInput) + " passed) ";
    throw std::invalid_argument(m);
  }

  framesPerMin = 60 * nominal_fps;
  framesPer10Min = framesPerMin * 10;

  // 29.97: drop 2 frames per minute, add 2 back for 10th minute
  // 59.94: drop 4 frames per minute, add 4 back for 10th minute
  if (_dropFrame) {
    framesPerMin -= (nominal_fps / 15);
    framesPer10Min = framesPerMin * 10 + (nominal_fps / 15);
  }

  framesPerHour = framesPer10Min * 6;

  _hours = frameInput / framesPerHour;
  frameInput %= framesPerHour;

  if (_dropFrame) {
    // By subtracting two frames while calculating minutes and adding them
    // back in afterward, you avoid landing on an invalid dropframe frame
    // (frame 0 on any minute except every tenth minute)
    uint_fast16_t ten_minute = frameInput / framesPer10Min;
    frameInput %= framesPer10Min;
    frameInput -= 2;
    uint_fast16_t unit_minute = frameInput / framesPerMin;
    frameInput %= framesPerMin;
    _minutes = ten_minute * 10 + unit_minute;
    frameInput += 2;
  } else {
    _minutes = (frameInput / framesPer10Min);
    frameInput %= framesPer10Min;
    _minutes += frameInput / framesPerMin;
    frameInput %= framesPerMin;
  }

  _seconds = frameInput / nominal_fps;
  frameInput %= nominal_fps;

  _frames = frameInput;
}

void Timecode::setTimecode(const std::string &s) { setTimecode(s.c_str()); }

void Timecode::setTimecode(const char *s) {
  sscanf(s, "%2hu%*1c%2hu%*1c%2hu%*1c%2hu", &_hours, &_minutes, &_seconds,
         &_frames);
}

void Timecode::validate() {
  if (_hours > 23) {
    std::string m = "Hours cannot be larger than 23 (" + to_string() + ")";
    throw std::invalid_argument(m);
  }
  if (_minutes > 59) {
    std::string m = "Minutes cannot be larger than 59 (" + to_string() + ")";
    throw std::invalid_argument(m);
  }
  if (_seconds > 59) {
    std::string m = "Seconds cannot be larger than 59 (" + to_string() + ")";
    throw std::invalid_argument(m);
  }
  if (_frames > _frameRate) {
    std::string m = "Frames cannot be larger than framerate (" + to_string() +
                    ", " + std::to_string(_frameRate);
    throw std::invalid_argument(m);
  }
  if (_dropFrame && _frames == 0 && _seconds == 0 && _minutes % 10 != 0) {
    std::string m = "Frames dropped in dropframe passed (" + to_string() + ")";
    throw std::invalid_argument(m);
  }
};

// Getters
uint_fast32_t Timecode::totalFrames() const {
  uint_fast16_t nominal_fps = _frameRate;
  uint_fast16_t framesPerMin;
  uint_fast16_t framesPer10Min;
  uint_fast32_t framesPerHour;
  uint_fast32_t totalFrames = 0;

  switch (static_cast<int>(_frameRate)) {
  case 29:
    nominal_fps = 30;
    break;
  case 59:
    nominal_fps = 60;
    break;
  case 23:
    nominal_fps = 24;
    break;
  }

  framesPerMin = 60 * nominal_fps;
  framesPer10Min = framesPerMin * 10;

  if (_dropFrame) {
    framesPerMin -= nominal_fps / 15;
    framesPer10Min = (framesPerMin * 10) + (nominal_fps / 15);
  }

  framesPerHour = framesPer10Min * 6;

  totalFrames += _hours * framesPerHour;
  totalFrames += (_minutes % 10) * framesPerMin;
  totalFrames += (_minutes / 10) * framesPer10Min;
  totalFrames += _seconds * nominal_fps;
  totalFrames += _frames;

  return totalFrames;
}

// Type conversion

Timecode::operator int() const { return totalFrames(); }

Timecode::operator char *() const { return c_str(); }

Timecode::operator std::string() const { return to_string(); }

std::string Timecode::to_string() const { return std::string(c_str()); }

char *Timecode::c_str() const {
  static char s[12];
  sprintf(s, "%02hu%c%02hu%c%02hu%c%02hu", _hours, _separator, _minutes,
          _separator, _seconds, _separator, _frames);
  return s;
}

// Operators
Timecode Timecode::operator+(const Timecode &t) const {
  return Timecode(totalFrames() + t.totalFrames(), _frameRate, _dropFrame);
}

Timecode Timecode::operator-(const Timecode &t) const {
  return Timecode(totalFrames() - t.totalFrames(), _frameRate, _dropFrame);
}

Timecode &Timecode::operator=(const Timecode &t) {
  if (this != &t) {
    _hours = t._hours;
    _minutes = t._minutes;
    _seconds = t._seconds;
    _frames = t._frames;
    _frameRate = t._frameRate;
    _dropFrame = t._dropFrame;
  }
  return *this;
}

bool Timecode::operator==(const Timecode &t) const {
  if (_frames != t._frames)
    return false;
  if (_hours != t._hours)
    return false;
  if (_minutes != t._minutes)
    return false;
  if (_frames != t._frames)
    return false;
  if (_frameRate != t._frameRate)
    return false;
  return _dropFrame == t._dropFrame;
}

bool Timecode::operator!=(const Timecode &t) const { return !(*this == t); }

bool Timecode::operator<(const Timecode &t) const {
  return totalFrames() < t.totalFrames();
}

bool Timecode::operator<=(const Timecode &t) const {
  uint_fast32_t a = totalFrames();
  uint_fast32_t b = t.totalFrames();
  return (a < b || a == b);
}

bool Timecode::operator>(const Timecode &t) const {
  return totalFrames() > t.totalFrames();
}

bool Timecode::operator>=(const Timecode &t) const {
  uint_fast32_t a = totalFrames();
  uint_fast32_t b = t.totalFrames();
  return (a > b || a == b);
}

std::ostream &operator<<(std::ostream &out, const Timecode &t) {
  out << t.c_str();
  return out;
}

std::istream &operator>>(std::istream &in, Timecode &t) {
  std::string input;
  in >> input;
  t.setTimecode(input);
  return in;
}
