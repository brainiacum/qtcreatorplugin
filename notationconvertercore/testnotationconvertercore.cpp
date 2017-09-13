/*
* Copyright (c) 2017 Vitali Parkhomenko
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include "notationconvertercore.h"

#include <QtTest/QtTest>

using namespace NotationConverter::Internal;

Q_DECLARE_METATYPE(ConversionResult)

class TestPostfixToInfix: public QObject
{
    Q_OBJECT
private slots:
    void checkBaseOperations();
    void checkBaseOperations_data();

    void checkExtendedOperations();
    void checkExtendedOperations_data();

    void checkEmptyNotationConversion();
    void checkEmptyNotationConversion_data();

    void checkInvalidNotationConversion();
    void checkInvalidNotationConversion_data();
};

void TestPostfixToInfix::checkBaseOperations()
{
    PostfixToInfix postfix2Infix;

    QFETCH(QString, bo);
    QFETCH(ConversionResult, bo_result);
    QCOMPARE(postfix2Infix.convert(bo), bo_result);
}

void TestPostfixToInfix::checkBaseOperations_data()
{
    QTest::addColumn<QString>("bo");
    QTest::addColumn<ConversionResult>("bo_result");

    QTest::newRow("Addition_1") << "i j +" << SuccessConversion;
    QTest::newRow("Addition_2") << "i j k l + + +" << SuccessConversion;
    QTest::newRow("Addition_3") << "i j + k + l +" << SuccessConversion;
    QTest::newRow("Addition_4") << "i j k + + l +" << SuccessConversion;
    QTest::newRow("Addition_5") << "i j + k l + +" << SuccessConversion;

    QTest::newRow("Subtraction_1") << "i j -" << SuccessConversion;
    QTest::newRow("Subtraction_2") << "i j k l - - -" << SuccessConversion;
    QTest::newRow("Subtraction_3") << "i j - k - l -" << SuccessConversion;
    QTest::newRow("Subtraction_4") << "i j k - - l -" << SuccessConversion;
    QTest::newRow("Subtraction_5") << "i j - k l - -" << SuccessConversion;

    QTest::newRow("Multiplication_1") << "i j *" << SuccessConversion;
    QTest::newRow("Multiplication_2") << "i j k l * * *" << SuccessConversion;
    QTest::newRow("Multiplication_3") << "i j * k * l *" << SuccessConversion;
    QTest::newRow("Multiplication_4") << "i j k * * l *" << SuccessConversion;
    QTest::newRow("Multiplication_5") << "i j * k l * *" << SuccessConversion;

    QTest::newRow("Division_1") << "i j /" << SuccessConversion;
    QTest::newRow("Division_2") << "i j k l / / /" << SuccessConversion;
    QTest::newRow("Division_3") << "i j / k / l /" << SuccessConversion;
    QTest::newRow("Division_4") << "i j k / / l /" << SuccessConversion;
    QTest::newRow("Division_5") << "i j / k l / /" << SuccessConversion;

    QTest::newRow("Exponentiation_1") << "i j ^" << SuccessConversion;
    QTest::newRow("Exponentiation_2") << "i j k l ^ ^ ^" << SuccessConversion;
    QTest::newRow("Exponentiation_3") << "i j ^ k ^ l ^" << SuccessConversion;
    QTest::newRow("Exponentiation_4") << "i j k ^ ^ l ^" << SuccessConversion;
    QTest::newRow("Exponentiation_5") << "i j ^ k l ^ ^" << SuccessConversion;

    QTest::newRow("Integer_division_1") << "i j %" << SuccessConversion;
    QTest::newRow("Integer_division_2") << "i j k l % % %" << SuccessConversion;
    QTest::newRow("Integer_division_3") << "i j % k % l %" << SuccessConversion;
    QTest::newRow("Integer_division_4") << "i j k % % l %" << SuccessConversion;
    QTest::newRow("Integer_division_5") << "i j % k l % %" << SuccessConversion;

    QTest::newRow("Mixed_base_operations_1") << "a b + c 2 * + " << SuccessConversion;
    QTest::newRow("Mixed_base_operations_2") << "A B + C D + *" << SuccessConversion;
    QTest::newRow("Mixed_base_operations_3") << "A B + C + D +" << SuccessConversion;
    QTest::newRow("Mixed_base_operations_4") << "A B * C D * +" << SuccessConversion;
    QTest::newRow("Mixed_base_operations_5") << "A B C + D + +" << SuccessConversion;
    QTest::newRow("Mixed_base_operations_6") << "A B C * + D +" << SuccessConversion;
    QTest::newRow("Mixed_base_operations_7") << "2 2 + 6 * 3 / 8 -" << SuccessConversion;
    QTest::newRow("Mixed_base_operations_8") << "a b c + - f c * / b v + *" << SuccessConversion;
    QTest::newRow("Mixed_base_operations_9") << "a b c + - f / c * b v + *" << SuccessConversion;
    QTest::newRow("Mixed_base_operations_10") << "a b c - + d e - f g - h + / *" << SuccessConversion;
    QTest::newRow("Mixed_base_operations_11") << "a b + c + 2 * " << SuccessConversion;
    QTest::newRow("Mixed_base_operations_12") << "15 7 1 1 + - / 3 * 2 1 1 + + -" << SuccessConversion;
    QTest::newRow("Mixed_base_operations_13") << "a b + 2 ^ c 2 ^ *" << SuccessConversion;
    QTest::newRow("Mixed_base_operations_14") << "a 2 ^ b + c * " << SuccessConversion;
    QTest::newRow("Mixed_base_operations_15") << "a b + 2 ^ c * " << SuccessConversion;
}

void TestPostfixToInfix::checkExtendedOperations()
{
    PostfixToInfix postfix2Infix;
    postfix2Infix.customOperations = {
        {"!", Operation{1, "%1 !", false, "Factorial"}},
        {"ln", Operation{1, "ln ( %1 )", false, "Natural logarithm"}},
        {"log", Operation{2, "log %1 ( %2 )", false, "Logarithm"}},
        {"range-based-integral", Operation{3, "integral | %1 , %2 ( %3 )", true, "Range-based integral"}}
    };

    QFETCH(QString, eo);
    QFETCH(ConversionResult, eo_result);
    QCOMPARE(postfix2Infix.convert(eo), eo_result);
}

void TestPostfixToInfix::checkExtendedOperations_data()
{
    QTest::addColumn<QString>("eo");
    QTest::addColumn<ConversionResult>("eo_result");

    QTest::newRow("Factorial") << "i !" << SuccessConversion;
    QTest::newRow("Natural logarithm") << "i ln" << SuccessConversion;
    QTest::newRow("Logarithm") << "i j log" << SuccessConversion;
    QTest::newRow("Range-based_integral") << "i j k range-based-integral" << SuccessConversion;

    QTest::newRow("Mixed_extended_operations_1") << "i ! ln" << SuccessConversion;
    QTest::newRow("Mixed_extended_operations_2") << "i ln j log" << SuccessConversion;
    QTest::newRow("Mixed_extended_operations_3") << "i ! j log" << SuccessConversion;
    QTest::newRow("Mixed_extended_operations_4") << "i ln !" << SuccessConversion;
    QTest::newRow("Mixed_extended_operations_5") << "i j log ln" << SuccessConversion;
    QTest::newRow("Mixed_extended_operations_6") << "i j log k log" << SuccessConversion;
    QTest::newRow("Mixed_extended_operations_7") << "i j log k log !" << SuccessConversion;
    QTest::newRow("Mixed_extended_operations_8") << "i j log k ! log " << SuccessConversion;
    QTest::newRow("Mixed_extended_operations_9") << "i j log k ! log !" << SuccessConversion;
    QTest::newRow("Mixed_extended_operations_10") << "i j k range-based-integral" << SuccessConversion;
    QTest::newRow("Mixed_extended_operations_11") << "i j k ln l log range-based-integral" << SuccessConversion;
    QTest::newRow("Mixed_extended_operations_12") << "i ! j ! k range-based-integral" << SuccessConversion;
    QTest::newRow("Mixed_extended_operations_13") << "i ln j ! k l log range-based-integral" << SuccessConversion;

    QTest::newRow("Mixed_base_and_extended_operations_1") << "i j ^ ln" << SuccessConversion;
    QTest::newRow("Mixed_base_and_extended_operations_2") << "i ! j ^" << SuccessConversion;
    QTest::newRow("Mixed_base_and_extended_operations_3") << "i j + ln" << SuccessConversion;
    QTest::newRow("Mixed_base_and_extended_operations_4") << "i j * k log" << SuccessConversion;
    QTest::newRow("Mixed_base_and_extended_operations_5") << "i j log k * k l + /" << SuccessConversion;
    QTest::newRow("Mixed_base_and_extended_operations_6") << "i ! j ! + k ln - l / 1 2 / ^" << SuccessConversion;
    QTest::newRow("Mixed_base_and_extended_operations_7") << "i j % k l % a b + c / d e / * n log range-based-integral" << SuccessConversion;
}

void TestPostfixToInfix::checkEmptyNotationConversion()
{
    PostfixToInfix postfix2Infix;

    QFETCH(QString, enc);
    QFETCH(ConversionResult, enc_result);
    QCOMPARE(postfix2Infix.convert(enc), enc_result);
}

void TestPostfixToInfix::checkEmptyNotationConversion_data()
{
    QTest::addColumn<QString>("enc");
    QTest::addColumn<ConversionResult>("enc_result");

    QTest::newRow("Empty_notation_1") << "" << EmptyNotation;
    QTest::newRow("Empty_notation_4") << QString() << EmptyNotation;
}

void TestPostfixToInfix::checkInvalidNotationConversion()
{
    PostfixToInfix postfix2Infix;

    QFETCH(QString, inc);
    QFETCH(ConversionResult, inc_result);
    QCOMPARE(postfix2Infix.convert(inc), inc_result);
}

void TestPostfixToInfix::checkInvalidNotationConversion_data()
{
    QTest::addColumn<QString>("inc");
    QTest::addColumn<ConversionResult>("inc_result");

    QTest::newRow("Failure_test_1") << " " << FailureConversion;
    QTest::newRow("Failure_test_2") << "    " << FailureConversion;
    QTest::newRow("Failure_test_3") << "a + b + c + - *" << FailureConversion;
    QTest::newRow("Failure_test_4") << "sin A B C D + + +" << FailureConversion;
    QTest::newRow("Failure_test_5") << "trash A B C D + + + trash " << FailureConversion;
    QTest::newRow("Failure_test_6") << " trash A B C D + + + " << FailureConversion;
    QTest::newRow("Failure_test_7") << "A B C D + + + trash" << FailureConversion;
}

QTEST_MAIN(TestPostfixToInfix)
#include "testnotationconvertercore.moc"
