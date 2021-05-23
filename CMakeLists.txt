cmake_minimum_required(VERSION 3.0.0)
project(wheel VERSION 0.3.0)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

file(GLOB_RECURSE HEADERS include/*.h include/*.hpp)
file(GLOB_RECURSE SOURCES src/*.cpp src/*.cc src/*.c)

add_executable(wheel ${SOURCES} ${HEADERS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

if(MSVC)
  target_compile_options(wheel PRIVATE /W4 /WX)
else()
  target_compile_options(wheel PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(wheel ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
