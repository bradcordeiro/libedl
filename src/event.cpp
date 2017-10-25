#include "../include/event.h"
#include <sstream>

Event::Event() {
  _comment = "";
};

Event::Event(const std::string &line, float frameRate, bool dropframe) {
  _frameRate = frameRate;
  _dropFrame = dropframe;

  std::istringstream buffer(line);
  std::string read;

  buffer >> read;
  eventNumber(read);
  buffer >> read;
  reel(read);
  buffer >> read;
  trackType(read);
  buffer >> read; // discard edit type
    if (read.front() == 'W')
      buffer >> read; // discard wipe info
  buffer >> read;
  sourceStartTimecode(read);
  buffer >> read;
  sourceEndTimecode(read);
  buffer >> read;
  recordStartTimecode(read);
  buffer >> read;
  recordEndTimecode(read);
};

Event::Event(const std::string &line, int frameRate, bool dropframe){}

void Event::dropFrame(const bool &b) { _dropFrame = b; }

void Event::eventNumber(const int &n) { _eventNumber = n; }

// TODO: error handling
void Event::eventNumber(const std::string &s) {
  try {
    _eventNumber = std::stoi(s);
  } catch (std::invalid_argument&) {
    std::cout << "Error processing line: " << std::endl;
    std::cout << s;
    exit(EXIT_FAILURE);
  }
}

void Event::reel(const std::string &s) { _reel = s; }

void Event::trackType(const char &c) { _trackType = c; }

void Event::trackType(const std::string &s) {
  _trackType = s.front();
  if (s.size() > 1) {
    trackNumber(s.substr(1));
  }
}

void Event::trackNumber(const int &n) { _trackNumber = n; }

void Event::trackNumber(const std::string &s) {
  try{
  _trackNumber = stoi(s);
  } catch (std::invalid_argument&) {
    std::cout << "Error processing line: " << std::endl;
    std::cout << s;
    exit(EXIT_FAILURE);
  }
}

void Event::sourceStartTimecode(const std::string &s) {
  _sourceStart = Timecode(s, _frameRate, _dropFrame);
}

void Event::sourceEndTimecode(const std::string &s) {
  _sourceEnd = Timecode(s, _frameRate, _dropFrame);
}

void Event::recordStartTimecode(const std::string &s) {
  _recordStart = Timecode(s, _frameRate, _dropFrame);
}

void Event::recordEndTimecode(const std::string &s) {
  _recordEnd = Timecode(s, _frameRate, _dropFrame);
}

void Event::setEventClipData(const std::string &s) {
  if (s.compare(0, 17, "* FROM CLIP NAME:") == 0) {
    sourceClipName(s.substr(18));
  } else if (s.compare(0, 8, "* REEL:") == 0) {
    reel(s.substr(7));
  } else {
    comment(s.substr(2));
  }
}

void Event::sourceClipName(const std::string &s) { _sourceClipName = s; }

void Event::comment(const std::string &s) { _comment += s; }

void Event::printCsv() {
  std::cout << _eventNumber << ',' << _reel << ',' << _trackType << ','
            << _trackNumber << ',' << _sourceStart << ',' << _sourceEnd << ','
            << _recordStart << ',' << _recordEnd << ',' <<_sourceClipName << ','
            << _comment << std::endl;
}
