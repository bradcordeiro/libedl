#include "../include/event.h"
#include "../include/timecode.h"
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>

Event::Event()
    : _eventNumber(0), _trackType('V'), _trackNumber(1), _fps(30.0), _df(false),
      _sourceStart(Timecode()), _sourceEnd(Timecode()),
      _recordStart(Timecode()), _recordEnd(Timecode()), _sourceClipName(""),
      _sourceFileName(""), _comment(""), _motionEffect(0) {}

Event::Event(const std::string &line, const double &f, const bool &b)
    : _eventNumber(0), _trackType('V'), _trackNumber(1), _fps(f), _df(b),
      _sourceStart(Timecode()), _sourceEnd(Timecode()),
      _recordStart(Timecode()), _recordEnd(Timecode()), _sourceClipName(""),
      _sourceFileName(""), _comment(""), _motionEffect(0) {
  _parseEvent(line);
}

Event::~Event() { removeMotionEffect(); }

void Event::_setEventClipData(const std::string &s) {
  if (s.compare(0, 17, "* FROM CLIP NAME:") == 0) {
    sourceClipName(s.substr(18));
  } else if (s.compare(0, 8, "* REEL:") == 0) {
    reel(s.substr(7));
  } else {
    comment(s.substr(2));
  }
}

void Event::_parseEvent(const std::string &line) {
  int eventNumberRead;
  char reelRead[9];
  char trackTypeRead;
  int trackNumberRead;
  char sstart[12];
  char send[12];
  char rstart[12];
  char rend[12];

  if (sscanf(line.c_str(), "%d %s %c %*s %*s %s %s %s %s", &eventNumberRead,
             reelRead, &trackTypeRead, sstart, send, rstart, rend) == 7) {
    // 077  EVL1_HIG V     W001 010 00:02:51:25 00:02:52:07 01:00:28:10
    // 01:00:28:23
    _eventNumber = eventNumberRead;
    _reel = reelRead;
    _trackType = trackTypeRead;
    _sourceStart = Timecode(sstart, _fps, _df);
    _sourceEnd = Timecode(send, _fps, _df);
    _recordStart = Timecode(rstart, _fps, _df);
    _recordEnd = Timecode(rend, _fps, _df);
  } else if (sscanf(line.c_str(), "%d %s %c %*c %s %s %s %s", &eventNumberRead,
                    reelRead, &trackTypeRead, sstart, send, rstart,
                    rend) == 7) {
    // 008  EVL1_HEI V     C        00:02:46:15 00:02:47:02 01:00:01:05
    // 01:00:01:16
    _eventNumber = eventNumberRead;
    _reel = reelRead;
    _trackType = trackTypeRead;
    _sourceStart = Timecode(sstart, _fps, _df);
    _sourceEnd = Timecode(send, _fps, _df);
    _recordStart = Timecode(rstart, _fps, _df);
    _recordEnd = Timecode(rend, _fps, _df);
  } else if (sscanf(line.c_str(), "%d %s %c%d %*c %s %s %s %s",
                    &eventNumberRead, reelRead, &trackTypeRead,
                    &trackNumberRead, sstart, send, rstart, rend) == 8) {
    // 0064 EP_107 A34    C        01:02:02:08 01:02:02:16 01:03:42:02
    // 01:03:42:10
    _eventNumber = eventNumberRead;
    _reel = reelRead;
    _trackType = trackTypeRead;
    _trackNumber = trackNumberRead;
    _sourceStart = Timecode(sstart, _fps, _df);
    _sourceEnd = Timecode(send, _fps, _df);
    _recordStart = Timecode(rstart, _fps, _df);
    _recordEnd = Timecode(rend, _fps, _df);
  } else {
    throw std::invalid_argument("Invalid event string");
  }
}

int_fast16_t Event::eventNumber() const { return _eventNumber; }
char Event::trackType() const { return _trackType; }
uint_fast16_t Event::trackNumber() const { return _trackNumber; }
std::string Event::reel() const { return _reel; }
std::string Event::sourceClipName() const { return _sourceClipName; }
std::string Event::sourceFileName() const { return _sourceFileName; }
std::string Event::comment() const { return _comment; }
Timecode Event::sourceStart() const { return _sourceStart; }
Timecode Event::sourceEnd() const { return _sourceEnd; }
Timecode Event::recordStart() const { return _recordStart; }
Timecode Event::recordEnd() const { return _recordEnd; }
MotionEffect Event::motionEffect() const { return *_motionEffect; }
bool Event::hasMotionEffect() const { return _motionEffect != 0; }

std::string Event::motionEffectReel() const {
  return hasMotionEffect() ? _motionEffect->reel() : reel();
}

double Event::motionEffectSpeed() const {
  return hasMotionEffect() ? _motionEffect->speed() : _fps;
}

Timecode Event::motionEffectEntryPoint() const {
  return hasMotionEffect() ? _motionEffect->entryPoint() : sourceStart();
}

void Event::dropFrame(const bool &b) { _df = b; }
void Event::eventNumber(const int_fast16_t &i) { _eventNumber = i; };
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

void Event::motionEffect(const std::string s) {
  _motionEffect = new MotionEffect(s);
}

void Event::motionEffect(const MotionEffect &m) {
  _motionEffect = new MotionEffect(m);
}

void Event::removeMotionEffect() {
  delete _motionEffect;
  _motionEffect = 0;
}

void Event::sourceFileName(const std::string &s) { _sourceFileName = s; }
void Event::sourceClipName(const std::string &s) { _sourceClipName = s; }

std::ostream &operator<<(std::ostream &out, const Event &e) {
  out << std::setw(3) << std::setfill('0') << e._eventNumber << "  "
      << std::left << std::setw(8) << std::setfill(' ') << e._reel << " "
      << std::setw(6) << e._trackType << std::setw(9)
      << 'C' // TODO: Should show proper transition
      << std::setw(12) << e._sourceStart << std::setw(12) << e._sourceEnd
      << std::setw(12) << e._recordStart << std::setw(12) << e._recordEnd
      << std::endl;
  if (e.hasMotionEffect()) {
    out << std::setw(5) << std::left << "M2" << std::setw(15) << std::left
        << e.motionEffectReel() << std::setw(21) << std::left
        << e.motionEffectSpeed() << e.motionEffectEntryPoint() << std::endl;
  }

  if (!e._comment.empty()) {
    for (int i = 0; i < e._comment.size(); i += 80) {
      out << "* " << e._comment.substr(i, 80) << std::endl;
    }
  }

  if (!e._sourceClipName.empty()) {
    out << "* FROM CLIP NAME:  " << e._sourceClipName << std::endl;
  }

  if (!e._sourceFileName.empty()) {
    out << "* FROM FILE: " << e._sourceFileName << std::endl;
  }

  out << std::resetiosflags(std::ios_base::adjustfield);

  return out;
}

std::istream &operator>>(std::istream &in, Event &e) {
  std::string input;
  e._parseEvent(input);
  return in;
}
