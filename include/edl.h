#ifndef EDL_H_
#define EDL_H_

#include "../include/event.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class Edl {

public:
  // constructors
  Edl();
  Edl(std::ifstream &);
  // getters
  std::string name() const;
  int size() const;
  Event *find_event(int i, int min = 0, int max = -1);
  // setters
  void name(const std::string &input);
  // other functions
  void sort();

  // operator overloads
  Event operator[](const int &) const;

private:
  std::string _name;
  std::vector<Event> _events;

  void _nameFromHeader(std::string);
  bool _readDropFrame(const std::string &);
  Event _parseEvent(const std::string &, const float &, const bool &);
  MotionEffect _parseMotionEffect(const std::string &, const Timecode &);
  void _parseComment(Event &e, std::string &line);
  static bool _eventcmp(const Event &a, const Event &b);
};

#endif // EDH_H_
