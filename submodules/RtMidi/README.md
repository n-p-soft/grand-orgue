# RtMidi-direct

This is an alternate version of RtMidi 6.0.0, which is a cross-platform library to access MIDI ports in C/C++.

Compared to the official RtMidi package, a "direct" API was added to access the raw devices under /dev directly (i.e. without an intermediary server such as Jack). Depending on the system the MIDI raw devices are commonly named /dev/midiX, /dev/midiX.Y, /dev/umidiX, /dev/umidiX.Y, or /dev/rmidiX.

This version works at least on FreeBSD 13+ but other BSDs and Linux should be supported.

The "direct" API does not allow creating virtual ports. A common use case of this API is when one program has exclusive access to one MIDI device.

For now, no filtering is done on MIDI-in bytes; they are sent "as it" to the RtMidi client (TODO: split frames properly).

## How to build

The build requires cmake. Create a build directory (`mkdir build`), then configure the build system (`cd build; cmake ..`) and build the library (`make`, then
as root `make install`).

For advanced configuration, edit the file CMakeLists.txt.

This distribution of RtMidi contains the following:

- `doc`:      RtMidi documentation
- `tests`:    example RtMidi programs

## Overview of the original package

RtMidi is a set of C++ classes (`RtMidiIn`, `RtMidiOut`, and API specific classes) that provide a common API (Application Programming Interface) for realtime MIDI input/output across Linux (ALSA, JACK), Macintosh OS X (CoreMIDI, JACK), and Windows (Multimedia Library) operating systems.  RtMidi significantly simplifies the process of interacting with computer MIDI hardware and software.  It was designed with the following goals:

  - object oriented C++ design
  - simple, common API across all supported platforms
  - one header and one source file for easy inclusion in programming projects
  - MIDI device enumeration

MIDI input and output functionality are separated into two classes, `RtMidiIn` and `RtMidiOut`.  Each class instance supports only a single MIDI connection.  RtMidi does not provide timing functionality (i.e., output messages are sent immediately).  Input messages are timestamped with delta times in seconds (via a `double` floating point type).  MIDI data is passed to the user as raw bytes using an `std::vector<unsigned char>`.

## Legal and ethical

The RtMidi license is similar to the MIT License, with the added *feature* that modifications be sent to the developer.  Please see [LICENSE](LICENSE).

## Authors

Gary P. Scavone, 2003-2023 for the original library (6.0.0),
Nicolas Provost, 2025 for the "direct" API.

