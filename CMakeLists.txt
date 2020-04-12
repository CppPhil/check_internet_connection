cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(check_internet_connection VERSION 1.0.0)

string(
  APPEND
  CMAKE_CXX_FLAGS
  " -Wall -Wextra -Wcast-align -Wcast-qual -Wmissing-field-initializers -Wmissing-include-dirs -Wpointer-arith -Wstack-protector -Wstrict-aliasing=2 -Wunreachable-code -Wlogical-op -Wformat=2 -Wduplicated-cond -Wnull-dereference -Wno-unused-parameter"
)
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -g -O0")
string(APPEND CMAKE_CXX_FLAGS_RELEASE
       " -O3 -march=native -funroll-loops -flto -DNDEBUG")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Threads REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets Network REQUIRED)

set(APP_NAME check_internet_connection_app)

set(HEADERS include/main_window.hpp include/ping.hpp)

set(SOURCES src/main.cpp src/main_window.cpp src/ping.cpp)

add_executable(${APP_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${APP_NAME} PUBLIC Threads::Threads Qt5::Core Qt5::Gui
                                         Qt5::Widgets Qt5::Network)

target_include_directories(
  ${APP_NAME} PUBLIC ${check_internet_connection_SOURCE_DIR}/include)
