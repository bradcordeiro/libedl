/*
Reads an EDL file into C++ classes and then spits it back out to stdout
*/

#include "../src/edl.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>

int main() {
  std::ifstream inputFile;

  inputFile.open("test/cmx3600.edl");

  if (inputFile.good()) {
    Edl e(inputFile);
    e.sort();
    std::cout << e.name() << std::endl;
    for (int i = 0; i < e.size(); i++) {
      std::cout << std::setw(3) << std::setfill('0') << std::right << e[i].eventNumber() << ' '
                << std::setw(8) << std::setfill(' ') << std::left << e[i].reel() << ' '
                << e[i].trackType() << ' '
                << std::setw(14) << ' '
                << e[i].sourceStart().to_string() << ' '
                << e[i].sourceEnd().to_string() << ' '
                << e[i].recordStart().to_string() << ' '
                << e[i].recordEnd().to_string() << ' '
                << std::endl;
      if (e[i].hasMotionEffect()) {
        std::cout << "M2\t" << e[i].motionEffectReel() << "\t" << e[i].motionEffectSpeed() << '\t'
                                                               << e[i].motionEffectEntryPoint().to_string() << std::endl;
      }
      if (!e[i].comment().empty()) {
        int length = 80;
        for (int j = 0; j < e[i].comment().size(); j+=length) {
          std::cout << "* " << e[i].comment().substr(j, length) << std::endl;
        }
      }
      if (!e[i].sourceClipName().empty()) {
        std::cout << "* FROM CLIP NAME: " << e[i].sourceClipName() << std::endl;
      }
      if (!e[i].sourceFileName().empty()) {
        std::cout << "* FROM FILE NAME: " << e[i].sourceFileName() << std::endl;
      }
      if (!e[i].sourceReel().empty()) {
        std::cout << "* REEL: " << e[i].sourceReel() << std::endl;
      }

    }
  } else {
    std::cerr << "Failed to open input file." << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
