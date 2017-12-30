#include "../include/motioneffect.h"

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
