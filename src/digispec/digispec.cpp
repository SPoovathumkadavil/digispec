#include <string>

#include "digispec/digispec.hpp"

#ifndef IS_TESTING
#define IS_TESTING false
#endif
namespace digispec {
  digispec::digispec()
      : m_name {"digispec"},
        m_test {IS_TESTING}
  {}
} // namespace digispec