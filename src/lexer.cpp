/**
 * @file lexer.cpp
 * @author Weiju Wang (weijuwang@aol.com)
 * @brief Tokenizes a string.
 * @date 2022-08-17
 */

#include "lexer.hpp"

using namespace burbank;
using burbank::lexer;

const decltype(lexer::tokens) lexer::tokens =
{
    {keyword, std::regex(KEYWORD)},
    {identifier, std::regex(IDENTIFIER)},
    {constant, std::regex(CONSTANT)},
    {stringLiteral, std::regex(STRING_LITERAL)},
    {punctuator, std::regex(PUNCTUATOR)},
};

lexer::lexer(const decltype(lexer::tokenTypes)& tokenTypes):
    tokenTypes(tokenTypes)
{}

std::vector<lexer::token> lexer::tokenize(const std::string& text) noexcept
{
    std::vector<token> output;
    std::optional<token> longest;
    std::string tokenContent;

    this->_pos = text.begin();

    // Until the end of the string
    while(_pos != text.end())
    {
        // For each lexeme
        for(const auto& [lexemeType, lexeme] : tokenTypes)
        {
            // If the lexeme matches at exactly `_pos`
            if(std::regex_search(_pos, text.end(), this->_match, lexeme)
                and this->_match[0].first == _pos)
            {
                tokenContent = this->_match.str();

                if(longest.has_value() && tokenContent.size() < longest->value.size())
                    break;

                // Add the token
                longest = { lexemeType, tokenContent };
            }
        }

        // If a token was matched
        if(longest.has_value())
        {
            // Add the token to the list
            output.push_back(*longest);

            // Move `pos` to after the token
            _pos += longest->value.size();

            longest.reset();
        }
        else goto done;
    }

    done: return output;
}

decltype(lexer::_pos) lexer::errpos(void) noexcept
{
    return this->_pos;
}