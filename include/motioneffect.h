#ifndef _MOTIONEFFECT_H_
#define _MOTIONEFFECT_H_

#include "timecode.h"
#include <string>

class MotionEffect {
private:
  std::string _reel;
  float _speed;
  Timecode _entryPoint;

public:
  // constructors
  MotionEffect();
  MotionEffect(const std::string &, const float &, const Timecode &);
  explicit MotionEffect(std::string);
  // getters
  std::string reel() const;
  float speed() const;
  Timecode entryPoint() const;
  // setters
  void reel(const std::string &s);
  void speed(const float &f);
  void entryPoint(const Timecode &t);
  void entryPoint(const std::string &s);
};

#endif // _MOTIONEFFECT_H_
