#include "include/event.h"
#include "include/edl.h"
#include "include/timecode.h"
#include <iostream>
#include <fstream>

int main() {
  std::ifstream inputFile;
  inputFile.open("../test/cmx3600.edl");

  if (inputFile.good()) {
    std::cout << "Opened input file." << std::endl;
    Edl e = Edl(inputFile);
  } else if (inputFile.fail()) {
    std::cout << "Failed to open input file." << std::endl;
  } else if (inputFile.bad()) {
    std::cout << "Bad input file.";
  } else {
    std::cout << "Something else." << std::endl;
  }

  return 0;
}
