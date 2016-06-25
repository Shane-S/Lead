cmake_minimum_required (VERSION 2.6)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
include_directories(${INCLUDE_DIR})

set(HEADERS ${INCLUDE_DIR}/Camera.h
            ${INCLUDE_DIR}/GLProgramUtils.h
            ${INCLUDE_DIR}/Scene.h
			${INCLUDE_DIR}/Shader.h
            ${INCLUDE_DIR}/Model.h
            ${INCLUDE_DIR}/TestModelVerts.h)

set(SOURCES Camera.cpp
            GLProgramUtils.cpp
            Scene.cpp
			Shader.cpp
            Model.cpp
            TestModelVerts.cpp
            main.cpp)

find_package(AntTweakBar REQUIRED)
include_directories(${AntTweakBar_INCLUDE_DIRS})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

find_package(SDL2_image REQUIRED)
include_directories(${SDL2IMAGE_INCLUDE_DIR})

find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

add_executable(Lead ${SOURCES} ${HEADERS})
target_link_libraries(Lead ${AntTweakBar_LIBRARIES} ${SDL2_LIBRARY} ${GLEW_LIBRARIES} ${SDL2IMAGE_LIBRARY})