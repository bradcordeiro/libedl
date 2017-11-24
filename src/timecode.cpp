#include "../include/timecode.h"
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>

// Constructors

Timecode::Timecode(uint_fast16_t h, uint_fast16_t m, uint_fast16_t s,
                   uint_fast16_t fr, double f, bool b)
    : _hours(h), _minutes(m), _seconds(s), _frames(fr), _frameRate(f),
      _dropFrame(b), _separator(':') {
  _setSeparator();
  _validate();
}

Timecode::Timecode(uint_fast32_t frameInput, double frameRate, bool b)
    : _hours(0), _minutes(0), _seconds(0), _frames(0), _frameRate(frameRate),
      _dropFrame(b), _separator(':') {
  _setTimecode(frameInput);
  _setSeparator();
}

Timecode::Timecode(const char *c, double f, bool b)
    : _hours(0), _minutes(0), _seconds(0), _frames(0), _frameRate(f),
      _dropFrame(b), _separator(':') {
  _setTimecode(c);
  _setSeparator();
  _validate();
}

Timecode::Timecode(std::string s, double f, bool b)
    : _hours(0), _minutes(0), _seconds(0), _frames(0), _frameRate(f),
      _dropFrame(b), _separator(':') {
  _setTimecode(s.c_str());
  _setSeparator();
}

// Private member functions
void Timecode::_setSeparator() { _separator = (_dropFrame ? ';' : ':'); }

void Timecode::_setTimecode(uint_fast32_t &frameInput) {
  uint_fast16_t nominal_fps = _nominalFramerate();
  uint_fast32_t frameLimit = maxFrames();
  uint_fast16_t framesPerMin = 60 * nominal_fps;
  uint_fast16_t framesPer10Min = framesPerMin * 10;
  uint_fast32_t framesPerHour = framesPer10Min * 6;

  if (frameInput >= frameLimit) {
    std::string m = "Invalid frame count, frames cannot be larger than " +
                    std::to_string(frameLimit) + " at " +
                    std::to_string(_frameRate) + ". (" +
                    std::to_string(frameInput) + " passed) ";
    throw std::invalid_argument(m);
  }

  // 29.97: drop 2 frames per minute, add 2 back for 10th minute
  // 59.94: drop 4 frames per minute, add 4 back for 10th minute
  if (_dropFrame) {
    framesPerMin -= (nominal_fps / 15);
    framesPer10Min = framesPerMin * 10 + (nominal_fps / 15);
    framesPerHour = framesPer10Min * 6;
  }

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

void Timecode::_setTimecode(const char *c) {

  if (sscanf(c, "%2hu%*1c%2hu%*1c%2hu%*1c%2hu", &_hours, &_minutes, &_seconds,
             &_frames) != 4) {
    _hours = _minutes = _seconds = _frames = 0;
    throw(std::invalid_argument("Invalid string passed."));
  }
}

uint_fast16_t Timecode::_nominalFramerate() const {
  uint_fast16_t nominal_fps = static_cast<int>(_frameRate);

  // nominal fps for 29.97 is 30, for 59.94 is 60, for 23.98 is 24
  if (nominal_fps == 29 || nominal_fps == 23 || nominal_fps == 59)
    nominal_fps++;

  return nominal_fps;
}

uint_fast32_t Timecode::maxFrames() const {
  // 1 less than frameRate times 60 seconds, 60 minutes, 24 hours
  return _nominalFramerate() * 60 * 60 * 24 - 1;
}

void Timecode::_validate() {
  if (_hours > 23) {
    throw std::invalid_argument("Hours cannot be larger than 23 (" +
                                to_string() + " passed)");
  }
  if (_minutes > 59) {
    throw std::invalid_argument("Minutes cannot be larger than 59 (" +
                                to_string() + " passed)");
  }
  if (_seconds > 59) {
    throw std::invalid_argument("Seconds cannot be larger than 59 (" +
                                to_string() + " passed)");
  }
  if (_frames > _frameRate) {
    throw std::invalid_argument("Frames cannot be larger than framerate (" +
                                to_string() + ", " +
                                std::to_string(_frameRate) + " passed)");
  }
  if (_dropFrame && _frames == 0 && _seconds == 0 && _minutes % 10 != 0) {
    throw std::invalid_argument("Frames dropped in dropframe passed (" +
                                to_string() + " passed)");
  }
};

// Getters

uint_fast16_t Timecode::hours() const { return _hours; }

uint_fast16_t Timecode::minutes() const { return _minutes; }

uint_fast16_t Timecode::seconds() const { return _seconds; }

double Timecode::framerate() const { return _frameRate; }

bool Timecode::dropframe() const { return _dropFrame; }

uint_fast16_t Timecode::frames() const { return _frames; }

uint_fast32_t Timecode::totalFrames() const {
  uint_fast16_t nominal_fps = _nominalFramerate();
  uint_fast16_t framesPerMin = 60 * nominal_fps;
  uint_fast16_t framesPer10Min = framesPerMin * 10;
  uint_fast32_t framesPerHour = framesPer10Min * 6;
  uint_fast32_t totalFrames = 0;

  if (_dropFrame) {
    framesPerMin -= nominal_fps / 15;
    framesPer10Min = (framesPerMin * 10) + (nominal_fps / 15);
    framesPerHour = framesPer10Min * 6;
  }

  totalFrames += _hours * framesPerHour;
  totalFrames += (_minutes % 10) * framesPerMin;
  totalFrames += (_minutes / 10) * framesPer10Min;
  totalFrames += _seconds * nominal_fps;
  totalFrames += _frames;

  return totalFrames;
}

// Setters
void Timecode::hours(const uint_fast16_t &h) { _hours = h; }

void Timecode::minutes(const uint_fast16_t &m) { _minutes = m; }

void Timecode::seconds(const uint_fast16_t &s) { _seconds = s; }

void Timecode::frames(const uint_fast16_t &f) { _frames = f; }

void Timecode::framerate(const double &f) { _frameRate = f; }

void Timecode::dropframe(const bool &b) { _dropFrame = b; }

// Type conversion

Timecode::operator int() const { return totalFrames(); }

Timecode::operator const char *() const { return c_str(); }

Timecode::operator std::string() const { return to_string(); }

std::string Timecode::to_string() const { return std::string(c_str()); }

const char *Timecode::c_str() const {
  static char s[12];
  sprintf(s, "%02u%c%02u%c%02u%c%02u", _hours, _separator, _minutes, _separator,
          _seconds, _separator, _frames);
  return s;
}

// Operators
Timecode Timecode::operator+(const Timecode &t) const {
  uint_fast32_t frameSum = totalFrames() + t.totalFrames();
  frameSum %= maxFrames();
  return Timecode(frameSum, _frameRate, _dropFrame);
}

Timecode Timecode::operator+(const int &i) const {
  uint_fast32_t frameSum = totalFrames() + i;
  frameSum %= (maxFrames() + 1);
  return Timecode(frameSum, _frameRate, _dropFrame);
}

Timecode Timecode::operator-(const Timecode &t) const {
  return operator-(t.totalFrames());
}

Timecode Timecode::operator-(const int &i) const {
  int_fast32_t f = totalFrames() - i;
  if (f < 0)
    f += maxFrames();
  return Timecode(f, _frameRate, _dropFrame);
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
  return totalFrames() == t.totalFrames();
}

bool Timecode::operator!=(const Timecode &t) const { return !(*this == t); }

bool Timecode::operator<(const Timecode &t) const {
  return totalFrames() < t.totalFrames();
}

bool Timecode::operator<=(const Timecode &t) const {
  return (*this < t || *this == t);
}

bool Timecode::operator>(const Timecode &t) const {
  return totalFrames() > t.totalFrames();
}

bool Timecode::operator>=(const Timecode &t) const {
  return (*this > t || *this == t);
}

std::ostream &operator<<(std::ostream &out, const Timecode &t) {
  out << t.c_str();
  return out;
}

std::istream &operator>>(std::istream &in, Timecode &t) {
  std::string input;
  in >> input;
  t._setTimecode(input.c_str());
  return in;
}
