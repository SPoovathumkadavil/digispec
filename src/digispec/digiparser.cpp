
#include "digispec/digiparser.hpp"
#include <numeric>

namespace digispec {

    validation digiparser::parse(const std::vector<token>& tokens) {
        
        // validate nesting of begin and end commands
        validation nesting_validation = validate_nesting(tokens);
        if (!nesting_validation.valid) {
            return nesting_validation;
        }

        // validate each command against the registered commands
        std::vector<token> captured_identities; // store identifiers for later validation
        for (size_t i = 0; i < tokens.size(); i++) {
            const token& current_token = tokens[i];
            if (current_token.type == token_type::COMMAND) {
                // find the command in the registered commands
                auto it = std::find_if(registered_commands.begin(), registered_commands.end(),
                                       [&current_token](const command& cmd) {
                    std::string lower_cmd = current_token.value;
                    std::transform(lower_cmd.begin(), lower_cmd.end(), lower_cmd.begin(), ::tolower);
                    return lower_cmd == cmd.name;
                });
                if (it == registered_commands.end()) {
                    return {false, "Unknown command '" + current_token.value + "' at token index " + std::to_string(i)};
                }
                // validate the command
                validation cmd_validation = validate_command(*it, tokens, i, &captured_identities);
                if (!cmd_validation.valid) {
                    return cmd_validation;
                }
                // skip till next command
                while (i+1 < tokens.size() && tokens[i+1].type != token_type::COMMAND) {
                    i++;
                }
            } else if (current_token.type == token_type::IDENTIFIER) {
            } else if (current_token.type == token_type::SYMBOL) {}
        }

        // If we reach here, all commands are valid and properly nested
        return {true, ""};
    }
    validation digiparser::validate_nesting(const std::vector<token>& tokens) {
        
        // check whether begin and end commands are properly nested
        std::vector<std::pair<size_t, std::string>> begin_identifiers;
        for (size_t i = 0; i < tokens.size(); i++) {
            if (tokens[i].type == token_type::COMMAND) {
                    if (tokens[i].value == "begin") {
                        // Check if the next token is a keyword
                        if (i + 2 >= tokens.size()) {
                            return {false, "Missing identifier for 'begin' command at token index " + std::to_string(i)};
                        }
                        if (tokens[i + 2].type != token_type::IDENTIFIER) {
                            return {false, "Expected identifier after 'begin' command at token index " + std::to_string(i + 2)};
                        }
                        // Register the begin command with its identifier
                        begin_identifiers.emplace_back(i, tokens[i + 2].value);
                        i++; // Skip the keyword token
                    } else if (tokens[i].value == "end") {
                    // Check if the next token is an identifier
                    if (i + 1 >= tokens.size()) {
                        return {false, "Missing identifier for 'end' command at token index " + std::to_string(i)};
                    }
                    if (tokens[i + 1].type != token_type::IDENTIFIER) {
                        return {false, "Expected identifier after 'end' command at token index " + std::to_string(i + 1) + " instead found '" + tokens[i + 1].value + "'"};
                    }
                    bool found_match = false;
                    for (auto it = begin_identifiers.begin(); it != begin_identifiers.end(); ++it) {
                        if (it->second == tokens[i + 1].value) {
                            begin_identifiers.erase(it); // Remove the matching begin command
                            found_match = true;
                            break;
                        }
                    }
                    if (!found_match) {
                        // If no matching begin command was found, return an error
                        return {false, "Unmatched 'end' command for identifier '" + tokens[i + 1].value + "' at token index " + std::to_string(i)};
                    }
                    i++; // Skip the identifier token
                }
            }
        }
        if (!begin_identifiers.empty()) {
            // If there are unmatched begin commands left, return an error
            std::string unmatched = "Unmatched 'begin' command(s):";
            for (const auto& begin : begin_identifiers) {
                unmatched += "\n\t'" + begin.second + "' at token index " + std::to_string(begin.first) + ", ";
            }
            unmatched.pop_back();
            unmatched.pop_back();
            return {false, unmatched};
        }
        // If we reach here, all begin and end commands are properly nested
        return {true, ""};

    }
    validation digiparser::validate_command(const command& cmd, const std::vector<token>& tokens, size_t index, std::vector<token> *captured_identities) const {
        // the start index is the command itself, so we start at index + 1, verifying the arguments
        index++; // Move past the command token
        
        for (const auto& arg : cmd.arguments) {
            if (index >= tokens.size()) {
                return {false, "Missing argument '" + arg.name + "' for command '" + cmd.name + "'"};
            }
            token current_token = tokens.at(index);
            if (current_token.type == token_type::IDENTIFIER && (arg.type == argument_type::IDENTIFIER || arg.type == argument_type::VALUE)) {
                bool is_capturing = false;
                bool is_declarative = false;
                bool is_multiple = false;
                for (const auto& option : arg.configs.value_or(std::vector<argument_options>{})) {
                    if (option == argument_options::CAPTURING) {
                        is_capturing = true;
                    }
                    if (option == argument_options::DECLARATIVE) {
                        is_declarative = true;
                    }
                    if (option == argument_options::MULTIPLE) {
                        is_multiple = true;
                    }
                }
                
                bool is_captured = false;
                for (const auto& captured : *captured_identities) {
                    if (captured.value == current_token.value) {
                        is_captured = true;
                        break;
                    }
                }
                if (is_capturing) {
                    if (is_multiple) {
                        while (index < tokens.size() && tokens[index].type == token_type::IDENTIFIER) {
                            is_captured = false;
                            for (const auto& captured : *captured_identities) {
                                if (captured.value == tokens[index].value) {
                                    is_captured = true;
                                    break;
                                }
                            }
                            if (!is_captured) {
                                captured_identities->push_back(tokens[index]);
                            } else {
                                return {false, "Identifier '" + tokens[index].value + "' used in command '" + cmd.name + "' on token index " + std::to_string(index) + " is already captured."};
                            }
                            index++;
                        }
                        index--; // Adjust index to point to the last identifier processed
                    } else {
                        if (!is_captured) {
                            // If the identifier is not declared, we can capture it
                            captured_identities->push_back(current_token);
                        } else {
                            return {false, "Identifier '" + current_token.value + "' used in command '" + cmd.name + "' on token index " + std::to_string(index) + " is already captured."};
                        }
                    }
                } else if (!is_captured && !is_declarative) {
                    return {false, "Identifier '" + current_token.value + "' used in command '" + cmd.name + "' on token index " + std::to_string(index) + " is not declared."};
                }
            } else if (current_token.type == token_type::KEYWORD && arg.type == argument_type::KEYWORD) {}
            
            index++;
        }
            
        return {true, ""};
    }
} // namespace digispec