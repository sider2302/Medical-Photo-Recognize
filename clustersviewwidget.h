#ifndef IMAGEVIEWWIDGET_H
#define IMAGEVIEWWIDGET_H

#include <QGraphicsView>
#include <QVector>
#include <recognizealgs.h>

class QImage;
class QGraphicsScene;
class QWheelEvent;

struct ClustersViewWidgetInfo
{
    QImage image;
    QVector<Cluster> info;

    ClustersViewWidgetInfo()
    {

    }

    ClustersViewWidgetInfo(QImage image_):image(image_)
    {
    }

    inline friend void addCluster(ClustersViewWidgetInfo& info, const Cluster& c)
    {
        info.info.push_front(c);
        std::sort(info.info.begin(), info.info.end());
        info.info.erase(std::unique(info.info.begin(), info.info.end()), info.info.end());
    }

    inline friend QDataStream& operator << (QDataStream& stream, const ClustersViewWidgetInfo& info)
    {
        stream << info.image;
        stream << info.info;
        return stream;
    }

    inline friend QDataStream& operator >> (QDataStream& stream, ClustersViewWidgetInfo& info)
    {
        stream >> info.image;
        stream >> info.info;
        return stream;
    }
};

class ClustersViewWidget: public QGraphicsView
{
    Q_OBJECT
public:
    explicit ClustersViewWidget(QWidget* parent = 0);
    const ClustersViewWidgetInfo& getClustersWidgetInfo()const{ return clustersWidgetInfo; }
    void update();
    ClustersViewWidgetInfo& getClustersWidgetInfo(){ return clustersWidgetInfo; }
protected:
    virtual void wheelEvent(QWheelEvent* event);
    QGraphicsScene* scene;
    ClustersViewWidgetInfo clustersWidgetInfo;
public:
    virtual ~ClustersViewWidget();
public slots:
    void setClustersWidgetInfo(const ClustersViewWidgetInfo& info);

};

#endif // IMAGEVIEWWIDGET_H
