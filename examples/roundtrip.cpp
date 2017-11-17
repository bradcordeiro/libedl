/*
Reads an EDL file into C++ classes and then spits it back out to stdout
*/


#include "../include/edl.h"
#include <fstream>
#include <iostream>

int main() {
  std::ifstream inputFile;
  Edl e;

  inputFile.open("../test/cmx3600.edl");

  if (inputFile.good()) {
    e = Edl(inputFile);
    for (int i = 0; i < e.size(); i++) {
    	std::cout << e[i];
    }
  } else {
    std::cout << "Failed to open input file." << std::endl;
    return 1;
  }

  return 0;
}
