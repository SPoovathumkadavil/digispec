// digispec token parser
// ensures valid token order and content based on tokenizer rules

#pragma once

#include <vector>
#include <array>

#include "digispec/digitokenizer.hpp"
#include "digispec/digispec_export.hpp"

namespace digispec {
    enum class DIGISPEC_EXPORT argument_type {
        VALUE,      // Represents a value argument
        KEYWORD,   // Represents a keyword argument
        IDENTIFIER, // Represents an identifier argument
        NONE        // Represents no specific type
    };
    enum class DIGISPEC_EXPORT argument_options {
        REQUIRED,    // Argument is required
        OPTIONAL,    // Argument is optional
        MULTIPLE,    // Argument can accept multiple values
        DEFAULT      // Argument has a default value
    };
    struct DIGISPEC_EXPORT argument {
        std::string name;
        argument_type type;
        std::vector<argument_options> options; // Optional list of valid options for the argument
        std::string default_value;        // Optional default value
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
        bool parse(const std::vector<token>& tokens);
    };
    // all registered commands
    static const std::array<command, 9> registered_commands = {{
        {"begin", {
            {{"type", argument_type::KEYWORD, {argument_options::REQUIRED}, ""},
             {"name", argument_type::IDENTIFIER, {argument_options::REQUIRED}, ""}}
        }},
        {"end", {
            {{"name", argument_type::IDENTIFIER, {argument_options::REQUIRED}, ""}}
        }},
        {"display", {
            {{"value", argument_type::VALUE, {argument_options::REQUIRED}, ""}}
        }},
        {"input", {
            {{"variable", argument_type::IDENTIFIER, {argument_options::REQUIRED, argument_options::MULTIPLE}, ""}}
        }},
        {"output", {
            {{"value", argument_type::VALUE, {argument_options::REQUIRED, argument_options::MULTIPLE}, ""}}
        }},
        {"alias", {
            {{"type", argument_type::IDENTIFIER, {argument_options::REQUIRED}, ""},
             {"name", argument_type::IDENTIFIER, {argument_options::REQUIRED}, ""}}
        }},
        {"line", {
            {{"start", argument_type::IDENTIFIER, {argument_options::REQUIRED}, ""},
             {"end", argument_type::IDENTIFIER, {argument_options::REQUIRED}, ""}}
        }},
        {"compare", {
            {{"left", argument_type::IDENTIFIER, {argument_options::REQUIRED}, ""},
             {"right", argument_type::IDENTIFIER, {argument_options::REQUIRED}, ""}}
        }},
        {"include", {
            {{"module", argument_type::IDENTIFIER, {argument_options::REQUIRED}, ""}}
        }}
    }};
}