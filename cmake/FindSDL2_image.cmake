# Stolen from: https://github.com/wheybags/freeablo/blob/master/cmake/FindSDL2_image.cmake
# Locate the SDL2_image library. This CMake module is a modified version
# of the original FindSDL_image.cmake file
# ###########################################################################
# Locate SDL_image library
# This module defines
# SDL2IMAGE_LIBRARY, the name of the library to link against
# SDLIMAGE_FOUND, if false, do not try to link to SDL
# SDL2IMAGE_INCLUDE_DIR, where to find SDL/SDL.h
#
# $SDLDIR is an environment variable that would
# correspond to the ./configure --prefix=$SDLDIR
# used in building SDL.
#
# Created by Eric Wing. This was influenced by the FindSDL.cmake 
# module, but with modifications to recognize OS X frameworks and 
# additional Unix paths (FreeBSD, etc).

#=============================================================================
# Copyright 2005-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distributed this file outside of CMake, substitute the full
#  License text for the above reference.)

set(SDL2IMAGE_LIB_PATH_GUESSES
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt)

if(WIN32)
    include(GetWin32ProgramFiles)
    win32_program_files(PROG_FILES_PATH)
    set(SDL2IMAGE_LIB_PATH_GUESSES ${SDL2IMAGE_LIB_PATH_GUESSES}
                                   "${PROG_FILES_PATH}\\SDL2"
                                   "${PROG_FILES_PATH}\\SDL2_image")
endif()

FIND_PATH(SDL2IMAGE_INCLUDE_DIR SDL_image.h
  HINTS
  $ENV{SDL2IMAGEDIR}
  $ENV{SDL2DIR}
  PATH_SUFFIXES include/SDL2 include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
)

FIND_LIBRARY(SDL2IMAGE_LIBRARY 
  NAMES SDL2_image
  HINTS
  $ENV{SDL2IMAGEDIR}
  $ENV{SDL2DIR}
  PATH_SUFFIXES lib64 lib
  PATHS ${SDL2IMAGE_LIB_PATH_GUESSES}
)

SET(SDL2IMAGE_FOUND "NO")
IF(SDL2IMAGE_LIBRARY AND SDL2IMAGE_INCLUDE_DIR)
  SET(SDL2IMAGE_FOUND "YES")
ENDIF(SDL2IMAGE_LIBRARY AND SDL2IMAGE_INCLUDE_DIR)