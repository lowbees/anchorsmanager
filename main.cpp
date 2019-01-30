#include "mainwindow.h"
#include "anchormanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AnchorManager manager;

    MainWindow w1, w2;
    w2.setWindowTitle("Second");
    w2.show();
    w1.show();

    manager.anchor(&w2, AnchorManager::Top, &w1, AnchorManager::Top);
    manager.anchor(&w2, AnchorManager::Bottom, &w1, AnchorManager::Bottom);
    manager.anchor(&w2, AnchorManager::Right, &w1, AnchorManager::Left);
    manager.update(&w2);


    return a.exec();
}
