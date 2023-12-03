#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "main_task.h"
#include <QPair>
#include <QtMath>
#include <QFile>
#include <QTextStream>
#include "chart1.h"
#include <QResizeEvent>
#include <QDebug>
#include <QMessageBox>
#include <QTime>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Construct Main window
    ui->setupUi(this);
    // Insert QGraphicsView
    chartU = new View_Chart1(ui->tabChart);
    chartU->axisX->setTitleText("Axis X");
    chartU->axisY->setTitleText("Axis U");
    //v_chart1->setAxisNames("X", "U");
    chartPhaze = new View_Chart1(ui->tab_chart2);
    //v_chart2->setAxisNames("U", "U'");
    chartPhaze->axisX->setTitleText("Axis U");
    chartPhaze->axisY->setTitleText("Axis U '");




    // Insert QTableWidget
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setColumnCount(11);
    QStringList labels;
    labels << tr("i") << tr("Xi")<< tr("hi") << tr("Vi")<< tr("V^i") << tr("Vi-V^i")<< tr("ОЛП") << tr("Деления")<< tr("Удвоения") << tr("Ui")<< tr("Глоб. П.");
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    //ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    for (int i=0;i<11;i++){
         ui->tableWidget->horizontalHeader()->resizeSection(i, 80);
    }


    // Insert QTableWidget 2
    ui->tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setColumnCount(11);
    ui->tableWidget_2->setHorizontalHeaderLabels(labels);
    //ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    for (int i=0;i<11;i++){
         ui->tableWidget_2->horizontalHeader()->resizeSection(i, 80);
    }
    //ui->tableWidget_2->horizontalHeader()->resizeSection(1, 180);

    connect(&mainCalcTh, &MainCalcTh::resultReady, this, &MainWindow::fillResultsProcess );
    connect(this, &MainWindow::evClearAll, this, &MainWindow::on_pbClearCharts_clicked, Qt::QueuedConnection);
    ui->twMain->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    QSize newSize = ui->tabChart->size();

    newSize.setHeight(newSize.height()-3); // I do not know why, this is 3 pix error
//    newSize.setWidth(newSize.width());
    chartU->resize(newSize);
    chartPhaze->resize(newSize);
    qDebug() << "MW Show() Size w: " << newSize.width() << "h: " << newSize.height();
    //7updateDataTable(int n);
}

void MainWindow::resizeCharts()
{
    QSize newSize = ui->tabChart->size();
    chartU->resize(newSize);
    chartPhaze->resize(newSize);
    qDebug() << "MW Resize() Size w: " << newSize.width() << "h: " << newSize.height();
}

void MainWindow::on_twMain_currentChanged(int index)
{
    Q_UNUSED(index);
    resizeCharts();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    resizeCharts();
}
/**
 * @brief answer Result container
 */
std::pair< std::pair<std::vector<double>, std::vector<std::vector<double>>>,
    std::pair<std::vector<std::pair<int,std::string>>,std::vector<std::pair<int,std::string>>>> answer;
std::pair< std::pair<std::vector<double>, std::vector<std::vector<double>>>,
    std::pair<std::vector<std::pair<int,std::string>>,std::vector<std::pair<int,std::string>>>> answer2;

/**
 * @brief MainWindow::updateDataTable Show Table data
 * @param num_of_lines1
 * @param num_of_lines2
 */
void MainWindow::updateDataTable(unsigned int num_of_lines1,unsigned int num_of_lines2)
{
    ui->tableWidget->setUpdatesEnabled(false);
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(11);
    QStringList labels;
    // Fill column names Table1
    labels << tr("i") << tr("Xi")<< tr("hi") << tr("Vi")<< tr("V^i") << tr("Vi-V^i")<< tr("ОЛП") << tr("Деления")<< tr("Удвоения") << tr("Ui")<< tr("Глоб. п.");
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    for (int i=0;i<11;i++){
         ui->tableWidget->horizontalHeader()->resizeSection(i, 80);
    }
    // Fill column names Table2
    ui->tableWidget_2->setUpdatesEnabled(false);
    ui->tableWidget_2->clear();
    ui->tableWidget_2->setColumnCount(11);
    ui->tableWidget_2->setHorizontalHeaderLabels(labels);
    //ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    for (int i=0;i<11;i++){
         ui->tableWidget_2->horizontalHeader()->resizeSection(i, 80);
    }
    // Fill data Table1
    ui->tableWidget->setRowCount(0);
    for (unsigned int i = 0; i < num_of_lines1; i++) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(row + 1);
        int N = N_DEF;
        //int temp=answer.second.first.size();
        QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(answer.second.first[i*N].first));
        ui->tableWidget->setItem(row, 0, item0);
        for(int col = 1; col < N; col++){
            QTableWidgetItem *item0 = new QTableWidgetItem(QString(answer.second.first[i*N+col].second.c_str()));
            ui->tableWidget->setItem(row, col, item0);
        }
    }
    // Fill data Table2
    ui->tableWidget_2->setRowCount(0);
    for (unsigned int i = 0; i < num_of_lines2; i++) {
        int row = ui->tableWidget_2->rowCount();
        ui->tableWidget_2->setRowCount(row + 1);
        int N = N_DEF;
        QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(answer.second.second[i*N].first));
        ui->tableWidget_2->setItem(row, 0, item0);
        for(int col = 1; col < N; col++){
            QTableWidgetItem *item0 = new QTableWidgetItem(QString(answer.second.second[i*N+col].second.c_str()));
            ui->tableWidget_2->setItem(row, col, item0);
        }
    }

    ui->tableWidget->setUpdatesEnabled(true);
    ui->tableWidget_2->setUpdatesEnabled(true);
}


int flag_for_chart=-1;

/**
 * @brief MainWindow::on_pbCalculate_clicked
 *  Main calculation process
 */
void MainWindow::on_pbCalculate_clicked()
{
    ui->pbCalculate->setEnabled(false);
    // Cобираю параметры с интерфейса
    inData.epsilon=ui->lineEdit->text().toDouble();
    //inData.precision= e_up and e_down
    double e_up=ui->lineEdit_2->text().toDouble();
    inData.precision=e_up;
    inData.Nmax=ui->lineEdit_3->text().toInt();
    inData.h0=ui->lineEdit_4->text().toDouble();
    inData.x0=ui->lineEdit_8->text().toDouble();
    inData.xT=ui->leX_end->text().toDouble();

    double u0=ui->lineEdit_7->text().toDouble();
    double u_0=ui->lineEdit_6->text().toDouble(); //производная в нуле
    inData.iv = { u0, u_0 };

    bool control_nothing=ui->radioButton_3->isChecked();
    bool control_up=ui->radioButton_2->isChecked();

    if (control_nothing){
        inData.dif_step=0;
    }
    else if(control_up) {
        inData.dif_step=1;
    }

    // Parameters for functions
    double k=ui->lineEdit_10->text().toDouble();
    double f=ui->lineEdit_11->text().toDouble();
    double m=ui->lineEdit_12->text().toDouble();
    inData.f=f;
    inData.k=k;
    inData.m=m;

    // Helper functions lambda functions
    inFunc.f1 = [](double x, std::vector<double> u) { (void)x; return u[1]; };
    //double F=(-1)*m*9.81*f*((u[1]>=0)?1:-1));
    inFunc.f2 = [f, k, m](double x, std::vector<double> u) { (void)x; return ((-m*9.81*f*((u[1]>=0)?1:-1))/m-k*u[0]/m);};//u(1) уточнить - уточнила
    inFunc.f3 = [](double x, std::vector<double> u) { (void)x; return u[1]; };
    double c=f;//доделать
    inFunc.f4 = [c, k, m](double x, std::vector<double> u) { (void)x; return (-k*u[0])/m+c*((u[1]>=0)?1:-1)*u[1]/m; };
    // Main calculation process
    if(mainCalcTh.isRunning()){
        QMessageBox msgBox;
        msgBox.setText("Attention");
        msgBox.setInformativeText("Calculation process already is running");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }
    // Run calculation into own thread
    mainCalcTh.setInputData(&inData, &inFunc);
    mainCalcTh.start();
    // Timer and controls
    calc100ms = 0;
    elapsedCalcTime.start();
    timerCalcID = this->startTimer(100);
    setControlState();
}

void MainWindow::fillResultsProcess()
{
    int N = N_DEF;

    //emit evClearAll();

    answer = mainCalcTh.get_answer();
    answer2 = mainCalcTh.get_answer2();
    int num_of_lines1=answer.second.first.size()/N; //9 - 11
    int num_of_lines2=answer.second.second.size()/N;//9 - 11
    // Show results U(x)
    QString ITERS_1= QString::number(num_of_lines1-1);//кол-во итераций
    QString b_xn_1 = QString::number(inData.xT - std::stod(answer.second.first[(num_of_lines1-1)*N+1].second));//b-xn
    QString MAX_H_x_1;          //x
    QString MIN_H_x_1;        //x
    double max1_olp=0;
    double max_h=0;
    double min_h=1000000;
    int divs=0;
    int doubles=0;
    double max_p=0;
    for(int i=1;i<num_of_lines1;i++){
        double temp=fabs(std::stod(answer.second.first[i*N+5].second));
        double temp_global=fabs(std::stod(answer.second.first[i*N+9].second));
        if(temp_global>max_p)
            max_p=temp_global;
        if (temp>max1_olp)
            max1_olp=temp;
        double this_h=std::stod(answer.second.first[i*N+2].second);
        if(this_h<min_h){
            min_h=this_h;
            MIN_H_x_1=QString::number(std::stod(answer.second.first[i*N+1].second));
        }
        if(this_h>max_h){
            max_h=this_h;
            MAX_H_x_1=QString::number(std::stod(answer.second.first[i*N+1].second));
        }
        divs+=std::stoi(answer.second.first[i*N+6].second);
        doubles+=std::stoi(answer.second.first[i*N+7].second);
    }
    QString MAX_P_1=QString::number(max_p);//Макс Погрешность
    QString MAX_OLP_1= QString::number(max1_olp); //Макс ОЛП
    QString DIVS_1=QString::number(divs);   //делений
    QString DOUBLES_1 =QString::number(doubles);   //удвоений
    QString MAX_H_1=QString::number(max_h);     //Max_h
    QString MIN_H_1=QString::number(min_h);       //Min_h
    QStringList spravka1;
    spravka1<<ITERS_1<<b_xn_1<<MAX_OLP_1<<DIVS_1<<DOUBLES_1<<MAX_H_1<<MAX_H_x_1<<MIN_H_1<<MIN_H_x_1<<MAX_P_1;

    //для справки 2 U'(x)
    QString ITERS_2= QString::number(num_of_lines2-1);//кол-во итераций
    QString b_xn_2 = QString::number(inData.xT-std::stod(answer.second.second[(num_of_lines2-1)*N+1].second));//b-xn
    QString MAX_H_x_2;          //x
    QString MIN_H_x_2;        //x
    max1_olp=0;
    max_p=0;
    max_h=0;
    min_h=1000000;
    divs=0;
    doubles=0;
    for(int i=1;i<num_of_lines2;i++){
        double temp_global=fabs(std::stod(answer.second.second[i*N+9].second));
        if(temp_global>max_p)
            max_p=temp_global;
        double temp=fabs(std::stod(answer.second.second[i*N+5].second));
        if (temp>max1_olp)
            max1_olp=temp;
        double this_h=std::stod(answer.second.second[i*N+2].second);
        if(this_h<min_h){
            min_h=this_h;
            MIN_H_x_2=QString::number(std::stod(answer.second.second[i*N+1].second));
        }
        if(this_h>max_h){
            max_h=this_h;
            MAX_H_x_2=QString::number(std::stod(answer.second.second[i*N+1].second));
        }
        divs+=std::stoi(answer.second.second[i*N+6].second);
        doubles+=std::stoi(answer.second.second[i*N+7].second);
    }
    QString MAX_P_2=QString::number(max_p);//Макс Погрешность
    QString MAX_OLP_2= QString::number(max1_olp); //Макс ОЛП
    QString DIVS_2=QString::number(divs);   //делений
    QString DOUBLES_2 =QString::number(doubles);   //удвоений
    QString MAX_H_2=QString::number(max_h);     //Max_h
    QString MIN_H_2=QString::number(min_h);       //Min_h
    QStringList spravka2;
    spravka2<<ITERS_2<<b_xn_2<<MAX_OLP_2<<DIVS_2<<DOUBLES_2<<MAX_H_2<<MAX_H_x_2<<MIN_H_2<<MIN_H_x_2<<MAX_P_2;

    QString for_spravka_1="Итераций:  "+spravka1[0]+"\n";
    for_spravka_1+= "xT - xn:  "+spravka1[1]+"\n";
    for_spravka_1+= "Макс ОЛП:  "+spravka1[2]+"\n";
    for_spravka_1+= "Делений:  "+spravka1[3]+"\n";
    for_spravka_1+= "Удвоений:  "+spravka1[4]+"\n";
    for_spravka_1+= "Макс h:  "+spravka1[5]+"\n";
    for_spravka_1+= "x, где достигается Макс h: "+spravka1[6]+"\n";
    for_spravka_1+= "Мин h:  "+spravka1[7]+"\n";
    for_spravka_1+= "x, где достигается Мин h:  "+spravka1[8]+"\n";
    for_spravka_1+= "Макс глоб. погрешность:  "+spravka1[9]+"\n";
    ui->textEdit->setText(for_spravka_1);

    QString for_spravka_2="Итераций:  "+spravka2[0]+"\n";
    for_spravka_2+= "xT - xn:  "+spravka2[1]+"\n";
    for_spravka_2+= "Макс ОЛП:  "+spravka2[2]+"\n";
    for_spravka_2+= "Делений:  "+spravka2[3]+"\n";
    for_spravka_2+= "Удвоений:  "+spravka2[4]+"\n";
    for_spravka_2+= "Макс h:  "+spravka2[5]+"\n";
    for_spravka_2+= "x, где достигается Макс h: "+spravka2[6]+"\n";
    for_spravka_2+= "Мин h:  "+spravka2[7]+"\n";
    for_spravka_2+= "x, где достигается Мин h:  "+spravka2[8]+"\n";
    for_spravka_2+= "Макс глоб. погрешность:  "+spravka2[9]+"\n";
    ui->textEdit_2->setText(for_spravka_2);

    //тут типа должно быть обновление графиков
    updateDataTable(num_of_lines1,num_of_lines2);
//    QString text=" Исходный дифур:\n mU''+kU=F;\n u(x0)=u0; u'(x0)=u'0\n x(нач)<x<x(конеч)\n ";
//    QString text2=" Точное решение";
//    QString all=text+text2;
//    ui->textEdit->setText(all);
    on_pbResults_clicked();
    this->killTimer(timerCalcID);
    setControlState();
}

void MainWindow::setControlState()
{
    QPalette p = palette();
    if(mainCalcTh.isRunning()){
         ui->pbCalculate->setEnabled(false);
         ui->pbResults->setEnabled(false);
         p.setColor(QPalette::Highlight, Qt::red);
    }else{
         ui->pbCalculate->setEnabled(true);
         ui->pbResults->setEnabled(true);
         p.setColor(QPalette::Highlight, Qt::green);
    }
    ui->progBar->setPalette(p);
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    calc100ms++;
    QString sTime;
    int ms = elapsedCalcTime.elapsed();
    sTime.sprintf("%02d:%03d", ms / 1000, ms % 1000);
    ui->lbElapsedTSecs->setText("Время: " + sTime+" c"); // sec:ms
    ui->progBar->setValue(calc100ms % (ui->progBar->maximum()));
}

double true_u (double x,double x0,double k,double m,double f_small,std::vector<double> u0){
    double g=9.81;
    double F=m*g*f_small;
    double A=sqrt(k/m);
    double C1=-u0[1]*sin(A*x0)/A+(u0[0]-F/k)*cos(A*x0);
    double C2=(u0[0]-F/k)*sin(A*x0)+cos(A*x0)*u0[1]/A;
    return C1*cos(A*x)+C2*sin(A*x)+F/k;
}//истинное решение
double true_u_dif (double x,double x0,double k,double m,double f_small,std::vector<double> u0){
    double g=9.81;
    double F=m*g*f_small;
    double A=sqrt(k/m);
    double C1=-u0[1]*sin(A*x0)/A+(u0[0]-F/k)*cos(A*x0);
    double C2=(u0[0]-F/k)*sin(A*x0)+cos(A*x0)*u0[1]/A;
    return C1*(-1)*A*sin(A*x)+C2*A*cos(A*x);
}//истинное решение
int flag_for_chart_true=-1;
int flag_for_chart_var4=-1;
void MainWindow::on_pbResults_clicked()
{
    chartU->Title("Зависимость координаты от времени");
    chartPhaze->Title("Фазовый портрет");
    std::vector<std::pair<double,double>> vec_chartU{}; // u(x)
    std::vector<std::pair<double,double>> vec_chartU_true{}; // u(x) истинное
    std::vector<std::pair<double,double>> vec_chartPhase_true{};// u'(x)  истинное
    std::vector<std::pair<double,double>> vec_chart_var4{}; //u(x) вариант4
    std::vector<std::pair<double,double>> vec_chartPhaze{}; // u'(u)
    unsigned int num_of_lines1=answer.second.first.size()/N_DEF;//9 - 11
    unsigned int num_of_lines2=answer.second.second.size()/N_DEF;//9 - 11
    for(unsigned int ln=0;ln<num_of_lines1;ln++) {
        //answer.first.first[i],answer.first.second[i].first;
        vec_chartU.push_back(std::make_pair(answer.first.first[ln],answer.first.second[ln][0]));
        vec_chartU_true.push_back(std::make_pair(answer.first.first[ln], true_u(answer.first.first[ln],inData.x0,inData.k,inData.m,inData.f,inData.iv)));
    }
//    for(unsigned int i=0;i<num_of_lines2;i++) {
//        //answer.first.first[i],answer.first.second[i].first;
//        vec_chart1_2.push_back(std::make_pair(answer.first.first[i],answer.first.second[i][1]));
//    }
    for(unsigned int ln=0;ln<num_of_lines2;ln++) {
        //answer.first.first[i],answer.first.second[i].first;
        vec_chartPhaze.push_back(std::make_pair(answer.first.second[ln][0],answer.first.second[ln][1]));
        vec_chartPhase_true.push_back(std::make_pair(answer.first.second[ln][0],true_u_dif(answer.first.first[ln],inData.x0,inData.k,inData.m,inData.f,inData.iv)));
    }


    unsigned int num_of_lines_var4=answer2.second.first.size()/N_DEF;
    for(unsigned int ln=0;ln<num_of_lines1;ln++) {
        vec_chart_var4.push_back(std::make_pair(answer.first.first[ln],answer.first.second[ln][0]));
    }


    if (flag_for_chart==-1){
        flag_for_chart=0;
    }else if(flag_for_chart==0){
        flag_for_chart=1;
    }
    chartU->axisX->setRange(ui->lineEdit_8->text().toDouble(),  ui->leX_end->text().toDouble());
    chartU->axisY->setRange(-10,  10);
    chartPhaze->axisX->setRange(-10.0,  10.0);
    chartPhaze->axisY->setRange(-30.0,  30.0);
    //chartU->axisX-сто>setRange(0.0,  ui->leX_end->text().toDouble()); // todo; Where get min max?
    chartU->make_chart(vec_chartU,flag_for_chart); //ERROR!!!!!!!!!!
    chartU->make_chart(vec_chartU_true,flag_for_chart_true);//serias 3
    chartU->make_chart(vec_chart_var4,flag_for_chart_var4);//serias 4
    chartPhaze->make_chart(vec_chartPhaze,flag_for_chart); //ERROR!!!!!!!!!!
    chartPhaze->make_chart(vec_chartPhase_true,flag_for_chart_true);

    //v_chart1->make_x_y("u","x"); // фигня какая-то выходит
    QString text=" Исходный дифур:\n mU''+kU=F;\n u(x0)=u0; u'(x0)=u'0\n x(нач)<x<x(конеч)\n ";
    QString text3="Точное решение:\n A=sqrt(k/m)\n ";
    QString text2="U(x)=u0*cos(A*x)+u'0*sin(A*x)\n U'(x)=-u0*A*sin(A*x)+u'0*A*cos(A*x)";
    QString all=text+text3+text2;
    ui->teInfo->setText(all);
}

void MainWindow::on_pbClearCharts_clicked()
{
    chartU->clear();// возможно там что-то неправильно
    chartPhaze->clear();

    ui->lbElapsedTSecs->setText("Время: 00:000 с");
}


void MainWindow::on_comboBox_activated(int index)
{

    if (index==1){
        main_task window2;
        window2.setModal(true);
        window2.exec();
    }
}



void MainWindow::on_check_var4_stateChanged(int arg1)
{
    if(arg1){
        flag_for_chart_var4=4;
    }
    else {
        flag_for_chart_var4=-1;
    }
}

void MainWindow::on_check_true_stateChanged(int arg1)
{
    if(arg1){
            flag_for_chart_true=3;
    }
    else {
        flag_for_chart_true=-1;
    }
}
