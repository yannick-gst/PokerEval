cmake_minimum_required(VERSION 3.0)
project(PokerEval)

if (CMAKE_BUILD_TYPE MATCHES Debug)
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/debug")
else()
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/release")
endif (CMAKE_BUILD_TYPE MATCHES Debug)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES PokerHand.cpp main.cpp)
add_executable(PokerEval ${SOURCE_FILES})