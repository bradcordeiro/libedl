#ifndef EDL_H_
#define EDL_H_

#include "event.h"
#include <fstream>
#include <iostream>
#include <string>

class Edl {

private:
  std::string _name;
  float _frameRate;
  bool _dropFrame;
  Event *_events[999];
  int _iterator;
  void setNameFromHeader(std::string);

public:
  // constructors
  Edl();                // default constructor
  Edl(std::ifstream &); // create from input file
  Edl(std::string);     // create from path to file
  // getters
  std::string name() const { return _name; }
  double frameRate() const { return _frameRate; }
  Event event(int i) const { return *(_events[i]); }
  // setters
  void setName(const std::string &);
  void setFrameRate(const std::string &);
  void setFrameRate(const float &);
  void setFrameRate(const int &);
  void setDropFrame(const bool &);
  void setDropFrame(const std::string &);
  // operator overloads
  Edl operator=(const Edl &);
  Edl operator++();
  Edl operator++(int);
  Edl operator--();
  Edl operator--(int);
};

#endif // EDH_H_
