drop-frame# libedl

## Introduction

libedl is a C++ library to read and write [Edit Decision Lists](https://en.wikipedia.org/wiki/Edit_decision_list), commonly referred to as EDLs. EDLs are plaintext documents describing an edited video sequence. The most common format is CMX 3600, which is currently the only supported format.

libedl provides four C++ classes to represent data present in an EDL:

* **EDL**: represents an EDL document (an edited sequence)
* **Event**: represents a single clip in an edited sequence
* **MotionEffect**: represents a single clip with a modified time base, such as a clip that has slowed down or sped up.
* **Timecode**: represents [SMPTE timecode](https://en.wikipedia.org/wiki/SMPTE_timecode) information

## Origin

I am a computer science student at Los Angeles City College, who works in the television industry. Many professional productions are required to deliver multiple documents to clients along with the final picture and sound of a program. For a long time, most of this work was done by hand, but EDLs provide most of the information needed to get this work done, if only it could be automated.

This library was written alongside my enrollment in a level two C++ programming course at LA City College, so if the code appears amateurish, it most definitely is.

## Usage

Definitions for each class are provided in the four header files in the *include/* folder, and implementations are in each source file in the *src/* folder.

My course covered using the C++ language, but not the ecosystem around it (how to include external libraries outside of the standard library, build systems, etc.), so this library is provided as source and header files for you to include in your project.

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
    inputFile.close();
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

An Edl class is mostly a container for Event objects. It also stores a title for the Edl. Passing an ifstream to the constructor triggers this class to read the Events into itself.

#### Edl Constructors

* **Edl()** : Create empty EDL object
* **Edl(std::ifstream)** : Read EDL from a file. Currently only CMX3600 EDLs are supported.
* **Edl(Edl)** : Copy constructor. Deep copies the EDL's events.

#### Edl Getters

* **std::string name()** : the EDL's title.
* **int size()** : returns the number of events in the EDL
* **Event\* event(int)** : returns a pointer to the event with the passed event number. Note that an event's number may be different than its index in the underlying array. Returns *nullptr* if the event number does not exist in the EDL.

#### Edl Setters

* **void name(std::string)** : Set the EDL's title

#### Other Edl Function

* **void sort()** : Sorts the events in the array by their event numbers. Modifies the EDL in-place.

#### Edl Operators
* **=** : Assignment operator. Deep copies the EDL's events.
* **[]** : Access an event in the EDL via the its index in the underlying array. Note that an Event's index may be different from its event number.

### Event Class

For reference, an event in a CMX 3600 EDL looks like the following block. Descriptions of class methods will refer to this block:

````
888  EVL1_HEI V     C        00:08:00:19 00:08:02:16 01:21:11:21 01:21:13:18
M2   EVL1_HEI       037.5                00:08:00:19
* POND5_T_QEVL1HEIS1040_HEISTS_DINNER SET GANG_CLOSEUP OF FULL TREASURE CHEST WITH
*  JEWELLERY_59691306
* REEL: T_QEVL1HEIS1040
* FROM CLIP NAME:  T_QEVL1HEIS1040.NEW.01.COPY.01
* FROM FILE NAME: T_QEVL1HEIS1040.MOV
````

#### Event Constructors

* **Event()** : Creates an empty event
* **Event(int, std::string, char, int, Timecode, Timecode, Timecode, Timecode, float, bool)** : Creates an event from the following parameters:
  * **int** : Event number.
  * **std::string** : Reel.
  * **char** : Track type.
  * **int** : Track number.
  * **Timecode** : Source start timecode.
  * **Timecode** : Source end timecode.
  * **Timecode** : Record start timecode.
  * **Timecode** : Record end timecode.
  * **float** : Frame rate.
  * **bool** : drop-frame-ness.

#### Event Getters

* **int eventNumber()** : Returns the event number (888 in the block above)
* **char trackType()** : Returns the track type of the event. Typically 'V' for video or 'A' for audio.
* **int trackNumber()** : Returns the track number of the event.
* **std::string reel()** : Returns a string object of the reel (EVL1_HEI in the block above)
* **Timecode sourceStart()** : Returns Timecode object of the event's source's start timecode ("00:08:00:19" above)
* **Timecode SourceEnd()** : Returns Timecode object of the event's source's end timecode ("00:08:02:16" above)
* **Timecode recordStart()** : Returns Timecode object of the event's record start timecode ("01:21:11:21" above)
* **Timecode recordEnd()** : Returns Timecode object of the event's record end timecode ("01:21:13:18" above)
* **bool hasMotionEffect()** : Returns a boolean on wether the event has a motion effect on it.
* **MotionEffect motionEffect()** : Returns a MotionEffect object for the event. If the event does not have a motion effect, throws a *nonexistent_motion_effect* exception.
* **std::string motionEffectReel()** : Returns the reel of the motion effected clip (EVL1_HEI In the block above). If the event does not have a motion effect, throws a *nonexistent_motion_effect* exception.
* **float motionEffectSpeed()** : Returns the speed at which a motion-effected clip is to be played. This is a float representing a frame-rate (037.05 in the block above). If the event does not have a motion effect, throws a *nonexistent_motion_effect* exception.
* **Timecode motionEffectEntryPoint()** : Returns a timecode object representing the motion-effected-clip's entry point; that is, the start timecode of the source clip to be sped or slowed (00:08:00:19 in the block above). If the event does not have a motion effect, throws a *nonexistent_motion_effect* exception.
* **std::string sourceReel()** : Some EDL files store an 8-character reel for backwards compatibility, and a full reel name in a comment line. This function returns the full reel name (T_QEVL1HEIS1040 in the block above)
* **std::string sourceClipName()** : Returns the event source name (T_QEVL1HEIS1040.NEW.01.COPY.01 in the block above)
* **std::string sourceFileName()** : Returns the event's source file name (T_QEVL1HEIS1040.MOV in the block above)
* **std::string comment()** : Returns any comments that belong to the event (POND5_T_QEVL1HEIS1040_HEISTS_DINNER SET GANG_CLOSEUP OF FULL TREASURE CHEST WITH JEWELLERY\_59691306 in the block above)

#### Event Setters

* **void eventNumber(int)** : Sets event number (note that this can be different from the event's index in an EDL)
* **void reel(std::string)** : Sets the Event's reel
* **void trackType(char)** : Sets the Event's track type. Typically 'V' for video or 'A' for audio.
* **void trackType(char)** : Sets the Event's track number.
* **void sourceStart(Timecode)** : Set source start timecode from a Timecode object.
* **void sourceEnd(Timecode)** : Set source end timecode from a Timecode object.
* **void recordStart(Timecode)** : Set record start timecode from a Timecode object.
* **void recordEnd(Timecode)** : Set record end timecode from a Timecode object.
* **void motionEffect(MotionEffect)** : Sets a MotionEffect to be applied to an event.
* **void motionEffectReel(std::string)** : Sets the reel of the motion-effect.
* **void motionEffectSpeed(float)** : Sets the speed at which a motion-effected clip is to be played. This is a float representing a frame-rate.
* **void motionEffectEntryPoint(Timecode)** : Sets the motion-effected-clip's entry point; that is, the start timecode of the source clip to be sped or slowed.
* **void addMotionEffect()** : Adds an empty motion effect to the event. An event can only have one motion effect, this will replace any current motion effect attached to the event.
* **void removeMotionEffect()** : Removes a motion effect from the event.
* **void sourceReel(std::string)** : Sets the source reel of an event's motionEffect.
* **void sourceFileName(std::string)** : Sets the Event source's file name.
* **void sourceClipName(std::string)** : Sets the Event source's clip name.
* **void comment(std::string, bool)** : Sets the Event's comment. Because comments can span multiple lines, you may also pass a boolean on wether you want the passed string comment to be appended to the current comment. This defaults to false, which overwrites any existing comment.

#### Event Operators

* **=** : Assignment operator.

### MotionEffect Class

The MotionEffect class models a source clip that has had its timebase modified, usually because it has been sped up or slowed down. Motion Effects are generally part of an event, and the Event class provides methods to get and set the properties of an attached MotionEffect.

### MotionEffect Constructors
* **MotionEffect()** : default constructor.
* **MotionEffect(std::string, float, Timecode)** : Create a MotionEffect from a reel, speed (as a frame-rate), and entry point.

### MotionEffect Getters
* **std::string reel()** : Get the reel of the MotionEffect.
* **float speed()** : Get the speed of the MotionEffect.
* **Timecode entryPoint()** : Get the source start timecode of the motion-effected clip.

### MotionEffect Setters
* **void reel(std::string)** : Set the reel of the MotionEffect.
* **void speed(float)** : Set the speed of the MotionEffect.
* **void entryPoint(Timecode)** : Set the start Timecode of the MotionEffect.

### Timecode Class

The Timecode class supports any frame rate, including 29.97 and 59.94 in both drop-frame and non-drop-frame.

Each field has a maximum value (23 for hours, 59 for minutes, 59 for seconds, and frames cannot be larger than the frame-rate), and passing parameters or calculating values beyond these maximum values will cause that field to roll over to or past zero, and increment the next larger field. That is, passing "01:00:61:00" to the constructor will result in a timecode of "01:01:02:00".

Drop-frame timecodes drop two frames each minute, except the each tenth minute, and passing a dropped frame as an argument will increment the timecode by two frames. For example, calling *Timecode("01:01:00;00", 29.97, true)*, will result in a timecode of "01:01:00;02".

#### Timecode Constructors

Frame rate and drop-frame arguments in constructors are optional, and will default to *30* for frame rate and *false* for drop-frame-ness.

The available constructors are:
* **Timecode()** : Default constructor, creates an object with all fields set to 0.
* **Timecode(Timecode)** : Copy Constructor.
* **Timecode(int, int, int, int, double, bool)** : Create Timecode using integers for the hours, minutes, seconds, and frames. A double for the frame rate and boolean for drop-frame-ness are optional, and default to *30* and *false*, respectively.
* **Timecode(int, double, bool)** : Create a Timecode object by passing an integer frame count. A double for frame rate and boolean for drop-frame-ness are optional, and default to *30* and *false*, respectively.
* **Timecode(std::string, double, bool)** : Create a timecode from a string object, formatted as "00:00:00:00". A double for the frame rate and boolean for drop-frame-ness are optional, and default to *30* and *false*, respectively.

### Static Helper Functions

If you just want to get a timecode string (formatted as "00:00:00:00") from a count of frames, you don't have to create a Timecode object. You can use the following static member function:

* **std::string framesToString(unsigned int, double, bool)** : Returns a string representing a given timecode, using an unsigned integer frame count, double frame rate, and boolean for drop-frame-ness.

### Timecode Getters

* **int hours()** : Returns the hours field.
* **int minutes()** : Returns the minutes field.
* **int seconds()** : Returns the seconds field.
* **int frames()** : Returns the frames field.
* **double framerate()** : Returns the Timecode's frame rate.
* **bool dropframe()** : Returns a boolean as to wether the timecode is drop-frame or not.
* **int totalFrames()** : Returns the total frame count of the Timecode object.

### Timecode Setters

* **void hours(int)** : Sets the hours field.
* **void minutes(int)** : Sets the minutes field.
* **void seconds(int)** : Sets the seconds field.
* **void frames(int)** : Sets the frames field.
* **void framerate(double)** : Sets the Timecode's frame rate.
* **void dropframe(bool)** : Sets a boolean as to wether the timecode is drop-frame or not.

### Type Conversion

* **std::string to_string()** : Returns a string object of the Timecode in the format "00:00:00:00". If the timecode is drop-frame, the final separator will be a semicolon (';'), not a colon (':').

### Operators

#### Arithmetic Operators
Arithmetic operators support Timecode objects of different frame rates and drop-frame-ness. The resulting Timecode object will retain the frame rate and drop-frame-ness of the left operand.

* **+ Timecode** : Adds two Timecode objects together.
* **+ int** : Adds a number of frames to the Timecode object.
* **- Timecode** : Subtracts one Timecode object from another.
* **- int** : Subtracts a number of frames from a Timecode object.
* **\* int** : Multiplies a timecode by a scalar value.
* **=** : Assignment operator.

#### Comparison Operators

Comparison operators test wether two Timecode objects represent the same number of frames, allowing you to evaluate timecodes of different frame rates and drop-frame-ness. If you need to test for exact equality, combine these with evaluations of the getters **framerate()** and **dropframe()**. The available operators are:

* **==**
* **!=**
* **<**
* **<=**
* **>**
* **>=**

#### Stream Operators
* **<< (stream insertion)** : Sends the Timecode to the output stream in the format "00:00:00:00". If the timecode is drop-frame, the final separator will be a semicolon (';'), not a colon (':').

## Tests

I've made an early attempt at writing some tests using [Google C++ Testing Framework](https://github.com/google/googletest).

I started learning C++ unit testing for this projects so tests are currently very minimal and I'm still learning best practices. I still need to explore possible edge cases for testing.

These tests can be found in the test/ folder, along with a CMX3600 EDL ("cmx3600.edl"), as well as another EDL produced by Avid Media Composer, claiming to be CMX3600 not matching the CMX3600 official specification. This file is named "cmx3600extended.edl".

## Acknowledgements

* I'm very grateful to [guerilla-di](http://guerilla-di.org) for their Ruby Gem [timecode](https://github.com/guerilla-di/timecode), which set me down this learning path.
*  I have to acknowledge the [BBC's Ingex project](http://ingex.sourceforge.net), whose timecode implementation was a very valuable learning resource for properly converting Timecode between frames and the component fields.
