#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent),
    maze(new Maze),
    startBtn  (new QPushButton("Start")),
    stopBtn   (new QPushButton("Stop")),
    clearBtn  (new QPushButton("Clear")),
    mainVBox  (new QVBoxLayout(this)),
    btnHBox   (new QHBoxLayout),
    modeBox   (new QComboBox),
    changeSize(new QSpinBox)
{
    mainVBox->addWidget(modeBox);
    mainVBox->addWidget(maze);
    mainVBox->addLayout(btnHBox);

    btnHBox->addWidget(startBtn);
    btnHBox->addWidget(stopBtn);
    btnHBox->addWidget(clearBtn);
    btnHBox->addWidget(changeSize);

    modeBox->addItem("Wave Newton");
    modeBox->addItem("Wave Moor");
    changeSize->setMinimum(5);
    changeSize->setMaximum(40);
    changeSize->setValue(10);
    maze->changeSize(changeSize->value());

    connecting();

    resize(400,410);
}

void MainWindow::connecting()
{
    connect(startBtn, SIGNAL(clicked()), maze, SLOT(startBtn()));
    connect(stopBtn,  SIGNAL(clicked()), maze, SLOT(stop()));
    connect(clearBtn, SIGNAL(clicked()), maze, SLOT(clear()));
    connect(modeBox,  SIGNAL(activated(QString)), maze, SLOT(modeChange(QString)));
    connect(changeSize,SIGNAL(valueChanged(int)), maze, SLOT(changeSize(int)));
}
