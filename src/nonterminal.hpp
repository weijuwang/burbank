/**
 * @file nonterminals.hpp
 * @author Weiju Wang (weijuwang@aol.com)
 * @brief 
 * @date 2022-08-21
 */

#pragma once

namespace burbank
{
    /**
     * @brief A nonterminal symbol.
     */
    enum nonterminal
    {
        newlines,
        whitespace,
        keyword,
        identifier,
        constant,
        stringLiteral,
        punctuator,

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
}