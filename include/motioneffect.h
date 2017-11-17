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
  explicit MotionEffect(std::string);
  MotionEffect(const std::string &, const float &, const Timecode &);
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
