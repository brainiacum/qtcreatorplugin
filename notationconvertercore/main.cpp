#include <QCoreApplication>
#include <QVector>

#include <functional>
#include <iostream>

#include "notationconvertercore.h"

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    QVector<QString> vector;
    vector.append("a b c + - f c * / b v + *");
    vector.append("a b c + - f / c * b v + *");
    vector.append("a b c - + d e - f g - h + / *");
    vector.append("a b + c + 2 * ");
    vector.append("a b + c 2 * + ");
    vector.append("a 2 ^ b + c * ");
    vector.append("a b + 2 ^ c * ");
    vector.append("a b + 2 ^ c 2 ^ *");
    vector.append("2 2 + 6 * 3 / 8 -");
    vector.append("A B C * + D +");
    vector.append("A B + C D + *");
    vector.append("A B * C D * +");
    vector.append("A B + C + D +");
    vector.append("A B C + D + +");
    vector.append("15 7 1 1 + - / 3 * 2 1 1 + + -");
    vector.append("Ai Bj +");
    vector.append("A B + C D + E ! sin / +");
    vector.append("A B % E ! sin +");
    vector.append("sin A B C D + + +");          // invalid postfix notation
    vector.append("trash A B C D + + + trash "); // invalid postfix notation
    vector.append(" trash A B C D + + + ");      // invalid postfix notation
    vector.append("A B C D + + + trash");        // invalid postfix notation

    // out vector
    std::function<void(QString const&)> outvector = [](QString const& str) {
        std::cout << ":># " + str.toStdString() << std::endl;
    };

    std::cout << " In strings: " << std::endl;
    std::for_each(vector.begin(), vector.end(), outvector);

    using NotationConverter::PostfixToInfix;
    using Operator = NotationConverter::PostfixToInfix::Operator;

    static PostfixToInfix convertPostfixToInfix;
    static QMap<QString, Operator> customOperators;
    static QMap<QString, QString> customOperatorsDescription;

    customOperators.insert("!", Operator{1, "( %1 ) !"});
    customOperators.insert("ln", Operator{1, "ln ( %1 )"});
    customOperators.insert("log", Operator{2, "log %1 ( %2 )"});
    customOperators.insert("sin", Operator{1, "sin ( %1 )"});
    customOperators.insert("cos", Operator{1, "cos ( %1 )"});
    customOperators.insert("tan", Operator{1, "tan ( %1 )"});
    customOperators.insert("atan", Operator{1, "atan ( %1 )"});
    customOperators.insert("atan2", Operator{2, "cotan ( %1 , %2 )"});

    customOperatorsDescription.insert("!", "Factorial");
    customOperatorsDescription.insert("ln", "Natural logarithm");
    customOperatorsDescription.insert("log", "Logarithm");
    customOperatorsDescription.insert("sin", "Sine");
    customOperatorsDescription.insert("cos", "Cosine");
    customOperatorsDescription.insert("tan", "Tangent");
    customOperatorsDescription.insert("atan", "Arc tangent");
    customOperatorsDescription.insert("atan2", "Arc tangent, using signs to determine quadrants");

    std::cout << std::endl;
    std::cout << " Start converting Postfix Notation to Infix Notation: " << std::endl;
    std::cout << std::endl;

    convertPostfixToInfix.setCustomOperators(customOperators);
    convertPostfixToInfix.setCustomOperatorsDescription(customOperatorsDescription);

    //    std::transform(vector.begin(), vector.end(), vector.begin(), convertPostfixToInfix);
    //    std::for_each(vector.begin(), vector.end(), outvector);
    for (QString string : vector) {
        QString convertedString = convertPostfixToInfix(string);
        if (string != convertedString) {
            std::cout << ":># " << convertedString.toStdString() << std::endl;
        }
        else {
            std::cout << ":># " << string.toStdString() << "|  Invalid postfix notation!" << std::endl;
        }
    }

    return a.exec();
}
