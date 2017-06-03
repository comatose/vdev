cmake_minimum_required(VERSION 3.7)
project(vdev)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(Hana_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/ext/hana")
find_package(Hana 1.1.0)

add_executable(vdev main.cpp)
add_dependencies(vdev Hana)

include_directories(
  "${CMAKE_SOURCE_DIR}/include"
  ${Hana_INCLUDE_DIRS}
)
list(APPEND CMAKE_CXX_FLAGS "-std=gnu++1z -O3")

# set_property(TARGET vdev PROPERTY CXX_INCLUDE_WHAT_YOU_USE "iwyu")
# set_property(TARGET vdev PROPERTY CXX_STANDARD 14)

message(STATUS CXX=${CXX})
