/**
 * @file parse.cpp
 * @author Weiju Wang (weijuwang@aol.com)
 * @brief Parses C code into an AST.
 * @date 2022-08-19
 */

#include "parse.hpp"

using namespace burbank::parse;

typeof(nonterminals) burbank::parse::nonterminals = {

    {operatorUnaryPositive, new lit("+")},
    {operatorUnaryNegate, new lit("-")},
    {operatorUnaryAddressOf, new lit("&")},
    {operatorUnaryDereference, new lit("*")},
    {operatorUnaryBitwiseNot, new lit("~")},
    {operatorUnaryLogicalNot, new lit("!")},
    {operatorIncrementPrefix, new lit("++")},
    {operatorDecrementPrefix, new lit("--")},

    {operatorAddition, new lit("+")},
    {operatorSubtraction, new lit("-")},
    {operatorMultiplication, new lit("*")},
    {operatorDivision, new lit("/")},
    {operatorModulo, new lit("%")},

    {operatorBitwiseLeftShift, new lit("<<")},
    {operatorBitwiseRightShift, new lit(">>")},

    {operatorLessThan, new lit("<")},
    {operatorGreaterThan, new lit(">")},
    {operatorLessThanOrEqualTo, new lit("<=")},
    {operatorGreaterThanOrEqualTo, new lit(">=")},
    {operatorEqualTo, new lit("==")},
    {operatorNotEqualTo, new lit("!=")},

    {operatorLogicalAnd, new lit("&&")},
    {operatorLogicalOr, new lit("||")},

    {operatorBitwiseAnd, new lit("&")},
    {operatorBitwiseOr, new lit("|")},
    {operatorBitwiseXor, new lit("^")},

    {operatorMember, new lit(".")},
    {operatorIndirect, new lit("->")},
    {operatorSizeof, new lit("sizeof")},

    {operatorIncrementPostfix, new lit("++")},
    {operatorDecrementPostfix, new lit("--")},

    {operatorAssign, new lit("=")},
    {operatorAssignMultiply, new lit("*=")},
    {operatorAssignDivide, new lit("/=")},
    {operatorAssignModulo, new lit("%=")},
    {operatorAssignAdd, new lit("+=")},
    {operatorAssignSubtract, new lit("-=")},
    {operatorAssignLeftShift, new lit("<<=")},
    {operatorAssignRightShift, new lit(">>=")},
    {operatorAssignBitwiseAnd, new lit("&=")},
    {operatorAssignBitwiseOr, new lit("|=")},
    {operatorAssignBitwiseXor, new lit("^=")},

    {storageClassSpecifierAuto, new lit("auto")},
    {storageClassSpecifierExtern, new lit("extern")},
    {storageClassSpecifierRegister, new lit("register")},
    {storageClassSpecifierStatic, new lit("static")},
    {storageClassSpecifierThreadLocal, new lit("_Thread_local")},
    {storageClassSpecifierTypedef, new lit("typedef")},

    {typeSpecifierVoid, new lit("void")},
    {typeSpecifierChar, new lit("char")},
    {typeSpecifierShort, new lit("short")},
    {typeSpecifierInt, new lit("int")},
    {typeSpecifierLong, new lit("long")},
    {typeSpecifierFloat, new lit("float")},
    {typeSpecifierDouble, new lit("double")},
    {typeSpecifierSigned, new lit("signed")},
    {typeSpecifierUnsigned, new lit("unsigned")},
    {typeSpecifierBool, new lit("_Bool")},

    {struct_, new lit("struct")},
    {union_, new lit("union")},

    {typeQualifierConst, new lit("const")},
    {typeQualifierRestrict, new lit("restrict")},
    {typeQualifierVolatile, new lit("volatile")},
    {typeQualifierAtomic, new lit("_Atomic")},

    {functionSpecifierInline, new lit("inline")},
    {functionSpecifierNoReturn, new lit("_Noreturn")},

    {starModifier, new lit("*")},
    {varArgs, new list({
        new lit(","),
        new lit("...")
    })},

    {gotoStatement,
    new list({
        new lit("goto"),
        new token(lexer::identifier),
        new lit(";"),
    })},

    {continueStatement,
    new list({
        new lit("continue"),
        new lit(";")
    })},

    {breakStatement,
    new list({
        new lit("break"),
        new lit(";")
    })},

    {returnStatement,
    new list({
        new lit("return"),
        new opt(new ref(expression)),
        new lit(";")
    })},

    {whileStatement,
    new list({
        new lit("while"),
        new lit("("),
        new ref(expression),
        new lit(")"),
        new ref(statement)
    })},

    {doWhileStatement,
    new list({
        new lit("do"),
        new ref(statement),
        new lit("while"),
        new lit("("),
        new ref(expression),
        new lit(")"),
        new ref(statement)
    })},

    {forStatement,
    new list({
        new lit("for"),
        new lit("("),
        new opt(new ref(expression)),
        new lit(";"),
        new opt(new ref(expression)),
        new lit(";"),
        new opt(new ref(expression)),
        new lit(")"),
        new ref(statement)
    })},

    {ifStatement,
    new list({
        new lit("if"),
        new lit("("),
        new ref(expression),
        new lit(")"),
        new ref(statement),
        new opt(new list({
            new lit("else"),
            new ref(statement)
        }))
    })},

    {switchStatement,
    new list({
        new lit("switch"),
        new lit("("),
        new ref(expression),
        new lit(")"),
        new ref(statement)
    })},

    {labelStatement,
    new list({
        new token(lexer::identifier),
        new lit(":"),
        new ref(statement)
    })},

    {caseStatement,
    new list({
        new lit("case"),
        new ref(constantExpression),
        new lit(":"),
        new ref(statement)
    })},

    {defaultStatement,
    new list({
        new lit("default"),
        new lit(":"),
        new ref(statement)
    })},

    {primaryExpression,
    new oneOf({
        new token(lexer::identifier),
        new token(lexer::constant),
        new token(lexer::stringLiteral),
        new list({
            new lit("("),
            new ref(expression),
            new lit(")"),
        }),
        new ref(genericSelection)
    })},

    {genericSelection,
    new list({
        new lit("_Generic"),
        new lit("("),
        new ref(assignmentExpression),
        new lit(","),
        new ref(genericAssocList),
    })},

    {genericAssocList,
    new oneOf({
        new ref(genericAssociation),
        new list({
            new ref(genericAssocList),
            new lit(","),
            new ref(genericAssociation)
        })
    })},

    {genericAssociation,
    new list({
        new oneOf({
            new ref(typeName),
            new lit("default")
        }),
        new ref(assignmentExpression)
    })},

    {postfixExpression,
    new oneOf({
        new ref(primaryExpression),
        new list({
            new ref(postfixExpression),
            new oneOf({
                new list({
                    new lit("["),
                    new ref(expression),
                    new lit("]")
                }),
                new list({
                    new lit("("),
                    new opt(new ref(argumentExpressionList)),
                    new lit(")")
                }),
                new list({
                    new oneOf({
                        new ref(operatorMember),
                        new ref(operatorIndirect)
                    }),
                    new token(lexer::identifier)
                }),
                new ref(operatorIncrementPostfix),
                new ref(operatorDecrementPostfix)
            })
        }),
        new list({
            new lit("("),
            new ref(typeName),
            new lit(")"),
            new lit("{"),
            new ref(initializerList),
            new opt(new lit(",")),
            new lit("}")
        })
    })},

    {argumentExpressionList,
    new oneOf({
        new ref(assignmentExpression),
        new list({
            new ref(argumentExpressionList),
            new lit(","),
            new ref(assignmentExpression)
        })
    })},

    {unaryExpression,
    new oneOf({
        new ref(postfixExpression),
        new list({
            new oneOf({
                new ref(operatorIncrementPrefix),
                new ref(operatorDecrementPrefix),
            }),
            new ref(unaryExpression)
        }),
        new list({
            new ref(unaryOperator),
            new ref(castExpression),
        }),
        new list({
            new ref(operatorSizeof),
            new oneOf({
                new ref(unaryExpression),
                new list({
                    new lit("("),
                    new ref(typeName),
                    new lit(")"),
                    new lit("_Alignof"),
                    new lit("("),
                    new ref(typeName)
                })
            })
        })
    })},

    {unaryOperator,
    new oneOf({
        new ref(operatorUnaryPositive),
        new ref(operatorUnaryNegate),
        new ref(operatorUnaryAddressOf),
        new ref(operatorUnaryDereference),
        new ref(operatorUnaryBitwiseNot),
        new ref(operatorUnaryLogicalNot)
    })},

    {castExpression,
    new oneOf({
        new ref(unaryExpression),
        new list({
            new lit("("),
            new ref(typeName),
            new lit(")"),
            new ref(castExpression)
        })
    })},

    {multiplicativeExpression,
    new oneOf({
        new ref(castExpression),
        new list({
            new ref(multiplicativeExpression),
            new oneOf({
                new ref(operatorMultiplication),
                new ref(operatorDivision),
                new ref(operatorModulo),
            }),
            new ref(castExpression)
        })
    })},

    {additiveExpression,
    new oneOf({
        new ref(multiplicativeExpression),
        new list({
            new ref(additiveExpression),
            new oneOf({
                new ref(operatorAddition),
                new ref(operatorSubtraction),
            }),
            new ref(multiplicativeExpression)
        })
    })},

    {shiftExpression,
    new oneOf({
        new ref(additiveExpression),
        new list({
            new ref(shiftExpression),
            new oneOf({
                new ref(operatorBitwiseLeftShift),
                new ref(operatorBitwiseRightShift),
            }),
            new ref(additiveExpression)
        })
    })},

    {relationalExpression,
    new oneOf({
        new ref(shiftExpression),
        new list({
            new ref(relationalExpression),
            new oneOf({
                new ref(operatorLessThan),
                new ref(operatorGreaterThan),
                new ref(operatorLessThanOrEqualTo),
                new ref(operatorGreaterThanOrEqualTo),
            }),
            new ref(shiftExpression)
        })
    })},

    {equalityExpression,
    new oneOf({
        new ref(relationalExpression),
        new list({
            new ref(equalityExpression),
            new oneOf({
                new ref(operatorEqualTo),
                new ref(operatorNotEqualTo)
            }),
            new ref(relationalExpression)
        })
    })},

    {bitwiseAndExpression,
    new oneOf({
        new ref(equalityExpression),
        new list({
            new ref(bitwiseAndExpression),
            new ref(operatorBitwiseAnd),
            new ref(equalityExpression)
        })
    })},

    {bitwiseXorExpression,
    new oneOf({
        new ref(bitwiseAndExpression),
        new list({
            new ref(bitwiseXorExpression),
            new ref(operatorBitwiseXor),
            new ref(bitwiseAndExpression)
        })
    })},

    {bitwiseOrExpression,
    new oneOf({
        new ref(bitwiseXorExpression),
        new list({
            new ref(bitwiseOrExpression),
            new ref(operatorBitwiseOr),
            new ref(bitwiseXorExpression)
        })
    })},

    {logicalAndExpression,
    new oneOf({
        new ref(bitwiseOrExpression),
        new list({
            new ref(logicalAndExpression),
            new ref(operatorLogicalAnd),
            new ref(bitwiseOrExpression)
        })
    })},

    {logicalOrExpression,
    new oneOf({
        new ref(logicalAndExpression),
        new list({
            new ref(logicalOrExpression),
            new ref(operatorLogicalAnd),
            new ref(logicalAndExpression)
        })
    })},

    {conditionalExpression,
    new list({
        new ref(logicalOrExpression),
        new opt(new list({
            new lit("?"),
            new ref(expression),
            new lit(":"),
            new ref(conditionalExpression)
        }))
    })},

    {assignmentExpression,
    new oneOf({
        new ref(conditionalExpression),
        new list({
            new ref(unaryExpression),
            new oneOf({
                new ref(operatorAssign),
                new ref(operatorAssignMultiply),
                new ref(operatorAssignDivide),
                new ref(operatorAssignModulo),
                new ref(operatorAssignAdd),
                new ref(operatorAssignSubtract),
                new ref(operatorAssignLeftShift),
                new ref(operatorAssignRightShift),
                new ref(operatorAssignBitwiseAnd),
                new ref(operatorAssignBitwiseOr),
                new ref(operatorAssignBitwiseXor),
            }),
            new ref(assignmentExpression)
        })
    })},

    {expression,
    new oneOf({
        new ref(assignmentExpression),
        new list({
            new ref(expression),
            new lit(","),
            new ref(assignmentExpression)
        })
    })},

    {constantExpression,
    new ref(conditionalExpression)},

    {declaration,
    new oneOf({
        new list({
            new ref(declarationSpecifiers),
            new opt(new ref(initDeclaratorList)),
            new lit(";")
        }),
        new ref(staticAssertDeclaration)
    })},

    {declarationSpecifiers,
    new list({
        new oneOf({
            new ref(storageClassSpecifier),
            new ref(typeSpecifier),
            new ref(typeQualifier),
            new ref(functionSpecifier),
            new ref(alignmentSpecifier)
        }),
        new opt(new ref(declarationSpecifiers))
    })},

    {initDeclaratorList,
    new oneOf({
        new ref(initDeclarator),
        new list({
            new ref(initDeclaratorList),
            new lit(","),
            new ref(initDeclarator)
        })
    })},

    {initDeclarator,
    new list({
        new ref(declarator),
        new opt(new list({
            new lit("="),
        })),
        new ref(initializer)
    })},

    {storageClassSpecifier,
    new oneOf({
        new ref(storageClassSpecifierAuto),
        new ref(storageClassSpecifierExtern),
        new ref(storageClassSpecifierRegister),
        new ref(storageClassSpecifierStatic),
        new ref(storageClassSpecifierThreadLocal),
        new ref(storageClassSpecifierTypedef)
    })},

    {typeSpecifier,
    new oneOf({
        new ref(typeSpecifierVoid),
        new ref(typeSpecifierChar),
        new ref(typeSpecifierShort),
        new ref(typeSpecifierInt),
        new ref(typeSpecifierLong),
        new ref(typeSpecifierFloat),
        new ref(typeSpecifierDouble),
        new ref(typeSpecifierSigned),
        new ref(typeSpecifierUnsigned),
        new ref(typeSpecifierBool)
    })},

    {structOrUnionSpecifier,
    new list({
        new ref(structOrUnion),
        new oneOf({
            new token(lexer::identifier),
            new list({
                new opt(new token(lexer::identifier)),
                new lit("{"),
                new ref(structDeclarationList),
                new lit("}")
            })
        })
    })},

    {structOrUnion,
    new oneOf({
        new ref(struct_),
        new ref(union_)
    })},

    {structDeclarationList,
    new oneOf({
        new ref(structDeclaration),
        new list({
            new ref(structDeclarationList),
            new ref(structDeclaration)
        })
    })},

    {structDeclaration,
    new oneOf({
        new list({
            new ref(specifierQualifierList),
            new opt(new ref(structDeclaratorList)),
            new lit(";")
        }),
        new ref(staticAssertDeclaration)
    })},

    {specifierQualifierList,
    new list({
        new oneOf({
            new ref(typeSpecifier),
            new ref(typeQualifier),
        }),
        new opt(new ref(specifierQualifierList))
    })},

    {structDeclaratorList,
    new oneOf({
        new ref(structDeclarator),
        new list({
            new ref(structDeclarationList),
            new lit(","),
            new ref(structDeclarator)
        })
    })},

    {structDeclarator,
    new oneOf({
        new ref(declarator),
        new list({
            new opt(new ref(declarator)),
            new lit(":"),
            new ref(constantExpression)
        })
    })},

    {enumSpecifier,
    new list({
        new lit("enum"),
        new oneOf({
            new token(lexer::identifier),
            new list({
                new opt(new token(lexer::identifier)),
                new lit("{"),
                new ref(enumeratorList),
                new opt(new lit(",")),
                new lit("}")
            })
        })
    })},

    {enumeratorList,
    new oneOf({
        new ref(enumerator),
        new list({
            new ref(enumeratorList),
            new lit(","),
            new ref(enumerator)
        })
    })},

    {enumerator,
    new list({
        new token(lexer::identifier),
        new opt(new list({
            new lit("="),
            new ref(constantExpression)
        }))
    })},

    {atomicTypeSpecifier,
    new list({
        new lit("_Atomic"),
        new lit("("),
        new ref(typeName),
        new lit(")")
    })},

    {typeQualifier,
    new oneOf({
        new ref(typeQualifierConst),
        new ref(typeQualifierRestrict),
        new ref(typeQualifierVolatile),
        new ref(typeQualifierAtomic)
    })},

    {functionSpecifier,
    new oneOf({
        new ref(functionSpecifierInline),
        new ref(functionSpecifierNoReturn)
    })},

    {alignmentSpecifier,
    new list({
        new lit("_Alignas"),
        new lit("("),
        new oneOf({
            new ref(typeName),
            new ref(constantExpression)
        }),
        new lit(")")
    })},

    {declarator,
    new list({
        new opt(new ref(pointer)),
        new ref(directDeclarator)
    })},

    {directDeclarator,
    new oneOf({
        new token(lexer::identifier),
        new list({
            new lit("("),
            new ref(declarator),
            new lit(")")
        }),
        new list({
            new ref(directDeclarator),
            new oneOf({
                new list({
                    new lit("("),
                    new ref(parameterTypeList),
                    new lit(")")
                }),
                new list({
                    new lit("["),
                    new oneOf({
                        new list({
                            new opt(new ref(typeQualifierList)),
                            new opt(new ref(assignmentExpression))
                        }),
                        new list({
                            new ref(storageClassSpecifierStatic),
                            new opt(new ref(typeQualifierList)),
                            new ref(assignmentExpression)
                        }),
                        new list({
                            new ref(typeQualifierList),
                            new ref(storageClassSpecifierStatic),
                            new opt(new ref(assignmentExpression))
                        }),
                        new list({
                            new opt(new ref(typeQualifierList)),
                            new ref(starModifier)
                        })
                    }),
                    new lit("]")
                })
            })
        })
    })},

    {pointer,
    new list({
        new lit("*"),
        new opt(new ref(typeQualifierList)),
        new opt(new ref(pointer))
    })},

    {typeQualifierList,
    new oneOf({
        new ref(typeQualifier),
        new list({
            new ref(typeQualifierList),
            new ref(typeQualifier)
        })
    })},

    {parameterTypeList,
    new list({
        new ref(parameterList),
        new ref(varArgs)
    })},

    {parameterList,
    new oneOf({
        new ref(parameterDeclaration),
        new list({
            new ref(parameterList),
            new lit(","),
            new ref(parameterDeclaration)
        })
    })},

    {parameterDeclaration,
    new list({
        new ref(declarationSpecifiers),
        new oneOf({
            new ref(declarator),
            new opt(new ref(abstractDeclarator))
        })
    })},

    {typeName,
    new list({
        new ref(specifierQualifierList),
        new opt(new ref(abstractDeclarator))
    })},

    {abstractDeclarator,
    new oneOf({
        new ref(pointer),
        new list({
            new opt(new ref(pointer)),
            new ref(directAbstractDeclarator)
        })
    })},

    {directAbstractDeclarator,
    new oneOf({
        new list({
            new lit("("),
            new ref(abstractDeclarator),
            new lit(")")
        }),
        new list({
            new ref(directAbstractDeclarator),
            new lit("["),
            new oneOf({
                new list({
                    new opt(new ref(typeQualifierList)),
                    new opt(new ref(assignmentExpression))
                }),
                new list({
                    new ref(storageClassSpecifierStatic),
                    new opt(new ref(typeQualifierList)),
                    new ref(assignmentExpression)
                }),
                new list({
                    new ref(typeQualifierList),
                    new ref(storageClassSpecifierStatic),
                    new ref(assignmentExpression),
                }),
                new list({
                    new opt(new ref(typeQualifierList)),
                    new ref(starModifier),
                })
            }),
            new lit("]")
        }),
        new list({
            new opt(new ref(directAbstractDeclarator)),
            new lit("("),
            new opt(new ref(parameterTypeList)),
            new lit(")")
        })
    })},

    {typedefName,
    new token(lexer::identifier)},

    {initializer,
    new oneOf({
        new ref(assignmentExpression),
        new list({
            new lit("{"),
            new ref(initializerList),
            new opt(new lit(",")),
            new lit("}")
        })
    })},

    {initializerList,
    new oneOf({
        new list({
            new opt(new ref(designation)),
            new ref(initializer)
        }),
        new list({
            new ref(initializerList),
            new lit(","),
            new opt(new ref(designation)),
            new ref(initializer)
        })
    })},

    {designation,
    new list({
        new ref(designatorList),
        new lit("=")
    })},

    {designatorList,
    new oneOf({
        new ref(designator),
        new list({
            new ref(designatorList),
            new ref(designator)
        })
    })},

    {designator,
    new oneOf({
        new list({
            new lit("["),
            new ref(constantExpression),
            new lit("]")
        }),
        new list({
            new lit("."),
            new token(lexer::identifier)
        })
    })},

    {staticAssertDeclaration,
    new list({
        new lit("_Static_assert"),
        new lit("("),
        new ref(constantExpression),
        new lit(","),
        new token(lexer::stringLiteral),
        new lit(")"),
        new lit(";")
    })},

    {statement,
    new oneOf({
        new ref(labelStatement),
        new ref(caseStatement),
        new ref(defaultStatement),
        new ref(compoundStatement),
        new ref(expressionStatement),
        new ref(ifStatement),
        new ref(switchStatement),
        new ref(gotoStatement),
        new ref(continueStatement),
        new ref(breakStatement),
        new ref(returnStatement)
    })},

    {compoundStatement,
    new list({
        new lit("{"),
        new opt(new ref(declarationList)),
        new opt(new ref(statementList)),
        new lit("}")
    })},

    {declarationList,
    new oneOf({
        new ref(declaration),
        new list({
            new ref(declarationList),
            new ref(declaration)
        })
    })},

    {statementList,
    new oneOf({
        new ref(statement),
        new list({
            new ref(statementList),
            new ref(statement)
        })
    })},

    {expressionStatement,
    new list({
        new opt(new ref(expression)),
        new lit(";")
    })}
};

#define DESTROY(NAME) \
    NAME::~NAME(void) noexcept

#define MATCH(NAME) \
    std::optional<ast> NAME::match( \
        const std::vector<lexer::token>& tokens, \
        std::vector<lexer::token>::const_iterator pos \
    ) const noexcept

DESTROY(abstractSyntax)
{} // Intentionally empty

DESTROY(lit)
{} // Intentionally empty

MATCH(lit)
{
    if(pos->value == this->data)
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
        std::optional<ast> result = nonterminals[this->data]->match(tokens, pos);

        // If the syntax was named
        if(result->name.has_value())
        {
            // Return a node with a single branch linking to the result, so that the original name is preserved
            return ast {
                this->data,
                result->value,
                { *result }
            };
        }
        else
        {
            result->name = this->data;
            return result;
        }
    }
    else
        return std::nullopt;
}

DESTROY(token)
{} // Intentionally empty

MATCH(token)
{
    if(pos->name == this->data)
        return ast(pos->value);
    else
        return std::nullopt;
}

DESTROY(opt)
{
    delete this->data;
}

MATCH(opt)
{
    std::optional<ast> result = this->data->match(tokens, pos);

    // Does the rule match? Great! Return the result as normal.
    if(result.has_value())
        return result;
    // It's fine if not; just return an empty AST to show that the text still "matches", rather than `std::nullopt` like usual.
    else
        return ast();
}

/*
DESTROY(rep)
{
    delete this->data;
}

MATCH(rep)
{
    ast output;
    std::optional<ast> result;

    // Until the end of the text
    while(pos != tokens.end())
    {
        // Try to match the repeated rule
        result = this->data->match(tokens, pos);

        // No more repeats; we're done
        if(not result.has_value())
            break;

        // Add the corresponding text of this repeat
        output.value.append(result->value);

        // Move forward in the text
        ++pos;

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
*/

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
        result = syntax->match(tokens, pos);

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

    // For every single syntax in the list
    for(const abstractSyntax* syntax : this->data)
    {
        // Try to match the rule
        result = syntax->match(tokens, pos);

        // Rule didn't match, therefore the whole list does not match
        if(not result.has_value())
            return std::nullopt;

        // Add the corresponding text of the current rule
        output.value.append(result->value);

        // Move forward in the text
        ++pos;

        // If this was a named rule, add the AST as a branch
        if(result->name.has_value())
            output.branches.push_back(*result);
    }

    // If we're still here, then every rule in the list matched, which is the only way the list as a whole can match.
    return output;
}
