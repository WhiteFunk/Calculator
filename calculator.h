#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE
class Button;

class Calculator : public QWidget
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);

private slots:

    void clear();
    void backspaceClicked();
    void equalClicked();

    void digitClicked();

    void pointClicked();
    void leftBracketClicked();
    void rightBracketClicked();

    void divisionButtonClicked();
    void timesButtonClicked();
    void minusClicked();
    void plusClicked();

private:
    Button *createButton(const QString &text, const char *member);

    void create_buttons();

    bool isDigit(char c);
    bool isOp(char c);
    int getPrecedence (char c);
    int operate (int val1, int val2, char op);
    int evaluate (std::string s);


    QLineEdit *display;

    enum { NumDigitButtons = 10 };
    Button *digitButtons[NumDigitButtons];
};


#endif
