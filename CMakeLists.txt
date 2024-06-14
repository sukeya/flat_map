cmake_minimum_required(VERSION 3.11)

project(flat_map VERSION 0.1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(FLAT_MAP_USE_TEST "use test in flat_map." OFF)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(flat_map INTERFACE)

target_compile_features(flat_map INTERFACE cxx_std_17)
target_include_directories(flat_map INTERFACE
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(TARGETS flat_map
  EXPORT flat_mapTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
  DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/flat_map
  DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT flat_mapTargets
  FILE flat_mapTargets.cmake
  NAMESPACE flat_map::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/flat_map
)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/flat_mapConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/flat_map"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/flat_mapConfigVersion.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY AnyNewerVersion
)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/flat_mapConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/flat_mapConfigVersion.cmake
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/flat_map
)

export(EXPORT flat_mapTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/flat_mapTargets.cmake"
  NAMESPACE flat_map::
)

enable_testing()

add_subdirectory(test)
add_subdirectory(bench)
