#include "main_task.h"
#include "ui_main_task.h"

main_task::main_task(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::main_task)
{
    ui->setupUi(this);
    QPixmap pix(":/img/task.png");
    int w= ui->image->width();
    int h= ui->image->height();
    ui->image->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
}

main_task::~main_task()
{
    delete ui;
}
