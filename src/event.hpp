#ifndef EVENT_H_
#define EVENT_H_

#include "motioneffect.hpp"
#include "timecode.hpp"
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

Event::Event()
    : _eventNumber(0), _trackType('V'), _trackNumber(1),
      _sourceStart(Timecode()), _sourceEnd(Timecode()),
      _recordStart(Timecode()), _recordEnd(Timecode()), _fps(30.0), _df(false),
      _sourceReel(""), _sourceClipName(""), _sourceFileName(""), _comment(""),
      _motionEffect(nullptr) {}

Event::Event(const Event &e) {
  _eventNumber = e._eventNumber;
  _reel = e._reel;
  _trackType = e._trackType;
  _trackNumber = e._trackNumber;
  _sourceStart = e._sourceStart;
  _sourceEnd = e._sourceEnd;
  _recordStart = e._recordStart;
  _recordEnd = e._recordEnd;
  _fps = e._fps;
  _df = e._df;
  _sourceReel = e._sourceReel;
  _sourceClipName = e._sourceClipName;
  _sourceFileName = e._sourceFileName;
  _comment = e._comment;

  _motionEffect = nullptr;
  if (e.hasMotionEffect())
    _motionEffect = new MotionEffect(*e._motionEffect);
}

Event::Event(int e, std::string r, char t, Timecode ss, Timecode se,
             Timecode rs, Timecode re, float f, bool b)
    : _eventNumber(e), _reel(r), _trackType(t), _sourceStart(ss),
      _sourceEnd(se), _recordStart(rs), _recordEnd(re), _fps(f), _df(b),
      _motionEffect(nullptr) {}

Event::~Event() { delete _motionEffect; }

int Event::eventNumber() const { return _eventNumber; }
char Event::trackType() const { return _trackType; }
int Event::trackNumber() const { return _trackNumber; }
std::string Event::reel() const { return _reel; }
Timecode Event::sourceStart() const { return _sourceStart; }
Timecode Event::sourceEnd() const { return _sourceEnd; }
Timecode Event::recordStart() const { return _recordStart; }
Timecode Event::recordEnd() const { return _recordEnd; }

MotionEffect Event::motionEffect() const {
  if (hasMotionEffect()) {
    return *_motionEffect;
  } else {
    throw std::runtime_error("No motion effect for this event.");
  }
}

bool Event::hasMotionEffect() const { return _motionEffect != nullptr; }
std::string Event::sourceReel() const { return _sourceReel; }
std::string Event::sourceClipName() const { return _sourceClipName; }
std::string Event::sourceFileName() const { return _sourceFileName; }
std::string Event::comment() const { return _comment; }

std::string Event::motionEffectReel() const {
  return hasMotionEffect() ? _motionEffect->reel() : reel();
}

double Event::motionEffectSpeed() const {
  return hasMotionEffect() ? _motionEffect->speed() : _fps;
}

Timecode Event::motionEffectEntryPoint() const {
  return hasMotionEffect() ? _motionEffect->entryPoint() : sourceStart();
}

void Event::eventNumber(const int &i) { _eventNumber = i; };
void Event::reel(const std::string &s) { _reel = s; }
void Event::trackType(const char &c) { _trackType = c; }
void Event::trackNumber(const int &i) { _trackNumber = i; }
void Event::sourceStart(const Timecode &t) { _sourceStart = t; }
void Event::sourceEnd(const Timecode &t) { _sourceEnd = t; }
void Event::recordStart(const Timecode &t) { _recordStart = t; }
void Event::recordEnd(const Timecode &t) { _sourceStart = t; }

void Event::comment(const std::string &s, bool append) {
  if (append) {
    _comment += s;
  } else {
    _comment = s;
  }
}

void Event::motionEffect(MotionEffect m) {
  delete _motionEffect;
  _motionEffect = nullptr;
  _motionEffect = new MotionEffect(m);
}
void Event::motionEffect(std::string s, float f, Timecode t) {
  delete _motionEffect;
  _motionEffect = nullptr;
  _motionEffect = new MotionEffect(s, f, t);
}
void Event::removeMotionEffect() {
  delete _motionEffect;
  _motionEffect = nullptr;
}

void Event::sourceReel(const std::string &s) { _sourceReel = s; }
void Event::sourceFileName(const std::string &s) { _sourceFileName = s; }
void Event::sourceClipName(const std::string &s) { _sourceClipName = s; }

Event Event::operator=(const Event &e) {
  if (this == &e)
    return *this;

  _eventNumber = e._eventNumber;
  _reel = e._reel;
  _trackType = e._trackType;
  _trackNumber = e._trackNumber;
  _sourceStart = e._sourceStart;
  _sourceEnd = e._sourceEnd;
  _recordStart = e._recordStart;
  _recordEnd = e._recordEnd;
  _fps = e._fps;
  _df = e._df;
  _sourceClipName = e._sourceClipName;
  _sourceFileName = e._sourceFileName;
  _comment = e._comment;

  delete _motionEffect;
  _motionEffect = nullptr;
  if (e.hasMotionEffect())
    _motionEffect = new MotionEffect(*e._motionEffect);

  return *this;
}

#endif // EVENT_H_
