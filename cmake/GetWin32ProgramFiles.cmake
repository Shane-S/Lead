function(win32_program_files output)

    # Determine whether we're compiling for 64-bit
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH "x86_64")
    else(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH "x86")
    endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

    # Determine whether the machine itself is 64-bit
    set(IS_64BIT_MACHINE (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64" OR
                          ${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "IA64" OR
                          ${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "EM64T"))

    if(${ARCH} STREQUAL "x86_64")
        set(${output} "C:\\Program Files" PARENT_SCOPE)
    else(${ARCH} STREQUAL "x86_64")
        # The target architecture is 32-bit, but it could be that we're still on a 64-bit machine
        # If we are, then "Program Files" will be "Program Files (x86)"
        if(IS_64BIT_MACHINE)
            set(${output} "C:\\Program Files (x86)" PARENT_SCOPE)
        else(IS_64BIT_MACHINE)
            set(${output} "C:\\Program Files" PARENT_SCOPE)
        endif(IS_64BIT_MACHINE)
    endif(${ARCH} STREQUAL "x86_64")

endfunction()