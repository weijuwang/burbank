/**
 * @file parse.hpp
 * @author Weiju Wang (weijuwang@aol.com)
 * @brief Parses C code into an AST.
 * @see https://docs.microsoft.com/en-us/cpp/c-language/phrase-structure-grammar?view=msvc-170
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
     * @brief 
     */
    enum nonterminal
    {
        operatorUnaryPositive,
        operatorUnaryNegate,
        operatorUnaryAddressOf,
        operatorUnaryDereference,
        operatorUnaryBitwiseNot,
        operatorUnaryLogicalNot,
        operatorIncrementPrefix,
        operatorDecrementPrefix,

        operatorAddition,
        operatorSubtraction,
        operatorMultiplication,
        operatorDivision,
        operatorModulo,

        operatorBitwiseLeftShift,
        operatorBitwiseRightShift,

        operatorLessThan,
        operatorGreaterThan,
        operatorLessThanOrEqualTo,
        operatorGreaterThanOrEqualTo,
        operatorEqualTo,
        operatorNotEqualTo,

        operatorLogicalAnd,
        operatorLogicalOr,

        operatorBitwiseAnd,
        operatorBitwiseOr,
        operatorBitwiseXor,

        operatorMember,
        operatorIndirect,
        operatorSizeof,

        operatorIncrementPostfix,
        operatorDecrementPostfix,

        operatorAssign,
        operatorAssignMultiply,
        operatorAssignDivide,
        operatorAssignModulo,
        operatorAssignAdd,
        operatorAssignSubtract,
        operatorAssignLeftShift,
        operatorAssignRightShift,
        operatorAssignBitwiseAnd,
        operatorAssignBitwiseOr,
        operatorAssignBitwiseXor,

        storageClassSpecifierAuto,
        storageClassSpecifierExtern,
        storageClassSpecifierRegister,
        storageClassSpecifierStatic,
        storageClassSpecifierThreadLocal,
        storageClassSpecifierTypedef,

        typeSpecifierVoid,
        typeSpecifierChar,
        typeSpecifierShort,
        typeSpecifierInt,
        typeSpecifierLong,
        typeSpecifierFloat,
        typeSpecifierDouble,
        typeSpecifierSigned,
        typeSpecifierUnsigned,
        typeSpecifierBool,

        struct_,
        union_,

        typeQualifierConst,
        typeQualifierRestrict,
        typeQualifierVolatile,
        typeQualifierAtomic,

        functionSpecifierInline,
        functionSpecifierNoReturn,

        starModifier,
        varArgs,

        gotoStatement,
        continueStatement,
        breakStatement,
        returnStatement,
        whileStatement,
        doWhileStatement,
        forStatement,
        ifStatement,
        switchStatement,
        labelStatement,
        caseStatement,
        defaultStatement,

        /* Expressions */
        primaryExpression,
        genericSelection,
        genericAssocList,
        genericAssociation,
        postfixExpression,
        argumentExpressionList,
        unaryExpression,
        unaryOperator,
        castExpression,
        multiplicativeExpression,
        additiveExpression,
        shiftExpression,
        relationalExpression,
        equalityExpression,
        bitwiseAndExpression,
        bitwiseXorExpression,
        bitwiseOrExpression,
        logicalAndExpression,
        logicalOrExpression,
        conditionalExpression,
        assignmentExpression,
        expression,
        constantExpression,

        /* Declarations */
        declaration,
        declarationSpecifiers,
        initDeclaratorList,
        initDeclarator,
        storageClassSpecifier,
        typeSpecifier,
        structOrUnionSpecifier,
        structOrUnion,
        structDeclarationList,
        structDeclaration,
        specifierQualifierList,
        structDeclaratorList,
        structDeclarator,
        enumSpecifier,
        enumeratorList,
        enumerator,
        atomicTypeSpecifier,
        typeQualifier,
        functionSpecifier,
        alignmentSpecifier,
        declarator,
        directDeclarator,
        pointer,
        typeQualifierList,
        parameterTypeList,
        parameterList,
        parameterDeclaration,
        identifierList,
        typeName,
        abstractDeclarator,
        directAbstractDeclarator,
        typedefName,
        initializer,
        initializerList,
        designation,
        designatorList,
        designator,
        staticAssertDeclaration,

        /* Statements */
        statement,
        jumpStatement,
        compoundStatement,
        declarationList,
        statementList,
        expressionStatement,
    };

    /**
     * @brief An abstract syntax tree.
     */
    struct ast
    {
        /**
         * @brief The name of the nonterminal that produced this AST.
         */
        std::optional<nonterminal> name = std::nullopt;

        /**
         * @brief The portion of the original code that corresponds to this AST.
         * @note Since Burbank's lexer ignores whitespace between tokens, whitespace will also not appear here, including whitespace that separates tokens. Therefore, this property is only meaningful when used for individual tokens (identifiers, literals, etc).
         */
        std::string value;

        /**
         * @brief Branches of this AST.
         */
        std::vector<ast> branches = {};

        /**
         * @brief Constructs an AST tree leaf (w/ no branches).
         */
        inline ast(const std::string& value = "") noexcept
        :
            value(value)
        {}
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
        };

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
    SYNTAX_SPECIFIER(token, lexer::nonterminalName)

    /**
     * @brief Optionally matches another nonterminal.
     */
    SYNTAX_SPECIFIER(opt, abstractSyntax*);

    /**
     * @brief Repeats the given syntax at least once.
     */
    //SYNTAX_SPECIFIER(rep, abstractSyntax*);

    /**
     * @brief Matches only one of the syntaxes given.
     */
    SYNTAX_SPECIFIER(oneOf, std::vector<abstractSyntax*>);

    /**
     * @brief Matches all of the syntaxes given, in order.
     */
    SYNTAX_SPECIFIER(list, std::vector<abstractSyntax*>);

    /**
     * @brief Nonterminal symbols understood by the parser.
     *
     * Key = nonterminal name
     *
     * Value = `std::regex` (token pattern)
     */
    extern std::map<nonterminal, abstractSyntax*> nonterminals;
};