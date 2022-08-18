/**
 * @file tokenizer.hpp
 * @author Weiju Wang (weijuwang@aol.com)
 * @brief Tokenizes a string.
 * @date 2022-08-17
 */

#pragma once

#include <compare>
#include <optional>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <regex>

namespace burbank
{
    /**
    * @brief Tokenizes a string.
    */
    class tokenizer
    {
    private:

        std::smatch _match;
        std::string::const_iterator _pos;

    public:

        /**
        * @brief A recognized token in a string. This is the data outputted by the tokenizer.
        */
        struct token
        {
            int type;
            std::string value;
        };

        /**
        * @brief List of all token types understood by the tokenizer.
        * Key = token type
        * Value = token pattern as `std::regex`
        */
        std::map<
            decltype(token::type),
            std::regex
        > tokenTypes;

        /**
            * @brief If `tokenizer()` is called on an ambiguous text (a certain token can be interpreted one of multiple ways), this will be set to `true`.
            */
        bool ambiguous;

        /**
        * @brief Construct a new tokenizer object.
        */
        tokenizer(const decltype(tokenizer::tokenTypes)& tokenTypes);

        /**
        * @brief Tokenize a string.
        *
        * @note Regardless of whether the string is valid, this function will never throw an exception. The operation only succeeded if `errpos() == text.end()`.
        *
        * @return std::vector<tokenizer::token> All tokens that were produced up until `errpos()`.
        */
        std::vector<tokenizer::token> tokenize(const std::string& text) noexcept;

        /**
        * @brief The position at which there was an error, or the end of the string if no error occurred.
        */
        decltype(_pos) errpos(void) noexcept;
    };
}