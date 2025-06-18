
#include "digispec/digitokenizer.hpp"
#include <iostream>

namespace digispec {
    const std::array<std::string_view, 3> digi_tokenizer::VALID_KEYWORDS = {
        "mod", "definition", "set"
    };
    const std::array<std::string_view, 10> digi_tokenizer::VALID_COMMANDS = {
        "begin", "end", "display", "input", "output", "alias", "line", "compare", "include", "return"
    };
    const std::array<std::string_view, 2> digi_tokenizer::VALID_SYMBOLS = {
        "runnable", "library"
    };
    std::vector<token> digi_tokenizer::tokenize(const std::string& input) {
        std::vector<token> tokens;
        std::string currentToken;
        std::string capture_to = " ";
        token_type capture_type = token_type::NONE;
        int i = 0;
        do
        {
            char ch = ' ';
            if (input.end() != input.begin() + i)
                ch = input.at(i);

            // delimiter reached or end of string reached and current token is not empty
            if ((ch == ' ' || ch == '\t' || ch == '\n')) {
                if (!currentToken.empty()) {
                    if (currentToken == "--") {
                        capture_to = "\n";
                        capture_type = token_type::COMMENT;
                    }
                    if (capture_type == token_type::COMMENT) {
                        if (ch == '\n') {
                            tokens.push_back({token_type::COMMENT, currentToken});
                            currentToken.clear();
                            capture_type = token_type::NONE;
                        } else {
                            currentToken += ch; // Continue capturing comment
                        }
                    } else if (capture_type == token_type::NONE) {
                        // Check if the current token is valid keyword or command
                        if (is_valid_keyword(currentToken)) {
                            tokens.push_back({token_type::KEYWORD, currentToken});
                        } else if (is_valid_command(currentToken)) {
                            tokens.push_back({token_type::COMMAND, currentToken});
                        } else if (std::all_of(currentToken.begin(), currentToken.end(), is_valid_identifier_char)) {
                            tokens.push_back({token_type::IDENTIFIER, currentToken});
                        } else if (is_valid_symbol(currentToken)) {
                            tokens.push_back({token_type::SYMBOL, currentToken});
                        } else {
                            tokens.push_back({token_type::NONE, currentToken});
                        }
                        currentToken.clear();
                    }
                }
            } else {
                currentToken += ch;
            }
            i++;
        } while (input.end() != input.begin() + i);
        
        return tokens;
    }

}