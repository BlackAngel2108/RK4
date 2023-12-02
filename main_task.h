#ifndef MAIN_TASK_H
#define MAIN_TASK_H

#include <QDialog>

namespace Ui {
class main_task;
}

class main_task : public QDialog
{
    Q_OBJECT

public:
    explicit main_task(QWidget *parent = nullptr);
    ~main_task();

private:
    Ui::main_task *ui;
};

#endif // MAIN_TASK_H
