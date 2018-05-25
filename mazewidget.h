#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QQueue>
#include <QTimer>

class Maze: public QWidget
{
    enum {Wall = -3, Path = -2,Empty = -1};
    struct Point{
        int x,y;
    };
    Q_OBJECT

public:
    Maze(QWidget *parent = 0);
    ~Maze();

protected:
    void paintEvent(QPaintEvent* e);
    void paintGrid(QPainter& p);
    void paintField(QPainter& p);
    void mouseMoveEvent(QMouseEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void waveAlg();
    bool inField(int x, int y);

    QVector<QVector<int>> field;
    QQueue<Point> q;
    QTimer* timer;
    Point startPoint;
    Point endPoint;
    int mode;
    int count;
    bool clearMode;

public slots:
    void start();
    void startBtn();
    void stop();
    void clear();
    void modeChange(QString name);
    void changeSize(int n);
};

#endif // WIDGET_H
