#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chart1.h"
#include "QTableWidget"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void showEvent(QShowEvent *event);
    void resizeCharts(void);
private:
    View_Chart1* v_chart1;
    View_Chart1* v_chart2;
    void updateDataTable(unsigned int,unsigned int );

protected:
    void  resizeEvent(QResizeEvent *event) override;
private slots:

    void on_pushButton_2_clicked();

   // void on_pushButton_3_clicked();

    void on_pbTest_clicked();

    void on_twMain_currentChanged(int index);

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
