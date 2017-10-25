#ifndef _TIMECODE_H_
#define _TIMECODE_H_

#include <string>

class Timecode {
private:
  int _hours;
  int _minutes;
  int _seconds;
  int _frames;
  float _frameRate;
  bool _dropFrame;
  void setTimecode(const std::string &);
  void setTimecode(char*);
  char separator() const { return isDropFrame() ? ';' : ':'; }

public:
  // constructors
  Timecode();
  Timecode(const std::string &, float, bool);
  Timecode(const std::string &, int, bool);
  Timecode(int, int, int, int, float, bool);
  Timecode(int, int, int, int, int, bool);
  Timecode(int, float, bool);
  // getters
  int frames() const { return _frames; }
  int seconds() const { return _seconds; }
  int minutes() const { return _minutes; }
  int hours() const { return _hours; }
  float frameRate() const { return _frameRate; }
  bool isDropFrame() const { return _dropFrame; }
  // setters
  void frames(int f) { _frames = f; }
  void seconds(int s) { _seconds = s; }
  void minutes(int m) { _minutes = m; }
  void hours(int h){ _hours = h; }
  // miscellaneous
  std::string to_string() const;
  // operators
  friend std::ostream &operator<<(std::ostream &, const Timecode &);
  friend std::istream &operator>>(std::istream &, Timecode &);
};
#endif //_TIMECODE_H_
