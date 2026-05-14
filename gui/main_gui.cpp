#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QMessageBox>
#include <QComboBox>
#include <sstream>
#include <string>

#include "mainer.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Calculator Polsha;
    SortedMap<std::string, Polinom> peremSortedMap;
    BlackRedTree<std::string, Polinom> peremBlackRedTree;
    VectorHash<std::string, Polinom> peremHash;
    UnsortedMap<std::string, Polinom> peremUnsortedMap;
    AVLTree<std::string, Polinom> peremAVLTree;
    HashMap<std::string, Polinom> peremHashMap;
    int whatToUse = 1; //1 - SortedMap, 2 - BlackRedTree, 3 - Hash, 4 - UnsortedMap, 5 - AVLTree, 6 - HashMap

    //Создаем окно
    QWidget window;
    window.setWindowTitle("Калькулятор полиномов");
    window.setFixedSize(700, 500);

    //Основной layout
    QVBoxLayout* mainLayout = new QVBoxLayout(&window);

    //Заголовок
    QLabel* titleLabel = new QLabel("<h2>Калькулятор полиномов</h2>");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    //Выбор структуры хранения
    QHBoxLayout* structLayout = new QHBoxLayout();
    QLabel* structLabel = new QLabel("Структура хранения переменных:");
    QComboBox* structCombo = new QComboBox();
    structCombo->addItem("SortedMap");
    structCombo->addItem("BlackRedTree");
    structCombo->addItem("VectorHash");
    structCombo->addItem("UnsortedMap");
    structCombo->addItem("AVLTree");
    structCombo->addItem("HashMap");
    structLayout->addWidget(structLabel);
    structLayout->addWidget(structCombo);
    structLayout->addStretch();
    mainLayout->addLayout(structLayout);

    //Поле ввода
    QLabel* inputLabel = new QLabel("Введите выражение:");
    mainLayout->addWidget(inputLabel);

    QLineEdit* inputEdit = new QLineEdit();
    inputEdit->setPlaceholderText("Пример: 2xxxyz + 15x^2yz^12 + 2*x*15*13*y*z*y");
    inputEdit->setMinimumHeight(35);
    mainLayout->addWidget(inputEdit);

    //Кнопка вычисления
    QPushButton* calcButton = new QPushButton("Вычислить");
    calcButton->setMinimumHeight(40);
    calcButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; font-size: 14px; }");
    mainLayout->addWidget(calcButton);

    //Поле результата
    QLabel* resultLabel = new QLabel("Результат (как в консоли):");
    mainLayout->addWidget(resultLabel);

    QTextEdit* resultEdit = new QTextEdit();
    resultEdit->setReadOnly(true);
    resultEdit->setMinimumHeight(250);
    resultEdit->setStyleSheet("QTextEdit { background-color: #f5f5f5; font-family: monospace; font-size: 12px; }");
    mainLayout->addWidget(resultEdit);

    //Кнопка очистки
    QPushButton* clearButton = new QPushButton("Очистить историю");
    clearButton->setMinimumHeight(30);
    mainLayout->addWidget(clearButton);

    //Обработчик изменения структуры
    QObject::connect(structCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [&](int index)
        {
            whatToUse = index + 1;  // 0->1, 1->2, 2->3, ...
            resultEdit->append(QString("--- Переключено на %1 ---").arg(structCombo->currentText()));
        });

    //Обработчик вычисления
    QObject::connect(calcButton, &QPushButton::clicked, [&]()
    {
        QString expr = inputEdit->text();
        if (expr.isEmpty())
        {
            QMessageBox::warning(&window, "Ошибка", "Введите выражение!");
            return;
        }

        try
        {
            Polinom result;
            switch (whatToUse)
            {
            case 1:
                result = Polsha.ArithmeticCalculator(expr.toStdString(), peremSortedMap);
                break;
            case 2:
                result = Polsha.ArithmeticCalculator(expr.toStdString(), peremBlackRedTree);
                break;
            case 3:
                result = Polsha.ArithmeticCalculator(expr.toStdString(), peremHash);
                break;
            case 4:
                result = Polsha.ArithmeticCalculator(expr.toStdString(), peremUnsortedMap);
                break;
            case 5:
                result = Polsha.ArithmeticCalculator(expr.toStdString(), peremAVLTree);
                break;
            case 6:
                result = Polsha.ArithmeticCalculator(expr.toStdString(), peremHashMap);
                break;
            default:
                break;
            }

            std::stringstream ss;
            ss << result;
            std::string resultStr = ss.str();

            resultEdit->append(QString::fromStdString("> " + expr.toStdString()));
            resultEdit->append(QString::fromStdString("Result: " + resultStr));
            resultEdit->append("");
        }
        catch (const std::exception& excep)
        {
            resultEdit->append(QString("Error: %1").arg(excep.what()));
        }
    });

    //Обработчик очистки
    QObject::connect(clearButton, &QPushButton::clicked, [&]()
    {
        resultEdit->clear();
        inputEdit->setFocus();
    });

    //Вычисление по Enter
    QObject::connect(inputEdit, &QLineEdit::returnPressed, [&]()
    {
        calcButton->click();
    });

    window.show();
    return app.exec();
}