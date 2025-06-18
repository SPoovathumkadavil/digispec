
#include "digispec/digiparser.hpp"

namespace digispec {

    validation digiparser::parse(const std::vector<token>& tokens) {
        
        // validate nesting of begin and end commands
        validation nesting_validation = validate_nesting(tokens);
        if (!nesting_validation.valid) {
            return nesting_validation;
        }
        // validate each command against the registered commands
        std::vector<token> identities; // Store identifiers for later validation
        for (size_t i = 0; i < tokens.size(); ++i) {
            const token& current_token = tokens[i];
            if (current_token.type == token_type::COMMAND) {
                // Find the command in the registered commands
                std::string command_name = current_token.value;
                auto it = std::find_if(registered_commands.begin(), registered_commands.end(),
                                       [&command_name](const command& cmd) { return cmd.name == command_name; });
                // Validate the command
                validation cmd_validation = validate_command(*it, tokens, i, identities);
                if (!cmd_validation.valid) {
                    return cmd_validation;
                }
            } else if (current_token.type == token_type::IDENTIFIER) {
                // Register the identifier
                identities.push_back(current_token);
            } else if (current_token.type == token_type::SYMBOL) {
                // validate that symbol is assigned to a module
            }
        }

        // If we reach here, all commands are valid and properly nested
        return {true, ""};
    }
    validation digiparser::validate_nesting(const std::vector<token>& tokens) {
        
        // check whether begin and end commands are properly nested
        std::vector<size_t> begin_indices;
        for (size_t i = 0; i < tokens.size(); ++i) {
            if (tokens[i].type == token_type::COMMAND) {
                if (tokens[i].value == "begin") {
                    begin_indices.push_back(i);
                } else if (tokens[i].value == "end") {
                    if (begin_indices.empty()) {
                        return {false, "Unmatched 'end' command at index " + std::to_string(i)};
                    }
                    begin_indices.pop_back();
                }
            }
        }
        if (!begin_indices.empty()) {
            return {false, "Unmatched 'begin' command at index " + std::to_string(begin_indices.back())};
        }
        // If we reach here, all begin and end commands are properly nested
        return {true, ""};

    }
    validation digiparser::validate_command(const command& cmd, const std::vector<token>& tokens, size_t& index, std::vector<token>& identities) const {
        // the start index is the command itself
        // so we start at index + 1, verifying the arguments
        index++; // Move past the command token
        for (const auto& arg : cmd.arguments) {
            if (index >= tokens.size()) {
                return {false, "Missing argument for command: " + cmd.name};
            }
            const token& current_token = tokens[index];
            if (current_token.type == token_type::IDENTIFIER) {
                // check if identity is already registered
                if (std::find(identities.begin(), identities.end(), current_token) == identities.end()) {
                    return {false, "Unregistered identifier: " + current_token.value};
                }
                if (arg.type != argument_type::IDENTIFIER && arg.type != argument_type::VALUE) {
                    return {false, "Invalid argument type for command: " + cmd.name};
                }
            } else if (current_token.type == token_type::KEYWORD && arg.type == argument_type::KEYWORD) {
            } else {
                return {false, "Invalid argument type for command: " + cmd.name};
            }
            index++;
        }
        return {true, ""};
    }
} // namespace digispec