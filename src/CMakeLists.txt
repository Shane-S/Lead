cmake_minimum_required (VERSION 2.6)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
include_directories(${INCLUDE_DIR})

set(HEADERS ${INCLUDE_DIR}/Camera.h
            ${INCLUDE_DIR}/GLProgramUtils.h
            ${INCLUDE_DIR}/Scene.h
			${INCLUDE_DIR}/Shader.h
            ${INCLUDE_DIR}/Drawable3D.h
            ${INCLUDE_DIR}/TestModelVerts.h
			${INCLUDE_DIR}/Utils.h ../include/Drawable.h)

set(SOURCES Camera.cpp
            GLProgramUtils.cpp
            Scene.cpp
			Shader.cpp
        	Drawable3D.cpp
            TestModelVerts.cpp
			Utils.cpp
            main.cpp ../include/Drawable.h Drawable3DContainer.cpp ../include/Drawable3DContainer.h FloorPlan.cpp ../include/FloorPlan.h Building.cpp ../include/Building.h)

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

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# TODO: use the package registry with a post-install script or something
find_path(pb_INCLUDE_DIR
          NAMES extrusion.h sq_house.h floor_plan.h
          PATHS /usr/local
          PATH_SUFFIXES include/ include/pb)
find_library(pb_LIBRARY
             NAMES pb
             PATHS /usr/local
             PATH_SUFFIXES lib lib64)
find_library(pb_util_LIBRARY
             NAMES pb_util
             PATHS /usr/local
             PATH_SUFFIXES lib lib64)
set(pb_LIBRARIES ${pb_LIBRARY} ${pb_util_LIBRARY})

add_executable(Lead ${SOURCES} ${HEADERS})
target_link_libraries(Lead ${AntTweakBar_LIBRARIES}
                           ${SDL2_LIBRARY}
                           ${OPENGL_LIBRARIES}
                           ${GLEW_LIBRARIES}
                           ${SDL2IMAGE_LIBRARY}
                           ${pb_LIBRARIES})

target_include_directories(Lead PRIVATE ${pb_INCLUDE_DIR})
