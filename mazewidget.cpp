#include "mazewidget.h"
#include <unistd.h>
#include <QTime>

enum {WaveNewtonMode, WaveMoorMode};

Maze::Maze(QWidget *parent)
    : QWidget(parent)
    ,startPoint({0, 0})
    ,timer(new QTimer(this)), mode(WaveNewtonMode)
    ,clearMode(false)
{
    timer->setInterval(50);
    connect(timer, SIGNAL(timeout()), this, SLOT(start()));
    q.enqueue(startPoint);
}
Maze::~Maze()
{
}

//Slots
void Maze::clear(){
    stop();
    if(!clearMode){
        for(int i = 0; i < field.size(); i++)
            for(int j = 0; j < field.size(); j++)
                if(field[i][j] != Wall)
                    field[i][j] = Empty;
        clearMode = true;
    }
    else {
        for(int i = 0; i < field.size(); i++)
            for(int j = 0; j < field.size(); j++)
                field[i][j] = Empty;
        clearMode = false;
    }
    update();
}
void Maze::startBtn(){
    if(timer->isActive())
        return;
    count = 0;
    field[startPoint.x][startPoint.y] = count;
    timer->start();

}
void Maze::stop(){
    timer->stop();
}
void Maze::modeChange(QString name){
    stop();
    if(name == "Wave Newton"){
        mode = WaveNewtonMode;
    }else if(name == "Wave Moor"){
        mode = WaveMoorMode;
    }
}

void Maze::changeSize(int n)
{
    field = QVector<QVector<int>>(n, QVector<int>(n, Empty));
    endPoint.x = n-1;
    endPoint.y = n-1;
    update();

}
void Maze::start(){
    if(mode == WaveNewtonMode || mode == WaveMoorMode){
        waveAlg();
    }
    clearMode = false;
}
//


void Maze::waveAlg(){
    int n;
    Point* d;
    if(mode == WaveNewtonMode){
        n = 4;
        d = new Point[n]{
            { 0, 1},
            { 0,-1},
            { 1, 0},
            {-1, 0}
        };
    }
    else if(mode == WaveMoorMode){
        n = 8;
        d = new Point[n]{
            {0,  1},
            {0, -1},
            { 1, 0},
            {-1, 0},
            { 1, 1},
            {-1, 1},
            {-1,-1},
            { 1,-1}
        };
    }
    for(int i = 0; i < field.size(); i++)
        for(int j = 0; j < field.size(); j++){
            if(field[i][j] == count){
                for(int k = 0; k < n; k++){
                    int di = i + d[k].x;
                    int dj = j + d[k].y;
                    if(!inField(di,dj) ||
                            (field[di][dj] == Wall ||
                             field[di][dj] != Empty))
                        continue;
                    field[di][dj] = count+1;
                    update();
                }
            }
        }
    count++;
    if(field[endPoint.x][endPoint.y] != Empty){
        stop();
        int i = endPoint.x;
        int j = endPoint.y;
        count = field[i][j];
        for(; count > 0; count--){
            for(int k = 0; k < n; k++){
                int di = i + d[k].x;
                int dj = j + d[k].y;
                if(inField(di,dj) && field[di][dj] == count - 1){
                    i = di;
                    j = dj;
                    field[i][j] = Path;
                    break;
                }
            }
        }
        update();
    }
    delete []d;
}

bool Maze::inField(int x, int y){
    if(x < 0                    || y < 0          ||
            x >=field.size()         || y >= field.size())
        return false;
    return true;
}

//Graphic interface
void Maze::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    paintGrid(p);
    paintField(p);
}
void Maze::mouseMoveEvent(QMouseEvent *e){
    double xn = double(width())/field.size();
    double yn = double(height())/field[0].size();
    int x = e->x()/xn;
    int y = e->y()/yn;
    if(x >= field.size() || y >= field.size() ||
            y < 0            || x < 0         )
        return;
    if((x == startPoint.x && y == startPoint.y) ||
            (x == endPoint.x && y == endPoint.y))
        return;
    if(field[x][y] == Empty)
        field[x][y] = Wall;
    update();
}
void Maze::mousePressEvent(QMouseEvent *e){
    double xn = double(width())/field.size();
    double yn = double(height())/field[0].size();
    int x = e->x()/xn;
    int y = e->y()/yn;
    if((x == startPoint.x && y == startPoint.y) ||
            (x == endPoint.x && y == endPoint.y))
        return;
    if(field[x][y] == Wall)
        field[x][y] = Empty;
    else if(field[x][y] == Empty)
        field[x][y] = Wall;
    update();
}
void Maze::paintGrid(QPainter &p){
    double xn = double(width())/field.size();
    double yn = double(height())/field[0].size();
    QColor color = QColor(255, 255, 255, 30);
    p.setPen(color);
    for(double k = xn; k <= width(); k += xn)
        p.drawLine(k, 0, k, height());
    for(double k = yn; k <= height(); k += yn)
        p.drawLine(0, k, width(), k);
}
void Maze::paintField(QPainter &p)
{
    double xn = double(width())/field.size();
    double yn = double(height())/field[0].size();
    for(int x = 0; x < field.size(); x++)
        for(int y = 0; y < field[x].size();y++){
            QColor color;
            if(x == startPoint.x && y == startPoint.y)
                color = QColor(Qt::darkCyan);
            else if(x == endPoint.x && y == endPoint.y)
                color = QColor(Qt::darkRed);
            else if(field[x][y] == Wall)
                color = QColor(Qt::darkGray);
            else if(field[x][y] == Empty)
                color = QColor(0,0,0,100);
            else if(field[x][y] == Path)
                color = QColor(Qt::white);
            else if(field[x][y] > 0)
                color =  QColor(Qt::darkGreen);
            qreal left = (qreal)(xn*(x+1)-xn);
            qreal top  = (qreal)(yn*(y+1)-yn);
            QRectF rec(left, top, (qreal)xn, (qreal)yn);
            p.fillRect(rec, QBrush(color));
        }
}
