#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clustersviewwidget.h"
#include "maximiumminimumform.h"
#include <QImage>
#include <QFileDialog>
#include <QFileInfo>
#include <QMdiSubWindow>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    QObject::connect(ui->actionMaximum_minimum_distance_method, SIGNAL(triggered()), this, SLOT(openMaxMinDialog()));
    QObject::connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    QObject::connect(ui->actionOpen_recognized_image, SIGNAL(triggered()), this, SLOT(openRecognizedImage()));
    QObject::connect(ui->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(setSubwindow(QMdiSubWindow*)));
}

void MainWindow::setSubwindow(QMdiSubWindow *subwnd)
{
    if(subwnd)
        subwindow = qobject_cast<ClustersViewWidget*>(subwnd->widget());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    QFileDialog *openFileDialog = new QFileDialog;
    connect(openFileDialog, SIGNAL(fileSelected(QString)), this, SLOT(showImageWidget(QString)));
    openFileDialog->exec();
}

void MainWindow::showImageWidget(const QString &filename)
{
    ClustersViewWidget *widget = new ClustersViewWidget;
    QImage image(filename);

    widget->setClustersWidgetInfo(ClustersViewWidgetInfo(image));
    widget->setWindowTitle(QFileInfo(filename).canonicalFilePath());
    ui->mdiArea->addSubWindow(widget);
    widget->setEnabled(true);
    widget->setVisible(true);
}

void MainWindow::openMaxMinDialog()
{
    MaximiumMinimumForm *form = new MaximiumMinimumForm(this);
    form->setClustersInfo(subwindow->getClustersWidgetInfo());
    form->exec();
    if(form->getResult()){
        ClustersViewWidget *widget = new ClustersViewWidget;
        widget->setWindowTitle(tr("Recognize result: ") + subwindow->windowTitle());
        widget->setClustersWidgetInfo(form->getClustersViewWidgetInfo());
        ui->mdiArea->addSubWindow(widget);
        widget->setEnabled(true);
        widget->setVisible(true);
    }
}

void MainWindow::save()
{
    QFileDialog *saveFileDialog = new QFileDialog;
    connect(saveFileDialog, SIGNAL(fileSelected(QString)), this, SLOT(saveToFile(QString)));
    saveFileDialog->exec();
}

void MainWindow::saveToFile(const QString &fn)
{
    QFile f(fn);
    f.open(QFile::WriteOnly);
    QDataStream stream(&f);
    ClustersViewWidget* subWnd = qobject_cast<ClustersViewWidget*>(ui->mdiArea->currentSubWindow()->widget());
    stream << subWnd->getClustersWidgetInfo();
    f.flush();
    f.close();
}

void MainWindow::openRecognizedImage()
{
    QFileDialog *openFileDialog = new QFileDialog;
    connect(openFileDialog, SIGNAL(fileSelected(QString)), this, SLOT(showRecognizedImage(QString)));
    openFileDialog->exec();
}

void MainWindow::showRecognizedImage(const QString &fn)
{
    ClustersViewWidget *widget = new ClustersViewWidget;
    ClustersViewWidgetInfo info;
    QFile f(fn);
    f.open(QFile::ReadOnly);
    QDataStream stream(&f);
    stream >> info;
    widget->setWindowTitle(f.fileName());
    f.flush();
    f.close();
    widget->setClustersWidgetInfo(info);
    ui->mdiArea->addSubWindow(widget);
    widget->setVisible(true);
    widget->setEnabled(true);
    widget->update();
}
