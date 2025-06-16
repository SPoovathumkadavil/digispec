# ---- Declare library ----

file(GLOB_RECURSE digispec_digispec_sources CONFIGURE_DEPENDS "src/digispec/*.cpp")
add_library(
    digispec_digispec
    ${digispec_digispec_sources}
)
add_library(digispec::digispec ALIAS digispec_digispec)

include(GenerateExportHeader)
generate_export_header(
    digispec_digispec
    BASE_NAME digispec
    EXPORT_FILE_NAME export/digispec/digispec_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(digispec_digispec PUBLIC DIGISPEC_STATIC_DEFINE)
endif()

set_target_properties(
    digispec_digispec PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME digispec
    OUTPUT_NAME digispec
)

target_include_directories(
    digispec_digispec ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    digispec_digispec SYSTEM
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(digispec_digispec PUBLIC cxx_std_17)

# ---- external libs ----

include(cmake/external-lib.cmake)
