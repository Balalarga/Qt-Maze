#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include "mazewidget.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:

public slots:

private:
    Maze* maze;
    QVBoxLayout* mainVBox;
    QHBoxLayout* btnHBox;
    QComboBox*   modeBox;
    QPushButton* startBtn;
    QPushButton* stopBtn;
    QPushButton* clearBtn;
    QSpinBox*    changeSize;

    void connecting();
};

#endif // MAINWINDOW_H
