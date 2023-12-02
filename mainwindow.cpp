#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "main_task.h"
#include <QPair>
#include <QtMath>
#include <QFile>
#include <QTextStream>
#include "numcpp.h"
#include "chart1.h"
#include <QResizeEvent>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Construct Main window
    ui->setupUi(this);
    // Insert QGraphicsView
    v_chart1 = new View_Chart1(ui->tabChart);
    v_chart2 = new View_Chart1(ui->tab_chart2);
    // Insert QTableWidget
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setColumnCount(11);
    QStringList labels;
    labels << tr("i") << tr("Xi")<< tr("hi") << tr("Vi")<< tr("V^i") << tr("Vi-V^i")<< tr("ОЛП") << tr("Деления")<< tr("Удвоения") << tr("Поменять название")<< tr("Поменять название");
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
    v_chart1->resize(newSize);
    v_chart2->resize(newSize);
    qDebug() << "MW Show() Size w: " << newSize.width() << "h: " << newSize.height();
    //7updateDataTable(int n);
}

void MainWindow::resizeCharts()
{
    QSize newSize = ui->tabChart->size();
    v_chart1->resize(newSize);
    v_chart2->resize(newSize);
    qDebug() << "MW Resize() Size w: " << newSize.width() << "h: " << newSize.height();
}

void MainWindow::on_twMain_currentChanged(int index)
{
    resizeCharts();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    resizeCharts();
}
std::pair< std::pair<std::vector<double>, std::vector<std::vector<double>>>,
    std::pair<std::vector<std::pair<int,std::string>>,std::vector<std::pair<int,std::string>>>> answer;

void MainWindow::updateDataTable(unsigned int num_of_lines1,unsigned int num_of_lines2)
{
    ui->tableWidget->setUpdatesEnabled(false);
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget_2->setColumnCount(11);
    QStringList labels;
    labels << tr("i") << tr("Xi")<< tr("hi") << tr("Vi")<< tr("V^i") << tr("Vi-V^i")<< tr("ОЛП") << tr("Деления")<< tr("Удвоения") << tr("Поменять название")<< tr("Поменять название");
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    for (int i=0;i<11;i++){
         ui->tableWidget->horizontalHeader()->resizeSection(i, 80);
    }

    ui->tableWidget_2->setUpdatesEnabled(false);
    ui->tableWidget_2->clear();
    ui->tableWidget_2->setColumnCount(11);
    ui->tableWidget_2->setHorizontalHeaderLabels(labels);
    //ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    for (int i=0;i<11;i++){
         ui->tableWidget_2->horizontalHeader()->resizeSection(i, 80);
    }
    ui->tableWidget_2->setRowCount(0);

    for (unsigned int i = 0; i < num_of_lines1; i++) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(row + 1);
        int N=9;//надо заменить на 11, когда будет истинное решение
        int temp=answer.second.first.size();
        QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(answer.second.first[i*N].first));
        ui->tableWidget->setItem(row, 0, item0);
        for(int j=1;j<N;j++){
            QTableWidgetItem *item0 = new QTableWidgetItem(QString(answer.second.first[i*N+j].second.c_str()));
            ui->tableWidget->setItem(row, j, item0);
        }
    }
    for (unsigned int i = 0; i < num_of_lines2; i++) {
        int row = ui->tableWidget_2->rowCount();
        ui->tableWidget_2->setRowCount(row + 1);
        int N=9;//надо заменить на 11, когда будет истинное решение
        QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(answer.second.second[i*N].first));
        ui->tableWidget_2->setItem(row, 0, item0);
        for(int j=1;j<N;j++){
            QTableWidgetItem *item0 = new QTableWidgetItem(QString(answer.second.second[i*N+j].second.c_str()));
            ui->tableWidget_2->setItem(row, j, item0);
        }
    }

    ui->tableWidget->setUpdatesEnabled(true);
    ui->tableWidget_2->setUpdatesEnabled(true);
}


int flag_for_chart=-1;


void MainWindow::on_pushButton_2_clicked()
{
    int N=9;//11
    ////////////////собираю параметры с интерфейса
    double epsilon=ui->lineEdit->text().toDouble();
    //precision= e_up and e_down
    double e_up=ui->lineEdit_2->text().toDouble();
    double precision=e_up;
    int Nmax=ui->lineEdit_3->text().toInt();
    double h0=ui->lineEdit_4->text().toDouble();
    double k=ui->lineEdit_10->text().toDouble();
    double f=ui->lineEdit_11->text().toDouble();
    double m=ui->lineEdit_12->text().toDouble();
    double u0=ui->lineEdit_7->text().toDouble();
    double u_0=ui->lineEdit_6->text().toDouble(); //производная в нуле
    double x0=ui->lineEdit_8->text().toDouble();
    double xT=ui->lineEdit_9->text().toDouble();

    bool control_nothing=ui->radioButton_3->isChecked();
    bool control_up=ui->radioButton_2->isChecked();
    bool dif_step=0;
    if (control_nothing){
        dif_step=0;
    }
    else if(control_up) {
        dif_step=1;
    }
    ////////////////

    ////////////////создаю функции и вспомогательные параметры
    std::vector<double> iv{ u0, u_0 };
    double F=m*f*9.81; //mgf
    std::function<double(double, std::vector<double>)> f1 = [](double x, std::vector<double> u) { return u[1]; };
    std::function<double(double, std::vector<double>)> f2 = [F, k, m](double x, std::vector<double> u) { return (-F/m-k*u[0])/m; };//u(1) уточнить - уточнила
    answer=numcpp::RK4_system(x0, xT, iv, { f1, f2 }, h0, precision, Nmax, dif_step,  epsilon); //с контролем шага эта функция глушит программу
    int num_of_lines1=answer.second.first.size()/N; //9 - 11
    int num_of_lines2=answer.second.second.size()/N;//9 - 11
    //для справки 1 U(x)
    //*
    QString ITERS_1= QString::number(num_of_lines1-1);//кол-во итераций
    QString b_xn_1 = QString::number(xT-std::stod(answer.second.first[(num_of_lines1-1)*N+1].second));//b-xn
    QString MAX_H_x_1;          //x
    QString MIN_H_x_1;        //x
    double max1_olp=0;
    double max_h=0;
    double min_h=1000000;
    int divs=0;
    int doubles=0;
    for(int i=1;i<num_of_lines1;i++){
        double temp=abs(std::stod(answer.second.first[i*N+5].second));
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
    QString MAX_OLP_1= QString::number(max1_olp); //Макс ОЛП
    QString DIVS_1=QString::number(divs);   //делений
    QString DOUBLES_1 =QString::number(doubles);   //удвоений
    QString MAX_H_1=QString::number(max_h);     //Max_h
    QString MIN_H_1=QString::number(min_h);       //Min_h
    QStringList spravka1;
    spravka1<<ITERS_1<<b_xn_1<<MAX_OLP_1<<DIVS_1<<DOUBLES_1<<MAX_H_1<<MAX_H_x_1<<MIN_H_1<<MIN_H_x_1;
    //для справки 2 U'(x)
    QString ITERS_2= QString::number(num_of_lines2-1);//кол-во итераций
    QString b_xn_2 = QString::number(xT-std::stod(answer.second.second[(num_of_lines2-1)*N+1].second));//b-xn
    QString MAX_H_x_2;          //x
    QString MIN_H_x_2;        //x
    max1_olp=0;
    max_h=0;
    min_h=1000000;
    divs=0;
    doubles=0;
    for(int i=1;i<num_of_lines2;i++){
        double temp=abs(std::stod(answer.second.second[i*N+5].second));
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
    QString MAX_OLP_2= QString::number(max1_olp); //Макс ОЛП
    QString DIVS_2=QString::number(divs);   //делений
    QString DOUBLES_2 =QString::number(doubles);   //удвоений
    QString MAX_H_2=QString::number(max_h);     //Max_h
    QString MIN_H_2=QString::number(min_h);       //Min_h
    QStringList spravka2;
    spravka2<<ITERS_2<<b_xn_2<<MAX_OLP_2<<DIVS_2<<DOUBLES_2<<MAX_H_2<<MAX_H_x_2<<MIN_H_2<<MIN_H_x_2;

    QString for_spravka_1="Итераций:  "+spravka1[0]+"\n";
    for_spravka_1+= "xT - xn:  "+spravka1[1]+"\n";
    for_spravka_1+= "Макс ОЛП:  "+spravka1[2]+"\n";
    for_spravka_1+= "Делений:  "+spravka1[3]+"\n";
    for_spravka_1+= "Удвоений:  "+spravka1[4]+"\n";
    for_spravka_1+= "Макс h:  "+spravka1[5]+"\n";
    for_spravka_1+= "x, где достигается Макс h: "+spravka1[6]+"\n";
    for_spravka_1+= "Мин h:  "+spravka1[7]+"\n";
    for_spravka_1+= "x, где достигается Мин h:  "+spravka1[8]+"\n";
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
    ui->textEdit_2->setText(for_spravka_2);
//*/
    ///////////////
    updateDataTable(num_of_lines1,num_of_lines2);
    //тут типо должно быть обновление графиков
//    QString text=" Исходный дифур:\n mU''+kU=F;\n u(x0)=u0; u'(x0)=u'0\n x(нач)<x<x(конеч)\n ";
//    QString text2=" Точное решение";
//    QString all=text+text2;
//    ui->textEdit->setText(all);
}

//void MainWindow::on_pushButton_3_clicked()
//{
//    ui->textEdit->setText(QString(numcpp::test_stl("123").c_str()));
//}

//void MainWindow::on_pushButton_3_clicked()
//{

//}


void MainWindow::on_pbTest_clicked()
{
    v_chart1->Title("Зависимость координаты от времени");
    v_chart2->Title("Фазовый портрет");
    std::vector<std::pair<double,double>> vec_chart1{}; // u(x)
    std::vector<std::pair<double,double>> vec_chart1_2{};// u'(x)  не надо пока что
    std::vector<std::pair<double,double>> vec_chart2{}; // u'(u)
    unsigned int num_of_lines1=answer.second.first.size()/9;//9 - 11
    unsigned int num_of_lines2=answer.second.second.size()/9;//9 - 11
    for(int i=0;i<num_of_lines1;i++) {
        //answer.first.first[i],answer.first.second[i].first;
        vec_chart1.push_back(std::make_pair(answer.first.first[i],answer.first.second[i][0]));
    }
    for(int i=0;i<num_of_lines2;i++) {
        //answer.first.first[i],answer.first.second[i].first;
        vec_chart1_2.push_back(std::make_pair(answer.first.first[i],answer.first.second[i][1]));
    }
    for(int i=0;i<num_of_lines2;i++) {
        //answer.first.first[i],answer.first.second[i].first;
        vec_chart2.push_back(std::make_pair(answer.first.second[i][0],answer.first.second[i][1]));
    }
    if (flag_for_chart==-1)
        flag_for_chart=0;
    else if(flag_for_chart==0)
        flag_for_chart=1;
    v_chart1->make_chart(vec_chart1,flag_for_chart);   //ERROR!!!!!!!!!!
    v_chart2->make_chart(vec_chart2,flag_for_chart); //ERROR!!!!!!!!!!

    //v_chart1->make_x_y("u","x"); // фигня какая-то выходит
    QString text=" Исходный дифур:\n mU''+kU=F;\n u(x0)=u0; u'(x0)=u'0\n x(нач)<x<x(конеч)\n ";
    QString text2=" Точное решение";
    QString all=text+text2;
    ui->teInfo->setText(all);
}


void MainWindow::on_pushButton_3_clicked()
{
    v_chart1->clear();// возможно там что-то неправильно
    v_chart2->clear();
}
