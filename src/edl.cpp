#include "../include/edl.h"
#include "../include/event.h"
#include <stdexcept>
#include <fstream>
#include <string>

Edl::Edl() : _size(0) {}

Edl::Edl(Edl &e) {
  _name = e._name;
  _frameRate = e._frameRate;
  _dropFrame = e._dropFrame;
  _size = e._size;
  for (int i = 0; i < _size; i++) {
    _events[i] = new Event;
    *(_events[i]) = *(e._events[i]);
  }
}

Edl::Edl(std::ifstream &inputFile)
    : _frameRate(30), _dropFrame(false), _size(0) {
  std::string line;
  Event *e;
  bool toWrite = false;

  getline(inputFile, line);
  setNameFromHeader(line);

  getline(inputFile, line);
  setDropFrame(line);

  getline(inputFile, line);
  while (!inputFile.eof()) {
    if (isdigit(line.front())) {
      e = new Event(line, _frameRate, _dropFrame);
      toWrite = true;
    }
    if (line.front() == 'M') {
      // TODO: Handle motion effect
    }
    if (line.front() == 'F') {
      setDropFrame(line);
    }
    getline(inputFile, line);

    while (line.front() == '*' && !inputFile.fail()) {
      e->_setEventClipData(line);
      getline(inputFile, line);
    }

    if (toWrite) {
      _events[_size++] = e;
      std::cout << *e;
      toWrite = false;
    }
  }
}

// TODO
Edl::Edl(std::string) {}

void Edl::setNameFromHeader(std::string input) { setName(input.substr(9)); }

// TODO: error handling
void Edl::setFrameRate(const std::string &input) {
  setFrameRate(std::stof(input));
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
    if (i >= _size) {
      throw std::out_of_range("");
    } else {
      return *(_events[i]);
    }
}

Edl& Edl::operator=(const Edl &rhs) {
  if (this != &rhs) {
    _name = rhs._name;
    _frameRate = rhs._frameRate;
    _dropFrame = rhs._dropFrame;
    _size = rhs._size;
    for (int i = 0; i < _size; i++) {
        _events[i] = rhs._events[i];
    }
  }
  return *this;
}

Event Edl::operator[](const int&i) const {
  return event(i);
}
