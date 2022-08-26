/**
 * @file parse.hpp
 * @author Weiju Wang (weijuwang@aol.com)
 * @brief Parses C code into an AST.
 * @date 2022-08-19
 */

#pragma once

#include "lexer.hpp"

#include <optional>
#include <variant>
#include <string>
#include <vector>
#include <map>

namespace burbank::parse
{
    /**
     * @brief An abstract syntax tree.
     */
    struct ast
    {
    public:
        /**
         * @brief The name of the nonterminal that produced this AST.
         */
        std::optional<nonterminal> name = std::nullopt;

        /**
         * @brief The beginning of the text corresponding to this AST.
         */
        std::vector<lexer::token>::const_iterator begin;

        /**
         * @brief The end of the text corresponding to this AST.
         */
        std::vector<lexer::token>::const_iterator end;

        /**
         * @brief Branches of this AST.
         */
        std::vector<ast> branches;

        /**
         * @brief Constructs an AST tree leaf (w/ no branches) beginning from the given token.
         */
        inline ast(const decltype(begin) begin, const decltype(end) end) noexcept
        :
            begin(begin), end(end)
        {}

        /**
         * @brief Constructs an AST tree leaf (w/ no branches) with the given beginning and end tokens.
         */
        inline ast(const decltype(begin) begin) noexcept
        :
            begin(begin), end(begin)
        {}

        /**
         * @brief Constructs an AST tree.
         */
        inline ast(
            const decltype(name)& name,
            const decltype(begin) begin,
            const decltype(end) end,
            const decltype(branches) branches = {}
        ) noexcept
        :
            name(name), begin(begin), end(end), branches(branches)
        {}

    private:
        /**
         * @brief Whether this AST will be saved as its own branch.
         */
        bool persist = false;
    };

    /**
     * @brief 
     */
    struct abstractSyntax
    {
        /**
         * @brief 
         */
        virtual ~abstractSyntax(void) noexcept
            = 0;

        /**
         * @brief 
         */
        virtual std::optional<ast> match(
            const std::vector<lexer::token>&,
            std::vector<lexer::token>::const_iterator
        ) const noexcept
            = 0;
    };

    #define SYNTAX_SPECIFIER(NAME, TYPE) \
        \
        struct NAME: public abstractSyntax \
        { \
            const TYPE data; \
        \
            inline NAME(const TYPE data) \
            : \
                data(data) \
            {} \
        \
            std::optional<ast> match( \
                const std::vector<lexer::token>&, \
                std::vector<lexer::token>::const_iterator \
            ) const noexcept; \
        \
            ~NAME(void) noexcept; \
        }

    /**
     * @brief Matches a string literal.
     */
    SYNTAX_SPECIFIER(lit, std::string);

    /**
     * @brief Matches another nonterminal symbol by its name.
     */
    SYNTAX_SPECIFIER(ref, nonterminal);

    /**
     * @brief Matches a lexical token.
     */
    SYNTAX_SPECIFIER(token, nonterminal);

    /**
     * @brief Optionally matches another nonterminal.
     */
    SYNTAX_SPECIFIER(opt, abstractSyntax*);

    /**
     * @brief Repeats the given syntax at least once.
     */
    SYNTAX_SPECIFIER(rep, abstractSyntax*);

    /**
     * @brief Matches only one of the syntaxes given.
     */
    SYNTAX_SPECIFIER(oneOf, std::vector<abstractSyntax*>);

    /**
     * @brief Matches all of the syntaxes given, in order.
     */
    SYNTAX_SPECIFIER(list, std::vector<abstractSyntax*>);

    /**
     * @brief Matches at least one of the given syntax delimited by a comma.
     */
    SYNTAX_SPECIFIER(csl, abstractSyntax*);

    /**
     * @brief Nonterminal symbols understood by the parser.
     *
     * Key = nonterminal name
     *
     * Value = `std::regex` (token pattern)
     */
    extern std::map<nonterminal, abstractSyntax*> nonterminals;
};