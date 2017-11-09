#include "../include/edl.h"
#include "../include/event.h"
#include <fstream>
#include <sstream>
#include <string>

Edl::Edl() : _iterator(0) {}

Edl::Edl(std::ifstream &inputFile) {
  std::string line;
  Event *e;
  bool toWrite = false;

  _iterator = 0;

  getline(inputFile, line);
  setNameFromHeader(line);

  getline(inputFile, line);
  setDropFrame(line);

  getline(inputFile, line);
  while (!inputFile.eof()) {
    if (isdigit(line.front())) {
      e = new Event(line, _frameRate, _dropFrame);
      toWrite = true;
      getline(inputFile, line);
    }
    if (line.front() == 'M') {
      getline(inputFile, line); // Motion effect, discard
    }
    if (line.front() == 'F') {
      setDropFrame(line);
      getline(inputFile, line);
    }
    while (line.front() == '*' && !inputFile.eof()) {
      e->setEventClipData(line);
      getline(inputFile, line);
    }

    if (toWrite) {
      _events[_iterator++] = e;
      toWrite = false;
      e->printCsv();
    }
  }
}

// TODO
Edl::Edl(std::string) {}

void Edl::setNameFromHeader(std::string input) { setName(input.substr(9)); }

void Edl::setName(const std::string &input) { _name = input; }

// TODO: error handling
void Edl::setFrameRate(const std::string &input) {
  setFrameRate(std::stof(input));
}

void Edl::setFrameRate(const float &input) {
  int fps = input;

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

void Edl::setFrameRate(const int &input) { _frameRate = input; }

void Edl::setDropFrame(const bool &b) { _dropFrame = b; }

void Edl::setDropFrame(const std::string& input) {
  _dropFrame = (input != "FCM: NON-DROP FRAME");
}

Edl Edl::operator=(const Edl &rhs) {
  if (this != &rhs) {
    _name = rhs._name;
    _frameRate = rhs._frameRate;
    _dropFrame = rhs._dropFrame;
    _iterator = rhs._iterator;
    for (int i = 0; i < _iterator; i++) {
      *(_events[i]) = *(rhs._events[i]);
    }
  }
  return *this;
}

Edl Edl::operator++() {
  _iterator++;
  return *this;
}

Edl Edl::operator++(int) {
  Edl temp(*this);
  operator++();
  return temp;
}

Edl Edl::operator--(){
  _iterator--;
  return *this;
}

Edl Edl::operator--(int){
  Edl temp(*this);
  operator--();
  return temp;
}
