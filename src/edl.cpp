#include "../include/edl.h"
#include "../include/event.h"
#include <fstream>
#include <stdexcept>
#include <string>

Edl::Edl() : _name(""), _frameRate(30), _dropFrame(false) {}

Edl::Edl(Edl &e)
    : _name(e._name), _frameRate(e._frameRate), _dropFrame(e._dropFrame),
      _events(std::vector<Event>(e._events)) {}

Edl::Edl(std::ifstream &inputFile) : _frameRate(30), _dropFrame(false) {
  std::string line;

  getline(inputFile, line);
  setNameFromHeader(line);

  getline(inputFile, line);
  setDropFrame(line);

  getline(inputFile, line);
  while (!inputFile.eof()) {
    if (isdigit(line.front())) {
      try {
        _events.emplace_back(Event(line, _frameRate, _dropFrame));
      } catch (std::invalid_argument) {
        // ignore incorrectly captured line
      }
    }
    if (line.front() == 'M') {
      try {
        _events.back().motionEffect(line);
      } catch (std::invalid_argument &e) {
        // ignore incorrectly captured line
      }
    }
    if (line.front() == 'F') {
      setDropFrame(line);
    }
    getline(inputFile, line);

    while (line.front() == '*' && !inputFile.fail()) {
      _events.back()._setEventClipData(line);
      getline(inputFile, line);
    }
  }
}

void Edl::setNameFromHeader(std::string input) { setName(input.substr(9)); }

void Edl::setFrameRate(const std::string &input) {
  try {
    setFrameRate(std::stof(input));
  } catch (std::invalid_argument &e) {
    // ignore framerate change
  }
}

void Edl::setFrameRate(const double &input) {
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

void Edl::setDropFrame(const std::string &input) {
  _dropFrame = (input != "FCM: NON-DROP FRAME");
}

Event Edl::event(const int &i) const {
  if (i >= size()) {
    throw std::out_of_range("Requested element is out of range.");
  } else {
    return _events[i];
  }
}

Edl &Edl::operator=(const Edl &rhs) {
  if (this == &rhs) return *this;

  _name = rhs._name;
  _frameRate = rhs._frameRate;
  _dropFrame = rhs._dropFrame;
  _events = std::vector<Event>(rhs._events);

  return *this;
}

Event Edl::operator[](const int &i) const { return event(i); }
