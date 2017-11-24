# libedl

## Introduction

libedl is a C++ library to read and write [Edit Decision Lists](https://en.wikipedia.org/wiki/Edit_decision_list), commonly referred to as EDLs. EDLs are plaintext documents describing an edited sequence in a list. The most common format is CMX 3600, which is currently the only supported format.

libedl provides three C++ classes to represent data present in an EDL:

* **EDL**: represents an EDL document (an editeed sequence)
* **Event**: represents a single clip in an edited sequence
* **Timecode**: represents [SMPTE timecode](https://en.wikipedia.org/wiki/SMPTE_timecode) information

The Timecode class supports NTSC drop-frame, and each class will pass this information to any classes they contain as members on initialization of the member.

## Origin

I am a computer science student at Los Angeles City College, who works in the television industry. Many professional productions are required to deliver multiple documents to clients along with the final picture and sound of a program. For a long time, most of this work was done by hand, but EDLs provide most of the information needed to get this work done, if only it could be automated.

This library was written alongside my enrollment in a level two C++ programming course at LA City College, so if the code appears amateurish, it most definitely is.

My course covered using the C++ language, but not the ecosystem around it (how to include external libraries outside of the standard library, build systems, etc.), so this library is provided as source and header files for you to include in your project.

## Usage

Definitions for each class are provided in the three header files in the *include/* folder, and implementations are in each source file in the *src/* folder.

Small one-line class member functions, such as getters and simple setters, are implemented inline in the header files.

### Example

```c++
#include "include/edl.h"
#include <fstream>
#include <iostream>

int main() {
  std::ifstream inputFile;
  Edl e;

  inputFile.open("test/cmx3600.edl");

  if (inputFile.good()) {
    e = Edl(inputFile);
    for (int i = 0; i < e.size(); i++) {
    	std::cout << "Event " << e[i].eventNumber()
		  << " starts at "
		  << e[i].recordStart()
		  << "." << std::endl;
    }
  } else {
    std::cout << "Failed to open input file." << std::endl;
    return 1;
  }

  return 0;
}
```

## Classes

### Edl

An Edl class is mostly a container for Event objects. It also stores a title for the Edl. Passing an ifstream or string object to the constructor triggers this class to read the Events into itself.

Each constructor provides defaults for the EDL's framerate and wether the timecode is dropframe, which are 30 for framerate and false for dropframe.

#### Edl Constructors

* **Edl()** : Create empty EDL
* **Edl(std::ifstream, double, bool)** : Create from a file, framerate, and dropframe-ness.
* **Edl(std::string, double, bool)** : Create EDL from string object, framerate, and dropframe-ness
* **Edl(Edl)** : Deep copy an EDL and all events

#### Edl Getters

* **std::string name()** : the EDL's title, pulled from the header of the input
* **int size()** : returns the number of events in the EDL
* **Event event(int)** : returns the event object at the passed index from the array of events in the EDL
	* throws *out\_of\_range* exception when passed index exceeds the size of the EDL

#### Edl Setters

* **void setName(std::string)** : Set the EDL's title
* **void setFrameRate(double)** : Set the EDL's framerate
* **void setDropFrame(bool)** : Set wether the timecode in the EDL is dropframe or not

#### Edl Operators
* **=** : Copies the title from another EDL, and shallow copies events from one EDL to another
* **[]** : Access an event in the EDL via index subscript

### Event Class

For reference, an event in a CMX 3600 EDL looks like the following block. Descriptons of class methods will refer to this block:

````
888  EVL1_HEI V     C        00:08:00:19 00:08:02:16 01:21:11:21 01:21:13:18
* POND5_T_QEVL1HEIS1040_HEISTS_DINNER SET GANG_CLOSEUP OF FULL TREASURE CHEST WITH
*  JEWELLERY_59691306
* FROM CLIP NAME:  T_QEVL1HEIS1040.NEW.01.COPY.01
````

Each constructor provides a default framerate and dropframe boolean. These are 30 for the framerate and false for dropframe-ness. When events are constructed by an EDL object, these variables are read from the EDL itself.

#### Event Constructors

* **Event()** : Creates an empty event
* **Event(std::string, double, bool)** : Creates an event from a string that matches the format above

#### Event Getters

* **int eventNumber()** : Returns the event number (888 in the block above)
* **std::string reel()** : Returns a string object of the reel (EVL1_HEI in the block above)
* **std::string sourceClipName()** : Returns the event source name (T_QEVL1HEIS1040.NEW.01.COPY.01 in the block above)
* **std::string sourceFileName()** : Returns the event's source file name (no example above)
* **std::string comment()** : Returns any comments that belong to the event (POND5_T_QEVL1HEIS1040_HEISTS_DINNER SET GANG_CLOSEUP OF FULL TREASURE CHEST WITH JEWELLERY\_59691306 in the block above)
* **Timecode sourceStart()** : Returns Timecode object of the event's source's start timecode ("00:08:00:19" above)
* **Timecode SourceEnd()** : Returns Timecode object of the event's source's end timecode ("00:08:02:16" above)
* **Timecode recordStart()** : Returns Timecode object of the event's record start timecode ("01:21:11:21" above)
* **Timecode recordEnd()** : Returns Timecode object of the event's record end timecode ("01:21:13:18" above)

#### Event Setters

* **void dropFrame(bool)** : Sets dropframe-ness
* **void eventNumber(int)** : Sets event number (not that this can be different from the event's index in an EDL)
* **void reel(std::string)** : Sets the Event's reel
* **void trackType(char)** : Sets the Event's track type (typicaly 'V' for video or 'A' for audio)
* **void sourceStart(Timecode)** : Set source start timecode from a Timecode object.
* **void sourceEnd(Timecode)** : Set source end timecode from a Timecode object.
* **void recordStart(Timecode)** : Set record start timecode from a Timecode object.
* **void recordEnd(Timecode)** : Set record end timecode from a Timecode object.
* **void comment(std::string, bool)** : Sets the Event's comment. Because comments can span multiple lines, you may also pass a boolean on wether you want the comment to be appended. This defaults to false, which overwrites any existing comment.
* **void sourceFileName(std::string)** : Sets the Event source's file name.
* **void sourceClipName(std::string)** : Sets the Event source's clip name.

#### Event Operators

* **<< (stream insertion)** : Sends the Event to the output stream in CMX3600 format (see the example in the block under the **Event Class** description).
* **>> (stream extraction)** : Reads an event line in CMX3600 and stores the fields (see the example in the block under the **Event Class** description).

### Timecode Class

The Timecode class supports any framerate, including 29.97 and 59.94 in both dropframe and non-dropframe.

#### Timecode Constructors

Framerate and dropframe arguments in constructors are optional, and will default to *30* for framerate and *false* for dropframe-ness.

Constructors will throw an *invalid_argument* exception under the following conditions:

* hours exceed 24
* minutes exceed 59
* seconds exceed 59
* frames exceed the framerate
* Total frames exceed the number of frames in 24 hours

The available constructors are:

* **Timecode(int, int, int, int, double, bool)** : Create Timecode using integers for the hours, minutes, seconds, and frames. A double for the frame rate and boolean for dropframe-ness are optional.
* **Timecode(int, double, bool)** : Create a Timecode object by passing an integer frame count, double for framerate, and boolean for dropframe-ness.
* **Timecode(char \*, double, bool)** : Create a timecode from a C-String, formatted as "00:00:00:00".
* **Timecode(std::string, double, bool)** : Create a timecode from a string object, formatted as "00:00:00:00".

### Timecode Getters

* **int hours()** : Returns the hours field.
* **int minutes()** : Returns the minutes field.
* **int seconds()** : Returns the seconds field.
* **int frames()** : Returns the frames field.
* **double framerate()** : Returns the Timecode's framerate.
* **bool dropframe()** : Returns a boolean as to wether the timecode is dropframe or not.
* **int totalFrames()** : Returns the total frame count of the Timecode object.

### Timecode Setters

* **void hours(int)** : Sets the hours field.
* **void minutes(int)** : Sets the minutes field.
* **void seconds(int)** : Sets the seconds field.
* **void frames(int)** : Sets the frames field.
* **void framerate(double)** : Sets the Timecode's framerate.
* **void dropframe(bool)** : Sets a boolean as to wether the timecode is dropframe or not.

### Type Conversion

* **int** : Converts the Timecode to an integer holding the total frame count.
* **char \*** : Converts the Timecode to a C-String, in the format "00:00:00:00".
* **std::string** : Converts the Timecode to a string object, in the format "00:00:00:00".
* **char \* c_str()** : Returns a C-String of the Timecode in the format "
* **std::string to_string()** : Returns a string object of the Timecode in the format "00:00:00:00";

### Operators

#### Arithmetic Operators
Arithmetic operators support Timecode objects of different framerates and dropframe-ness. The resulting Timecode object will retain the framerate and dropframe-ness of the left operand.

* **+ Timecode** : Adds two Timecodes together.
* **+ int** : Adds a number of frames to the Timecode object.
* **- Timecode** : Subtracts one Timecode from another.
* **- int** : Subtracts a number of frames from a Timecode object.
* **=** : Provides a deep copy of a Timecode object.

#### Comparison Operators

Comparison operators test wether two Timecode objects represent the same number of frames, allowing you to evaluate timecodes of different framerates and dropframe-ness. If you need to test for exact equality, combine these with evaluations of the getters **framerate()** and **dropframe()**. The available operators are:

* **==**
* **!=**
* **<**
* **<=**
* **>**
* **>=**

#### Stream Operators
* **<< (stream insertion)** : Sends the Timecode to the output stream in the format "00:00:00:00".
* **>> (stream extraction)** : Reads Timecode from the input stream in the format "00:00:00:00".

## Tests

I've made an early attempt at writing some tests using [Google C++ Testing Framework](https://github.com/google/googletest). So far they've all been tests for the Timecode class to make sure conversion between timecode and frame counts are correct.

I started learning C++ unit testing for this projects so tests are currently very minimal.

These tests can be found in the test/ folder, along with a CMX3600 EDL ("cmx3600.edl"), as well as another EDL produced by Avid Media Composer, claiming to be CMX3600 not matching the CMX3600 official specification. This file is named "cmx3600extended.edl".

## Acknowledgements

* I'm very grateful to [guerilla-di](http://guerilla-di.org) for their Ruby Gem [timecode](https://github.com/guerilla-di/timecode), which set me down this learning path.
*  I have to acknowledge the [BBC's Ingex project](http://ingex.sourceforge.net), whose timecode implementation was a very valuable learning resource for properly converting Timecode between frames and the component fields.
