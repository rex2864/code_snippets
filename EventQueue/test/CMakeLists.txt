CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
PROJECT(Kyle::EventQueue VERSION 0.1.0)
SET(CMAKE_CXX_STANDARD 11)
ADD_COMPILE_OPTIONS(-g -Wall)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/..)
ADD_EXECUTABLE(singleton
    singleton.cpp
    ../EventQueue.cpp
)
TARGET_LINK_LIBRARIES(singleton pthread)
ADD_EXECUTABLE(multi
    multi_main.cpp
    multi1.cpp
    multi2.cpp
    ../EventQueue.cpp
)
TARGET_LINK_LIBRARIES(multi pthread)
