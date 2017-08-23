#ifndef NOTATIONCONVERTERCORE_H
#define NOTATIONCONVERTERCORE_H

#include <QObject>

namespace NotationConverter {
class PostfixToInfix {
public:
    /**
     +* @brief Returns converted string from Postfix Notation to Infix Notation if Postfix Notation is valid
     +* @param[in] string in Postfix Notation
     +* @return @c string in Infix Notation, @c string in Postfix Notation otherwise
     +*/
    QString operator()(QString postfixNotation);
};
}
#endif // NOTATIONCONVERTERCORE_H
