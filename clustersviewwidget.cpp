#include "clustersviewwidget.h"
#include <QWheelEvent>

ClustersViewWidget::ClustersViewWidget(QWidget *parent): QGraphicsView(parent)
{
    scene = new QGraphicsScene;
    setScene(scene);
}

ClustersViewWidget::~ClustersViewWidget()
{

}

void ClustersViewWidget::wheelEvent(QWheelEvent *event)
{
    const qreal scaleFactor = 1.15;
        if(event->delta() > 0)
        {
            // Zoom in
            scale(scaleFactor, scaleFactor);
        }
        else
        {
            // Zooming out
            scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        }
    centerOn(mapToScene(event->pos()));
}

void ClustersViewWidget::setClustersWidgetInfo(const ClustersViewWidgetInfo& info)
{
    clustersWidgetInfo = info;
    update();
}

void ClustersViewWidget::update()
{
    QGraphicsView::update();
    scene->clear();
    //scene->addPixmap(QPixmap::fromImage(*clustersWidgetInfo.image));
    foreach (Cluster cluster, clustersWidgetInfo.info) {
        if(cluster.center.x() >= 0 && cluster.center.y() >= 0)
            /*scene->addEllipse(cluster.center.x() - 2.5,
                              cluster.center.y() - 2.5, 5, 5, QPen(QColor(0, 255, 0)), QBrush(QColor(0, 255, 0)));*/
            clustersWidgetInfo.image.setPixel(cluster.center.x(), cluster.center.y(),
                                               QColor(0xff, 0xff, 0xff).rgb());
        foreach(QPointF point, cluster.pointsCluster){
            /*scene->addEllipse(point.x() - 2.5,
                              point.y() - 2.5, 5, 5, QPen(QColor(255, 0, 0)), QBrush(QColor(255, 0, 0)));*/
            clustersWidgetInfo.image.setPixel(point.x(), point.y(),
                                               QColor(0xff, 0xff, 0xff).rgb());
        }
    }
    scene->addPixmap(QPixmap::fromImage(clustersWidgetInfo.image));
}

