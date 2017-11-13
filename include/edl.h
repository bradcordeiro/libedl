#ifndef EDL_H_
#define EDL_H_

#include "event.h"
#include <fstream>
#include <iostream>
#include <string>

extern void _setEventClipData(const std::string &s);

class Edl {

private:
  std::string _name;
  double _frameRate;
  bool _dropFrame;
  Event *_events[999];
  int _size;
  void setNameFromHeader(std::string);
  void setDropFrame(const std::string &);
  void setFrameRate(const std::string &);

public:
  // constructors
  Edl();
  explicit Edl(std::ifstream &);
  explicit Edl(std::string);
  Edl(Edl &);
  // getters
  inline std::string name() const { return _name; }
  inline int size() const { return _size; }
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
