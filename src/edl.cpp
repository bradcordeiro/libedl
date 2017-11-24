#include "../include/edl.h"
#include "../include/event.h"
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

Edl::Edl() : _name(""), _frameRate(30), _dropFrame(false) {}

Edl::Edl(Edl &e)
    : _name(e._name), _frameRate(e._frameRate), _dropFrame(e._dropFrame),
      _events(e._events) {}

Edl::Edl(std::ifstream &inputFile) : _frameRate(30), _dropFrame(false) {
  std::string line;

  getline(inputFile, line);
  if (line.substr(0, 6) == "TITLE:") {
    _nameFromHeader(line);
    getline(inputFile, line);
  }

  while (!inputFile.eof()) {
    if (line.substr(0, 4) == "FCM:") {
      _setDropFrame(line);
      getline(inputFile, line);
    }
    if (isdigit(line.front())) {
      _events.emplace_back(Event(line));
      getline(inputFile, line);
    }
    if (line.substr(0, 2) == "M2") {
      _events.back().motionEffect(line);
      getline(inputFile, line);
    }

    while (line.front() == '*' && !inputFile.fail()) {
      _events.back()._setEventClipData(line);
      getline(inputFile, line);
    }
  }

  _events.shrink_to_fit();
}

void Edl::_nameFromHeader(std::string input) { name(input.substr(9)); }

void Edl::_setFrameRate(const std::string &input) {
  try {
    frameRate(std::stof(input));
  } catch (std::invalid_argument &e) {
    // ignore framerate change
  }
}

void Edl::_setDropFrame(const std::string &input) {
  _dropFrame = (input != "FCM: NON-DROP FRAME");
}

std::string Edl::name() const { return _name; }

void Edl::dropFrame(const bool &b) { _dropFrame = b; }

unsigned long Edl::size() const { return _events.size(); }

Event Edl::event(int i, int min, int max) const {
  if (max < 0)
    max = size() - 1;
  if (max >= min) {
    int mid = min + (max - min) / 2;
    if (_events[mid].eventNumber() == i)
      return _events[mid];
    if (_events[mid].eventNumber() > i)
      return event(i, min, --mid);
    return event(i, ++mid, max);
  }
  Event e = Event();
  e.eventNumber(-1);
  return e;
}

void Edl::name(const std::string &input) { _name = input; }

void Edl::frameRate(const double &input) {
  uint_fast16_t fps = static_cast<uint_fast16_t>(input);

  switch (fps) {
  case 29:
    _frameRate = 30000.0 / 1001.0;
    break;
  case 23:
    _frameRate = 24000.0 / 1001.0;
    break;
  case 59:
    _frameRate = 60000.0 / 1001.0;
    break;
  default:
    _frameRate = fps;
  }
}

Edl &Edl::operator=(const Edl &rhs) {
  if (this == &rhs)
    return *this;

  _name = rhs._name;
  _frameRate = rhs._frameRate;
  _dropFrame = rhs._dropFrame;
  _events = rhs._events;

  return *this;
}

Event Edl::operator[](const int &i) const {
  if (i >= size()) {
    throw std::out_of_range("Requested element is out of range.");
  } else {
    return _events[i];
  }
}
