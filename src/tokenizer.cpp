/**
 * @file tokenizer.cpp
 * @author Weiju Wang (weijuwang@aol.com)
 * @brief Tokenizes a string.
 * @date 2022-08-17
 */

#include "tokenizer.hpp"

using namespace burbank;

tokenizer::tokenizer(const decltype(tokenizer::tokenTypes)& tokenTypes):
    tokenTypes(tokenTypes)
{}

std::vector<tokenizer::token> tokenizer::tokenize(const std::string& text) noexcept
{
    std::vector<token> output;
    std::optional<token> longest;
    std::string tokenContent;

    this->ambiguous = false;
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

                if(longest.has_value())
                {
                    // Only continue if this token is longer
                    // (Always try to match the longest possible token)
                    if(tokenContent.size() < longest->value.size())
                        break;
                    // If two different types of tokens can be matched, then the text is ambiguous here.
                    else if(tokenContent.size() == longest->value.size())
                    {
                        this->ambiguous = true;
                        goto done;
                    }
                }

                // Add the token
                longest = { lexemeType, tokenContent };

                // Move `pos` to after the token
                _pos = this->_match[0].second;
            }
        }

        // If a token was matched
        if(longest.has_value())
            output.push_back(*longest);
        else goto done;
    }

    done: return output;
}

decltype(tokenizer::_pos) tokenizer::errpos(void) noexcept
{
    return this->_pos;
}