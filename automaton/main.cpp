#include "qt/MainWindow.h"
#include "core/CellularAutomaton.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    nkh::MainWindow w;
    w.setMainQmlFile(QStringLiteral("qrc:/view.qml"));
    w.showExpanded();
    
    return a.exec();
}
