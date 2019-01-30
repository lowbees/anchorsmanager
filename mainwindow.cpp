#include "mainwindow.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(300, 400);

    {
        QPushButton *anchorBtn = new QPushButton("Anchor");
        QWidget *w = new QWidget;
        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(anchorBtn, 0, Qt::AlignCenter);
        w->setLayout(layout);
        setCentralWidget(w);

        QPushButton *targetBtn = new QPushButton("Target", this);
        manager.anchor(targetBtn, AnchorManager::Top, anchorBtn, AnchorManager::Bottom);
        manager.anchor(targetBtn, AnchorManager::Left, anchorBtn, AnchorManager::Right);
    }


    {
        QPushButton *roundedBtn = new QPushButton("", this);
        roundedBtn->setFixedSize(50, 50);

        roundedBtn->setStyleSheet("border-radius: 25px; background-color:yellow;");

        manager.anchor(roundedBtn, AnchorManager::Right, this, AnchorManager::Right);
        manager.anchor(roundedBtn, AnchorManager::Bottom, this, AnchorManager::Bottom);
        manager.setMargins(roundedBtn, QMargins(0, 0, 8, 8));
    }
}

MainWindow::~MainWindow()
{

}
