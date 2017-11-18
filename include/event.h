#ifndef EVENT_H_
#define EVENT_H_

#include "../include/motioneffect.h"
#include "../include/timecode.h"
#include <iostream>
#include <string>

typedef Timecode tc; // for brevity

class Event {

  friend class Edl;

private:
  int_fast16_t _eventNumber;
  char _trackType;
  uint_fast16_t _trackNumber;
  double _fps;
  bool _df;
  Timecode _sourceStart;
  Timecode _sourceEnd;
  Timecode _recordStart;
  Timecode _recordEnd;
  std::string _reel;
  std::string _sourceClipName;
  std::string _sourceFileName;
  std::string _comment;
  MotionEffect _motionEffect;
  void _setEventClipData(const std::string &s);
  void _parseEvent(const std::string &);

public:
  // constructors
  Event();
  explicit Event(const std::string&, const double& = 30.0, const bool& = false);
  // getters
  inline int_fast16_t eventNumber() const { return _eventNumber; }
  inline char trackType() const { return _trackType; }
  inline uint_fast16_t trackNumber() const { return _trackNumber; } // TODO
  inline std::string reel() const { return _reel; }
  inline std::string sourceClipName() const { return _sourceClipName; }
  inline std::string sourceFileName() const { return _sourceFileName; }
  inline std::string comment() const { return _comment; }
  inline Timecode sourceStart() const { return _sourceStart; }
  inline Timecode sourceEnd() const { return _sourceEnd; }
  inline Timecode recordStart() const { return _recordStart; }
  inline Timecode recordEnd() const { return _recordEnd; }
  inline MotionEffect motionEffect() const { return _motionEffect; }
  std::string motionEffectReel() const;
  double motionEffectSpeed() const;
  Timecode motionEffectEntryPoint() const;
  inline bool hasMotionEffect() const { return _motionEffect.speed() != 0; }
  // setters
  void dropFrame(const bool &b) { _df = b; }
  void eventNumber(const int_fast16_t &i) { _eventNumber = i; };
  void reel(const std::string &s) { _reel = s; }
  void trackType(const char &c) { _trackType = c; }
  void sourceStart(const std::string &s) { _sourceStart = tc(s, _fps, _df); }
  void sourceStart(const int &i) { _sourceStart = tc(i, _fps, _df); }
  void sourceStart(const Timecode &t) { _sourceStart = t; }
  void sourceEnd(const std::string &s) { _sourceEnd = tc(s, _fps, _df); }
  void sourceEnd(int &i) { _sourceEnd = tc(i, _fps, _df); }
  void sourceEnd(const Timecode &t) { _sourceEnd = t; }
  void recordStart(const std::string &s) { _recordStart = tc(s, _fps, _df); }
  void recordStart(const int &i) { _recordStart = tc(i, _fps, _df); }
  void recordStart(const Timecode &t) { _recordStart = t; }
  void recordEnd(const std::string &s) { _recordEnd = tc(s, _fps, _df); }
  void recordEnd(const int &i) { _recordEnd = tc(i, _fps, _df); }
  void recordEnd(const Timecode &t) { _sourceStart = t; }
  void comment(const std::string &, bool = true);
  void motionEffect(const std::string);
  void motionEffect(const MotionEffect &);
  void sourceFileName(const std::string &s) { _sourceFileName = s; }
  void sourceClipName(const std::string &s) { _sourceClipName = s; }
  // operator overloads
  friend std::ostream &operator<<(std::ostream &, const Event &);
  friend std::istream &operator>>(std::istream &, Event &);
};

#endif // EVENT_H_
