# Copyright 2006 Milan Digital Audio LLC
# Copyright 2009-2024 GrandOrgue contributors (see AUTHORS)
# License GPL-2.0 or later (https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).

message(STATUS "RtAudio build Configuration")

INCLUDE (CheckIncludeFileCXX)

set(RtAudio_Sources "${RTAUDIO_SRC_DIR}/RtAudio.cpp")

include_directories("${RTAUDIO_SRC_DIR}/include")
add_library(RtAudio STATIC ${RtAudio_Sources})

   message(STATUS "  Building a Linux configuration")
   message(STATUS "    Support for Jack          : ${RTAUDIO_USE_JACK}")
   message(STATUS "    Support for Alsa          : ${RTAUDIO_USE_ALSA}")
   message(STATUS "    Support for OSS           : ${RTAUDIO_USE_OSS}")

   if (RTAUDIO_USE_JACK)
      add_definitions(-D__UNIX_JACK__)
      target_link_libraries(RtAudio PkgConfig::JACK)
   endif ()

   if (RTAUDIO_USE_ALSA)
      add_definitions(-D__LINUX_ALSA__)
      target_link_libraries(RtAudio asound)
   endif ()

#   if (RTAUDIO_USE_OSS)
#      add_definitions(-D__LINUX_OSS__)
#      target_link_libraries(RtAudio aoss)
#   endif ()

   target_link_libraries(RtAudio pthread)

message(STATUS "============================================================================")
