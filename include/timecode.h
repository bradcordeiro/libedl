#ifndef _TIMECODE_H_
#define _TIMECODE_H_

#include <cstdint>
#include <string>

class Timecode {
private:
  uint_fast16_t _hours;
  uint_fast16_t _minutes;
  uint_fast16_t _seconds;
  uint_fast16_t _frames;
  float _frameRate;
  bool _dropFrame;
  char _separator;
  void setSeparator() { _separator = (_dropFrame ? ';' : ':'); }
  void setTimecode(int);
  void setTimecode(const std::string &);
  void setTimecode(const char *);
  void validate();

public:
  // constructors
  Timecode(int = 0, int = 0, int = 0, int = 0 , float = 30, bool b = false);
  Timecode(int, float = 30.0, bool = false);
  Timecode(const std::string &, float = 30.0, bool = false);
  Timecode(const char *, float = 30.0, bool = false);

  // getters
  inline uint_fast16_t hours() const { return _hours; }
  inline uint_fast16_t minutes() const { return _minutes; }
  inline uint_fast16_t seconds() const { return _seconds; }
  inline uint_fast16_t frames() const { return _frames; }
  inline float framerate() const { return _frameRate; }
  inline bool dropframe() const { return _dropFrame; }
  inline char separator() const { return _separator; }
  uint_fast32_t totalFrames() const;
  // setters
  inline void hours(int h) { _hours = h; }
  inline void minutes(int m) { _minutes = m; }
  inline void seconds(int s) { _seconds = s; }
  inline void frames(int f) { _frames = f; }
  inline void framerate(float f) { _frameRate = f; }
  inline void dropframe(bool b) { _dropFrame = b; }
  inline void separator(char c) { _separator = c; }
  // type conversion
  operator int() const;
  operator char *() const;
  operator std::string() const;
  std::string to_string() const;
  char *c_str() const;

  // operators
  Timecode operator+(const Timecode &) const;

  Timecode operator-(const Timecode &) const;
  Timecode& operator=(const Timecode &);
  bool operator==(const Timecode &) const;
  bool operator!=(const Timecode &) const;
  bool operator<(const Timecode &) const;
  bool operator<=(const Timecode &) const;
  bool operator>(const Timecode &) const;
  bool operator>=(const Timecode &) const;
  friend std::ostream &operator<<(std::ostream &, const Timecode &);
  friend std::istream &operator>>(std::istream &, Timecode &);
};

#endif //_TIMECODE_H_
