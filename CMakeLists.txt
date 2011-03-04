cmake_minimum_required(VERSION 2.8)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND CMAKE_POLICY)

project(ImageRegistration)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

option(USE_VISUALIZATION "Use VTK and QT visualization" OFF)

add_subdirectory(Source)
