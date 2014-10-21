#ifndef MAXIMIUMMINIMUMFORM_H
#define MAXIMIUMMINIMUMFORM_H

#include <QDialog>
#include <QGraphicsView>
#include <QTemporaryFile>
#include "clustersviewwidget.h"

namespace Ui {
class MaximiumMinimumForm;
}

class QMouseEvent;

class MaxMinImageViewWidget: public ClustersViewWidget
{
Q_OBJECT
public:
    explicit MaxMinImageViewWidget(QWidget* parent = 0);
    virtual ~MaxMinImageViewWidget();
signals:
    void pointSetted(const QPointF&);
    void clicked();
protected:
    //virtual void mousePressEvent(QMouseEvent *);
};


class MaximiumMinimumForm : public QDialog
{
    Q_OBJECT

public:
    explicit MaximiumMinimumForm(QWidget *parent = 0);
    const ClustersViewWidgetInfo& getClustersViewWidgetInfo()const{return view->getClustersWidgetInfo();}
    ClustersViewWidgetInfo& getClustersViewWidgetInfo(){return view->getClustersWidgetInfo();}
    double getStopParam(){ return stopParam; }
    bool getResult()const{return result_;}
    ~MaximiumMinimumForm();
public slots:
    void setClustersInfo(const ClustersViewWidgetInfo& info);
private slots:
    void setStopParam(const QString&);
    void handleCancel();
    void handleStart();
    void setDistance(const QString& s);
private:
    Ui::MaximiumMinimumForm *ui;
    ClustersViewWidget* view;
    QString strStopParam;
    QString distance;
    double stopParam;
    QTemporaryFile tmpFile;
    bool result_;
};

#endif // MAXIMIUMMINIMUMFORM_H
