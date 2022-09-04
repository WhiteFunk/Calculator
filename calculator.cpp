#include "calculator.h"
#include "button.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QtMath>

#include <QDebug>

#include <stack>



Calculator::Calculator(QWidget *parent)
    : QWidget(parent)
{
    display = new QLineEdit("0");


    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);

    QFont font = display->font();
    font.setPointSize(font.pointSize() + 8);
    display->setFont(font);


    create_buttons();

    setWindowTitle(tr("Calculator"));
}


void Calculator::create_buttons()
{

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addWidget(display, 0, 0, 1, 6);

    //Create buttons

    for (int i = 0; i < NumDigitButtons; ++i)
        digitButtons[i] = createButton(QString::number(i), SLOT(digitClicked()));

    Button *pointButton = createButton(tr("."), SLOT(pointClicked()));
    Button *leftBracket = createButton(tr("("), SLOT(leftBracketClicked()));
    Button *rightBracket = createButton(tr(")"), SLOT(rightBracketClicked()));

    Button *backspaceButton = createButton(tr("Backspace"), SLOT(backspaceClicked()));
    Button *clearButton = createButton(tr("Clear"), SLOT(clear()));
    Button *equalButton = createButton(tr("="), SLOT(equalClicked()));

    Button *sinButton = createButton(tr("sin"), SLOT(trigFunctionClicked()));
    Button *cosButton = createButton(tr("cos"), SLOT(trigFunctionClicked()));
    Button *tnButton = createButton(tr("tn"), SLOT(trigFunctionClicked()));
    Button *ctgButton = createButton(tr("ctg"), SLOT(trigFunctionClicked()));

    Button *divisionButton = createButton(tr("\303\267"), SLOT(divisionButtonClicked()));
    Button *timesButton = createButton(tr("\303\227"), SLOT(timesButtonClicked()));
    Button *minusButton = createButton(tr("-"), SLOT(minusClicked()));
    Button *plusButton = createButton(tr("+"), SLOT(plusClicked()));

    Button *squareRootButton = createButton(tr("√"), SLOT(unaryOperatorClicked()));
    Button *powerButton = createButton(tr("x\302\262"), SLOT(unaryOperatorClicked()));
    Button *factorialButton = createButton(tr("x!"), SLOT(unaryOperatorClicked()));

    //Add buttons to layout

    mainLayout->addWidget(backspaceButton, 1, 0, 1, 2);
    mainLayout->addWidget(clearButton, 1, 2, 1, 2);
    mainLayout->addWidget(equalButton, 1, 4, 1, 2);

    mainLayout->addWidget(sinButton, 2, 0);
    mainLayout->addWidget(cosButton, 3, 0);
    mainLayout->addWidget(tnButton, 4, 0);
    mainLayout->addWidget(ctgButton, 5, 0);

    for (int i = 1; i < NumDigitButtons; ++i) {
        int row = ((9 - i) / 3) + 2;
        int column = ((i - 1) % 3) + 1;
        mainLayout->addWidget(digitButtons[i], row, column);
    }

    mainLayout->addWidget(digitButtons[0], 5, 1);
    mainLayout->addWidget(pointButton, 5, 2);
    mainLayout->addWidget(leftBracket, 5, 3);

    mainLayout->addWidget(squareRootButton, 2, 4);
    mainLayout->addWidget(powerButton, 3, 4);
    mainLayout->addWidget(factorialButton, 4, 4);
    mainLayout->addWidget(rightBracket, 5, 4);

    mainLayout->addWidget(divisionButton, 2, 5);
    mainLayout->addWidget(timesButton, 3, 5);
    mainLayout->addWidget(minusButton, 4, 5);
    mainLayout->addWidget(plusButton, 5, 5);

    setLayout(mainLayout);

    setWidgetStyle();
}

template <typename T>
T factorial(T n){
    return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}


void Calculator::unaryOperatorClicked(){

        Button *clickedButton = qobject_cast<Button *>(sender());
        QString clickedOperator = clickedButton->text();
        double operand = (QString::number(evaluate((display->text()).toStdString()))).toDouble();
        double result = 0.0;

        display->setText(QString::number(evaluate((display->text()).toStdString())));

        if (clickedOperator == tr("√"))
        {
            if (operand < 0.0) {
                return;
            }
            result = std::sqrt(operand);
        }
        if (clickedOperator == tr("x\302\262"))
        {
            result = std::pow(operand, 2.0);
        }
        if (clickedOperator == tr("x!"))
        {
            if (operand == 0.0) {
                return;
            }
            result = factorial(operand);
        }
        display->setText(QString::number(result));
}

void Calculator::trigFunctionClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    QString clickedOperator = clickedButton->text();
    double operand = (QString::number(evaluate((display->text()).toStdString()))).toDouble();
    double result = 0.0;

    double pi = 3.1415926535;

    display->setText(QString::number(evaluate((display->text()).toStdString())));

    if (clickedOperator == tr("sin"))
    {
        result = std::sin(operand* (pi/180));
    }

    if (clickedOperator == tr("cos"))
    {
        result = std::cos(operand* (pi/180));
    }

    if (clickedOperator == tr("tn"))
    {
        result = std::tan(operand* (pi/180));
    }

    if (clickedOperator == tr("ctg"))
    {
        result = 1/std::tan(operand* (pi/180));
    }

    display->setText(QString::number(result));
}


bool Calculator::isDigit(char c)
{
    return  (c >= '0' && c <= '9');
}

bool Calculator::isOp(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')');
}

int Calculator::getPrecedence(char c)
{
    switch (c)
        {
            case '+':
            case '-': return 1;
            case '*':
            case '/': return 2;
            case '(':
            case ')': return 3;
            default: return -1;
    }
}

int Calculator::operate(int val1, int val2, char op)
{
    if (op == '+') return val1 + val2;
        if (op == '-') return val1 - val2;
        if (op == '*') return val1 * val2;
        return val1 / val2;
}

int Calculator::evaluate(std::string s)
{
    std::stack<int> vals;
        std::stack<char> ops;

        int val = 0;
        int pos = 0;

        while (pos < s.length())
        {
            char spot = s[pos];
            if (isDigit(spot))
            {
                val = (val * 10) + (int)(spot - '0');
            }
            else if (isOp(spot))
            {
                if (spot == '(')
                {
                    ops.push (spot);
                    val = 0;
                }

                else if (vals.empty())
                {

                    vals.push(val);
                    ops.push(spot);
                    val = 0;
                }
                else if (spot == ')')
                {

                    vals.push(val);
                    while (ops.top() != '(')
                    {
                        spot = ops.top();
                                ops.pop();
                        val = vals.top();
                                vals.pop();
                        int prev = vals.top();
                                vals.pop();
                        val = operate(prev, val, spot);
                        vals.push(val);
                    }
                    ops.pop();
                    vals.pop();

                }
                else
                {
                    char prev = ops.top();
                    if (getPrecedence(spot) > getPrecedence(prev))
                    {

                        vals.push(val);
                        ops.push(spot);
                        val = 0;
                    }
                    else
                    {

                        int prevval = vals.top();
                                vals.pop();
                        int prevop = ops.top();
                                ops.pop();
                        prevval = operate(prevval, val, prevop);
                        vals.push(prevval);
                        ops.push(spot);
                        val = 0;
                    }
                }
            }
            pos ++;
        }

        while (!ops.empty())
        {

            int prev = vals.top();
                    vals.pop();
            char spot = ops.top();
                    ops.pop();
            val = operate(prev, val, spot);

        }
        return val;
}




void Calculator::digitClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    int digitValue = clickedButton->text().toInt();

    if(display->text()=='0') display->clear();

    display->setText(display->text() + QString::number(digitValue));
}

void Calculator::pointClicked()
{
    display->setText(display->text()+'.');
}

void Calculator::leftBracketClicked()
{
    if(display->text()=='0') display->clear();
     display->setText(display->text()+'(');
}

void Calculator::rightBracketClicked()
{
    display->setText(display->text()+')');
}

void Calculator::divisionButtonClicked()
{
    display->setText(display->text()+"÷");
}

void Calculator::timesButtonClicked()
{
    display->setText(display->text()+'*');
}

void Calculator::minusClicked()
{
    display->setText(display->text()+'-');
}

void Calculator::plusClicked()
{
    display->setText(display->text()+'+');
}


void Calculator::clear()
{
    display->setText("0");
}


void Calculator::backspaceClicked()
{
   QString qs = display->text();
   qs.erase(qs.end()-1,qs.end());
   display->setText(qs);
}

void Calculator::equalClicked()
{
    display->setText(QString::number(evaluate((display->text()).toStdString())));
}


Button *Calculator::createButton(const QString &text, const char *member)
{
    Button *button = new Button(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}



void Calculator::setWidgetStyle()
{
    this->setStyleSheet(
    "QWidget{"
        "background-color:#8c8c8c;"
    "}"
    "QLineEdit{"
        "border:1px solid #aaa;"
        "background:#d9d9d9;"
        "border-radius:3px;"
        "color:#1a1a1a;"
    "}"
    "QLineEdit:hover{"
        "border:1px solid #0078d7;"
    "}"
    "QToolButton{"
        "background-color: qlineargradient(spread:pad, x1:0.585, y1:1, x2:0.506, y2:0, stop:0 rgba(150, 150, 150, 255), stop:1 rgba(202, 202, 202, 255));"
        "border-radius:7px;"
        "border:1px solid #aaa;"
        "color:#1a1a1a;"
    "}"
    "QToolButton:hover{"
        "background-color: qlineargradient(spread:pad, x1:0.585, y1:1, x2:0.506, y2:0, stop:0 rgba(200, 200, 200, 200), stop:1 rgba(232, 232, 232, 255));"
        "border-radius:7px;"
        "border:1px solid #0078d7;"
        "color:#111;"
    "}");
}



