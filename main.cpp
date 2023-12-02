#include "mainwindow.h"
#include <QApplication>
//#include "main_task.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Задание 11 Вариант 5 Соболь Л.А");
    w.show();

    return a.exec();
}
