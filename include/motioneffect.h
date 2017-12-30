#ifndef _MOTIONEFFECT_H_
#define _MOTIONEFFECT_H_

#include "../include/timecode.h"
#include <string>

class MotionEffect {

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

private:
  std::string _reel;
  float _speed;
  Timecode _entryPoint;
};

#endif // _MOTIONEFFECT_H_
