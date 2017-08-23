#include <QSet>
#include <QStack>

#include <algorithm>
#include <functional>

#include "notationconvertercore.h"

using namespace NotationConverter;
using Operator = PostfixToInfix::Operator;

QMap<QString, Operator> PostfixToInfix::m_defaultOperators;
QMap<QString, QString> PostfixToInfix::m_defaultOperatorsDescription;

PostfixToInfix::PostfixToInfix()
{
    static bool init = false;
    if (not init) {
        m_defaultOperators.insert("+", Operator{2, "%1 + %2"});
        m_defaultOperators.insert("-", Operator{2, "%1 - %2"});
        m_defaultOperators.insert("*", Operator{2, "( %1 ) * ( %2 )"});
        m_defaultOperators.insert("/", Operator{2, "( %1 ) / ( %2 )"});
        m_defaultOperators.insert("^", Operator{2, "( %1 ) ^ ( %2 )"});
        m_defaultOperators.insert("%", Operator{2, "( %1 ) % ( %2 )"});

        m_defaultOperatorsDescription.insert("+", "Addition");
        m_defaultOperatorsDescription.insert("-", "Subtraction");
        m_defaultOperatorsDescription.insert("*", "Multiplication");
        m_defaultOperatorsDescription.insert("/", "Division");
        m_defaultOperatorsDescription.insert("^", "Exponentiation");
        m_defaultOperatorsDescription.insert("%", "Integer division");

        init = true;
    }
}

QMap<QString, Operator> PostfixToInfix::defaultOperators()
{
    return m_defaultOperators;
}

QMap<QString, QString> PostfixToInfix::defaultOperatorsDescription()
{
    return m_defaultOperatorsDescription;
}

QMap<QString, Operator> PostfixToInfix::customOperators() const
{
    return m_customOperators;
}

bool PostfixToInfix::setCustomOperators(QMap<QString, Operator> const& customOperators)
{
    static QSet<QString> set_defaultOperators = QSet<QString>::fromList(m_defaultOperators.keys());
    QSet<QString> set_customOperators         = QSet<QString>::fromList(customOperators.keys());

    if (set_defaultOperators.intersects(set_customOperators)) {
        return false;
    }
    m_customOperators = customOperators;
    return true;
}

QMap<QString, QString> PostfixToInfix::customOperatorsDescription() const
{
    return m_customOperatorsDescription;
}

bool PostfixToInfix::setCustomOperatorsDescription(QMap<QString, QString> const& customOperatorsDescription)
{
    static QSet<QString> set_defaultOperatorDescriptions = QSet<QString>::fromList(m_defaultOperatorsDescription.keys());
    QSet<QString> set_customOperatorsDescription         = QSet<QString>::fromList(customOperatorsDescription.keys());

    if (set_defaultOperatorDescriptions.intersects(set_customOperatorsDescription)) {
        return false;
    }
    m_customOperatorsDescription = customOperatorsDescription;
    return true;
}

QString PostfixToInfix::operator()(QString postfixNotation)
{
    if (postfixNotation.isEmpty()) {
        return postfixNotation;
    }
    QStringList l_subString = postfixNotation.split(' ', QString::SkipEmptyParts);

    static std::function<QString(QString const&, QString const&)> insertInNotation = [](QString const& notation, QString const& operand) -> QString {
        return notation.arg(operand);
    };
    static std::function<QString(QString, QStringList const&)> fillNotation = [](QString notation, QStringList const& operands) -> QString {
        return std::accumulate(operands.begin(), operands.end(), notation, insertInNotation);
    };

    QStack<QString> stackIntermediate;
    static QSet<QString> set_defaultOperators = QSet<QString>::fromList(m_defaultOperators.keys());
    QSet<QString> set_allOperators            = QSet<QString>::fromList(m_customOperators.keys()).unite(set_defaultOperators);

    for (QString token : l_subString) {
        if (set_allOperators.contains(token)) {
            Operator operation;
            if (set_defaultOperators.contains(token)) {
                operation = m_defaultOperators.value(token);
            }
            else {
                operation = m_customOperators.value(token);
            }
            if (stackIntermediate.size() < operation.operands) {
                return postfixNotation;
            }

            QStringList operands;
            for (int i = 0; i < operation.operands; ++i) {
                operands.prepend(stackIntermediate.pop());
            }
            QString expression = fillNotation(operation.notation, operands);
            stackIntermediate.push(expression);
        }
        else {
            stackIntermediate.push(token);
        }
    }

    if (stackIntermediate.size() != 1) {
        return postfixNotation;
    }
    return stackIntermediate.pop();
}
