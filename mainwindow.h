#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <QVector>
#include <QPair>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int currentGraph= 0;
private slots:
    void initialization_algorithms();
    void setAlgorithm();
    void swap(double& a, double& b);
    void paint();
    QVector<QPair<int, int>> step_by_step(double x1, double y1, double x2, double y2);
    QVector<QPair<int, int>> bresenhamline(double x1, double y1, double x2, double y2);
    QVector<QPair<int, int>> DDA(double x1, double y1, double x2, double y2);
    void bresenhamcircle(double x0, double y0, double radius);
    QVector<QPair<int, int>> castle(double x1, double y1, double x2, double y2);

    void on_paintButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
