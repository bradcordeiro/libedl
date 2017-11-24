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
  MotionEffect *_motionEffect;
  void _setEventClipData(const std::string &s);
  void _parseEvent(const std::string &);

public:
  // constructors
  Event();
  explicit Event(const std::string &, const double & = 30.0,
                 const bool & = false);
  ~Event();
  // getters
  int_fast16_t eventNumber() const;
  char trackType() const;
  uint_fast16_t trackNumber() const;
  std::string reel() const;
  std::string sourceClipName() const;
  std::string sourceFileName() const;
  std::string comment() const;
  Timecode sourceStart() const;
  Timecode sourceEnd() const;
  Timecode recordStart() const;
  Timecode recordEnd() const;
  MotionEffect motionEffect() const;
  bool hasMotionEffect() const;
  std::string motionEffectReel() const;
  double motionEffectSpeed() const;
  Timecode motionEffectEntryPoint() const;

  // setters
  void dropFrame(const bool &b);
  void eventNumber(const int_fast16_t &i);
  void reel(const std::string &s);
  void trackType(const char &c);
  void trackNumber(const int &i);
  void sourceStart(const Timecode &t);
  void sourceEnd(const Timecode &t);
  void recordStart(const Timecode &t);
  void recordEnd(const Timecode &t);
  void comment(const std::string &, bool = true);
  void motionEffect(const std::string);
  void motionEffect(const MotionEffect &);
  void removeMotionEffect();
  void sourceFileName(const std::string &s);
  void sourceClipName(const std::string &s);
  // operator overloads
  friend std::ostream &operator<<(std::ostream &, const Event &);
  friend std::istream &operator>>(std::istream &, Event &);
};

#endif // EVENT_H_
