/**
 * @file parse.cpp
 * @author Weiju Wang (weijuwang@aol.com)
 * @brief Parses C code into an AST. @see https://docs.microsoft.com/en-us/cpp/c-language/c-language-syntax-summary?view=msvc-170
 * @date 2022-08-19
 */

#include "parse.hpp"
#include "nonterminal.hpp"

using namespace burbank;
using namespace burbank::parse;

#define LIST_WITH_SEPARATOR(LIST, ITEM, SEPARATOR) \
    {LIST, \
    new list({ \
        ITEM, \
        new opt(new rep(new list({ \
            SEPARATOR, \
            ITEM \
        }))) \
    })}

std::map<nonterminal, parse::abstractSyntax*> burbank::parse::nonterminals = {

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
    {varArgs,
    new list({
        new lit(","),
        new lit("...")
    })},

    {gotoStatement,
    new list({
        new lit("goto"),
        new token(identifier),
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
        new token(identifier),
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
        new token(identifier),
        new token(constant),
        new token(stringLiteral),
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
    new csl(new ref(genericAssociation))},

    {genericAssociation,
    new list({
        new oneOf({
            new ref(typeName),
            new lit("default")
        }),
        new ref(assignmentExpression)
    })},

    {postfixExpression,
    new list({
        new oneOf({
            new ref(primaryExpression),
            new list({
                new lit("("),
                new ref(typeName),
                new lit(")"),
                new lit("{"),
                new ref(initializerList),
                new opt(new lit(",")),
                new lit("}")
            })
        }),
        new opt(new rep(new oneOf({
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
                new token(identifier)
            }),
            new ref(operatorIncrementPostfix),
            new ref(operatorDecrementPostfix)
        })))
    })},

    {argumentExpressionList,
    new csl(new ref(assignmentExpression))},

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

    LIST_WITH_SEPARATOR(
        multiplicativeExpression,
        new ref(castExpression),
        new oneOf({
            new ref(operatorMultiplication),
            new ref(operatorDivision),
            new ref(operatorModulo)
        })
    ),

    LIST_WITH_SEPARATOR(
        additiveExpression,
        new ref(multiplicativeExpression),
        new oneOf({
            new ref(operatorAddition),
            new ref(operatorSubtraction)
        })
    ),

    LIST_WITH_SEPARATOR(
        shiftExpression,
        new ref(additiveExpression),
        new oneOf({
            new ref(operatorBitwiseLeftShift),
            new ref(operatorBitwiseRightShift)
        })
    ),

    LIST_WITH_SEPARATOR(
        relationalExpression,
        new ref(shiftExpression),
        new oneOf({
            new ref(operatorLessThan),
            new ref(operatorGreaterThan),
            new ref(operatorLessThanOrEqualTo),
            new ref(operatorGreaterThanOrEqualTo)
        })
    ),

    LIST_WITH_SEPARATOR(
        equalityExpression,
        new ref(relationalExpression),
        new oneOf({
            new ref(operatorEqualTo),
            new ref(operatorNotEqualTo)
        })
    ),

    LIST_WITH_SEPARATOR(
        bitwiseAndExpression,
        new ref(equalityExpression),
        new ref(operatorBitwiseAnd)
    ),

    LIST_WITH_SEPARATOR(
        bitwiseXorExpression,
        new ref(bitwiseAndExpression),
        new ref(operatorBitwiseXor)
    ),

    LIST_WITH_SEPARATOR(
        bitwiseOrExpression,
        new ref(bitwiseXorExpression),
        new ref(operatorBitwiseOr)
    ),

    LIST_WITH_SEPARATOR(
        logicalAndExpression,
        new ref(bitwiseOrExpression),
        new ref(operatorLogicalAnd)
    ),

    LIST_WITH_SEPARATOR(
        logicalOrExpression,
        new ref(logicalAndExpression),
        new ref(operatorLogicalOr)
    ),

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
    new list({
        new opt(new rep(new list({
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
                new ref(operatorAssignBitwiseXor)
            })
        }))),
        new ref(conditionalExpression)
    })},

    {expression,
    new csl(new ref(assignmentExpression))},

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
    new rep(
        new oneOf({
            new ref(storageClassSpecifier),
            new ref(typeSpecifier),
            new ref(typeQualifier),
            new ref(functionSpecifier),
            new ref(alignmentSpecifier)
        })
    )},

    {initDeclaratorList,
    new csl(new ref(initDeclarator))},

    {initDeclarator,
    new list({
        new ref(declarator),
        new opt(new list({
            new lit("="),
            new ref(initializer)
        }))
    })},

    {storageClassSpecifier,
    new oneOf({
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
            new token(identifier),
            new list({
                new opt(new token(identifier)),
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
    new rep(new ref(structDeclaration))},

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
    new rep(new oneOf({
        new ref(typeSpecifier),
        new ref(typeQualifier),
        new ref(alignmentSpecifier)
    }))},

    {structDeclaratorList,
    new csl(new ref(structDeclarator))},

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
            new token(identifier),
            new list({
                new opt(new token(identifier)),
                new lit("{"),
                new ref(enumeratorList),
                new opt(new lit(",")),
                new lit("}")
            })
        })
    })},

    {enumeratorList,
    new csl(new ref(enumerator))},

    {enumerator,
    new list({
        new token(identifier),
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
    new list({
        new oneOf({
            new token(identifier),
            new list({
                new lit("("),
                new ref(declarator),
                new lit(")")
            })
        }),
        new opt(new rep(new oneOf({
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
        })))
    })},

    {pointer,
    new list({
        new lit("*"),
        new opt(new ref(typeQualifierList)),
        new opt(new ref(pointer))
    })},

    {typeQualifierList,
    new rep(new ref(typeQualifier))},

    {parameterTypeList,
    new list({
        new ref(parameterList),
        new opt(new ref(varArgs))
    })},

    {parameterList,
    new csl(new ref(parameterDeclaration))},

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
    new token(identifier)},

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
    new csl(new list({
        new opt(new ref(designation)),
        new ref(initializer)
    }))},

    {designation,
    new list({
        new ref(designatorList),
        new lit("=")
    })},

    {designatorList,
    new rep(new ref(designator))},

    {designator,
    new oneOf({
        new list({
            new lit("["),
            new ref(constantExpression),
            new lit("]")
        }),
        new list({
            new lit("."),
            new token(identifier)
        })
    })},

    {staticAssertDeclaration,
    new list({
        new lit("_Static_assert"),
        new lit("("),
        new ref(constantExpression),
        new lit(","),
        new token(stringLiteral),
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
    new rep(new ref(declaration))},

    {statementList,
    new rep(new ref(statement))},

    {expressionStatement,
    new list({
        new opt(new ref(expression)),
        new lit(";")
    })},

    {translationUnit,
    new rep(new ref(externalDeclaration))},

    {externalDeclaration,
    new oneOf({
        new ref(functionDefinition),
        new ref(declaration)
    })},

    {functionDefinition,
    new list({
        new opt(new ref(declarationSpecifiers)),
        new ref(declarator),
        new opt(new ref(declarationList)),
        new ref(compoundStatement)
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
    if(pos == tokens.cend()
        or std::string(pos->begin, pos->end) != this->data
    )
        return std::nullopt;
    else
        return ast(pos, pos + 1);
}

DESTROY(ref)
{} // Intentionally empty

MATCH(ref)
{
    // If the referenced nonterminal exists
    if(nonterminals.contains(this->data))
    {
        // Get its syntax, return the result of matching it
        std::optional<ast> result = nonterminals[this->data]->match(tokens, pos);

        if(result.has_value())
        {
            // If the syntax was named
            if(result->name.has_value())
            {
                // Return a node with a single branch linking to the result, so that the original name is preserved
                return ast {
                    this->data,
                    result->begin,
                    result->end,
                    { *result }
                };
            }
            else
            {
                // Record the name of this AST
                result->name = this->data;
                return result;
            }
        }
        else return std::nullopt;
    }
    else return std::nullopt;
}

DESTROY(token)
{} // Intentionally empty

MATCH(token)
{
    if(pos == tokens.cend() or pos->name != this->data)
        return std::nullopt;
    else
        return ast(pos->name, pos, std::next(pos));
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
        return ast(pos);
}

DESTROY(rep)
{
    delete this->data;
}

MATCH(rep)
{
    ast output(pos);
    std::optional<ast> result;

    if(pos == tokens.cend())
        return std::nullopt;

    // Until the end of the text
    while(output.end != tokens.cend())
    {
        // Try to match the repeated rule
        result = this->data->match(tokens, pos);

        // No more repeats; we're done
        if(not result.has_value())
            break;

        // Extend the matched text
        output.end = result->end;

        // Move forward in the text
        pos = result->end;

        // If this was a named rule, add the AST as a branch
        if(result->name.has_value())
            output.branches.push_back(*result);
    }

    // No matches, therefore this whole repeat rule does not match
    if(output.begin == output.end)
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

    if(pos == tokens.cend())
        return std::nullopt;

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
    ast output(pos);
    std::optional<ast> result;

    if(pos == tokens.cend())
        return std::nullopt;

    // For every single syntax in the list
    for(const abstractSyntax* syntax : this->data)
    {
        // Try to match the rule
        result = syntax->match(tokens, pos);

        // Rule didn't match, therefore the whole list does not match
        if(not result.has_value())
            return std::nullopt;

        // Extend the matched text
        output.end = result->end;

        // Move forward in the text
        pos = result->end;

        // If this was a named rule, add the AST as a branch
        if(result->name.has_value())
            output.branches.push_back(*result);
        else
            output.branches.insert(
                output.branches.end(),
                result->branches.begin(),
                result->branches.end()
            );
    }

    // If we're still here, then every rule in the list matched, which is the only way the list as a whole can match.
    return output;
}

DESTROY(csl)
{
    delete this->data;
}

MATCH(csl)
{
    ast output(pos);
    std::optional<ast> result;
    bool onFirstMatch = true;
    lit* comma = new lit(",");

    if(pos == tokens.cend())
        return std::nullopt;

    // Until the end of the text
    while(output.end != tokens.cend())
    {
        // Match a comma before matching the actual token every time except the first.
        if(onFirstMatch)
            onFirstMatch = false;
        else
        {
            result = comma->match(tokens, pos);

            if(not result.has_value())
                break;

            // Matching a literal can only move the position forward by one token.
            ++output.end;
            ++pos;
        }

        // Try to match the repeated rule
        result = this->data->match(tokens, pos);

        // No more repeats; we're done
        if(not result.has_value())
            break;

        // Extend the matched text
        output.end = result->end;

        // Move forward in the text
        pos = result->end;

        // If this was a named rule, add the AST as a branch
        if(result->name.has_value())
            output.branches.push_back(*result);
    }

    delete comma;

    // No matches, therefore this whole repeat rule does not match
    if(output.begin == output.end)
        return std::nullopt;
    else
        return output;
}