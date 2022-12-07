#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->widget->xAxis->setLabel("x");
    ui->widget->yAxis->setLabel("y");
    ui->widget->xAxis->setRange(-10, 10);
    ui->widget->yAxis->setRange(-10, 10);
    ui->widget->xAxis2->setVisible(true);
    ui->widget->xAxis2->setTickLabels(false);
    ui->widget->yAxis2->setVisible(true);
    ui->widget->yAxis2->setTickLabels(false);
    connect(ui->widget->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->widget->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget->yAxis2, SLOT(setRange(QCPRange)));
    ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    ui->x1->setVisible(false);
    ui->x2->setVisible(false);
    ui->y1->setVisible(false);
    ui->y2->setVisible(false);
    ui->R->setVisible(false);
    ui->R_spin->setVisible(false);
    ui->x1_spin->setVisible(false);
    ui->x2_spin->setVisible(false);
    ui->y1_spin->setVisible(false);
    ui->y2_spin->setVisible(false);
    ui->paintButton->setVisible(false);
    ui->label->setVisible(false);

    ui->widget->addGraph();

    initialization_algorithms();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialization_algorithms()
{
    ui->algorithms->addItem("");
    ui->algorithms->addItem("Пошаговый");
    ui->algorithms->addItem("Брезенхема");
    ui->algorithms->addItem("ЦДА");
    ui->algorithms->addItem("Брезенхема (окружность)");
    ui->algorithms->addItem("Кастла-Питвея");
    ui->algorithms->addItem("Сглаженные линии");

    connect(ui->algorithms,SIGNAL(activated(int)),this,SLOT(setAlgorithm()));
}

void MainWindow::setAlgorithm()
{
    ui->x1->setVisible(true);
    ui->x2->setVisible(true);
    ui->y1->setVisible(true);
    ui->y2->setVisible(true);
    ui->R->setVisible(true);
    ui->R_spin->setVisible(true);
    ui->x1_spin->setVisible(true);
    ui->x2_spin->setVisible(true);
    ui->y1_spin->setVisible(true);
    ui->y2_spin->setVisible(true);
    ui->paintButton->setVisible(true);
    ui->label->setVisible(true);

    if(ui->algorithms->currentIndex() == 0) {
        ui->x1->setVisible(false);
        ui->x2->setVisible(false);
        ui->y1->setVisible(false);
        ui->y2->setVisible(false);
        ui->R->setVisible(false);
        ui->R_spin->setVisible(false);
        ui->x1_spin->setVisible(false);
        ui->x2_spin->setVisible(false);
        ui->y1_spin->setVisible(false);
        ui->y2_spin->setVisible(false);
        ui->paintButton->setVisible(false);
    }
    else if(ui->algorithms->currentIndex() == 4) {
        ui->y2->setVisible(false);
        ui->x2->setVisible(false);
        ui->y2_spin->setVisible(false);
        ui->x2_spin->setVisible(false);
        ui->label->setVisible(false);
    }
    else if(ui->algorithms->currentIndex() == 5) {
        ui->R->setVisible(false);
        ui->R_spin->setVisible(false);
    }
    else {
        ui->R->setVisible(false);
        ui->R_spin->setVisible(false);
        ui->label->setVisible(false);
    }
}

void MainWindow::swap(double&a,double&b){
    double tmp = a;
    a = b;
    b = tmp;
}

void MainWindow::paint()
{
    double x1 = ui->x1_spin->value(), x2 = ui->x2_spin->value(),
            y1 = ui->y1_spin->value(), y2 = ui->y2_spin->value();
    double r = ui->R_spin->value();
    QVector<QPair<int, int>> line;
    if(ui->algorithms->currentIndex() == 1) {
        line = step_by_step(x1, y1, x2, y2);
        QVector<double> x, y;
        ui->widget->graph(0)->setPen(QPen(Qt::black));
        for(int i = 0; i < line.size(); i++) {
            x.push_back(line[i].first);
            y.push_back(line[i].second);
        }

        ui->widget->graph(0)->setAntialiased(true);
        ui->widget->graph(0)->setData(x, y);
        ui->widget->replot();
    }
    else if(ui->algorithms->currentIndex() == 2) {
        line = bresenhamline(x1, y1, x2, y2);
        QVector<double> x, y;
        ui->widget->graph(0)->setPen(QPen(Qt::black));
        for(int i = 0; i < line.size(); i++) {
            x.push_back(line[i].first);
            y.push_back(line[i].second);
        }

        ui->widget->graph(0)->setAntialiased(true);
        ui->widget->graph(0)->setData(x, y);
        ui->widget->replot();
    }
    else if(ui->algorithms->currentIndex() == 3) {
        line = DDA(x1, y1, x2, y2);
        QVector<double> x, y;
        ui->widget->graph(0)->setPen(QPen(Qt::black));
        for(int i = 0; i < line.size(); i++) {
            x.push_back(line[i].first);
            y.push_back(line[i].second);
        }

        ui->widget->graph(0)->setAntialiased(true);
        ui->widget->graph(0)->setData(x, y);
        ui->widget->replot();
    }
    else if(ui->algorithms->currentIndex() == 4) {
        bresenhamcircle(x1, y1, r);
    }
    else if(ui->algorithms->currentIndex() == 5) {
        line = castle(x1, y1, x2, y2);
        QVector<double> x, y;
        ui->widget->graph(0)->setPen(QPen(Qt::black));
        for(int i = 0; i < line.size(); i++) {
            x.push_back(line[i].first);
            y.push_back(line[i].second);
        }
        ui->widget->graph(0)->setAntialiased(true);
        ui->widget->graph(0)->setData(x, y);
        ui->widget->replot();
    }
    else if(ui->algorithms->currentIndex() == 6) {
        ui->widget->graph(0)->setAntialiased(false);
        ui->widget->replot();
    }
}

QVector<QPair<int, int>> MainWindow::step_by_step(double x1, double y1, double x2, double y2)
{
    QVector<QPair<int, int>> points;
    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (dx == 0 && dy == 0) {
        points.push_back(std::make_pair(x1, y1));
    }
    else {
        if (std::abs(dx) > std::abs(dy)) {
            for(int x = x1; x <= x2; ++x) {
                     qreal temp = y1 + dy * (x - x1) / (qreal)dx;
                     points.push_back(std::make_pair(x, (int)temp));
            }
        }
        else {
            if (y1 > y2) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }
            for (int y = y1; y <= y2; ++y) {
                     qreal temp = dx / (qreal)dy * (y - y1) + x1;
                     points.push_back(std::make_pair((int)temp, y));
            }
        }
    }
    return points;
}

QVector<QPair<int, int> > MainWindow::bresenhamline(double x1, double y1, double x2, double y2)
{
    QVector<QPair<int, int>> points;

    int dx = std::abs(x2 - x1);
    int sx = x1 < x2 ? 1 : -1;
    int dy = -std::abs(y2 - y1);
    int sy = y1 < y2 ? 1 : -1;
    int error = dx + dy;
    while (true) {
        points.push_back(std::make_pair(x1, y1));
        if (x1 == x2 && y1 == y2) {
            break;
        }
        int e2 = 2 * error;
        if (e2 >= dy) {
            if (x1 == x2) {
                break;
            }
            error = error + dy;
            x1 = x1 + sx;
        }
        if (e2 <= dx) {
            if (y1 == y2) {
                break;
            }
            error = error + dx;
            y1 = y1 + sy;
        }
    }
    return points;
}

QVector<QPair<int, int> > MainWindow::DDA(double x1, double y1, double x2, double y2)
{
    QVector<QPair<int, int>> points;
    qreal dx = (x2 - x1);
    qreal dy = (y2 - y1);
    int step;
    if (abs(dx) >= abs(dy)) {
        step = abs(dx);
    }
    else {
        step = abs(dy);
    }
    dx = dx / step;
    dy = dy / step;
    qreal x = x1;
    qreal y = y1;
    int i = 0;
    while (i <= step) {
        points.push_back(std::make_pair(x, y));
        x = x + dx;
        y = y + dy;
        ++i;
    }
    return points;
}

void MainWindow::bresenhamcircle(double x0, double y0, double rad)
{
    QVector<int> xCoord, yCoord;
    if (rad == 1)
    {
        ui->widget->addGraph();
        ui->widget->addGraph();
        ui->widget->addGraph();

        ui->widget->graph(currentGraph)->addData (x0 + 1, y0);
        ui->widget->graph(currentGraph)->addData (x0, y0 + 1);
        ui->widget->graph(currentGraph+1)->addData (x0, y0 + 1);
        ui->widget->graph(currentGraph+1)->addData (x0 - 1, y0);
        ui->widget->graph(currentGraph+2)->addData (x0 - 1, y0);
        ui->widget->graph(currentGraph+2)->addData (x0, y0 - 1);
        ui->widget->graph(currentGraph+3)->addData (x0, y0 - 1);
        ui->widget->graph(currentGraph+3)->addData (x0 + 1, y0);
        ui->widget->replot();
        currentGraph += 3;
        return;
    }

    ui->widget->addGraph();
    ui->widget->addGraph();
    ui->widget->addGraph();
    ui->widget->addGraph();
    ui->widget->addGraph();
    ui->widget->addGraph();
    ui->widget->addGraph();

    int x = rad;
    int y = 0;
    int radiusError = 1 - x;

    ui->widget->graph(currentGraph+1)->addData(y + x0, x + y0);
    ui->widget->graph(currentGraph+2)->addData(-x + x0, y + y0);
    ui->widget->graph(currentGraph+4)->addData(-x + x0, -y + y0);
    ui->widget->graph(currentGraph+7)->addData(y + x0, -x + y0);

    xCoord.push_back(x);
    yCoord.push_back(y);

    while (x >= y)
    {
        y++;
        if (radiusError < 0)
        {
            radiusError += 2 * y + 1;
        }
        else
        {
            x--;
            radiusError += 2 * (y - x + 1);
        }
        ui->widget->graph(currentGraph+1)->addData(y + x0, x + y0);
        ui->widget->graph(currentGraph+2)->addData(-x + x0, y + y0);
        ui->widget->graph(currentGraph+4)->addData(-x + x0, -y + y0);
        ui->widget->graph(currentGraph+7)->addData(y + x0, -x + y0);
        ui->widget->replot();

        xCoord.push_back(x);
        yCoord.push_back(y);
    }

    for(int i = xCoord.size() - 1; i >= 0; i--)
    {
        ui->widget->graph(currentGraph)->addData(xCoord[i] + x0, yCoord[i] + y0);
        ui->widget->graph(currentGraph+3)->addData(-yCoord[i] + x0, xCoord[i]  + y0);
        ui->widget->graph(currentGraph+5)->addData(-yCoord[i] + x0, -xCoord[i]  + y0);
        ui->widget->graph(currentGraph+6)->addData(xCoord[i]  + x0, -yCoord[i] + y0);
        ui->widget->replot();
    }

    currentGraph+=7;
}

QVector<QPair<int, int> > MainWindow::castle(double x1, double y1, double x2, double y2)
{
    QVector<QPair<int, int>> points;
    QString str;

    if(abs(y2 - y1) > abs(x2 - x1)) {
        std::swap(x1,y1);
        std::swap(x2,y2);
    }
    if (x1 > x2) {
        std::swap(x1,x2);
        std::swap(y1,y2);
    }
    int x_move=0,y_move=0;
    if (x1 !=0) {
        x_move=x1;
    }
    if (y1 !=0) {
        y_move = y1;
    }
    double x,y;
    x= x2 - y2 - x_move + y_move;
    y = y2 - y_move;
    QString m1 = "s", m2 = "d", tmp;
    while(x!=y) {
        if(x>y){
            x-=y;
            tmp = m2;
            std::reverse(tmp.begin(), tmp.end());
            m2 = m1 + tmp;
        }
        else {
            y-=x;
            tmp = m1;
            std::reverse(tmp.begin(), tmp.end());
            m1 = m2 + tmp;
        }
    }
    std::reverse(m1.begin(), m1.end());
    tmp = m2 +m1;
    ui->label->setText(tmp);
    points.push_back(std::make_pair(x1, y1));
    for(int i = 0; i < tmp.size(); i++) {
        if(tmp[i]=='s') {
            x1++;
            points.push_back(std::make_pair(x1,y1));
        }
        else {
            x1++;
            y1++;
            points.push_back(std::make_pair(x1,y1));
        }
    }
    return points;
}

void MainWindow::on_paintButton_clicked()
{
    paint();
}

