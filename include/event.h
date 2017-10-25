#ifndef EVENT_H_
#define EVENT_H_

#include "../include/timecode.h"
#include <iostream>
#include <sstream>
#include <string>

class Event {
private:
  // maximum possible value should be 999
  uint16_t _eventNumber;
  char _trackType;
  uint8_t _trackNumber;
  bool _dropFrame;
  float _frameRate;
  // maximum possible value for frames should be 5,403,660
  Timecode _sourceStart;
  Timecode _sourceEnd;
  Timecode _recordStart;
  Timecode _recordEnd;
  std::string _reel;
  std::string _sourceClipName;
  std::string _sourceFileName;
  std::string _comment;

public:
  // constructors
  Event();
  Event(const std::string&, float, bool);
  Event(const std::string&, int, bool);
  // getters
  int eventNumber() const { return _eventNumber; }
  char trackType() const { return _trackType; }
  int trackNumber() const { return _trackNumber; }
  bool isDropFrame() const { return _dropFrame; }
  std::string reel() const { return _reel; }
  std::string sourceClipName() const { return _sourceClipName; }
  std::string sourceFileName() const { return _sourceFileName; }
  std::string comment() const { return _comment; }
  // setters
  void dropFrame(const bool&);
  void eventNumber(const int&);
  void eventNumber(const std::string&);
  void reel(const std::string&);
  void trackType(const char&);
  void trackType(const std::string&);
  void trackNumber(const int&);
  void trackNumber(const std::string&);
  void sourceStartTimecode(const std::string&);
  void sourceStartTimecode(const int&);
  void sourceEndTimecode(const std::string&);
  void sourceEndTimecode(int&);
  void recordStartTimecode(const std::string&);
  void recordStartTimecode(const int&);
  void recordEndTimecode(const std::string&);
  void recordEndTimecode(const int&);
  void comment(const std::string&);
  void sourceFileName(const std::string&);
  void sourceClipName(const std::string&);
  void setEventClipData(const std::string &);
  // operator overloads
  friend std::ostream &operator<<(std::ostream &, const Event &);
  // other
  void printCsv();
};

#endif // EVENT_H_
