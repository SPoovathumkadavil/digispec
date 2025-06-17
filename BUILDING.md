# Building with CMake

## Build

This project doesn't require any special command-line flags to build to keep
things simple.

_cmake_ is required for the build

run `make build` in the project directory to build the project.
```

### Building with MSVC

Note that MSVC by default is not standards compliant and you need to pass some
flags to make it behave properly. See the `flags-msvc` preset in the
[CMakePresets.json](CMakePresets.json) file for the flags and with what
variable to provide them to CMake during configuration.

### Building on Apple Silicon

CMake supports building on Apple Silicon properly since 3.20.1. Make sure you
have the [latest version][1] installed.

## Install

This project doesn't require any special command-line flags to install to keep
things simple. As a prerequisite, the project has to be built with the above
command already.

This project follows a "feta-framework" build directory which finds the intended
bin output directroy from a .loc.json file in the user's home directory. Edit the
PATH_PREFIX variable in the Makefile to the intended prefix to bypass feta.

### CMake package

This project exports a CMake package to be used with the [`find_package`][3]
command of CMake:

* Package name: `digispec`
* Target name: `digispec::digispec`

Example usage:

```cmake
find_package(digispec REQUIRED)
# Declare the imported target as a build requirement using PRIVATE, where
# project_target is a target created in the consuming project
target_link_libraries(
    project_target PRIVATE
    digispec::digispec
)
```

### Note to packagers

The `CMAKE_INSTALL_INCLUDEDIR` is set to a path other than just `include` if
the project is configured as a top level project to avoid indirectly including
other libraries when installed to a common prefix. Please review the
[install-rules.cmake](cmake/install-rules.cmake) file for the full set of
install rules.

[1]: https://cmake.org/download/
[2]: https://cmake.org/cmake/help/latest/manual/cmake.1.html#install-a-project
[3]: https://cmake.org/cmake/help/latest/command/find_package.html
