cmake_minimum_required(VERSION 3.24)
project(viterbi_decoding)

set(CMAKE_CXX_STANDARD 14)

add_executable(viterbi_decoding main.cpp  include/viterbi.h src/viterbi.cpp src/state_change.cpp include/state_change.h)
