#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class ClustersViewWidget;
class QMdiSubWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ClustersViewWidget* subwindow;

private slots:
    void openFile();
    void save();
    void saveToFile(const QString& s);
    void showImageWidget(const QString& filename);
    void openMaxMinDialog();
    void openRecognizedImage();
    void showRecognizedImage(const QString& fn);
    void setSubwindow(QMdiSubWindow* subwnd);
};

#endif // MAINWINDOW_H
