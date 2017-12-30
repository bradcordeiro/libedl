#ifndef EVENT_H_
#define EVENT_H_

#include "motioneffect.h"
#include "timecode.h"
#include <iostream>
#include <string>

class Event {

  friend class Edl;

public:
  // constructors
  Event();
  Event(const Event &);
  Event(int, std::string, char, Timecode, Timecode, Timecode, Timecode, float,
        bool);
  // destructor
  ~Event();
  // getters
  int eventNumber() const;
  char trackType() const;
  int trackNumber() const;
  std::string reel() const;
  Timecode sourceStart() const;
  Timecode sourceEnd() const;
  Timecode recordStart() const;
  Timecode recordEnd() const;
  bool hasMotionEffect() const;
  MotionEffect motionEffect() const;
  std::string motionEffectReel() const;
  double motionEffectSpeed() const;
  Timecode motionEffectEntryPoint() const;
  std::string sourceReel() const;
  std::string sourceClipName() const;
  std::string sourceFileName() const;
  std::string comment() const;

  // setters
  void eventNumber(const int &i);
  void reel(const std::string &s);
  void trackType(const char &c);
  void trackNumber(const int &i);
  void sourceStart(const Timecode &t);
  void sourceEnd(const Timecode &t);
  void recordStart(const Timecode &t);
  void recordEnd(const Timecode &t);
  void motionEffect(MotionEffect);
  void motionEffect(std::string, float, Timecode);
  void addMotionEffect();
  void removeMotionEffect();
  void sourceReel(const std::string &);
  void sourceFileName(const std::string &);
  void sourceClipName(const std::string &);
  void comment(const std::string &, bool = true);

  // operators
  Event operator=(const Event &);

private:
  int _eventNumber;
  std::string _reel;
  char _trackType;
  int _trackNumber;
  Timecode _sourceStart;
  Timecode _sourceEnd;
  Timecode _recordStart;
  Timecode _recordEnd;
  float _fps;
  bool _df;
  std::string _sourceReel;
  std::string _sourceClipName;
  std::string _sourceFileName;
  std::string _comment;
  MotionEffect *_motionEffect;
};

#endif // EVENT_H_
