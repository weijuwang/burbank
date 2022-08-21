/**
 * @file lexer.hpp
 * @author Weiju Wang (weijuwang@aol.com)
 * @brief Tokenizes C code.
 * @see https://docs.microsoft.com/en-us/cpp/c-language/lexical-grammar?view=msvc-170
 * @date 2022-08-17
 */

#pragma once

#include <optional>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <regex>

/* Helpers for regular expressions */
#define OR "|"
#define LITERAL(c) "\\" c
#define BLOCK(r) "(" r ")"
#define ZERO_OR_MORE(r) BLOCK(r) "*"
#define ONE_OR_MORE(r) BLOCK(r) "+"
#define OPTIONAL(r) BLOCK(r) "?"
#define ANY_EXCEPT(r) "[^" r "]"
#define ANY_OF(r) "[" r "]"

#define BACKSLASH LITERAL("\\")
#define QUOTE "\""
#define CARRIAGE_RETURN "\\r"
#define NEWLINE "\\n"
#define WHITESPACE ANY_EXCEPT("\\S" CARRIAGE_RETURN NEWLINE)

#define NEWLINES ONE_OR_MORE(NEWLINE OR CARRIAGE_RETURN)

/* Keywords */

#define KEYWORD BLOCK( \
    "auto" OR "break" OR "case" OR "char" OR "const" OR "continue" OR "default" \
    OR "do" OR "double" OR "else" OR "enum" OR "extern" OR "float" OR "for" \
    OR "goto" OR "if" OR "inline" OR "int" OR "long" OR "register" OR "restrict" \
    OR "return" OR "short" OR "signed" OR "sizeof" OR "static" OR "struct" \
    OR "switch" OR "typedef" OR "union" OR "unsigned" OR "void" OR "volatile" \
    OR "while" OR "_Bool" \
)

/* Identifiers */

#define IDENTIFIER BLOCK( \
    IDENTIFIER_NONDIGIT \
    OPTIONAL(ONE_OR_MORE( \
        IDENTIFIER_NONDIGIT OR DIGIT \
    )) \
)

#define IDENTIFIER_NONDIGIT BLOCK( \
    NONDIGIT \
    OR UNIVERSAL_CHARACTER_NAME \
)

#define NONDIGIT ANY_OF("_a-zA-Z")
#define DIGIT ANY_OF("0-9")

#define UNIVERSAL_CHARACTER_NAME BLOCK( \
    BACKSLASH "u" HEX_QUAD \
    OR BACKSLASH "U" HEX_QUAD HEX_QUAD \
)

#define HEX_QUAD HEXADECIMAL_DIGIT "{4}"

/* Constants */

#define CONSTANT BLOCK( \
    INTEGER_CONSTANT \
    OR FLOATING_CONSTANT \
    /* OR ENUMERATION_CONSTANT */ \
    OR CHARACTER_CONSTANT \
)

#define INTEGER_CONSTANT BLOCK( \
    BLOCK( \
        DECIMAL_CONSTANT \
        OR BINARY_CONSTANT \
        OR OCTAL_CONSTANT \
        OR HEXADECIMAL_CONSTANT \
    ) \
    OPTIONAL(INTEGER_SUFFIX) \
)

#define DECIMAL_CONSTANT BLOCK(NONZERO_DIGIT ZERO_OR_MORE(DIGIT))
#define BINARY_CONSTANT BLOCK(BINARY_PREFIX ONE_OR_MORE(BINARY_DIGIT))

#define BINARY_PREFIX "0" ANY_OF("b" "B")
#define BINARY_DIGIT ANY_OF("0" "1")

#define OCTAL_CONSTANT BLOCK( \
    "0" ONE_OR_MORE(OCTAL_DIGIT) \
)

#define HEXADECIMAL_CONSTANT BLOCK( \
    HEXADECIMAL_PREFIX ONE_OR_MORE(HEXADECIMAL_DIGIT) \
)

#define HEXADECIMAL_PREFIX "0" ANY_OF("x" "X")

#define NONZERO_DIGIT ANY_OF("1-9")
#define OCTAL_DIGIT ANY_OF("0-7")
#define HEXADECIMAL_DIGIT ANY_OF("0-9" "a-f" "A-F")

#define INTEGER_SUFFIX BLOCK( \
    BLOCK( \
        UNSIGNED_SUFFIX OPTIONAL(LONG_SUFFIX OR LONG_LONG_SUFFIX) \
    ) \
    OR BLOCK( \
        BLOCK(LONG_SUFFIX OR LONG_LONG_SUFFIX) OPTIONAL(UNSIGNED_SUFFIX) \
    ) \
)

#define UNSIGNED_SUFFIX ANY_OF("u" "U")
#define LONG_SUFFIX ANY_OF("l" "L")
#define LONG_LONG_SUFFIX BLOCK("ll" OR "LL")

#define FLOATING_CONSTANT BLOCK( \
    DECIMAL_FLOATING_CONSTANT \
    OR HEXADECIMAL_FLOATING_CONSTANT \
)

#define DECIMAL_FLOATING_CONSTANT BLOCK( \
    BLOCK( \
        FRACTIONAL_CONSTANT \
        OPTIONAL(EXPONENT_PART) \
        OPTIONAL(FLOATING_SUFFIX) \
    ) \
    OR BLOCK ( \
        DIGIT_SEQUENCE \
        EXPONENT_PART \
        OPTIONAL(FLOATING_SUFFIX) \
    ) \
)

#define HEXADECIMAL_FLOATING_CONSTANT BLOCK( \
    HEXADECIMAL_PREFIX \
    BLOCK( \
        BLOCK( \
            HEXADECIMAL_FRACTIONAL_CONSTANT \
            OPTIONAL(BINARY_EXPONENT_PART) \
        ) \
        OR BLOCK( \
            HEXADECIMAL_DIGIT_SEQUENCE \
            BINARY_EXPONENT_PART \
        ) \
    ) \
    OPTIONAL(FLOATING_SUFFIX) \
)

#define FRACTIONAL_CONSTANT BLOCK( \
    BLOCK(OPTIONAL(DIGIT_SEQUENCE) LITERAL(".") DIGIT_SEQUENCE) \
    OR BLOCK(DIGIT_SEQUENCE LITERAL(".")) \
)

#define EXPONENT_PART BLOCK( \
    ANY_OF("e" "E") \
    OPTIONAL(SIGN) \
    DIGIT_SEQUENCE \
)

#define SIGN ANY_OF("+" "-")

#define DIGIT_SEQUENCE ONE_OR_MORE(DIGIT)

#define HEXADECIMAL_FRACTIONAL_CONSTANT BLOCK( \
    BLOCK( \
        OPTIONAL(HEXADECIMAL_DIGIT_SEQUENCE) \
        LITERAL(".") \
        HEXADECIMAL_DIGIT_SEQUENCE \
    ) \
    OR BLOCK(HEXADECIMAL_DIGIT_SEQUENCE LITERAL(".")) \
)

#define BINARY_EXPONENT_PART BLOCK( \
    ANY_OF("p" "P") OPTIONAL(SIGN) DIGIT_SEQUENCE \
)

#define HEXADECIMAL_DIGIT_SEQUENCE ONE_OR_MORE(HEXADECIMAL_DIGIT)

#define FLOATING_SUFFIX ANY_OF("f" "F" "l" "L")

/*
This seems rather useless. It only exists in the original lexical grammar
because it's referenced later by the parser, but since my lexer cannot
distinguish between an `identifier` and an `enumeration-constant`, the
parser will never see an `enumeration-constant`.
*/
//#define ENUMERATION_CONSTANT IDENTIFIER

#define CHARACTER_CONSTANT BLOCK( \
    OPTIONAL("L") "'" C_CHAR_SEQUENCE "'" \
)

#define C_CHAR_SEQUENCE ONE_OR_MORE(C_CHAR)
#define C_CHAR BLOCK(ANY_EXCEPT("'" BACKSLASH "n") OR ESCAPE_SEQUENCE)

#define ESCAPE_SEQUENCE BLOCK( \
    SIMPLE_ESCAPE_SEQUENCE \
    OR OCTAL_ESCAPE_SEQUENCE \
    OR HEXADECIMAL_ESCAPE_SEQUENCE \
    OR UNIVERSAL_CHARACTER_NAME \
)

#define SIMPLE_ESCAPE_SEQUENCE BLOCK( \
    BACKSLASH \
    ANY_OF( \
        "a" "b" "f" "n" "r" "t" "v" "'" \
        QUOTE \
        BACKSLASH \
        LITERAL("?") \
    ) \
)

#define OCTAL_ESCAPE_SEQUENCE BLOCK( \
    BACKSLASH \
    OCTAL_DIGIT "{1,3}" \
)

#define HEXADECIMAL_ESCAPE_SEQUENCE BLOCK( \
    BACKSLASH "x" ONE_OR_MORE(HEXADECIMAL_DIGIT) \
)

/* String literals */

#define STRING_LITERAL BLOCK( \
    OPTIONAL(ENCODING_PREFIX) QUOTE OPTIONAL(S_CHAR_SEQUENCE) QUOTE \
)

#define ENCODING_PREFIX BLOCK( \
    "u8" OR "u" OR "U" OR "L" \
)

#define S_CHAR_SEQUENCE ONE_OR_MORE(S_CHAR)

#define S_CHAR BLOCK( \
    ANY_EXCEPT(QUOTE BACKSLASH NEWLINE) \
    OR ESCAPE_SEQUENCE \
)

/* Punctuators
    Must be sorted by length, longest to shortest.
*/
#define PUNCTUATOR BLOCK( \
/* 3 chars */ \
    "<<=" OR ">>=" OR LITERAL(".") "{3}" \
/* 2 chars */ \
    OR BLOCK(LITERAL("+") LITERAL("+")) OR "--" OR "<<" OR ">>" \
    OR "<=" OR ">=" OR "==" OR "!=" \
    OR "&&" OR BLOCK(LITERAL("|") LITERAL("|")) \
     OR LITERAL("*=") OR LITERAL("/=") OR "%=" OR LITERAL("+=") OR "-=" \
    OR "&=" OR LITERAL("^=") OR LITERAL("|=") OR "##" \
    OR "<:" OR ":>" OR "<%" OR "%>" OR "->" \
/* 1 char */ \
    OR ANY_OF("&" LITERAL("*") LITERAL("?") LITERAL("-") "~!%<>:;=,#") \
    OR LITERAL("+") OR LITERAL("/") \
    OR LITERAL("[") OR LITERAL("]") OR LITERAL("(") OR LITERAL(")") \
    OR LITERAL("{") OR LITERAL("}") OR LITERAL(".") OR LITERAL("^") OR LITERAL("|") \
)

/* Header names */

#define HEADER_NAME BLOCK( \
    BLOCK("<" H_CHAR_SEQUENCE ">") \
    OR BLOCK(QUOTE Q_CHAR_SEQUENCE QUOTE) \
)

#define H_CHAR_SEQUENCE ONE_OR_MORE(H_CHAR)
#define Q_CHAR_SEQUENCE ONE_OR_MORE(Q_CHAR)

#define H_CHAR ANY_EXCEPT(NEWLINE ">")
#define Q_CHAR ANY_EXCEPT(NEWLINE QUOTE)

/* TODO Preprocessing numbers */

namespace burbank
{
    /**
     * @brief Tokenizes a string.
     */
    class lexer
    {
    private:
        std::smatch _match;
        std::string::const_iterator _pos;

    public:
        /**
         * @brief A nonterminal symbol.
         */
        enum nonterminalName
        {
            newlines,
            whitespace,
            keyword,
            identifier,
            constant,
            stringLiteral,
            punctuator
        };

        /**
         * @brief A recognized token in a string.
         */
        struct token
        {
            nonterminalName name;
            std::string value;
        };

        static const std::map<decltype(token::name), std::regex> tokens;

        /**
         * @brief Nonterminal symbols outputted by the lexer.
         *
         * Key = nonterminal name
         *
         * Value = `std::regex` (token pattern)
         */
        std::map<
            decltype(token::name),
            std::regex
        > nonterminals;

        /**
         * @brief Whether to include newlines in the output.
         */
        const bool includeNewlines;

        /**
         * @brief Construct a new tokenizer object.
         */
        inline lexer(
            const decltype(lexer::nonterminals)& nonterminals,
            const bool includeNewlines = false
        ) noexcept
        :
            nonterminals(nonterminals), includeNewlines(includeNewlines)
        {}

        /**
         * @brief Tokenize a string.
         *
         * @note Regardless of whether the string is valid, this function will never throw an exception. The operation only succeeded if `errpos() == text.end()`.
         *
         * @param text The text to tokenize.
         *
         * @return std::vector<tokenizer::token> All tokens that were produced up until `errpos()`.
         */
        std::vector<lexer::token> tokenize(const std::string& text) noexcept;

        /**
         * @brief The position at which there was an error, or the end of the string if no error occurred.
         */
        inline decltype(_pos) errpos(void) noexcept
        {
            return this->_pos;
        }
    };
}