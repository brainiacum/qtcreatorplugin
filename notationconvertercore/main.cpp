#include "notationconvertercore.h"
#include <QCoreApplication>
#include <QVector>

#include <functional>
#include <iostream>

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
    vector.append("trash A B C D + + + trash ");
    vector.append(" trash A B C D + + + ");
    vector.append("A B C D + + + trash");

    // out vector
    std::function<void(QString const&)> outvector = [](QString const& str) {
        std::cout << ":># " + str.toStdString() << std::endl;
    };

    std::cout << " In strings: " << std::endl;
    std::for_each(vector.begin(), vector.end(), outvector);

    static NotationConverter::PostfixToInfix convertPostfixToInfix;

    std::cout << std::endl;
    std::cout << " Start converting Postfix Notation to Infix Notation: " << std::endl;
    std::cout << std::endl;

    std::transform(vector.begin(), vector.end(), vector.begin(), convertPostfixToInfix);
    std::for_each(vector.begin(), vector.end(), outvector);

    return a.exec();
}
