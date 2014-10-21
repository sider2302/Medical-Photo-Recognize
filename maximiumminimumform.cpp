#include "maximiumminimumform.h"
#include "ui_maximiumminimumform.h"
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QMessageBox>
#include <QFile>
#include <algorithm>
#include <cmath>

MaximiumMinimumForm::MaximiumMinimumForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaximiumMinimumForm)
{
    ui->setupUi(this);
    view = new MaxMinImageViewWidget;
    view->setParent(ui->widget);
    view->setCursor(QCursor(Qt::CrossCursor));
    ui->widget->setVisible(true);
    ui->widget->setEnabled(true);
    QObject::connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(setStopParam(QString)));
    QObject::connect(ui->distance, SIGNAL(textChanged(QString)), this, SLOT(setDistance(QString)));
    QObject::connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(handleCancel()));
    QObject::connect(ui->startButton, SIGNAL(clicked()), this, SLOT(handleStart()));
}

MaximiumMinimumForm::~MaximiumMinimumForm()
{
    delete ui;
}

void MaximiumMinimumForm::setClustersInfo(const ClustersViewWidgetInfo& info)
{
    view->setClustersWidgetInfo(info);
}

MaxMinImageViewWidget::MaxMinImageViewWidget(QWidget *parent): ClustersViewWidget(parent){}

/*void MaxMinImageViewWidget::mousePressEvent(QMouseEvent* event)
{
    QPointF p = mapToScene(event->pos());
    addCluster(getClustersWidgetInfo(), Cluster(p));
    update();
    emit pointSetted(p);
}*/

void MaximiumMinimumForm::setStopParam(const QString & val)
{
    strStopParam = val;
}

void MaximiumMinimumForm::handleCancel()
{
    result_ = false;
    close();
}

double maxmin_distance(Point& p1, Point& p2)
{
    return distance(p1, p2);
}

Cluster maxmin_make_cluster(Point& p, QVector<Point>& v){
    Cluster c = p;

    for(QVector<Point>::iterator i = v.begin(); i != v.end(); i++){
        if(maxmin_distance(p, *i) <= 100 && p != *i){
            c.pointsCluster.push_front(*i);
            v.erase(i);
        }
    }

    v.erase(std::remove(v.begin(), v.end(), p), v.end());

    return c;
}

void MaximiumMinimumForm::handleStart()
{
    bool ok;

    double d = distance.toDouble(&ok);
    if(!ok){
        QMessageBox::warning(this, tr("Error"), tr("Invalid distance parameter"), QMessageBox::Ok, QMessageBox::NoButton);
        distance.clear();
        return;
    }


    stopParam = strStopParam.toDouble(&ok);
    if(!ok){
        QMessageBox::warning(this, tr("Error"), tr("Invalid stop parameter"), QMessageBox::Ok, QMessageBox::NoButton);
        stopParam = 0;
        strStopParam.clear();
        return;
    }

    for(int x = 0; x < getClustersViewWidgetInfo().image.width(); x++){
        for(int y = 0; y < getClustersViewWidgetInfo().image.height(); y++){
            getClustersViewWidgetInfo().info.push_back(Cluster
                                                       (Point(
                                                            QColor(
                                                                getClustersViewWidgetInfo().image.pixel(x, y)),
                                                            QPoint(x, y))));
        }
    }

    if(getClustersViewWidgetInfo().info.size() < 1){
        QMessageBox::warning(this, tr("Error"), tr("You must set one minimum point"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    result_ = true;

    QVector<Cluster> recogResult = maxmin(getClustersViewWidgetInfo().info, stopParam);
    QVector<Cluster> points = getClustersViewWidgetInfo().info;
    ClustersViewWidgetInfo info = getClustersViewWidgetInfo();

    QVector<Cluster> createdClusters;

    for(QVector<Cluster>::iterator i = recogResult.begin(); i != recogResult.end(); i++){
        for(QVector<Cluster>::iterator j = points.begin(); j != points.end(); j++){
            if((maxmin_distance(i->center, j->center) <= d) && (i->center != j->center)){
                if(std::find(createdClusters.begin(), createdClusters.end(), *j) == createdClusters.end()){
                    i->pointsCluster.push_front(j->center);
                    createdClusters.push_front(j->center);
                }
            }
        }
    }

    info.info = recogResult;

    info.image.fill(Qt::black);


    setClustersInfo(info);
    /*view->update();*/

    close();
}

void MaximiumMinimumForm::setDistance(const QString &s)
{
    distance = s;
}

MaxMinImageViewWidget::~MaxMinImageViewWidget()
{

}
