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
        set(AntTweakBar_FOUND ON)
        set(AntTweakBar_INCLUDE_DIRS ${AntTweakBar_INSTALL_DIR}/include)
        set(AntTweakBar_LIBRARIES ${AntTweakBar_INSTALL_DIR}/lib/AntTweakBar.lib)
    endif(NOT IS_DIRECTORY ${AntTweakBar_INSTALL_DIR})

else(WIN32)
# TODO Write as necessary for other platforms

    
endif(WIN32)