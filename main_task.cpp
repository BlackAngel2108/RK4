#include "main_task.h"
#include "ui_main_task.h"

main_task::main_task(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::main_task)
{
    ui->setupUi(this);
}

main_task::~main_task()
{
    delete ui;
}
