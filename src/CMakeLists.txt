include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(Proxy INTERFACE)
target_include_directories(Proxy INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(Proxy INTERFACE adkostatt::Http)

install(TARGETS Proxy EXPORT ${PROJECT_NAME}Targets)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/adkostatt/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/adkostatt)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  VERSION 1.0.0
  COMPATIBILITY AnyNewerVersion
  ARCH_INDEPENDENT)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

install(EXPORT ${PROJECT_NAME}Targets
  NAMESPACE adkostatt::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
