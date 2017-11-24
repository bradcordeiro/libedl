#ifndef EDL_H_
#define EDL_H_

#include "event.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

extern void _setEventClipData(const std::string &s);

class Edl {

private:
  std::string _name;
  double _frameRate;
  bool _dropFrame;
  std::vector<Event> _events;
  void _nameFromHeader(std::string);
  void _setDropFrame(const std::string &);
  void _setFrameRate(const std::string &);

public:
  // constructors
  Edl();
  explicit Edl(std::ifstream &);
  Edl(Edl &);
  // getters
  std::string name() const;
  unsigned long size() const;
  Event event(int, int = 0, int = -1) const;
  // setters
  void name(const std::string &input);
  void frameRate(const double &);
  void dropFrame(const bool &b);

  // operator overloads
  Edl &operator=(const Edl &); // shallow copy
  Event operator[](const int &) const;
};

#endif // EDH_H_
