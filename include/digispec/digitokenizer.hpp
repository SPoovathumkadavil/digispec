// digispec tokenizer

#pragma once

#include <string>
#include <array>
#include <vector>
#include <filesystem>
#include <algorithm>

#include "digispec/digispec_export.hpp"

namespace digispec {
    /**
     * @brief Enum class representing different types of tokens in the digitokenizer.
     */
    enum class DIGISPEC_EXPORT token_type {
        IDENTIFIER, // Represents a variable or function name
        COMMAND,    // Represents a command or instruction
        COMMENT,    // Represents a comment in the code
        KEYWORD,    // Represents a reserved keyword in the language
        SYMBOL,     // Represents a special character or symbol
        NONE        // Represents an unrecognized token
    };
    class token {
    public:
        token(token_type type, const std::string& value)
            : type(type), value(value) {}

        token_type getType() const { return type; }
        std::string getValue() const { return value; }

    private:
        token_type type;
        std::string value;
    };
    class DIGISPEC_EXPORT digi_tokenizer {
    public:
        /**
         * @brief Tokenizes the input string based on the specified delimiter.
         * 
         * @param input The string to tokenize.
         * @param delimiter The character used to split the string.
         * @return A vector of tokens extracted from the input string.
         */
        static std::vector<token> tokenize(const std::string& input);
    private:
        /**
         * @brief Valid keywords in the digitokenizer.
         * 
         * These keywords are reserved and cannot be used as identifiers.
         */
        static const std::array<std::string_view, 3> VALID_KEYWORDS;
        /**
         * @brief Valid commands in the digitokenizer.
         * 
         * These commands are recognized and can be used in the input.
         */
        static const std::array<std::string_view, 9> VALID_COMMANDS;
        /**
         * @brief Helper function to determine if a character is a valid identifier character.
         * 
         * @param ch The character to check.
         * @return True if the character is valid, false otherwise.
         */
        static bool is_valid_identifier_char(char ch) {
            return std::isalnum(ch) || ch == '_' || ch == '-' || ch == '.';
        }
        /**
         * @brief Helper function to determine if a string is a valid command.
         * @param command The string to check.
         * @return True if the string is a valid command, false otherwise.
         */
        static bool is_valid_command(const std::string& command) {
            // case insensitive comparison
            std::string lower_command = command;
            std::transform(lower_command.begin(), lower_command.end(), lower_command.begin(), ::tolower);
            return std::find(VALID_COMMANDS.begin(), VALID_COMMANDS.end(), lower_command) != VALID_COMMANDS.end();
        }
        /**
         * @brief Helper function to determine if a string is a valid keyword.
         * @param keyword The string to check.
         * @return True if the string is a valid keyword, false otherwise.
         */
        static bool is_valid_keyword(const std::string& keyword) {
            std::string lower_keyword = keyword;
            std::transform(lower_keyword.begin(), lower_keyword.end(), lower_keyword.begin(), ::tolower);
            return std::find(VALID_KEYWORDS.begin(), VALID_KEYWORDS.end(), lower_keyword) != VALID_KEYWORDS.end();
        } 
        /**
         * @brief Helper function to check if a string is a valid symbol.
         * @param symbol The string to check.
         * @return True if the string is a valid symbol, false otherwise.
         */
        static bool is_valid_symbol(const std::string& symbol) {
            return symbol == "::";
        }
    };
}