cmake_minimum_required(VERSION 3.0.0)
project(wheel VERSION 0.3.0)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

# set(HEADERS
#     include/trackers/controllertracker.h
#     include/trackers/pedaltracker.h
#     include/trackers/wheeltracker.h
#     include/controllers/controller.h
#     include/controllers/pedal.h
#     include/controllers/wheel.h
#     include/utils/keyboard.h)


# set(SOURCES
#     src/main.cpp
#     src/controllers/pedal.cpp
#     src/controllers/wheel.cpp
#     src/utils/keyboard.cpp)

file(GLOB_RECURSE HEADERS include/*.h include/*.hpp)
file(GLOB_RECURSE SOURCES src/*.cpp src/*.cc src/*.c)

add_executable(wheel ${SOURCES} ${HEADERS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(wheel ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
