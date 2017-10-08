cmake_minimum_required(VERSION 2.8.12)

project(chaparral NONE)

include(ExternalProject)
ExternalProject_Add(chaparral
  GIT_REPOSITORY    https://github.com/ericdingle/chaparral.git
  GIT_TAG           master
  SOURCE_DIR        "${CMAKE_CURRENT_SOURCE_DIR}/third_party/chaparral"
  BINARY_DIR        ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)
