#include <QSet>
#include <QStack>
#include <QString>
#include <algorithm>

#include <iostream>

#include "notationconvertercore.h"

namespace NotationConverter {
struct Intermediate {
    QString sub_expr; // subexpression string
    bool oper;        // the operator used to create this subexpression
};
}

QString NotationConverter::PostfixToInfix::operator()(QString postfixNotation)
{
    if (postfixNotation.isEmpty()) return postfixNotation;
    QStringList l_subString = postfixNotation.split(' ', QString::SkipEmptyParts);

    static QSet<QString> additionalOperators     = {"+", "-"};
    static QSet<QString> multiplicativeOperators = {"^", "*", "/"};
    QStack<Intermediate> stackIntermediate;

    foreach (QString token, l_subString) {
        if (additionalOperators.contains(token)) {
            if (stackIntermediate.size() < 2) {
                return postfixNotation;
            }
            auto rhs = stackIntermediate.pop();
            auto lhs = stackIntermediate.pop();

            stackIntermediate.push(Intermediate{lhs.sub_expr + " " + token + " " + rhs.sub_expr, true});
        }
        else if (multiplicativeOperators.contains(token)) {
            if (stackIntermediate.size() < 2) {
                return postfixNotation;
            }
            auto rhs = stackIntermediate.pop();
            auto lhs = stackIntermediate.pop();

            stackIntermediate.push(Intermediate{(lhs.oper ? "( " + lhs.sub_expr + " )" : lhs.sub_expr)
                                                    + " "
                                                    + token
                                                    + " "
                                                    + (rhs.oper ? "( " + rhs.sub_expr + " )" : rhs.sub_expr),
                                                true});
        }
        else {
            stackIntermediate.push(Intermediate{token, false});
        }
    }
    if (stackIntermediate.size() != 1) return postfixNotation;
    return stackIntermediate.pop().sub_expr;
}
