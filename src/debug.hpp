/**
 * @file debug.hpp
 * @author Weiju Wang (weijuwang@aol.com)
 * @brief Debug utility functions.
 * @date 2022-08-26
 */

#pragma once

#include <iostream>

#include "parse.hpp"

namespace burbank::debug
{
    /**
     * @brief Prints an AST to the console.
     */
    void print(const parse::ast&, const std::size_t x = 0) noexcept;

    /**
     * @brief Names of each nonterminal token.
     */
    extern std::map<nonterminal, std::string> nonterminalNames;
}