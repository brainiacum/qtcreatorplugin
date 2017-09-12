/*
* Copyright (c) 2017 Vitali Parkhomenko
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef NOTATIONCONVERTERCORE_H
#define NOTATIONCONVERTERCORE_H

#include <QMap>
#include <QString>

namespace NotationConverter {
/**
 * @brief Object that can be used as specification for operations.
 * @param operandsCount is a count of operands in the notation of operation.
 * @param notation is a form of record commonly used in arithmetical and logical formulae and statements.
 *        Sample of notation: "sameFunction ( %1 , %2 , %3 )", there %1, %2 and %3 is a operands for
 *        function sameFunction. If a custom operation is a overridden default operation then
 *        the notations should be placed in parentheses.
 * @param inParenthesis is an indication that notation should be placed in parentheses.
 * @param description is a string of operation description.
 */
struct Operation {
    uint operandsCount;
    QString notation;
    bool inParenthesis;
    QString description;
};

enum ConversionResult : quint8 { FailureConversion, SuccessConversion, EmptyNotation };

/**
 * @brief PostfixToInfix conversion algorithm.
 */
struct PostfixToInfix {
    static QMap<QString, Operation> const defaultOperations;
    QMap<QString, Operation> customOperations;

    /**
     * @brief Converting string from Postfix Notation
     *        to Infix Notation if Postfix Notation is valid.
     * @param[in,out] postfixNotation modifiable string.
     * @return @c true if conversion was done, @c false otherwise.
     */
    ConversionResult convert(QString &postfixNotation) const;
};
}
#endif // NOTATIONCONVERTERCORE_H
