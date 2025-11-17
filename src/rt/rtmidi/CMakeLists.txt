# Copyright 2006 Milan Digital Audio LLC
# Copyright 2009-2023 GrandOrgue contributors (see AUTHORS)
# License GPL-2.0 or later (https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).

message(STATUS "RtMidi build Configuration")

include_directories("${RTMIDI_SRC_DIR}")
# not more necessary for the recent RtMidi
# include_directories(../../core/contrib)

add_library(RtMidi STATIC "${RTMIDI_SRC_DIR}/RtMidi.cpp")

message(STATUS "    Support for Jack          : ${RTMIDI_USE_JACK}")
message(STATUS "    Support for Alsa          : ${RTMIDI_USE_ALSA}")
message(STATUS "    Support for Direct API    : ${RTMIDI_USE_DIRECT}")

if (RTMIDI_USE_DIRECT)
   add_definitions(-D__DIRECT__)
endif ()

if (RTMIDI_USE_JACK)
   add_definitions(-D__UNIX_JACK__)
   target_link_libraries(RtMidi PkgConfig::JACK)
endif ()

if (RTMIDI_USE_ALSA)
   add_definitions(-D__LINUX_ALSA__)
   target_link_libraries(RtMidi asound udev)
   set(CMAKE_REQUIRED_LIBRARIES asound)
   check_function_exists(snd_seq_client_info_get_card HAVE_GET_CLIENT_INFO_CARD)
   if (HAVE_GET_CLIENT_INFO_CARD)
     add_definitions(-DHAVE_GET_CLIENT_INFO_CARD)
   endif ()
endif ()

message(STATUS "============================================================================")
