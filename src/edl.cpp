#include "../include/edl.h"

Edl::Edl() : _name("") {}

Edl::Edl(std::ifstream &inputFile) {
  std::string line;
  bool dropframe = false;
  float framerate = 30.0;
  Event e;

  getline(inputFile, line);
  while (!inputFile.eof()) {
    // uses multiple ifs since multipe conditions can be satisfied in sequences
    if (line.front() == 'T') {
      _nameFromHeader(line);
      getline(inputFile, line);
    }
    if (line.front() == 'F') {
      dropframe = _readDropFrame(line);
      getline(inputFile, line);
    }
    if (isdigit(line.front())) {
      e = _parseEvent(line, framerate, dropframe);
      getline(inputFile, line);
    }
    if (line.front() == 'M') {
      e.motionEffect(_parseMotionEffect(line, e.sourceStart()));
      getline(inputFile, line);
    }
    if (line.front() == '*') {
      while (line.front() == '*' && !inputFile.eof()) {
        _parseComment(e, line);
        getline(inputFile, line);
      }
    }
    _events.push_back(e);
  }
}

void Edl::_nameFromHeader(std::string input) { name(input.substr(9)); }

bool Edl::_readDropFrame(const std::string &input) {
  return input != "FCM: NON-DROP FRAME";
}

Event Edl::_parseEvent(const std::string &line, const float &f, const bool &b) {
  Event e;
  int enr;
  char rr[9];
  char ttr;
  int tnr = 1;
  char start[12];
  char end[12];
  char rstart[12];
  char rend[12];

  if (sscanf(line.c_str(), "%d %s %c %*s %*s %s %s %s %s", &enr, rr, &ttr,
             start, end, rstart, rend) == 7) {
    // 077  EVL1_HIG V     W001 010 00:02:51:25 00:02:52:07 01:00:28:10
    // 01:00:28:23
    Timecode ss(start, f, b);
    Timecode se(end, f, b);
    Timecode rs(rstart, f, b);
    Timecode re(rend, f, b);
    std::string r(rr);
    return Event(enr, r, ttr, tnr, ss, se, rs, re, f, b);
  } else if (sscanf(line.c_str(), "%d %s %c %*c %s %s %s %s", &enr, rr, &ttr,
                    start, end, rstart, rend) == 7) {
    // 008  EVL1_HEI V     C        00:02:46:15 00:02:47:02 01:00:01:05
    // 01:00:01:16
    Timecode ss(start, f, b);
    Timecode se(end, f, b);
    Timecode rs(rstart, f, b);
    Timecode re(rend, f, b);
    std::string r(rr);
    return Event(enr, r, ttr, tnr, ss, se, rs, re, f, b);
  } else if (sscanf(line.c_str(), "%d %s %c%d %*c %s %s %s %s", &enr, rr, &ttr,
                    &tnr, start, end, rstart, rend) == 8) {
    // 0064 EP_107 A34    C        01:02:02:08 01:02:02:16 01:03:42:02
    // 01:03:42:10
    Timecode ss(start, f, b);
    Timecode se(end, f, b);
    Timecode rs(rstart, f, b);
    Timecode re(rend, f, b);
    std::string r(rr);
    return Event(enr, r, ttr, tnr, ss, se, rs, re, f, b);
  } else {
    throw std::invalid_argument("Invalid event string");
  }
}

MotionEffect Edl::_parseMotionEffect(const std::string &s, const Timecode &t) {
  char r[9];
  float f;
  char e[12];
  MotionEffect m;

  if (sscanf(s.c_str(), "%*s %s %f %s", r, &f, e) == 3) {
    Timecode x(e, t.framerate(), t.dropframe());
    m = MotionEffect(std::string(r), f, x);
  } else {
    throw std::invalid_argument("Invalid motion effect line\n" + s);
  }
  return m;
}

void Edl::_parseComment(Event &e, std::string &line) {
  if (line.substr(2, 4) == "REEL") {
    e.sourceReel(line.substr(8));
  } else if (line.substr(2, 14) == "FROM CLIP NAME") {
    e.sourceClipName(line.substr(19));
  } else {
    e.comment(line.substr(2));
  }
}

bool Edl::_eventcmp(const Event &a, const Event &b) {
  return a.eventNumber() < b.eventNumber();
}

std::string Edl::name() const { return _name; }

int Edl::size() const { return static_cast<int>(_events.size()); }

Event *Edl::find_event(int x, int min, int max) {
  if (x < 0 || x >= size()) {
    return nullptr;
  }

  if (max < 0) {
    max = _events.size() - 1;
  }

  if (max >= min) {
    int mid = min + (((max - min) / 2) >> 1);
    if (_events[mid]._eventNumber == x) {
      return &_events[mid];
    } else if (_events[mid]._eventNumber > x) {
      return find_event(x, min, --mid);
    }
    return find_event(x, ++mid, max);
  }

  return nullptr;
}

void Edl::name(const std::string &input) { _name = input; }

void Edl::sort() {
  bool (*compare)(const Event &, const Event &) = &_eventcmp;
  std::stable_sort(_events.begin(), _events.end(), compare);
}

Event Edl::operator[](const int &i) const {
  if (i >= size()) {
    throw std::out_of_range("Requested element is out of range.");
  } else {
    return _events[i];
  }
}
