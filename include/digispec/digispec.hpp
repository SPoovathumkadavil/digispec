#pragma once

#include <string>

#include "digispec/digispec_export.hpp"

namespace digispec {
  class DIGISPEC_EXPORT digispec
  {
  public:
    /**
     * @brief Initializes the name field to the name of the project
     */
    digispec();

    std::string name() { return m_name; }

  private:
    std::string m_name;
    bool m_test;
  };
} // namespace digispec