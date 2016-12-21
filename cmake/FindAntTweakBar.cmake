if(WIN32)

    # Get the target architecture on Windows so that we can try to set AntTweakBar_INSTALL_DIR to something reasonable
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH "x86_64")
    else(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH "x86")
    endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

    set(IS_64BIT_MACHINE (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64" OR
                          ${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "IA64" OR
                          ${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "EM64T"))

    if(${ARCH} STREQUAL "x86_64")
            set(AntTweakBar_INSTALL_DIR "C:\\Program Files\\AntTweakBar")
    else(${ARCH} STREQUAL "x86_64")
        # The target architecture is 32-bit, but it could be that we're still on a 64-bit machine
        # If we are, then "Program Files" will be "Program Files (x86)"
        if(IS_64BIT_MACHINE)
            set(AntTweakBar_INSTALL_DIR "C:\\Program Files (x86)\\AntTweakBar")
        else(IS_64BIT_MACHINE)
            set(AntTweakBar_INSTALL_DIR "C:\\Program Files\\AntTweakBar")
        endif(IS_64BIT_MACHINE)
    endif(${ARCH} STREQUAL "x86_64")
    
    if(NOT IS_DIRECTORY ${AntTweakBar_INSTALL_DIR})
        set(AntTweakBar_FOUND OFF)
    else(NOT IS_DIRECTORY ${AntTweakBar_INSTALL_DIR})
        if(IS_64BIT_MACHINE AND ${ARCH} STREQUAL "x86_64")
            set(LIB_NAME "AntTweakBar64")
        else(IS_64BIT_MACHINE AND ${ARCH} STREQUAL "x86_64")
            set(LIB_NAME "AntTweakBar")
        endif()
        
        set(AntTweakBar_FOUND ON)
        set(AntTweakBar_INCLUDE_DIRS ${AntTweakBar_INSTALL_DIR}/include)
        set(AntTweakBar_LIBRARIES ${AntTweakBar_INSTALL_DIR}/lib/${LIB_NAME}.lib)
    endif(NOT IS_DIRECTORY ${AntTweakBar_INSTALL_DIR})

else(WIN32)
# Stolen from FindSDL2.cmake - these are still reasonable guesses
set(AntTweakBar_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt)

find_path(AntTweakBar_INCLUDE_DIRS
          AntTweakBar.h
          HINTS $ENV{ANTTWEAKBAR_HOME}
          PATHS ${AntTweakBar_SEARCH_PATHS}
          PATH_SUFFIXES include)

find_library(AntTweakBar_LIBRARIES
             AntTweakBar
             PATHS ${AntTweakBar_SEARCH_PATHS}
             PATH_SUFFIXES lib lib64)

if(NOT ${AntTweakBar_LIBRARIES} OR NOT ${AntTweakBar_INCLUDE_DIRS})
  set(AntTweakBar_FOUND OFF)
else()
  set(AntTweakBar_FOUND ON)
endif()
    
endif(WIN32)
