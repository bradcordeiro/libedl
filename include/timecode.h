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
  explicit Timecode(std::string, double = 30.0, bool = false);

  // getters
  uint_fast16_t hours() const;
  uint_fast16_t minutes() const;
  uint_fast16_t seconds() const;
  uint_fast16_t frames() const;
  double framerate() const;
  bool dropframe() const;
  uint_fast32_t totalFrames() const;
  // setters
  void hours(const uint_fast16_t &);
  void minutes(const uint_fast16_t &);
  void seconds(const uint_fast16_t &);
  void frames(const uint_fast16_t &);
  void framerate(const double &);
  void dropframe(const bool &);

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
