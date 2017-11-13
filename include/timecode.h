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
  double _frameRate;
  bool _dropFrame;
  char _separator;

  char _setSeparator();
  void _setTimecode(uint_fast32_t &);
  void _setTimecode(const char *);
  uint_fast16_t _nominalFramerate() const;
  uint_fast32_t _maxFrames() const;
  inline char separator() const { return _separator; }
  inline void separator(char c) { _separator = c; }
  void _validate();

public:
  // constructors
  explicit Timecode(uint_fast16_t = 0, uint_fast16_t = 0, uint_fast16_t = 0,
                    uint_fast16_t = 0, double = 30, bool b = false);
  explicit Timecode(uint_fast32_t, double = 30.0, bool = false);
  explicit Timecode(const char *, double = 30.0, bool = false);
  explicit Timecode(const std::string, double = 30.0, bool = false);

  // getters
  inline uint_fast16_t hours() const { return _hours; }
  inline uint_fast16_t minutes() const { return _minutes; }
  inline uint_fast16_t seconds() const { return _seconds; }
  inline uint_fast16_t frames() const { return _frames; }
  inline double framerate() const { return _frameRate; }
  inline bool dropframe() const { return _dropFrame; }
  uint_fast32_t totalFrames() const;
  // setters
  inline void hours(uint_fast16_t h) { _hours = h; }
  inline void minutes(uint_fast16_t m) { _minutes = m; }
  inline void seconds(uint_fast16_t s) { _seconds = s; }
  inline void frames(uint_fast16_t f) { _frames = f; }
  inline void framerate(double f) { _frameRate = f; }
  inline void dropframe(bool b) { _dropFrame = b; }

  // type conversion
  operator int() const;
  operator char *() const;
  operator std::string() const;
  char *c_str() const;
  std::string to_string() const;

  // operators
  Timecode operator+(const Timecode &) const;
  Timecode operator+(const int &i) const;
  Timecode operator-(const Timecode &) const;
  Timecode operator-(const int &i) const;
  Timecode &operator=(const Timecode &);
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
