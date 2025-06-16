if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/digispec-${PROJECT_VERSION}"
      CACHE STRING ""
  )
  set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package digispec)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT digispec_Development
)

install(
    TARGETS digispec_digispec
    EXPORT digispecTargets
    RUNTIME #
    COMPONENT digispec_Runtime
    LIBRARY #
    COMPONENT digispec_Runtime
    NAMELINK_COMPONENT digispec_Development
    ARCHIVE #
    COMPONENT digispec_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    digispec_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE STRING "CMake package config location relative to the install prefix"
)
set_property(CACHE digispec_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(digispec_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${digispec_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT digispec_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${digispec_INSTALL_CMAKEDIR}"
    COMPONENT digispec_Development
)

install(
    EXPORT digispecTargets
    NAMESPACE digispec::
    DESTINATION "${digispec_INSTALL_CMAKEDIR}"
    COMPONENT digispec_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
