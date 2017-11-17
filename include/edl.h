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
  void setNameFromHeader(std::string);
  void setDropFrame(const std::string &);
  void setFrameRate(const std::string &);

public:
  // constructors
  Edl();
  Edl(std::ifstream &);
  Edl(Edl &);
  // destructor
  // getters
  inline std::string name() const { return _name; }
  inline unsigned long size() const { return _events.size(); }
  Event event(const int &i) const;
  // setters
  void setName(const std::string &input) { _name = input; }
  void setFrameRate(const double &);
  inline void setDropFrame(const bool &b) { _dropFrame = b; }
  // operator overloads
  Edl &operator=(const Edl &); // shallow copy
  Event operator[](const int &) const;
};

#endif // EDH_H_
