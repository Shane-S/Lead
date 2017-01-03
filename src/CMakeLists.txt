cmake_minimum_required (VERSION 2.6)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set(HEADERS ${INCLUDE_DIR}/Camera.h
            ${INCLUDE_DIR}/GLProgramUtils.h
            ${INCLUDE_DIR}/Scene.h
			${INCLUDE_DIR}/Shader.h
            ${INCLUDE_DIR}/Drawable3D.h
            ${INCLUDE_DIR}/TestModelVerts.h
			${INCLUDE_DIR}/Utils.h
            ${INCLUDE_DIR}/Drawable.h
            ${INCLUDE_DIR}/Drawable3DContainer.h
            ${INCLUDE_DIR}/FloorPlan.h
            ${INCLUDE_DIR}/Building.h)

set(SOURCES Camera.cpp
            GLProgramUtils.cpp
            Scene.cpp
			Shader.cpp
        	Drawable3D.cpp
            TestModelVerts.cpp
			Utils.cpp
            main.cpp 
            Drawable3DContainer.cpp
            FloorPlan.cpp
            Building.cpp
            glad.c)

find_package(AntTweakBar REQUIRED)
include_directories(${AntTweakBar_INCLUDE_DIRS})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

find_package(SDL2_image REQUIRED)
include_directories(${SDL2IMAGE_INCLUDE_DIR})

find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(pb REQUIRED)

add_executable(Lead ${SOURCES} ${HEADERS})
target_link_libraries(Lead ${AntTweakBar_LIBRARIES}
                           ${SDL2_LIBRARY}
                           ${OPENGL_LIBRARIES}
                           ${SDL2IMAGE_LIBRARY}
                           pb pb_util)
target_include_directories(Lead PUBLIC ${INCLUDE_DIR})
