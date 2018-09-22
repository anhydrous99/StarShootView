cmake_minimum_required(VERSION 3.5.1)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
project(StarShootView)

set(CMAKE_BUILD_TYPE Release)

find_package(OpenGL REQUIRED)
find_package(FLTK REQUIRED)
find_package(CFITSIO REQUIRED)

include_directories(${FLTK_INCLUDED_DIR})
include_directories(${CFITSIO_INCLUDE_DIR})
include_directories(include)

add_executable(StarShootView src/Fl_FITS_Image.cxx src/main.cxx)

target_link_libraries(StarShootView ${FLTK_LIBRARIES} ${OPENGL_LIBRARIES} ${CFITSIO_LIBRARIES})
