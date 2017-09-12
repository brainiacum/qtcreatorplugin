/*
* Copyright (c) 2017 Vitali Parkhomenko
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <QSet>
#include <QStack>
#include <QString>

#include <algorithm>

#include "notationconvertercore.h"

using namespace NotationConverter;

QMap<QString, Operation> const PostfixToInfix::defaultOperations = {
    {"+", Operation {2, QString("%1 + %2"), true, QString("Addition")} },
    {"-", Operation {2, QString("%1 - %2"), true, QString("Subtraction")} },
    {"*", Operation {2, QString("%1 * %2"), true, QString("Multiplication")} },
    {"/", Operation {2, QString("%1 / %2"), true, QString("Division")} },
    {"^", Operation {2, QString("%1 ^ %2"), true, QString("Exponentiation")} },
    {"%", Operation {2, QString("%1 % %2"), true, QString("Integer division")} }
};

ConversionResult PostfixToInfix::convert(QString &postfixNotation) const
{
    static auto insertInNotation =
        [] (QString const &notation, QString const &operand) -> QString
    {
        return notation.arg(operand);
    };
    static auto fillNotation =
        [] (QString notation, QStringList const &operands, bool inParenthesis) -> QString
    {
        notation = std::accumulate(operands.begin(), operands.end(), notation, insertInNotation);
        return inParenthesis ? "(" + notation + ")" : notation;
    };

    if (postfixNotation.isEmpty()) {
        return EmptyNotation;
    }
    QStringList subStrings = postfixNotation.split(' ', QString::SkipEmptyParts);

    QStack<QString> operandsStack;
    static auto defaultOperationsSet = QSet<QString>::fromList(defaultOperations.keys());
    auto customOperationsSet = QSet<QString>::fromList(customOperations.keys());

    for (QString const &token : subStrings) {
        bool isCustomOperation = customOperationsSet.contains(token);
        if (!defaultOperationsSet.contains(token) && !isCustomOperation){
            operandsStack.push(token);
        } else {
            Operation operation = isCustomOperation ?
                        customOperations.value(token) :
                        defaultOperations.value(token);
            if (operandsStack.count() < operation.operandsCount) {
                return FailureConversion;
            }
            QStringList operands;
            for (uint i = 0; i < operation.operandsCount; ++i) {
                operands.prepend(operandsStack.pop());
            }
            operandsStack.push(fillNotation(operation.notation, operands, operation.inParenthesis));
        }
    }

    if (operandsStack.count() == 1) {
        postfixNotation = operandsStack.pop();
        return SuccessConversion;
    }
    return FailureConversion;
}
