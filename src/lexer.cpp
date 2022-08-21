/**
 * @file lexer.cpp
 * @author Weiju Wang (weijuwang@aol.com)
 * @brief Tokenizes C code.
 * @date 2022-08-17
 */

#include "lexer.hpp"

using burbank::lexer;

const decltype(lexer::tokens) lexer::tokens =
{
    {newlines, std::regex(NEWLINES)},
    {whitespace, std::regex(WHITESPACE)},
    {keyword, std::regex(KEYWORD)},
    {identifier, std::regex(IDENTIFIER)},
    {constant, std::regex(CONSTANT)},
    {stringLiteral, std::regex(STRING_LITERAL)},
    {punctuator, std::regex(PUNCTUATOR)}
};

std::vector<lexer::token> lexer::tokenize(const std::string& text) noexcept
{
    std::vector<token> output;
    std::optional<token> longest;
    std::string tokenContent;

    this->_pos = text.begin();

    // Until the end of the string
    while(this->_pos != text.end())
    {
        // For each nonterminal
        for(const auto& [tokenName, regex] : nonterminals)
        {
            // If the lexeme matches at exactly `_pos`
            if(std::regex_search(this->_pos, text.end(), this->_match, regex)
                and this->_match[0].first == this->_pos)
            {
                tokenContent = this->_match.str();

                // Skip whitespace, or newlines if not `inclNewlines`
                if(tokenName == whitespace
                    or (not this->includeNewlines and tokenName == newlines))
                {
                    this->_pos += tokenContent.length();
                    goto nextToken;
                }

                if(longest.has_value()
                    and tokenContent.length() < longest->value.length())
                    continue;

                // Add the token
                longest = { tokenName, tokenContent };
            }
        }

        // If a token was matched
        if(longest.has_value())
        {
            // Add the token to the list
            output.push_back(*longest);

            // Move `pos` to after the token
            this->_pos += longest->value.length();

            longest.reset();
        }
        else return output;

        nextToken:;
    }

    return output;
}