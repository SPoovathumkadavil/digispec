#include <string>

#include "digispec/digispec.hpp"

#ifndef IS_TESTING
#define IS_TESTING false
#endif

exported_class::exported_class()
    : _name {"digispec"},
      _test {IS_TESTING}
{}

