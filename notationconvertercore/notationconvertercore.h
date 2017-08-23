#ifndef NOTATIONCONVERTERCORE_H
#define NOTATIONCONVERTERCORE_H

#include <QMap>
#include <QString>

namespace NotationConverter {

class PostfixToInfix {
public:
    struct Operator {
        int operands;
        QString notation;
    };

private:
    static QMap<QString, Operator> m_defaultOperators;
    static QMap<QString, QString> m_defaultOperatorsDescription;
    QMap<QString, Operator> m_customOperators;
    QMap<QString, QString> m_customOperatorsDescription;

public:
    PostfixToInfix();
    ~PostfixToInfix()                     = default;
    PostfixToInfix(PostfixToInfix const&) = default;
    PostfixToInfix& operator=(PostfixToInfix const&) = default;

    QMap<QString, Operator> defaultOperators();
    QMap<QString, QString> defaultOperatorsDescription();

    QMap<QString, Operator> customOperators() const;
    bool setCustomOperators(QMap<QString, Operator> const& customOperators);

    QMap<QString, QString> customOperatorsDescription() const;
    bool setCustomOperatorsDescription(QMap<QString, QString> const& customOperatorsDescription);

    /**
     +* @brief Returns converted string from Postfix Notation to Infix Notation if Postfix Notation is valid
     +* @param[in] string in Postfix Notation
     +* @return @c string in Infix Notation, @c string in Postfix Notation otherwise
     +*/
    QString operator()(QString postfixNotation);
};
}
#endif // NOTATIONCONVERTERCORE_H
