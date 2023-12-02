#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chart1.h"
#include "QTableWidget"
#include "main_calc_th.h"
#include <QTime>

#define N_DEF (9) // Надо заменить на 11 когда будет финальное решение

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
    void timerEvent(QTimerEvent *event) override;
    int timerCalcID;
    QString lastPbName;
    int calc100ms;
private:
    View_Chart1* v_chart1;
    View_Chart1* v_chart2;

    MainCalcTh mainCalcTh;

    InputData inData;
    InputFunc inFunc;

    void updateDataTable(unsigned int,unsigned int );
    void fillResultsProcess(void);

    void setControlState(void);

protected:
    void  resizeEvent(QResizeEvent *event) override;
private slots:

    void on_pbCalculate_clicked();

    void on_pbResults_clicked();

    void on_twMain_currentChanged(int index);

    void on_pbClearCharts_clicked();

private:
    Ui::MainWindow *ui;
signals:
    void evClearAll(void);
};

#endif // MAINWINDOW_H
