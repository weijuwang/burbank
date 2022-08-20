/**
 * @file parse.cpp
 * @author Weiju Wang (weijuwang@aol.com)
 * @brief Parses C code into an AST.
 * @date 2022-08-19
 */

#include "parse.hpp"

using namespace burbank::parse;

typeof(nonterminals) burbank::parse::nonterminals = {
};

#define DESTROY(NAME) \
    NAME::~NAME(void) noexcept

#define MATCH(NAME) \
    std::optional<ast> NAME::match(const std::string& text) const noexcept

DESTROY(abstractSyntax)
{} // Intentionally empty

DESTROY(lit)
{} // Intentionally empty

MATCH(lit)
{
    if(text.starts_with(this->data))
        return ast(this->data);
    else
        return std::nullopt;
}

DESTROY(ref)
{} // Intentionally empty

MATCH(ref)
{
    // If the referenced nonterminal exists
    if(nonterminals.contains(this->data))
    {
        // Get its syntax, return the result of matching it, and record that this was a named rule
        auto result = nonterminals[this->data]->match(text);
        result->name = this->data;
        return result;
    }
    else
        return std::nullopt;
}

DESTROY(opt)
{
    delete this->data;
}

MATCH(opt)
{
    std::optional<ast> result = this->data->match(text);

    // Does the rule match? Great! Return the result as normal.
    if(result.has_value())
        return result;
    // It's fine if not; just return an empty AST to show that the text still "matches", rather than `std::nullopt` like usual.
    else
        return ast();
}

DESTROY(rep)
{
    delete this->data;
}

MATCH(rep)
{
    ast output;
    std::optional<ast> result;
    std::string::const_iterator pos = text.begin();

    // Until the end of the text
    while(pos != text.end())
    {
        // Try to match the repeated rule
        result = this->data->match(
            std::string(pos, text.end()));

        // No more repeats; we're done
        if(not result.has_value())
            break;

        // Add the corresponding text of this repeat
        output.value += result->value;

        // Move forward in the text
        pos += result->value.length();

        // If this was a named rule, add the AST as a branch
        if(result->name.has_value())
            output.branches.push_back(*result);
    }

    // No matches, therefore this whole repeat rule does not match
    if(output.value.empty())
        return std::nullopt;
    else
        return output;
}

DESTROY(oneOf)
{
    for(const abstractSyntax* syntax : this->data)
        delete syntax;
}

MATCH(oneOf)
{
    std::optional<ast> result;

    // Go through all of the alternatives
    for(const auto syntax : this->data)
    {
        result = syntax->match(text);

        // As soon as one of them matches, return the AST
        if(result.has_value())
            return result;
    }

    // If we've gotten here, then none of them have matched.
    return std::nullopt;
}

DESTROY(list)
{
    for(const abstractSyntax* syntax : this->data)
        delete syntax;
}

MATCH(list)
{
    ast output;
    std::optional<ast> result;
    std::string::const_iterator pos = text.begin();

    // For every single syntax in the list
    for(const abstractSyntax* syntax : this->data)
    {
        // Try to match the rule
        result = syntax->match(
            std::string(pos, text.end()));

        // Rule didn't match, therefore the whole list does not match
        if(not result.has_value())
            return std::nullopt;

        // Add the corresponding text of the current rule
        output.value += result->value;

        // Move forward in the text
        pos += result->value.length();

        // If this was a named rule, add the AST as a branch
        if(result->name.has_value())
            output.branches.push_back(*result);
    }

    // If we're still here, then every rule in the list matched, which is the only way the list as a whole can match.
    return output;
}
