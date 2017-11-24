#include "../include/motioneffect.h"
#include "../include/timecode.h"
#include <string>

MotionEffect::MotionEffect() : _speed(0), _entryPoint(Timecode()) {}

MotionEffect::MotionEffect(const MotionEffect &m) : _reel(m._reel), _speed(m._speed), _entryPoint(m._entryPoint) {

}

MotionEffect::MotionEffect(std::string s)
    : _reel(""), _speed(0), _entryPoint(Timecode()) {
  char reelRead[9];
  float speedRead;
  char timecodeRead[12];

  if (sscanf(s.c_str(), "%*s %s %f %s", reelRead, &speedRead, timecodeRead) ==
      3) {
    _reel = std::string(reelRead);
    _speed = speedRead;
    _entryPoint = Timecode(timecodeRead);
  } else {
    throw std::invalid_argument("Invalid motion effect line");
  }
}

MotionEffect::MotionEffect(const std::string &s, const float &d,
                           const Timecode &t)
    : _reel(s), _speed(d), _entryPoint(t) {}

std::string MotionEffect::reel() const { return _reel; }

float MotionEffect::speed() const { return _speed; }

Timecode MotionEffect::entryPoint() const { return _entryPoint; }

void MotionEffect::reel(const std::string &s) { _reel = s; }

void MotionEffect::speed(const float &f) { _speed = f; }

void MotionEffect::entryPoint(const Timecode &t) { _entryPoint = t; }

void MotionEffect::entryPoint(const std::string &s) {
  _entryPoint = Timecode(s);
}
