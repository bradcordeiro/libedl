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

Event::~Event() {
  if (_motionEffect)
    delete _motionEffect;
}

std::string Event::motionEffectReel() const {
  return hasMotionEffect() ? _motionEffect->reel() : reel();
}

double Event::motionEffectSpeed() const {
  return hasMotionEffect() ? _motionEffect->speed() : _fps;
}

Timecode Event::motionEffectEntryPoint() const {
  return hasMotionEffect() ? _motionEffect->entryPoint() : sourceStart();
}

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
  uint_fast16_t eventNumberRead;
  char reelRead[9];
  char trackTypeRead;
  char sstart[12];
  char send[12];
  char rstart[12];
  char rend[12];

  if (sscanf(line.c_str(), "%3hu %s %c %*s %*s %s %s %s %s", &eventNumberRead,
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
  } else if (sscanf(line.c_str(), "%3hu %s %c %*c %s %s %s %s",
                    &eventNumberRead, reelRead, &trackTypeRead, sstart, send,
                    rstart, rend) == 7) {
    // 008  EVL1_HEI V     C        00:02:46:15 00:02:47:02 01:00:01:05
    // 01:00:01:16
    _eventNumber = eventNumberRead;
    _reel = reelRead;
    _trackType = trackTypeRead;
    _sourceStart = Timecode(sstart, _fps, _df);
    _sourceEnd = Timecode(send, _fps, _df);
    _recordStart = Timecode(rstart, _fps, _df);
    _recordEnd = Timecode(rend, _fps, _df);
  } else {
    throw std::invalid_argument("Invalid event string");
  }
}

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
    for (int i = 0; i < e._comment.size(); i += 80){
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
