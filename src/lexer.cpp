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
    std::string tokenContent;

    this->_pos = text.cbegin();

    // Until the end of the string
    while(this->_pos != text.cend())
    {
        // For each nonterminal
        for(const auto& [tokenName, regex] : nonterminals)
        {
            // If the lexeme matches at exactly `_pos`
            if(std::regex_search(this->_pos, text.cend(), this->_match, regex)
                and this->_match[0].first == this->_pos
            ){
                tokenContent = this->_match.str();

                // Skip whitespace, or newlines if not `includeNewlines`
                if(tokenName == whitespace
                    or (not this->includeNewlines and tokenName == newlines)
                ){
                    this->_pos += tokenContent.length();
                    goto nextToken;
                }

                // Add the token
                output.push_back({
                    tokenName,
                    this->_pos,
                    // Move forward in the text
                    (this->_pos += tokenContent.length())
                });

                goto nextToken;
            }
        }

        break;

        nextToken:;
    }

    return output;
}