#ifndef NOTATIONCONVERTERCORE_H
#define NOTATIONCONVERTERCORE_H

#include <QObject>

class NotationConverterCore : QObject {
    Q_OBJECT

public:
    NotationConverterCore(QObject* parent);

    QString fromPostfixToInfix(QString string);
};

#endif // NOTATIONCONVERTERCORE_H
