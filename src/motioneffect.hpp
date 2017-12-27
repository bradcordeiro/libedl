#ifndef _MOTIONEFFECT_H_
#define _MOTIONEFFECT_H_

#include "timecode.hpp"
#include <string>

class MotionEffect {
private:
  std::string _reel;
  float _speed;
  Timecode _entryPoint{};

public:
  // constructors
  MotionEffect();
  MotionEffect(const std::string &, const float &, const Timecode &);
  // getters
  std::string reel() const;
  float speed() const;
  Timecode entryPoint() const;
  // setters
  void reel(const std::string &s);
  void speed(const float &f);
  void entryPoint(const Timecode &t);
};

MotionEffect::MotionEffect() : _reel(""), _speed(0), _entryPoint(Timecode()) {}

MotionEffect::MotionEffect(const std::string &s, const float &f,
                           const Timecode &t)
    : _reel(s), _speed(f), _entryPoint(t) {}

std::string MotionEffect::reel() const { return _reel; }

float MotionEffect::speed() const { return _speed; }

Timecode MotionEffect::entryPoint() const { return _entryPoint; }

void MotionEffect::reel(const std::string &s) { _reel = s; }

void MotionEffect::speed(const float &f) { _speed = f; }

void MotionEffect::entryPoint(const Timecode &t) { _entryPoint = t; }

#endif // _MOTIONEFFECT_H_
