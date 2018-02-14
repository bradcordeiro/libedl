#ifndef _TIMECODE_H_
#define _TIMECODE_H_

#include <cstdint>
#include <string>

class Timecode {
public:
  // constructors
  Timecode();
  Timecode(unsigned int, unsigned int, unsigned int, unsigned int,
           double = 30.0, bool = false);
  Timecode(uint_fast32_t, double = 30.0, bool = false);
  Timecode(const std::string &, const float & = 30.0, const bool & = false);

  // static member functions
  static std::string framesToString(uint_fast32_t, double, bool);

  // getters
  unsigned int hours() const;
  unsigned int minutes() const;
  unsigned int seconds() const;
  unsigned int frames() const;
  float framerate() const;
  bool dropframe() const;

  uint_fast32_t totalFrames() const;
  // setters
  void hours(const unsigned int &);
  void minutes(const unsigned int &);
  void seconds(const unsigned int &);
  void frames(const unsigned int &);
  void framerate(const float &);
  void dropframe(const bool &);

  // type conversion
  operator int() const;
  std::string to_string() const;

  // operators
  Timecode operator+(const Timecode &) const;
  Timecode operator+(const int &i) const;
  Timecode operator-(const Timecode &) const;
  Timecode operator-(const int &i) const;
  Timecode operator*(const int &i) const;
  bool operator==(const Timecode &) const;
  bool operator!=(const Timecode &) const;
  bool operator<(const Timecode &) const;
  bool operator<=(const Timecode &) const;
  bool operator>(const Timecode &) const;
  bool operator>=(const Timecode &) const;

  friend std::ostream &operator<<(std::ostream &, const Timecode &);

private:
  uint_fast16_t _hours;
  uint_fast16_t _minutes;
  uint_fast16_t _seconds;
  uint_fast16_t _frames;
  double _frameRate;
  bool _dropFrame;

  uint_fast32_t _maxFrames() const;
  char _separator() const;
  void _setTimecode(uint_fast32_t &);
  void _setTimecode(const char *);
  uint_fast16_t _nominalFramerate() const;
  void _validate();
};

#endif //_TIMECODE_H_
