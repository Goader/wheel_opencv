cmake_minimum_required(VERSION 3.0.0)
project(wheel VERSION 0.3.0)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

set(HEADERS
    include/keyboard.h
    include/pedal.h)

set(SOURCES
    src/main.cpp
    src/pedal.cpp
    src/keyboard.cpp)

add_executable(wheel ${SOURCES} ${HEADERS})

# add_executable(wheel include/keyboard.h include/pedal.h src/keyboard.cpp src/pedal.cpp src/main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(wheel ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
