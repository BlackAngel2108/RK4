/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "chart1.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtWidgets/QGraphicsTextItem>
//#include "callout.h"
#include <QtGui/QMouseEvent>
#include <QDebug>

View_Chart1::View_Chart1(QWidget *parent)
    : QGraphicsView(new QGraphicsScene, parent)
{
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
    sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);

    // chart
    m_chart = new QChart;
    // It is minimus Chart size
    m_chart->setMinimumSize(100, 100);
    //m_chart->setTitle("График");
    m_chart->legend()->hide();

    series = new QLineSeries;
    series2 = new QLineSeries;

    m_chart->addSeries(series);
    m_chart->addSeries(series2);

    axisX = new QValueAxis;
    axisY = new QValueAxis;

    m_chart->setAcceptHoverEvents(true);

//    setRenderHint(QPainter::Antialiasing);
    scene()->addItem(m_chart);

    m_coordX = new QGraphicsSimpleTextItem(m_chart);
    m_coordX->setText("X: ");
    m_coordY = new QGraphicsSimpleTextItem(m_chart);
    m_coordY->setText("Y: ");

    //scene()->setSceneRect(QRect(QPoint(0, 0), this->parentWidget()->size()));


//    connect(series, &QLineSeries::clicked, this, &View_Chart1::keepCallout);
//    connect(series, &QLineSeries::hovered, this, &View_Chart1::tooltip);

//    connect(series2, &QSplineSeries::clicked, this, &View_Chart1::keepCallout);
//    connect(series2, &QSplineSeries::hovered, this, &View_Chart1::tooltip);

//    this->setMouseTracking(true);
}
void View_Chart1::make_x_y(QString strx, QString stry){
    m_coordX = new QGraphicsSimpleTextItem(m_chart);
    m_coordX->setText(strx);
    m_coordY = new QGraphicsSimpleTextItem(m_chart);
    m_coordY->setText(stry);
}
void View_Chart1::Title(QString str){
    m_chart->setTitle(str);
}
void View_Chart1::make_chart(std::vector<std::pair<double,double>> &v,bool flag){


    if(flag==0){
        m_chart->removeSeries(series);
        series->clear();
        int n=v.size();
        for(int i=0;i<n;i++){
            series->append(v[i].first, v[i].second);
        }           
        m_chart->addSeries(series);
        m_chart->setAxisX(axisX, series);
        m_chart->setAxisY(axisY, series);
    }
    if(flag==1){
        m_chart->removeSeries(series2);
        series2->clear();
        int n=v.size();
        for(int i=0;i<n;i++){
            series2->append(v[i].first, v[i].second);
        }
        m_chart->addSeries(series2);
        m_chart->setAxisX(axisX, series2);
        m_chart->setAxisY(axisY, series2);
    }

    //axisX->setRange(0, 20.5);
    axisX->setTickCount(10);
    axisX->setLabelFormat("%.2f");

    axisY->setRange(-10, 10);
    axisY->setTickCount(10);
    axisY->setLabelFormat("%.2f");
}
void View_Chart1::resizeEvent(QResizeEvent *event)
{
    if (scene()) {
        QSize sz = event->size();
        //sz.setHeight(sz.height() + 2);
        scene()->setSceneRect(QRect(QPoint(0, 0), sz));
        m_chart->resize(sz);
        m_coordX->setPos(m_chart->size().width()/2 - 100, m_chart->size().height() - 20);
        m_coordY->setPos(m_chart->size().width()/2 + 50, m_chart->size().height() - 20);
//      const auto callouts = m_callouts;
//         for (Callout *callout : callouts)
//             callout->updateGeometry();
        qDebug() << "VC Size w: " << sz.width() << "h: " << sz.height();
    }
    QGraphicsView::resizeEvent(event);
}

void View_Chart1::mouseMoveEvent(QMouseEvent *event)
{
    m_coordX->setText(QString("X: %1").arg(m_chart->mapToValue(event->pos()).x()));
    m_coordY->setText(QString("Y: %1").arg(m_chart->mapToValue(event->pos()).y()));
    QGraphicsView::mouseMoveEvent(event);
}

void View_Chart1::clear(){
    series->clear();
    series2->clear();
    //m_chart->series().removeFirst();
}

void View_Chart1::setAxisNames(QString xName, QString yName)
{
    axisX->setTitleText(xName);
    axisY->setTitleText(yName);
}
//void View::keepCallout()
//{
//    m_callouts.append(m_tooltip);
//    m_tooltip = new Callout(m_chart);
//}

//void View::tooltip(QPointF point, bool state)
//{
//    if (m_tooltip == 0)
//        m_tooltip = new Callout(m_chart);

//    if (state) {
//        m_tooltip->setText(QString("X: %1 \nY: %2 ").arg(point.x()).arg(point.y()));
//        m_tooltip->setAnchor(point);
//        m_tooltip->setZValue(11);
//        m_tooltip->updateGeometry();
//        m_tooltip->show();
//    } else {
//        m_tooltip->hide();
//    }
//}
