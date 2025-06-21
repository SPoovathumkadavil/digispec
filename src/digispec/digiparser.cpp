
#include "digispec/digiparser.hpp"

namespace digispec {

    validation digiparser::parse(const std::vector<token>& tokens) {
        
        // validate nesting of begin and end commands
        validation nesting_validation = validate_nesting(tokens);
        if (!nesting_validation.valid) {
            return nesting_validation;
        }

        // validate each command against the registered commands
        // std::vector<token> declared_identities; // Store identifiers for later validation
        // for (size_t i = 0; i < tokens.size(); ++i) {
        //     const token& current_token = tokens[i];
        //     if (current_token.type == token_type::COMMAND) {
        //         // Find the command in the registered commands
        //         std::string command_name = current_token.value;
        //         auto it = std::find_if(registered_commands.begin(), registered_commands.end(),
        //                                [&command_name](const command& cmd) { return cmd.name == command_name; });
        //         // Validate the command
        //         validation cmd_validation = validate_command(*it, tokens, &i, &declared_identities);
        //         if (!cmd_validation.valid) {
        //             // print out the declared_identities registered so far
        //             std::string identities_str;
        //             for (const auto& id : declared_identities) {
        //                 identities_str += id.value + " ";
        //             }
        //             return cmd_validation;
        //         }
        //     } else if (current_token.type == token_type::IDENTIFIER) {
        //         // Register the identifier
        //         declared_identities.push_back(current_token);
        //     } else if (current_token.type == token_type::SYMBOL) {
        //         // validate that symbol is assigned to a module
        //     }
        // }

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
    validation digiparser::validate_command(const command& cmd, const std::vector<token>& tokens, size_t *index, std::vector<token> *declared_identities) const {
        // the start index is the command itself
        // so we start at index + 1, verifying the arguments
        (*index)++; // Move past the command token
        
        for (const auto& arg : cmd.arguments) {
            if (*index >= tokens.size()) {
                return {false, "Missing argument '" + arg.name + "' for command '" + cmd.name + "'"};
            }
            const token& current_token = tokens[*index];
            if (current_token.type == token_type::IDENTIFIER && (arg.type == argument_type::IDENTIFIER || arg.type == argument_type::VALUE)) {
                // Check if the identifier is declared
                if (std::find_if(declared_identities->begin(), declared_identities->end(),
                                 [&current_token](const token& id) { return id.value == current_token.value; }) == declared_identities->end()
                    && arg.configs && std::find(arg.configs->begin(), arg.configs->end(), argument_options::DECLARATIVE) == arg.configs->end()) {
                    return {false, "Undeclared identifier '" + current_token.value + "' for command '" + cmd.name + "'"};
                }
            } else if (current_token.type == token_type::KEYWORD && arg.type == argument_type::KEYWORD) {

            }
            // Move to the next token
            (*index)++;
        }
            
        return {true, ""};
    }
} // namespace digispec