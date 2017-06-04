cmake_minimum_required(VERSION 3.7)
project(vdev)

add_executable(vdev main.cpp)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(Boost 1.64.0)
add_dependencies(vdev Boost)

# set(Hana_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/ext/hana")
# find_package(Hana 1.1.0)
# add_dependencies(vdev Hana)

# set(Sml_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/ext/sml")
# find_package(Sml 1.0.1)
# add_dependencies(vdev Sml)
set(Sml_INCLUDE_DIRS "~/src/sml/include")

include_directories(
  "${CMAKE_SOURCE_DIR}/include"
  ${Boost_INCLUDE_DIRS}
  ${Hana_INCLUDE_DIRS}
  ${Sml_INCLUDE_DIRS}
  "~/src/variant/include"
)

list(APPEND CMAKE_CXX_FLAGS "-std=gnu++1z -O3 -fno-rtti -fno-exceptions")

# set_property(TARGET vdev PROPERTY CXX_INCLUDE_WHAT_YOU_USE "iwyu")
# set_property(TARGET vdev PROPERTY CXX_STANDARD 14)
