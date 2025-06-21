// digispec token parser
// ensures valid token order and content based on tokenizer rules

#pragma once

#include <vector>
#include <array>
#include <algorithm>

#include "digispec/digitokenizer.hpp"
#include "digispec/digispec_export.hpp"
#include "digispec/util.hpp"

namespace digispec {
    enum class DIGISPEC_EXPORT argument_type {
        VALUE,      // Represents a value argument
        KEYWORD,    // Represents a keyword argument
        IDENTIFIER, // Represents an identifier argument
        NONE        // Represents no specific type
    };
    enum class DIGISPEC_EXPORT argument_options {
        REQUIRED,        // Argument is required
        OPTIONAL,        // Argument is optional
        MULTIPLE,        // Argument can accept multiple values
        DECLARATIVE,     // Argument is declarative
        CAPTURING,       // Argument captures the identifier
    };
    struct DIGISPEC_EXPORT argument {
        std::string name;
        argument_type type;
        std::optional<std::vector<argument_options>> configs;          // Optional list of configs for the argument
        std::optional<std::vector<std::string>> values; // Optional list of values for the argument
        std::optional<std::string> default_value;             // Optional default value
    };
    struct DIGISPEC_EXPORT command {
        std::string name;
        std::vector<argument> arguments;
    };
    class DIGISPEC_EXPORT digiparser {
    public:
        digiparser() = default;
        ~digiparser() = default;

        // Parses the tokens and ensures valid order and content
        // exits on first error
        validation parse(const std::vector<token>& tokens);
    private:
        // Helper function to validate whether blocks are properly nested
        validation validate_nesting(const std::vector<token>& tokens);
        // Helper function to validate a single command against the registered commands
        validation validate_command(const command& cmd, const std::vector<token>& tokens, size_t *index, std::vector<token> *identities) const;
    };
    // all registered commands
    static const std::array<command, 11> registered_commands = {{
        {"begin", {
            {"type", argument_type::KEYWORD, std::make_optional(std::vector<argument_options>{argument_options::REQUIRED}), std::make_optional(std::vector<std::string>{"mod", "definition"})},
            {"name", argument_type::IDENTIFIER, std::make_optional(std::vector<argument_options>{argument_options::REQUIRED, argument_options::DECLARATIVE, argument_options::CAPTURING})}
        }},
        {"end", {
            {"name", argument_type::IDENTIFIER, std::make_optional(std::vector<argument_options>{argument_options::REQUIRED})}
        }},
        {"display", {
            {"value", argument_type::VALUE, std::make_optional(std::vector<argument_options>{argument_options::REQUIRED})}
        }},
        {"input", {
            {"variable", argument_type::IDENTIFIER, std::make_optional(std::vector<argument_options>{argument_options::REQUIRED, argument_options::MULTIPLE, argument_options::DECLARATIVE})}
        }},
        {"output", {
            {"value", argument_type::VALUE, std::make_optional(std::vector<argument_options>{argument_options::REQUIRED, argument_options::MULTIPLE})}
        }},
        {"alias", {
            {"type", argument_type::IDENTIFIER, std::make_optional(std::vector<argument_options>{argument_options::REQUIRED})},
            {"name", argument_type::IDENTIFIER, std::make_optional(std::vector<argument_options>{argument_options::REQUIRED})}
        }},
        {"line", {
            {"start", argument_type::IDENTIFIER, std::make_optional(std::vector<argument_options>{argument_options::REQUIRED})},
            {"end", argument_type::IDENTIFIER, std::make_optional(std::vector<argument_options>{argument_options::REQUIRED})}
        }},
        {"compare", {
            {"left", argument_type::IDENTIFIER, std::make_optional(std::vector<argument_options>{argument_options::REQUIRED})},
            {"right", argument_type::IDENTIFIER, std::make_optional(std::vector<argument_options>{argument_options::REQUIRED})}
        }},
        {"include", {
            {"module", argument_type::IDENTIFIER, std::make_optional(std::vector<argument_options>{argument_options::REQUIRED})}
        }},
        {"return"},
        {"export", {
            {"variable", argument_type::IDENTIFIER, std::make_optional(std::vector<argument_options>{argument_options::REQUIRED, argument_options::MULTIPLE, argument_options::DECLARATIVE})}
        }}
    }};
} // namespace digispec